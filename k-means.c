#include <stdio.h>
#include <stdlib.h>
#include "FileHandling.h"
#include <math.h>
#include <string.h>
#include <time.h>


int main(int argc, char *argv[])
{

  int n = 0;           // n Elements               // return 0 --> programm ended
      int k;               // k clusters                                                               // return -1 --> problem with the given file
      int i, j, d;         // i counter for n, j counter for k, d counter for dimensions
      char filename[100];  // The Variable for loading Custom Dataset
      int dim = 0;         // Dimensions of Elements
      char c;              // Temporary character for file analysis
      int a = 1;           // Use a = 1 to generate random Centroids
  		int iteration = 0;   // Amount of repetitions
  		int flag_end = 0;    // Flag for ending k-means
  		int flag = -1;       // Flag for filtering
      clock_t start, end;  // Count the time
  int choise;// Choise holder for switch


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

    printf("\n Choose the amount of Clusters:");
    scanf("%d", &k);

/* -------------------------------------------------------------------------- */
        // All the necessary memory allocation

        double **X;   // Array of Elements
        X = (double **)calloc(n, sizeof(double *));
        for (d = 0; d < n; d++)
        X[d] = (double *)calloc(dim, sizeof(double));

        double **V;  // Array of Centroids
            V = (double **)calloc(k, sizeof(double *));
            for (d = 0; d < k; d++)
                V[d] =(double *) calloc(dim, sizeof(double));

                double **FlagCentroids;  // Array of flag Centroids
            FlagCentroids = (double **)calloc(k, sizeof(double *));
            for (d = 0; d < k; d++)
                FlagCentroids[d] = (double *)calloc(dim, sizeof(double));

            int *counter;  // Counter of elements for each cluster
            counter =(int *) calloc(k, sizeof(int));

        		double **totalCluster;  // Sum of every element for each cluster
            totalCluster = (double **)calloc(k,sizeof(double *));
        		for(d = 0; d < k; d++)
        		   totalCluster[d] =(double *) calloc(dim,sizeof(double));

        		double **distance;  // Array of Distance
        		distance = (double **)calloc(n,sizeof(double *));
        			 for(j = 0; j < n; j++)
        			 distance[j] =(double *)calloc(k,sizeof(double));

            double *min;  // Array of minimum distance
            min = (double *)calloc(n,sizeof(double));

            int *location;  // Array of Locations
            location = (int *)calloc(n,sizeof(int));

            int *Cluster;  // Array of Clusters
            Cluster = (int *)calloc(n,sizeof(int));


/* -------------------------------------------------------------------------- */
                  // Passing elements to Array X[n][dim]


 n--;
                    X = getData(Dataset,n,dim,X);



            fclose(Dataset);
  start = clock();
/* -------------------------------------------------------------------------- */
/* ---------------------------------K-MEANS---------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
    // Generating Initial Random Centroids

    for (j = 0; j < k; j++)
    {
      i = rand() % n;
        for (d = 0; d < dim; d++)
            V[j][d] = X[i][d];
    }

/* -------------------------------------------------------------------------- */
    // Assigning the initial random centroids to FlagCentroids

    for (j = 0; j < k; j++)
        for (d = 0; d < dim; d++)
            FlagCentroids[j][d] = V[j][d];

/* -------------------------------------------------------------------------- */

do{


  //   Setting Sum and Counter of each cluster to 0
  for(j = 0; j < k; j++)
  {
    for(d = 0; d < dim; d++)
    {
      totalCluster[j][d] = 0;
      counter[j] = 0;
    }
  }

/* -------------------------------------------------------------------------- */
  // Calculating Distances of each element for each cluster and each dimension

  for(i = 0; i < n; i++)
  {
    for(j = 0; j < k; j++)
    {
      for(d = 0; d < dim; d++)
      {
      distance[i][j] += ((X[i][d] - V[j][d])*(X[i][d] - V[j][d]));
      }
      distance[i][j] = sqrt(distance[i][j]);
    }
  }


/*-------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
      // Comparing Distances and getting the proper Cluster for each Element

      for(i = 0; i < n; i++)
      {
        min[i] = distance[i][0];
        for(j = 0; j < k; j++)
        {
          if(distance[i][j] <= min[i])
          {
            min[i] = distance[i][j];
            location[i] = j;
          }
        }
      }
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
      // Getting Elements and Clusters all together

      for(i = 0; i < n; i++)
      Cluster[i] = location[i];
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
      // Calculating total Sum of each dim for each cluster and counter
      for(i = 0; i < n; i++)
      {
        for(j = 0; j < k; j++)
        {
          if(Cluster[i] == j)
          {
            for(d = 0; d < dim; d++)
            {
              totalCluster[j][d] += X[i][d];
            }
            counter[j]++;
          }
        }
      }
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
    // Calculating New Centroid

    for(j = 0; j < k; j++)
    {
      for(d = 0; d < dim; d++)
      {
        V[j][d] = totalCluster[j][d]/counter[j];
      }
    }
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
			// Here goes the comparison last FlagCentroid with new centroids

      for(j = 0; j < k; j++)
      {
        for(d = 0; d < dim; d++)
        {
          if(FlagCentroids[j][d] != V[j][d])
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

 for(j = 0; j < k; j++)
 {
   for(d = 0; d < dim; d++)
   {
     FlagCentroids[j][d] = V[j][d];
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

for(j = 0 ; j < k ; j++)
   {
     char *fileName;
    fileName = calloc(n,sizeof(char));
     snprintf(fileName,n*sizeof(char),"C%d.txt",j);
      ClusterFile[j] = fopen(fileName,"w");
      free(fileName);
   }

for(j = 0; j < k; j++)
{
  for(i = 0; i < n; i++)
  {
    if(Cluster[i] == j)
    {
      for(d = 0; d < dim; d++)
      {
        fprintf(ClusterFile[j], "%lf ",X[i][d] );
      }
      fprintf(ClusterFile[j], "\n");
    }
  }
}

   for (j = 0; j < k; j++)
   fclose(ClusterFile[j]);

   for(j = 60 ; j >= k ; j--)
{
  char *fileName;
  fileName = calloc(n,sizeof(char));
  snprintf(fileName,n*sizeof(char),"C%d.txt",j);
   remove(fileName);
   free(fileName);
}

  for(i = 0; i < n; i++)
  free(X[i]);
  free(X);
  for(i = 0; i < k; i++)
  free(V[i]);
  free(V);
  free(Cluster);
  for(i = 0; i < k; i++)
  free(FlagCentroids[i]);
  free(FlagCentroids);
  free(counter);
  for(i = 0; i < k; i++)
  free(totalCluster[i]);
  free(totalCluster);
  for(i = 0; i < n; i++)
  free(distance[i]);
  free(distance);
  free(min);
  free(location);
  free(ClusterFile);
return 0;
}
