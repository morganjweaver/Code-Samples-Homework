#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <string>

using namespace std;

 struct vowelCount{
int a;
int e;
int i;
int o;
int u;
 } globalVowels;

pthread_mutex_t  vowLock;
int * ReturnVowels(string x);
void* ThreadFunc(void * arg);

int main(int argc, char *argv[])
{
	globalVowels.a = 0;
	globalVowels.e = 0;
	globalVowels.i = 0;
	globalVowels.o = 0;
	globalVowels.u = 0;

  if (argc != 2) // argc should be 2 for correct execution
	// We print argv[0] assuming it is the program name
	{
	  cout << "Please try again with one argument.\n";
	  return -1;
	}
  string dirName = argv[1]; //get user's desired directory
  pthread_t threads[20]; //create thread array
  int threadling;
  int IDCounter = 0;
  
  DIR *dp;
  struct dirent *dirp;
  dp = opendir( dirName.c_str()); //try to open the directory by assigning dp

  if (dp == NULL) //opendir result
  {
    cout << "Error opening " << dirName << endl;
    return 0;
  }
  
  while ((dirp = readdir( dp ))) //pointer to readdir
  {
        string *filepath = new string(dirName + "/" + dirp->d_name);
	if (dirp->d_name[0] == '.')
	  {continue;}
	
	//	cout << "Opening: " << *filepath << endl;
	threadling = pthread_create(&threads[IDCounter], NULL, ThreadFunc, filepath);
	
	IDCounter++;
//threadling should return 0
      if (threadling){
         cout << "Error:unable to create thread," << threadling << endl;
         exit(-1);
      }
  }
  closedir( dp );
  for(int k = 0; k<IDCounter; k++)
  {
    pthread_join(threads[k], NULL);
  }

  cout << "Vowel Count: \n" << "a's: " << globalVowels.a << "\ne's: " << globalVowels.e << "\ni's: " << globalVowels.i << "\no's: " << globalVowels.o << "\nu's: " << globalVowels.u << endl;
  return 0;
}

int * ReturnVowels(string x){
	int * vowCount = new int[5];
	for(int i = 0; i<5; i++)
	{
		vowCount[i] = 0;
	}

	for (int j = 0; j<(int)x.size(); j++){
		if(x[j] == 'a' || x[j] == 'A')
		    vowCount[0]++;
		else if (x[j] == 'e' ||x[j]== 'E')
			vowCount[1]++;
		else if (x[j] == 'i' ||x[j]== 'I')
			vowCount[2]++;
		else if (x[j] == 'o' ||x[j]== 'O')
			vowCount[3]++;
		else if (x[j] == 'u' ||x[j]== 'U')
			vowCount[4]++;
	}

	return vowCount;
}

void * ThreadFunc(void * arg){
  string filename = * (string*) arg;
  string currentLine;
  // cout << "Filename: " << filename << endl;
	 int * localVowels = new int[5];
	 int * vowels;
   	 ifstream file(filename.c_str());

   	 for(int i = 0; i<5; i++)
   	 {
   	 	localVowels[i] = 0;
   	 }
   	
	 if(file.good()){
	   
	   file >> currentLine;
	   
	  while (file.good())//while there are remaining characters...
		{
		  
		  // cout << currentLine << endl;
		  vowels = ReturnVowels(currentLine); //returns pointer to int array
		  localVowels[0]+= vowels[0];
		  localVowels[1]+= vowels[1];
		  localVowels[2]+= vowels[2];
		  localVowels[3]+= vowels[3];
		  localVowels[4]+= vowels[4];
		  file >> currentLine;
		 
		}
		//now the thread's vowel count for the file is complete.
	}
	  file.close();
	  
	  //lock the global vowel count before updating

	  pthread_mutex_lock(&vowLock);


	  globalVowels.a += localVowels[0];
	  globalVowels.e += localVowels[1];
	  globalVowels.i += localVowels[2];
	  globalVowels.o += localVowels[3];
	  globalVowels.u += localVowels[4];
	  
	  pthread_mutex_unlock(&vowLock);
	  return 0;
	  
}
