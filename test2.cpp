#include <iostream>
#include <stdio.h>
#include <map>
#include <string>
#include "zlib.h"
#include <unistd.h> //for lseek
#include <sstream>
#include <assert.h>
#include <BStore.h>
#include <SerialisableObject.h>
#include <archive.h>

class mydata: public SerialisableObject{

public:

  int a;
  double b;
  std::string c;
  std::vector<float> d;

  bool Print(){;}
  void Serialise(archive &ar){

    ar & a;
    ar & b;
    ar & c;
    ar & d;

  }

};


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
    int a=i*10;
    double b=3253.36;
    std::vector<float> d;
    for(int j=0; j<5; j++){
      d.push_back(j*3.31);
    }
    std::cout<<"e1"<<std::endl;
    store.Set("c",tmp.str());
    std::cout<<"e2"<<std::endl;
    store.Set("a",a);
    a=9;
    store.Get("a",a);
    std::cout<<"e3 a="<<a<<std::endl;
    store.Set("b",b);
    std::cout<<"e4"<<std::endl;
    store.Set("d",d);
    std::cout<<"e4"<<std::endl;
 
    mydata test;
    test.a=3;
    test.b=345.678;
    test.c="hello world";
    test.d.push_back(56);
    test.d.push_back(23.45);
    store.Set("e",test);
 
    float* f=new float;
    *f=888;
    store.Set("f",f);
   //    store.m_variables["c"]=tmp.str().c_str();
    //    store.test2["c"]=tmp.str().c_str();
    //  store.Header[tmp.str().c_str()]=tmp.str().c_str();
    store.Save();
    store.Delete();
  }

  store.Close();
  
  BStore store2;
  store2.Init2();


  for(unsigned int i=0; i<10; i+=1){
    store2.GetEntry(i);
    //std::cout<<"testsize="<<store2.test.size()<<std::endl;  
    //std::cout<<"a="<<store2.test["a"]<<std::endl;
    //std::cout<<"b="<<store2.test["b"]<<std::endl;
    int a=0;
    double b=3;
    std::string c="";
    std::vector<float> d;
    store2.Get("a",a);
    store2.Get("b",b);
    std::cout<<"a="<<a<<std::endl;
    std::cout<<"b="<<b<<std::endl;
    store2.Get("c",c);
    std::cout<<"c="<<c<<std::endl;
    store2.Get("d",d);
    for(int j=0;j<d.size();j++){
    std::cout<<"d[j]="<<d[j]<<std::endl;
    }
    mydata test;
    store2.Get("e",test);
    std::cout<<"e.a="<<test.a<<std::endl;
    std::cout<<"e.b="<<test.b<<std::endl;
    std::cout<<"e.c="<<test.c<<std::endl;
    std::cout<<"e.d[0]="<<test.d[0]<<std::endl;
    std::cout<<"e.d[1]="<<test.d[1]<<std::endl;
    //    float f=0;
    // store2.Get("f",f);
    // std::cout<<"f="<<f<<std::endl;
    float* f2= new float;
    store2.Get("f",f2);
    std::cout<<"*f="<<*f2<<std::endl;
    int* a2= new int;
    store2.Get("a",a2);
    std::cout<<"*a="<<*a2<<std::endl;
    //    std::cout<<"c="<<store2.m_variables["c"]<<std::endl;
    //  FILE * pFile;
    // char buffer[] = { 'x' , 'y' , 'z' };
    //pFile = fopen ("myfile.bin", "wb+");
    //fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
    //fclose (pFile);
  }
  
  //   store2.GetHeader();
  // for(unsigned int i=0; i<10; i+=1){
  
  //std::stringstream tmp;
  // tmp<<i;
  // std::cout<<store2.Header[tmp.str().c_str()]<<std::endl;
  
  //}
  //    store2.Print(true);
  
  int a=3;
  double b=6.9;
  std::string c="1234";

  archive br;

  for(int i=0; i<1000; i++){
    br & a;
    br & b;
    br & c;
  }

  br.Save("testarchive");

  a=0;
  b=0;
  c="";

  archive cr;

  cr.Load("testarchive");

  cr & a;
  cr & b;
  cr & c;

  std::cout<<"a="<<a<<std::endl;
  std::cout<<"b="<<b<<std::endl;
  std::cout<<"c="<<c<<std::endl;

  return 0;
}

