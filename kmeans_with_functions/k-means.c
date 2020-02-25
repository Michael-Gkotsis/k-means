#include "k-meansH.h"

#include "FileHandling.h"


int main(int argc, char * argv[]) {

  int n = 0; // n Elements
  int k; // k clusters
  int i, j, d; // i counter for n, j counter for k, d counter for dimensions
  char filename[100]; // The Variable for loading Custom Dataset
  int dim = 0; // Dimensions of Elements
  char c; // Temporary character for file analysis
  int a = 1; // Use a = 1 to generate random Centroids
  int iteration = 0; // Amount of repetitions
  int flag_end = 0; // Flag for ending k-means
  int flag = -1; // Flag for filtering
  clock_t start, end; // Count the time
  int choise; // Choise holder for switch
srand(time(NULL));
  /* -------------------------------------------------------------------------- */
  // Reading Dataset, counting n elements, assigning to X Array

  FILE * Dataset;

  printf("\n Give the DataSet file:");
  scanf("%s", filename);
  printf("\n");

  Dataset = fopen(filename, "r");
  if (!Dataset) {
    printf("\n There is something wrong with the Data file! \n\n");
    return -1;
  }
  /* -------------------------------------------------------------------------- */
  dim = getColumns(Dataset);

  n = getRows(Dataset); // Getting the Elements of the Dataset
  rewind(Dataset);

  printf("\n Elements:%d", n - 1);
  printf("\n Dimensions:%d\n\n", dim);

  n--;

  printf("\n Choose the amount of Clusters:");  //User gives the amount of clusters
  scanf("%d", &k);
  /* -------------------------------------------------------------------------- */
    // All the necessary memory allocation

    float *Elements; //Array of elements
    Elements = (float*)calloc(n*dim, sizeof(float));

    float *Centroids; //Array of Centroids
    Centroids = (float*)calloc(k*dim,sizeof(float));

    float *FlagCentroids; //Array for Holding the Previous Iteration FlagCentroids
    FlagCentroids = (float*)calloc(k*dim,sizeof(float));

    unsigned int *Counter; //Array for holding the amount of elements for each cluster
    Counter = (int*)calloc(k,sizeof(int));

    float *ClusterTotalSum; //Sum of every element of each Cluster
    ClusterTotalSum = (float*)calloc(k*dim,sizeof(float));

    float *Distance; //Array holding distances for each element of each Centroid
    Distance = (float*)calloc(n*k,sizeof(float));



    unsigned int *Location;//Array holding which elements belong to which cluster
    Location = (int*)calloc(n,sizeof(int));

    int *Cluster; //Array of holding each's element corresponding cluster
    Cluster = (int*)calloc(n,sizeof(int));

    /* -------------------------------------------------------------------------- */
      // Passing elements to Array X[n][dim]

      for(unsigned int i = 0; i < n; ++i)
      {
          for(unsigned int d = 0; d < dim; ++d)
              fscanf(Dataset,"%f, ",&Elements[i*dim + d]);
      }

      fclose(Dataset);

      /*Generate Inital Random Centroids */

      Centroids = getRandomCentroids(Elements,n,dim,k,Centroids);

      /*Assign inital random centroids to Flag Centroids */
      FlagCentroids = copyInitialCentroids(Centroids,dim,k,FlagCentroids);


  start = clock();
  do{

     ++iteration;
     Counter = zero1D(Counter,k);
     ClusterTotalSum = zero2D(ClusterTotalSum,k,dim);
/* -------------------------------------------------------------------------- */
// Calculating Distances of each element for each cluster and each dimension
    Distance = getDistances(Elements,Centroids,n,k,dim,Distance);
    Location = getLocation(Distance,n,k,Location);
    ClusterTotalSum = getTotalSum(Elements,Location,n,k,dim,ClusterTotalSum);
    Counter = getCounter(Location,n,k,Counter);
    Centroids = getNewCentroids(ClusterTotalSum,Counter,k,dim,Centroids);
    flag_end = checkEquality(FlagCentroids,Centroids,k,dim);
    FlagCentroids = copyInitialCentroids(Centroids,dim,k,FlagCentroids);



  }while(flag_end != -1);

printf("\nRuns initialized %d\n",iteration);

end = clock();
/*----------------------------------------------------------------------------*/
double total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
printf("\n Time of Algorithm Execution: %lf \n\n",total_time);


FILE** ArrayOfFiles = calloc(k,sizeof(FILE*));
char fileName[40];
for(unsigned int j = 0; j < k; ++j)
{
  sprintf(fileName,"c%d.txt",j);
  ArrayOfFiles[j] = fopen(fileName,"w");
}

for(unsigned int j = 0; j < k; ++j)
{
  for(unsigned int i = 0; i < n; ++i)
  {
    if(Location[i] == j)
    {
      for(unsigned int d = 0; d < dim; ++d)
      {
        fprintf(ArrayOfFiles[j],"%f ",Elements[i*dim + d]);
      }
      fprintf(ArrayOfFiles[j], "\n");
    }
  }
}


for(unsigned int j = 1000; j >=k; --j)
{
  sprintf(fileName,"c%d.txt",j);
  remove(fileName);
}


free(ArrayOfFiles);
free(Elements);
free(Centroids);
free(FlagCentroids);
free(Counter);
free(ClusterTotalSum);
free(Distance);
free(Location);
free(Cluster);
}
