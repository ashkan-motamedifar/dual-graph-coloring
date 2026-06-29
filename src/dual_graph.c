#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "./lib/hashmap.h"
#include <time.h> 
#define INF 99999
#define INTMAX 99999
float *Vertices;
int VertexCount=0;
float *DualVertices;
int *Facets,FacetCount=0;
int *Aretes,AretesCount=0;
int *MeshEdges,MeshEdgesCount=0;
int MaxDistance=0;
int counter=0;
int *Distance;
void Read(char *inputFileName){
    int i=0,j=0;
    FILE * fp;
    char* line=(char*)malloc(100*sizeof(char));
    fp = fopen(inputFileName, "r");
    if(!fp){
     printf("\n Unable to open or create the file: %s ", inputFileName);
     getchar();
     exit(0);
    }
    do{
      strcpy(line,"");
      fgets(line, 100, fp);
      if(*line=='v')
        VertexCount++;
      if(*line=='f')
        FacetCount++;
    }while(*line);
    fseek(fp, 0, SEEK_SET);
    Vertices=(float *)malloc(3 * VertexCount * sizeof(float));
    char *token1,*token2,*token3;
    Facets=(int *)malloc(3 * FacetCount * sizeof(int));
    MeshEdges=(int *)malloc(2 * 3 * FacetCount * 3 * sizeof(int));
    do{
      strcpy(line,"");
      fgets(line, 100, fp);
      if(*line=='v')
       {
      token1 =strtok(line," ");
      token1 =strtok(NULL," ");
      *(Vertices + i*3)=atof(token1);
      token2 =strtok(NULL," ");
      *(Vertices + i*3+1)=atof(token2);
      token3 =strtok(NULL," ");
      *(Vertices + i*3+2)=atof(token3);
      i++;
      }
      if(*line=='f'){
      token1 = strtok(line," ");
      token1 = strtok(NULL," ");
      *(Facets + j*3)=atoi(token1);
      token2 = strtok(NULL," ");
      *(Facets + j*3+1)=atoi(token2);
      token3 = strtok(NULL," ");
      *(Facets + j*3+2)=atoi(token3);
      *(MeshEdges+j*18+0)=atoi(token1);
      *(MeshEdges+j*18+1)=atoi(token2);
      *(MeshEdges+j*18+2)=j+1;
      *(MeshEdges+j*18+3)=atoi(token2);
      *(MeshEdges+j*18+4)=atoi(token3);
      *(MeshEdges+j*18+5)=j+1;
      *(MeshEdges+j*18+6)=atoi(token3);
      *(MeshEdges+j*18+7)=atoi(token1);
      *(MeshEdges+j*18+8)=j+1;
      // reverse direction so equal-edge comparison in Convert() finds the adjacent facet
      *(MeshEdges+j*18+9)=atoi(token2);
      *(MeshEdges+j*18+10)=atoi(token1);
      *(MeshEdges+j*18+11)=j+1;
      *(MeshEdges+j*18+12)=atoi(token3);
      *(MeshEdges+j*18+13)=atoi(token2);
      *(MeshEdges+j*18+14)=j+1;
      *(MeshEdges+j*18+15)=atoi(token1);
      *(MeshEdges+j*18+16)=atoi(token3);
      *(MeshEdges+j*18+17)=j+1;
      j++;
      }
    }while(*line);
    MeshEdgesCount=2 * 3 * FacetCount;
    fclose(fp);
}
int areEquvalent(int *FirstArete,int *SecondArete){
  if((*FirstArete==*SecondArete && *(FirstArete+1)==*(SecondArete+1))||(*FirstArete==*(SecondArete+1) && *(FirstArete+1)==*SecondArete))
    return 1;
  else
    return 0;
}
int estSuperieureA(int *FirstArete,int *SecondArete){
   if(*FirstArete<*SecondArete||(*FirstArete==*SecondArete && *(FirstArete+1)<*(SecondArete+1)))
      return 1;
   else
      return 0;
}
void SelectionSortEdges(void){
    int i,j,MinIndex,temp0,temp1,temp2;
     for (i = 0; i < MeshEdgesCount - 1; i++){ 
        MinIndex = i;
        for (j = i + 1; j < MeshEdgesCount; j++)
            if (estSuperieureA((MeshEdges + 3*j),(MeshEdges+3*MinIndex)))
                MinIndex = j;
        if (MinIndex != i)
            {
            temp0=*(MeshEdges+3*MinIndex);
            temp1=*(MeshEdges+3*MinIndex+1);
            temp2=*(MeshEdges+3*MinIndex+2);
            *(MeshEdges+3*MinIndex)=*(MeshEdges+3*i);
            *(MeshEdges+3*MinIndex+1)= *(MeshEdges+3*i+1);
            *(MeshEdges+3*MinIndex+2)=*(MeshEdges+3*i+2);
            *(MeshEdges+3*i)=temp0;
            *(MeshEdges+3*i+1)=temp1;
            *(MeshEdges+3*i+2)=temp2;
            }
    }
}
void HeapCreate(int *MeshEdges, int N, int i)
{
    int temp0,temp1,temp2;
    int largest = i;
    int left  = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < N && estSuperieureA((MeshEdges + 3*largest),(MeshEdges+3*left)))
        largest = left;
    if (right < N && estSuperieureA((MeshEdges + 3*largest),(MeshEdges+3*right)))
        largest = right;
    if (largest != i) {
            temp0=*(MeshEdges+3*largest);
            temp1=*(MeshEdges+3*largest+1);
            temp2=*(MeshEdges+3*largest+2);
            *(MeshEdges+3*largest)=*(MeshEdges+3*i);
            *(MeshEdges+3*largest+1)= *(MeshEdges+3*i+1);
            *(MeshEdges+3*largest+2)=*(MeshEdges+3*i+2);
            *(MeshEdges+3*i)=temp0;
            *(MeshEdges+3*i+1)=temp1;
            *(MeshEdges+3*i+2)=temp2;
       HeapCreate(MeshEdges, N, largest);
    }
}
void HeapSortEdges(void){
    int i,j,temp0,temp1,temp2;
    for (i = MeshEdgesCount / 2 - 1; i >= 0; i--)
        HeapCreate(MeshEdges, MeshEdgesCount, i);
    for (i = MeshEdgesCount - 1; i >= 0; i--) {
        temp0=*(MeshEdges);
        temp1=*(MeshEdges+1);
        temp2=*(MeshEdges+2);
        *(MeshEdges)=*(MeshEdges+3*i);
        *(MeshEdges+1)= *(MeshEdges+3*i+1);
        *(MeshEdges+2)=*(MeshEdges+3*i+2);
        *(MeshEdges+3*i)=temp0;
        *(MeshEdges+3*i+1)=temp1;
        *(MeshEdges+3*i+2)=temp2;
        HeapCreate(MeshEdges, i, 0);    
    }
}
struct AVL_Node
{
    int Vertex1,Vertex2,FacetNumber;
    struct AVL_Node *left;
    struct AVL_Node *right;
    int AVL_NodeHeight;
};
struct AVL_Node* new_AVL_Node(int Vertex1,int Vertex2,int FacetNumber)
{
    struct AVL_Node* AVL_node = (struct AVL_Node*)malloc(sizeof(struct AVL_Node));
    AVL_node->Vertex1 = Vertex1;
    AVL_node->Vertex2 = Vertex2;
    AVL_node->FacetNumber = FacetNumber;
    AVL_node->left  = NULL;
    AVL_node->right = NULL;
    AVL_node->AVL_NodeHeight = 1;
    return(AVL_node);
}
int Height(struct AVL_Node *AVL_node)
{
    if (AVL_node == NULL)
        return 0;
    return AVL_node->AVL_NodeHeight;
}
struct AVL_Node *RotateRight(struct AVL_Node *nodeR)
{
    struct AVL_Node *nodeA= nodeR->left;
    struct AVL_Node *nodeA_Right = nodeA->right;
    nodeA->right = nodeR;
    nodeR->left = nodeA_Right;
    if(Height(nodeR->left)<Height(nodeR->right))
        nodeR->AVL_NodeHeight = Height(nodeR->right)+ 1;
    else
        nodeR->AVL_NodeHeight = Height(nodeR->left) + 1;
    if(Height(nodeA->left)<Height(nodeA->right))
        nodeA->AVL_NodeHeight = Height(nodeA->right)+ 1;
    else
        nodeA->AVL_NodeHeight = Height(nodeA->left) + 1;
    return nodeA;
}
struct AVL_Node *RotateLeft(struct AVL_Node *nodeR)
{
    struct AVL_Node *nodeA = nodeR->right;
    struct AVL_Node *nodeA_Left = nodeA->left;
    nodeA->left = nodeR;
    nodeR->right = nodeA_Left;
    if(Height(nodeR->left)<Height(nodeR->right))
        nodeR->AVL_NodeHeight = Height(nodeR->right) + 1;
    else
        nodeR->AVL_NodeHeight = Height(nodeR->left) + 1;
    if(Height(nodeA->left)<Height(nodeA->right))
        nodeA->AVL_NodeHeight = Height(nodeA->right) + 1;
    else
        nodeA->AVL_NodeHeight = Height(nodeA->left) + 1;
    return nodeA;
}
struct AVL_Node* InsertAVL(struct AVL_Node* AVL_node, int Vertex1,int Vertex2, int FacetNumber)
{
    int *InsertingEdge=(int *)malloc(3 * sizeof(int));
    *(InsertingEdge)=Vertex1;
    *(InsertingEdge+1)=Vertex2;
    *(InsertingEdge+2)=FacetNumber;
    int *AVL_RootEdge=(int *)malloc(3 * sizeof(int));

