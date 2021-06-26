#include <iostream>
#include <stdio.h>
#include <map>
#include <string>
#include "zlib.h"
#include <unistd.h> //for lseek
#include <sstream>
#include <assert.h>
#include <BStore.h>


int main ()
{
  
  
  BStore store;
  store.Init();

  for(unsigned int i=0; i<10; i++){
    //    store.test["a"]="a";
    //  std::cout<<"a="<<store.test["a"]<<std::endl;
    // store.test["b"]="b";
    std::stringstream tmp;
    tmp<<i;
    store.test["c"]=tmp.str().c_str();
    store.test2["c"]=tmp.str().c_str();
    store.Header[tmp.str().c_str()]=tmp.str().c_str();
    store.Save();
  }

  store.Close();
  
  BStore store2;
  store2.Init2();


  for(unsigned int i=0; i<10; i+=2){
    store2.GetEntry(i);
    //std::cout<<"testsize="<<store2.test.size()<<std::endl;  
    //std::cout<<"a="<<store2.test["a"]<<std::endl;
    //std::cout<<"b="<<store2.test["b"]<<std::endl;
    std::cout<<"c="<<store2.test["c"]<<std::endl;
  //  FILE * pFile;
  // char buffer[] = { 'x' , 'y' , 'z' };
  //pFile = fopen ("myfile.bin", "wb+");
  //fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
  //fclose (pFile);
  }

   store2.GetHeader();
    for(unsigned int i=0; i<10; i+=1){

    std::stringstream tmp;
    tmp<<i;
    std::cout<<store2.Header[tmp.str().c_str()]<<std::endl;

    }

  return 0;
}

