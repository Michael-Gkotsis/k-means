#include <time.h>
#include <math.h>
#include <stdlib.h>

int *zero1D(int *,int);
float *zero2D(float *,int , int);
float *getRandomCentroids(float *,int , int ,int , float *);
float *copyInitialCentroids(float *,int, int , float *);
float *getDistances(float *,float *,int ,int ,int ,float *);
int *getLocation(float *,int , int , int *);
float *getTotalSum(float *,int *,int ,int ,int ,float *);
int *getCounter(int *,int ,int ,int *);
float *getNewCentroids(float *, int *,int ,int , float *);
int checkEquality(float *,float *,int ,int );

int *zero1D(int *Array,int size)
{
  for(unsigned int i = 0; i < size; ++i)
  {
    Array[i] = 0;
  }
  return Array;
}

float *zero2D(float *Array,int n,int dim)
{
  for(unsigned int i = 0; i < n; ++i)
  {
    for(unsigned int j = 0; j < dim; ++j)
    {
      Array[i*dim + j] = 0;
    }
  }
  return Array;
}

float *getRandomCentroids(float *Elements,int n, int dim,int k, float *Centroids)
{

  for(unsigned int j = 0; j < k; ++j)
  {
    unsigned int i = rand() % n;
     for(unsigned int d = 0; d < dim; ++d)
     Centroids[j*dim + d] = Elements[i*dim + d];
  }
  return Centroids;
}


float *copyInitialCentroids(float *Centroids,int dim,int k,float *FlagCentroids)
{
  for (unsigned int j = 0; j < k; ++j)
      for (unsigned int d  = 0; d < dim; ++d)
          FlagCentroids[j*dim + d] = Centroids[j*dim + d];

          return FlagCentroids;
}


float *getDistances(float *Elements,float *Centroids,int n,int k,int dim,float *Distance)
{
   for(unsigned int i = 0; i < n; ++i)
   {
     for(unsigned int j = 0; j < k; ++j)
     {
       Distance[i*k + j] = 0;
       for(unsigned int d = 0; d < dim; ++d)
       {
         Distance[i*k + j] +=((Elements[i*dim + d] - Centroids[j*dim + d])*(Elements[i*dim + d] - Centroids[j*dim + d]));
       }
       Distance[i*k + j] = sqrt(Distance[i*k + j]);
     }

   }


  return Distance;
}


int *getLocation(float *Distance,int n, int k, int *Location)
{
  float *Min;
  Min = (float*)calloc(n,sizeof(float));

  for(unsigned int i = 0; i < n; ++i)
  {
    Min[i] =  Distance[i*k + 0];
    for(unsigned int j = 0; j < k; ++j)
    {
      if(Distance[i*k + j] <= Min[i])
      {
        Min[i] = Distance[i*k + j];
        Location[i] = j;
      }
    }
  }
  free(Min);
  return Location;
}


float *getTotalSum(float *Elements,int *Location,int n, int k, int dim, float *ClusterTotalSum)
{

  for(unsigned int i = 0; i < n; ++i)
  {
    for(unsigned int j = 0; j < k; ++j)
    {
      if(Location[i] == j)
      {
        for(unsigned int d = 0; d < dim; ++d)
        ClusterTotalSum[j*dim + d] += Elements[i*dim + d];
      }
    }
  }
  return ClusterTotalSum;
}

int *getCounter(int *Location, int n, int k, int *Counter)
{
  for(unsigned int i = 0; i < n; ++i)
  {
    for(unsigned int j = 0; j < k; ++j)
    {
      if(Location[i] == j)
      Counter[j]++;
    }
  }
  return Counter;
}

float *getNewCentroids(float *ClusterTotalSum,int *Counter,int k, int dim,float *Centroids)
{
  for(unsigned int j = 0; j < k; ++j)
  {
    for(unsigned int d = 0; d < dim; ++d)
    {
      Centroids[j*dim + d] = ClusterTotalSum[j*dim + d]/Counter[j];
    }
  }
  return Centroids;
}


int checkEquality(float *FlagCentroids,float *Centroids,int k,int dim)
{
  int flag;

  for(unsigned int j = 0; j < k; ++j)
    {
      for(unsigned int d = 0; d < dim; ++d)
      {
        if(FlagCentroids[j*dim + d] != Centroids[j*dim + d])
        {
          flag = 0;
          return flag;
        }else
        {
          flag = -1;
          return flag;
        }
      }
    }
}
