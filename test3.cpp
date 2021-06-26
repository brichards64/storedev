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
#include <BinaryStream2.h>

class mydata: public SerialisableObject{

public:

  int a;
  double b;
  std::string c;
  std::vector<float> d;
  
  bool Print(){return true;}
  std::string GetVersion(){return "test";}
  bool Serialise(BinaryStream &bs){

    bs & a;
    bs & b;
    bs & c;
    bs & d;

    return true;
  }

};


int main (){
  
  /*
  BinaryStream bs;
  bs.Bopen("appendtest",READ_APPEND,UNCOMPRESSED);
  for(unsigned int i=0; i<10000; i++){
  int b=i;
  std::cout<<"before="<<bs.Btell()<<std::endl;
  bs.Bseek(0,SEEK_SET);
  bs<<b;
  std::cout<<"after="<<bs.Btell()<<std::endl;
  }
  bs.Bclose();
  */
  std::vector<int> stufftest;

  
  BStore store;
  std::cout<<"e0"<<std::endl;
  store.Initnew("storedata");
  std::cout<<"e0.1"<<std::endl;
  
  /*
    for(unsigned int i=0; i<5; i++){
    int a=i;
    store.Set("a",a);
    a=0;
    store.Get("a",a);
    std::cout<<"a="<<a<<std::endl;
    store.Save();
    }
    store.Close();
  */
  

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

    std::cout<<"e4"<<std::endl;
    
    float* f=new float;
    *f=888;
    std::cout<<"e5"<<std::endl;
    store.Set("f",f);
    std::cout<<"e6"<<std::endl;
   //    store.m_variables["c"]=tmp.str().c_str();
    //    store.test2["c"]=tmp.str().c_str();
    //  store.Header[tmp.str().c_str()]=tmp.str().c_str();
    std::cout<<"e7"<<std::endl;
    store.Save();
    std::cout<<"e8"<<std::endl;
    store.Delete();
    std::cout<<"e9"<<std::endl;
  }
  std::cout<<"e10"<<std::endl;
  std::cout<<"e10.5 store close="<<store.Close()<<std::endl;
  std::cout<<"pfile state="<<store.output.pfile<<std::endl; 
  std::cout<<"e11"<<std::endl;
  
  std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! part 2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;  

  




  
  BStore store2;
  std::cout<<"e12"<<std::endl;
  store2.Initnew("storedata");
  std::cout<<"e13"<<std::endl;
  std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! init end !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;  
  for(unsigned int i=0; i<store2.m_lookup.size(); i+=1){
    std::cout<<"lookup["<<i<<"] : "<<store2.m_lookup[i]<<std::endl;
  }

  for(unsigned int i=0; i<store2.m_lookup.size()-1; i+=1){
    std::cout<<"entry["<<i<<"] : "<<store2.m_lookup.size()<<std::endl;    
    store2.GetEntry(i);
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! get entry end !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    std::cout<<"entry["<<i<<"]"<<std::endl;
    
    //std::cout<<"testsize="<<store2.test.size()<<std::endl;  
    //std::cout<<"a="<<store2.test["a"]<<std::endl;
    //std::cout<<"b="<<store2.test["b"]<<std::endl;
    int a=0;
    double b=3;
    std::string c="";
    std::vector<float> d;
    std::cout<<"store.size="<<store2.m_variables.size()<<std::endl;
    store2.Get("a",a);
    store2.Get("b",b);
    stufftest.push_back(a);
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

  std::cout<<"did it close?="<<store2.Close()<<std::endl;
  std::cout<<"pfile state="<<store2.output.pfile<<std::endl;  
  //   store2.GetHeader();
  // for(unsigned int i=0; i<10; i+=1){
  
  //std::stringstream tmp;
  // tmp<<i;
  // std::cout<<store2.Header[tmp.str().c_str()]<<std::endl;
  
  //}
  //    store2.Print(true);

  /*  
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
  */

  for(int i=0;i<stufftest.size();i++){
    std::cout<<"a["<<i<<"]="<<stufftest.at(i)<<std::endl;
  }


   return 0;

}

