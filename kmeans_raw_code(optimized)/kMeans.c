#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 100
#define FLAG_MAX 9999
#define PREVIOUS_FILES 100
/*Sorry for Caps */
/*CURRENT VERSION WORKS WITHOUT HEADER FILES AND FUNCTIONS, IT'S WRITTEN THIS WAY
FOR EASE OF USING AND READING OF RAW CODE*/

int main(int argc, char const **argv) {

    /* Starting Variables */
    int N = -1; /*Size of data|| Initializing at -1 because our finder always count the last \n*/
    unsigned int K = 0; /* Amount of clusters to be created,input given by the user */
    unsigned int D = 0; /* Number of features(dimensions) */
    srand(time(NULL));/*True Random*/
    unsigned int randVar = 0;/*Variable for picking random initial Centroids*/
    unsigned int iteration = 0;/*Amount of algorithm's iterations counter*/
    int flagEnd = 0;/*Initializing flag variable for do-while break*/
    clock_t start, end; //Timers

    /*(loop var) i is used for N, j is used for K,d is used for D */
    register int i; //Elements
    register int j; //Clusters
    register int d; //Features

    /*-----------------------------*/
    char filename[MAX_SIZE];/* Holder for Dataset Filename */
    unsigned int choise = 0; /*Flag variable for filename(given by the user) error check */
    /*-----------------------------*/

    FILE *Dataset; /*Stating Dataset file Variable */

    /* Asking Data File from user */
    printf("\n Input Dataset filename : ");
    scanf("%s",filename);
    printf("\n");
    /*------------Filename Error Check-----------------*/
    /*access function checks if the file exists,if we get -1 it means it doesnt exists*/
    if(access(filename, F_OK) == -1)
    {
     /*Informs the user about the error with the filename and initiates the loop */
      printf(" There is something wrong with the File\n");
      while(choise == 0)
      {
        printf("\n Input New File(Type 0 to Exit): ");
        scanf("%s",filename);
        printf("\n");
        if(strcmp(filename,"0") == 0)/*If users types 0 at 2nd attempt exits program*/
          {
            printf("\n Program has been terminated\n\n");
            return -1;
          }else if(access(filename, F_OK) == -1)/* >2 attempt asking till user types correctly or press 0 to exit*/
          {
            choise = 0;/*Continue with the asking filename loop*/
          }else
          {
            choise = 1;/*User typed the correct file and breaks while-loop normally*/
          }
      }


    }
    /*-----------------------------*/
    /*Opening the file after error checking */
    Dataset = fopen(filename,"r");


    /*------------Finding the number of features from the dataset -----------------*/


    int flag = 0;/*temp flag */
    int flagPrev = 0;/*Holder of previous iteration's flag */
    int counter = 0;/*switching between 0-1,necessary for the loop bellow*/
    char c;/*Temp char for file scaning*/

    /*It increases D by 1 everytime counter == 0 and flagPrev != flag,the way
     it works is by checking first's row characters one by one, if it's a number(0-9),
     operation sign(+,-) or a dot(.) the sets flags = 1 and increases counter by 1.
     If it's something else(space( ),tab(\t),comma(,) etc) sets both to zero.
     With the correct combination D increases by 1 every time we pass through a new feature*/
     while(c != '\n')
      {
        fscanf(Dataset, "%c", &c);
        if((c >= '0' && c <= '9') ||( c == '-' || c == '+' ) || (c == '.' ))
        {
           flag = 1;
           counter++;
        } else
        {
           flag = 0;
           counter = 0;
        }
        if(counter == 0 && flagPrev != flag)
        {
           ++D;
        }
        flagPrev = flag;
      }
      rewind(Dataset);/* sets the pointer at the begining of the file*/

    /*----------Finding the number of elements-------------------*/
    while(!feof(Dataset))
        {
           fscanf(Dataset,"%c",&c);
           if(c == '\n')
              ++N;
        }
    rewind(Dataset);/* sets the pointer at the begining of the file*/
        /*-----------------------------*/
    /*---Printing size of Data and number of features(speaks for itself though)---*/
    printf("\n Size of Data : %d\n",N);
    printf("\n Number of features : %d\n",D);

    /*---Getting K from user----*/

    /*Asking the user to give the amount of clusters */
    printf("\n Give Clusters(k > 0 required) : ");
    scanf("%d",&K);
    printf("\n");

    /* Error check statement,cant run with K <= 0 */
    if(K <= 0)
    {
      printf("\n Can't be executed with K = %d!(k > 0 required)\n",K);
      printf("\n Program has been terminated\n\n");
      return -2;
    }
    /*-----------------------------*/

    /*------Memory Allocation-----*/
    float *DataArray;//Main Array for loading the initial Data
    DataArray = (float*)calloc(N*D,sizeof(float));/*Allocating space for N(rows) * D(features) */

    float *Centroids;/*Array holding Centroids throughout the execution of the algorithm*/
    Centroids = (float*)calloc(K*D,sizeof(float));/*Allocating space for K(clusters) * D(features) */

    float *FlagCentroids;/*Array for holding Centroids of a previous iteration,used in loop testing */
    FlagCentroids = (float*)calloc(K*D,sizeof(float));/*Same allocation as line:120*/

    int *Counter;/*Array for holding the counter about how many elements each cluster has */
    Counter = (int*)calloc(K,sizeof(int));/*Allocating space the same as K(clusters)*/

    float *ClusterTotalSum;/*Array for holding the total sum of each cluster*/
    ClusterTotalSum = (float*)calloc(K*D,sizeof(float));/*Allocating space for K(rows) and D(features)*/

    float *Distance;/*Array for holding the distance between each element from each Centroid*/
    Distance = (float*)calloc(N*K,sizeof(float));/*Allocating space for N(elements) * K(Centroids)*/

    float *Min;/*Array for holding the min Distance*/
    Min = (float*)calloc(N,sizeof(float));

    int *Location;/*Array holding each element's location(cluster)*/
    Location = (int*)calloc(N,sizeof(int));


    /*--------------------------*/

    /*---------Loading Data---------*/
    for(i = 0; i < N; ++i)
    {
      for(d = 0; d < D; ++d)
        fscanf(Dataset,"%f,",&DataArray[i*D + d]);/*"%f," is c undefined behaviour */
    }
    fclose(Dataset);//Closing the initial Data File
    /*--------------------------*/

    /*--------Generating Initial Random Centroids-----*/
    for(j = 0; j < K; ++j)
    {
      randVar = rand() % N;//Getting a different rand for every Centroid
      for(d = 0; d < D; ++d)
      {
        //Picking a random Element each iteration till < K for Centroid
        Centroids[j*D + d] = DataArray[randVar*D + d];
        //Copy Centroids[] to FlagCentroids[]
        FlagCentroids[j*D + d] = Centroids[j*D + d];
      }
    }
      /*--------------------------*/

start = clock();
  /*--Initializing the algoritm---*/
    do {

      /*For every iteration after the initial one resets the counter and sums to 0*/
      if(iteration > 0)
      {
        for(j = 0; j < K; ++j)
        {
          Counter[j] = 0;//Resets counter array to 0
          for(d = 0; d < D; ++d)
          {
            ClusterTotalSum[j*D + d] = 0;//Resets Sum of each cluster to 0
          }
        }
      }

      for(i = 0; i < N; ++i)
      {
        Min[i] = FLAG_MAX;
        for(j = 0; j < K; ++j)
        {
          Distance[i*K + j] = 0;/*Reseting Distance at every iteration to 0 to calculate new ones */
          for(d = 0; d < D; ++d)
          {
            /*Calculating distance for each element from each centroid by using sqrt(pow((x-y),2))*/
            Distance[i*K + j] +=((DataArray[i*D + d] - Centroids[j*D + d])*(DataArray[i*D + d] - Centroids[j*D + d]));
          }
          Distance[i*K + j] = sqrt(Distance[i*K + j]);/*Getting the sqrt of the total features distance*/

         /*Everytime it finds a distance Smaller than previous it stores it's cluster location*/
          if(Distance[i*K + j] < Min[i])
          {
            Min[i] = Distance[i*K + j];
            Location[i] = j;
          }
        }
  /*For every element's  j(current Cluster)==location add it to Cluster's total sum
  and increase counter by 1 for the corresponding cluster */
        for(j = 0; j < K; ++j)
        {
          if(Location[i] == j)
          {
            for(d = 0; d < D; ++d)
            {
              ClusterTotalSum[j*D + d] += DataArray[i*D + d];
            }
            ++Counter[j];
          }
        }
      }

 /*Calculate new Centroids by dividing each feature sum with Counter */
      for(j = 0; j < K; ++j)
      {
        for(d = 0; d < D; ++d)
        {
          Centroids[j*D + d] = ClusterTotalSum[j*D +d]/Counter[j];
        }
      }

/*If even one feature of flag is different than the equal to Centroids
it set's flagEnd to 0 and breaks the nested for loop and follows with an if to
break the parent for loop. If all features are equal then set flagEnd to -1 which
breaks the do-while loop */
      for(j = 0; j < K; ++j)
      {
        for(d = 0; d < D; ++d)
        {
          if(FlagCentroids[j*D + d] != Centroids[j*D + d])
          {
            flagEnd = 0;
             break;
          }else
          {
            flagEnd = -1;
          }
          
        }

        if(flagEnd == 0)
          break;
      }


/*Copy new Centroids to FlagCentroids */
      for(j = 0; j < K; ++j)
      {
        for(d = 0; d < D; ++d)
        {
          FlagCentroids[j*D + d] = Centroids[j*D + d];
        }
      }

      ++iteration;

    } while(flagEnd != -1);

    end = clock();

    printf("\n Iterations : %d\n",iteration );
    double total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n Time of Algorithm Execution: %lf \n\n",total_time);

    FILE **OutputArray;
    OutputArray = calloc(K,sizeof(FILE*));
    char fileName[MAX_SIZE];

    for(j = 0; j < K; ++j)
    {
      sprintf(fileName,"Cluster_%d.txt",j);
      OutputArray[j] = fopen(fileName,"w");
    }

    for(j = 0; j < K; ++j)
    {
      for(i = 0; i < N; ++i)
      {
        if(Location[i] == j)
        {
          for(d = 0; d < D; ++d)
          {
            fprintf(OutputArray[j],"%f ",DataArray[i*D + d]);
          }
          fprintf(OutputArray[j],"\n");
        }
      }
    }

    for(j = 0; j < K; ++j)
    {
      fclose(OutputArray[j]);
    }

    for(j = PREVIOUS_FILES; j >= K; --j)
    {
      sprintf(fileName,"Cluster_%d.txt",j);
      remove(fileName);
    }




    free(DataArray);
    free(Centroids);
    free(FlagCentroids);
    free(Counter);
    free(ClusterTotalSum);
    free(Distance);
    free(Min);
    free(Location);
    free(OutputArray);

  return 0;
}
