/* rand example: guess the number */
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>

using namespace std;
//using namespace cv;

int main ()
{
  int toss, iGuess;

  /* initialize random seed: */
  srand (time(NULL));

  /* generate secret number between 1 and 10: */
  
  int count ;
  int repeat=0;
  int val =0;
  int x[101];
  int var_count=0;
 int head[1000] = {0};
  while(repeat<1000)
  {
      count=0;
    while(count<100)
    {     toss = rand() % 2;
       // cout<<toss<<endl;
        if(toss==0)
        {
            head[repeat]++;
        }    
        count++;
        //cout<<head[repeat]<<endl;
    }
      if(head[repeat]<65 && head[repeat]>35)
      {    var_count++;}
      val = head[repeat]+val;
      x[head[repeat]]++;
       // cout<<head[repeat]<<"   "<<repeat<<endl;
        repeat++;
        
        
  }  
 int mark=0;
 cout<<val/1000<<endl;
 cout<<var_count<<endl;
 for(int i= 0;i<101;i++)
 {
     mark=0;
     while(mark<x[i])
     {
         cout<<"*";
         mark++;
     }    
     cout<<endl;
 }    
 
 
  return 0;
} 