    if (AVL_node == NULL)
      return(new_AVL_Node(Vertex1,Vertex2,FacetNumber));
    *(AVL_RootEdge)=AVL_node->Vertex1;
    *(AVL_RootEdge+1)=AVL_node->Vertex2;
    *(AVL_RootEdge+2)=AVL_node->FacetNumber;

    if (estSuperieureA(InsertingEdge,AVL_RootEdge))
        AVL_node->left  = InsertAVL(AVL_node->left, Vertex1,Vertex2,FacetNumber);
    else
        AVL_node->right = InsertAVL(AVL_node->right, Vertex1,Vertex2,FacetNumber);

    if(Height(AVL_node->left)<Height(AVL_node->right))
        AVL_node->AVL_NodeHeight = Height(AVL_node->right) + 1;
    else
        AVL_node->AVL_NodeHeight = Height(AVL_node->left) + 1;

    int BalanceValue;
    if (AVL_node == NULL)
        BalanceValue= 0;
    else
        BalanceValue= Height(AVL_node->left) - Height(AVL_node->right);

     int *AVL_RootLeftEdge=(int *)malloc(3 * sizeof(int));
     int *AVL_RootRightEdge=(int *)malloc(3 * sizeof(int));

     if(AVL_node->left){
     *(AVL_RootLeftEdge)  =AVL_node->left->Vertex1;
     *(AVL_RootLeftEdge+1)=AVL_node->left->Vertex2;
     *(AVL_RootLeftEdge+2)=AVL_node->left->FacetNumber;}

