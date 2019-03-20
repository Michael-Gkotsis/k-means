#include <stdio.h>
#include <stdlib.h>
#include "FileHandling.h"
#include <math.h>
#include <string.h>
#include <time.h>


int main(int argc, char *argv[])
{

      unsigned int n = 0;           // n Elements               // return 0 --> programm ended
      unsigned int k;               // k clusters                                                               // return -1 --> problem with the given file
      unsigned int i, j, d;         // i counter for n, j counter for k, d counter for dimensions
      char filename[100];  // The Variable for loading Custom Dataset
      unsigned int dim = 0;         // Dimensions of Elements
      char c;              // Temporary character for file analysis
      unsigned int a = 1;           // Use a = 1 to generate random Centroids
  		unsigned int iteration = 0;   // Amount of repetitions
  		int flag_end = 0;    // Flag for ending k-means
  		int flag = -1;       // Flag for filtering
      clock_t start, end;  // Count the time



/* -------------------------------------------------------------------------- */
    // Reading Dataset

   FILE* Dataset;

      printf("\n Give the DataSet file:");
      scanf("%s", filename);

    Dataset = fopen(filename, "r");
    if (!Dataset)
    {
        printf("\n There is something wrong with the Dataset file! \n\n");
        return -1;
    }

/* -------------------------------------------------------------------------- */





    dim = getColumns(Dataset); //Getting the dimensions of each Element of the Dataset
    rewind(Dataset);

    n = getRows(Dataset);       // Getting the Elements of the Dataset
    rewind(Dataset);

    printf("\n Elements:%d \n", n-1);
    printf("\n Dimensions:%d \n", dim);
    n--;
    printf("\n Choose the amount of Clusters:");
    scanf("%d", &k);

/* -------------------------------------------------------------------------- */
        // All the necessary memory allocation

        float *X;  // Array of Elements
        X = (float *)calloc(n*dim, sizeof(float));

        float *V;  // Array of Centroids
        V = (float *)calloc(k*dim, sizeof(float));


        float *FlagCentroids;  // Array of flag Centroids
        FlagCentroids = (float *)calloc(k*dim, sizeof(float));


        unsigned int *counter;  // Counter of elements for each cluster
        counter =(int *) calloc(k, sizeof(int));

        float *totalCluster;  // Sum of every element for each cluster
        totalCluster = (float *)calloc(k*dim,sizeof(float));


        float *distance;  // Array of Distance
        distance = (float *)calloc(n*n,sizeof(float));


        float *min;  // Array of minimum distance
        min = (float *)calloc(n,sizeof(float));

        unsigned int *location;  // Array of Locations
        location = (int *)calloc(n,sizeof(int));

        unsigned int *Cluster;  // Array of Clusters
        Cluster = (int *)calloc(n,sizeof(int));


/* -------------------------------------------------------------------------- */
                  // Passing elements to Array X[n][dim]



                    X = getData(Dataset,n,dim,X);

                    fclose(Dataset);




  start = clock();
/* -------------------------------------------------------------------------- */
/* ---------------------------------K-MEANS---------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
    // Generating Initial Random Centroids

    for (j = k; j--;)
    {
      i = rand() % n;
        for (d = dim; d--;)
        {
            V[j*dim + d] = X[i*dim + d];
              // Assigning the initial random centroids to FlagCentroids
            FlagCentroids[j*dim + d] = V[j*dim + d];
          }
    }


/* -------------------------------------------------------------------------- */

do{


  //   Setting Sum and Counter of each cluster to 0
  for(j = k; j--;)
  {
    for(d = dim; d--;)
    {
      totalCluster[j*dim + d] = 0;

    }
        counter[j] = 0;
  }

/* -------------------------------------------------------------------------- */
  // Calculating Distances of each element for each cluster and each dimension

  for(i = n; i--;)
  {
    for(j = k; j--;)
    {
      distance[i*n + j] = 0;
      for(d = dim; d--;)
      {
      distance[i*n + j] += ((X[i*dim + d] - V[j*dim + d])*(X[i*dim + d] - V[j*dim + d]));
      }
      distance[i*n + j] = sqrt(distance[i*n + j]);
    }
  }


/*-------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
      // Comparing Distances and getting the proper Cluster for each Element

      for(i = n; i--;)
      {
        min[i] = 9999;
        for(j = k; j--;)
        {
          if(distance[i*n + j] < min[i])
          {
            min[i] = distance[i*n + j];
            location[i] = j;
          }
        }
      }
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
      // Getting Elements and Clusters all together

      for(i = n; i--;)
      Cluster[i] = location[i];
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
      // Calculating total Sum of each dim for each cluster and counter
      for(i = n; i--;)
      {
        for(j = k; j--;)
        {
          if(Cluster[i] == j)
          {
            for(d = dim; d--;)
            {
              totalCluster[j*dim + d] += X[i*dim + d];
            }
            counter[j]++;
          }
        }
      }
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
    // Calculating New Centroid

    for(j = k; j--;)
    {
      for(d = dim; d--;)
      {
        V[j*dim + d] = totalCluster[j*dim + d]/counter[j];
      }
    }
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
			// Here goes the comparison last FlagCentroid with new centroids

      for(j = k; j--;)
      {
        for(d = dim; d--;)
        {
          if(FlagCentroids[j*dim + d] != V[j*dim + d])
          {
            flag_end = 0;
            break;
          }else
          {
            flag_end = -1;
            break;
          }
        }
      }
/* -------------------------------------------------------------------------- */
 // Passing New centroids to flag;

 for(j = k; j--;)
 {
   for(d = dim; d--;)
   {
     FlagCentroids[j*dim + d] = V[j*dim + d];
   }
 }

 iteration++;


}while(flag_end != -1);

/* -------------------------------------------------------------------------- */
/* ---------------------------------END-------------------------------------- */
/* -------------------------------------------------------------------------- */
end = clock();
/*----------------------------------------------------------------------------*/
double total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
printf("\n Time of Algorithm Execution: %lf \n\n",total_time);

FILE** ClusterFile =calloc(k+1,sizeof(FILE*));

for(j = k; j--;)
   {
     char *fileName;
    fileName = calloc(n,sizeof(char));
     snprintf(fileName,n*sizeof(char),"C%d.txt",j);
      ClusterFile[j] = fopen(fileName,"w");
      free(fileName);
   }

for(j = k; j--;)
{
  for(i = n; i--;)
  {
    if(Cluster[i] == j)
    {
      for(d = dim; d--;)
      {
        fprintf(ClusterFile[j], "%lf ",X[i*dim + d] );
      }
      fprintf(ClusterFile[j], "\n");
    }
  }
}

   for (j = k; j--;)
   fclose(ClusterFile[j]);

   for(j = 60 ; j >= k ; j--)
{
  char *fileName;
  fileName = calloc(n,sizeof(char));
  snprintf(fileName,n*sizeof(char),"C%d.txt",j);
   remove(fileName);
   free(fileName);
}


  free(X);
  free(V);
  free(Cluster);
  free(FlagCentroids);
  free(counter);
  free(totalCluster);
  free(distance);
  free(min);
  free(location);
  free(ClusterFile);
return 0;
}