    if(AVL_node->right){
     *(AVL_RootRightEdge)  =AVL_node->right->Vertex1;
     *(AVL_RootRightEdge+1)=AVL_node->right->Vertex2;
     *(AVL_RootRightEdge+2)=AVL_node->right->FacetNumber;}

    if (BalanceValue > 1  && estSuperieureA(InsertingEdge,AVL_RootLeftEdge))
        return RotateRight(AVL_node);
    if (BalanceValue < -1 && estSuperieureA(AVL_RootRightEdge,InsertingEdge))
        return RotateLeft(AVL_node);
    if (BalanceValue > 1  && estSuperieureA(AVL_RootLeftEdge,InsertingEdge))
    {
        AVL_node->left = RotateLeft(AVL_node->left);
        return RotateRight(AVL_node);
    }
    if (BalanceValue < -1 && estSuperieureA(InsertingEdge,AVL_RootRightEdge))
    {
        AVL_node->right = RotateRight(AVL_node->right);
        return RotateLeft(AVL_node);
    }
    return AVL_node;
}
void InorderAVL(struct AVL_Node *AVL_Root)
{
    if(AVL_Root != NULL)
    {
      InorderAVL(AVL_Root->left);
      *(MeshEdges+counter*3)=AVL_Root->Vertex1;
      *(MeshEdges+counter*3+1)=AVL_Root->Vertex2;
      *(MeshEdges+counter*3+2)=AVL_Root->FacetNumber;
      counter++;
      InorderAVL(AVL_Root->right);
    }
}
void AVL_SortEdges(void){
    int i;
	struct AVL_Node *AVL_Root = NULL;
    for (i=0; i < MeshEdgesCount; i++)
      AVL_Root=InsertAVL(AVL_Root,*(MeshEdges+i*3),*(MeshEdges+i*3+1),*(MeshEdges+i*3+2));
    counter=0;
    InorderAVL(AVL_Root);
}
char* CreateKey(int Vertex1,int NextIndex)
{   char Key[30];
    char TempString[20];
    int i = 0,j=0,k=0,rem;
    while (Vertex1 != 0){
        rem = Vertex1 % 10;
        TempString[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        Vertex1 = Vertex1 / 10;
    }
    for(j=i-1;j>=0;j--)
        Key[i-j-1]=TempString[j];
    Key[i++]='_';
    if(NextIndex==0)
      Key[i++]='0';
    else
    {  k=0;
      while (NextIndex != 0) {
        rem = NextIndex % 10;
        TempString[k++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        NextIndex = NextIndex / 10;
      }
      for(j=k-1;j>=0;j--)
         Key[i++]=TempString[j];
    }
    Key[i] = '\0';
    return strdup(Key);
}
void HashSortEdges(void){
   int i,j,k;
   const unsigned initial_size = MeshEdgesCount;
   struct hashmap_s hashmap;
   hashmap_create(initial_size, &hashmap);
   int* Value;
   char* Key1;
   char* Key2;
   void* element;
   // key "<vertex>_<index>" lets multiple edges share a first-vertex bucket without collisions
   for (i=0; i < MeshEdgesCount; i++)
        for(j=0;;j++)
        {
            Key1=NULL;
            Key1=(char *)malloc(30 * sizeof(char));
            strcpy(Key1,CreateKey(*(MeshEdges+i*3),j));
            element= hashmap_get(&hashmap, Key1, strlen(Key1));
            if(element==NULL)
            {
                Value=NULL;
                Value=(int *)malloc(3 * sizeof(int));
                *(Value)=*(MeshEdges+i*3);
                *(Value+1)=*(MeshEdges+i*3+1);
                *(Value+2)=*(MeshEdges+i*3+2);
                hashmap_put(&hashmap, Key1, strlen(Key1),Value);
                break;
            }
        }
    int* TempEdge1;
    int* TempEdge2;
    counter=0;
    for (i=1;i<=VertexCount;i++)
      for(j=0;;j++)
      {
        Key1=NULL;
        Key1=(char *)malloc(30 * sizeof(char));
        strcpy(Key1,CreateKey(i,j));
        element = hashmap_get(&hashmap, Key1, strlen(Key1));
        if(NULL == element)
           break;
        else
        {
            TempEdge1=NULL;
            TempEdge1=(int *)malloc(3 * sizeof(int));
            *(TempEdge1)=*((int*)element);
            *(TempEdge1+1)=*((int*)element+1);
            *(TempEdge1+2)=*((int*)element+2);
            for(k=0;;k++){
                 Key2=NULL;
                 Key2=(char *)malloc(30 * sizeof(char));
                 strcpy(Key2,CreateKey(*(TempEdge1+1),k));
                 element = hashmap_get(&hashmap, Key2, strlen(Key2));
                 if(NULL == element)
                    break;
                 else if(*(TempEdge1)==*((int*)element+1)&&*(TempEdge1+2)!=*((int*)element+2))
                 {
                    hashmap_remove(&hashmap, Key1, strlen(Key1));
                    TempEdge2=NULL;
                    TempEdge2=(int *)malloc(3 * sizeof(int));
                    *(TempEdge2)=*(int*)element;
                    *(TempEdge2+1)=*((int*)element+1);
                    *(TempEdge2+2)=*((int*)element+2);
                    counter++;
                    *(MeshEdges+counter*3)=*(TempEdge1);
                    *(MeshEdges+counter*3+1)=*(TempEdge1+1);
                    *(MeshEdges+counter*3+2)=*(TempEdge1+2);
                    counter++;
                    *(MeshEdges+counter*3)=*(TempEdge2);
                    *(MeshEdges+counter*3+1)=*(TempEdge2+1);
                    *(MeshEdges+counter*3+2)=*(TempEdge2+2);
                 }
            }
        }
     }
     hashmap_destroy(&hashmap);
}
void Convert(void){
    int i,j;
    DualVertices=(float *)malloc(3 * FacetCount * sizeof(float));
    Aretes=(int *)malloc(2*MeshEdgesCount*sizeof(int));
    for(i=0;i<FacetCount;i++)
       {
        *(DualVertices+i*3)  =(*(Vertices+(*(Facets+i*3)-1)*3)
                              +*(Vertices+(*(Facets+i*3+1)-1)*3)
                              +*(Vertices+(*(Facets+i*3+2)-1)*3))/3;

        *(DualVertices+i*3+1)=(*(Vertices+(*(Facets+i*3)-1)*3+1)
                              +*(Vertices+(*(Facets+i*3+1)-1)*3+1)
                              +*(Vertices+(*(Facets+i*3+2)-1)*3+1))/3;

        *(DualVertices+i*3+2)=(*(Vertices+(*(Facets+i*3)-1)*3+2)
                              +*(Vertices+(*(Facets+i*3+1)-1)*3+2)
                              +*(Vertices+(*(Facets+i*3+2)-1)*3+2))/3;
       }
    AretesCount=0;
    for(i=0;i<MeshEdgesCount;i++)
        if(areEquvalent(MeshEdges+3*i,MeshEdges+3*(i+1)))
        {
          *(Aretes+2*AretesCount)=*(MeshEdges+3*i+2);
          *(Aretes+2*AretesCount+1)=*(MeshEdges+3*(i+1)+2);
          AretesCount++;
        }
}
void WriteWithVertexColor(char *OutputFileName)
{
    int i,j;

    float Redcolor,GreenColor,BlueColor;
    FILE * fp;
    fp = fopen(OutputFileName, "w");
    if(!fp)
        {
         printf("\n Unable to open or create the : %s ", OutputFileName);
         getchar();
         exit(0);
         }
    char* str1 = (char*)malloc(100);
    for(i=0;i<FacetCount;i++)
       {
        Redcolor=(float)1.0/MaxDistance**(Distance+i);
        GreenColor=1.0-(float)1.0/MaxDistance**(Distance+i);
        BlueColor=0.0;
        snprintf(str1, 100,"v %f %f %f %f %f %f",
        *(DualVertices + i*3),*(DualVertices + i*3 + 1),*(DualVertices + i*3 + 2),Redcolor,GreenColor,BlueColor);
        fputs(str1,fp);
        fputs("\n",fp);
       }
    for(i=0;i<AretesCount;i++)
       {
        snprintf(str1, 30,"l %d %d ", *(Aretes + i*2), *(Aretes + i*2 + 1));
        fputs(str1,fp);
        fputs("\n",fp);
       }
    fclose(fp);
}
int MinDistance(int* IncludeSet,int* Distance)
{
    int MinDis = INTMAX, MinIndex,Ver;
    for (Ver = 0; Ver < FacetCount; Ver++)
        if (*(IncludeSet+Ver) == 0 && *(Distance+Ver) <= MinDis)
            {
                MinDis = *(Distance+Ver);
                MinIndex = Ver;
            }
    return MinIndex;
}
void Dijkstra(int* DualGraph, int SourceVertex,int* Distance)
{
    int* IncludeSet, i, j;
    IncludeSet=(int*)malloc(FacetCount*sizeof(int));
    for (i = 0; i < FacetCount; i++)
        {
            *(Distance+i) = INTMAX;
            *(IncludeSet+i) = 0;
        }
    *(Distance+SourceVertex) = 0;
    for (i = 0; i < FacetCount - 1; i++)
    {
        int MinIndex = MinDistance(IncludeSet,Distance);
        *(IncludeSet+MinIndex) = 1;
        for (j = 0; j < FacetCount; j++)
            if (!*(IncludeSet+j) && *(DualGraph+MinIndex*FacetCount+j)
                 && *(Distance+MinIndex) != INTMAX && *(Distance+MinIndex) + *(DualGraph+MinIndex*FacetCount+j) < *(Distance+j))
                    *(Distance+j) = *(Distance+MinIndex) + *(DualGraph+MinIndex*FacetCount+j);
    }
}
void Coloring(){
int i, j;
Distance=(int*)malloc(FacetCount*sizeof(int));
  int* DualGraphAdjacency=(int *)malloc(FacetCount * FacetCount * sizeof(int));
  for (i = 0; i < FacetCount; i++)
     for (j = 0; j < FacetCount; j++)
        if(i==j)
            *(DualGraphAdjacency+i*FacetCount+j)=0;
        else
            *(DualGraphAdjacency+i*FacetCount+j)=INF;
  for(i=0;i<AretesCount;i++)
       {
        *(DualGraphAdjacency+(*(Aretes+i*2)-1)*FacetCount+*(Aretes+i*2+1)-1)=1;
        *(DualGraphAdjacency+(*(Aretes+i*2+1)-1)*FacetCount+*(Aretes+i*2)-1)=1;
       }
  Dijkstra(DualGraphAdjacency,1,Distance);
  for (i = 0; i < FacetCount; i++)
      if(MaxDistance<*(Distance+i))
          MaxDistance=*(Distance+i);
}
void WriteWithoutVertexColor(char *outputFileName)
{
    int i;
    FILE * fp;
    fp = fopen(outputFileName, "w");
    if(!fp)
        {
         printf("\n Unable to open or create the : %s ", outputFileName);
         getchar();
         exit(0);
         }
    char* str1 = (char*)malloc(100);
    for(i=0;i<FacetCount;i++)
       {
        snprintf(str1, 100,"v %f %f %f",
        *(DualVertices + i*3),*(DualVertices + i*3 + 1),*(DualVertices + i*3 + 2));
        fputs(str1,fp);
        fputs("\n",fp);
       }
    for(i=0;i<AretesCount;i++)
       {
        snprintf(str1, 30,"l %d %d ", *(Aretes + i*2), *(Aretes + i*2 + 1));
        fputs(str1,fp);
        fputs("\n",fp);
       }
    fclose(fp);
}
int main()
{
    char* InputCommand=(char*)malloc(100*sizeof(char));
    char* InputFileName=(char*)malloc(20*sizeof(char));
    char* OutputFileName=(char*)malloc(20*sizeof(char));
    clock_t time;
    char *Token1,*Token2,*Token3,*Token4;
   	printf("Enter \"input file name with .obj\"\n\"destination file name with .obj\"\n\"the algorithm name to generate the dual graph\"\n (selectionsort or heapsort or avltree or hashtable)\n and \"y for coloring the dual graph and n for not coloring\" with space for separation:\nExample: bunny10k.obj out_bunny10k.obj avltree y\n");
	gets(InputCommand);
   	Token1 =strtok(InputCommand," ");
   	Token2 =strtok(NULL," ");
   	Token3 =strtok(NULL," ");
 	Token4 =strtok(NULL," ");
   	strcpy(InputFileName,Token1);
  	strcpy(OutputFileName,Token2);
    Read(InputFileName);
   	time=clock();
    if(!strcmp(Token3,"selectionsort"))
       	SelectionSortEdges();
	else if(!strcmp(Token3,"heapsort"))
   		HeapSortEdges();
	else if(!strcmp(Token3,"avltree"))
	   	AVL_SortEdges();
	else if(!strcmp(Token3,"hashtable"))
		HashSortEdges();    	
    Convert();   
    time=clock()-time;         
    if(!strcmp(Token4,"y"))
    {
        Coloring();
        WriteWithVertexColor(OutputFileName);
    }
    else
        WriteWithoutVertexColor(OutputFileName);
    double consumed_time =((double)time)/CLOCKS_PER_SEC;
    printf("\n Consumed time to convert a mesh with %d vertices and %d edges to its dual graph with %s method is:%f seconds",VertexCount,MeshEdgesCount,Token3,consumed_time);
 }
