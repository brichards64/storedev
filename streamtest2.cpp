#include <iostream>
#include <string>
#include <vector>
#include <BinaryStream2.h>


int main(){
  
  BinaryStream bs;
  if(!bs.Bopen("bstest",NEW,UNCOMPRESSED))std::cout<<"failed to open bs"<<std::endl;

  std::cout<<"d1"<<std::endl;
  
  int a=1;
  double b=2.3;
  std::string c="hello";
  std::vector<float> d;
  for (int i=0;i<10000; i++){
    d.push_back(1.1);
    d.push_back(2.2);
    d.push_back(3.3);
    d.push_back(4.4);
  }
  
  std::cout<<"d2"<<std::endl;
  
  BinaryStream e;
  if(!e.Bopen("bstest2",NEW,UNCOMPRESSED))std::cout<<"failed to open e"<<std::endl;
  int f=56;
  float g=55;
  
  std::cout<<"d3"<<std::endl;
  
  bs.m_write=true;
  e.m_write=true;
  
  std::cout<<"d3.2"<<std::endl;
  
  if(!(bs & a))std::cout<<"failed to write a"<<std::endl;
  if(!(bs & b))std::cout<<"failed to write a"<<std::endl;
  if(!(bs & c))std::cout<<"failed to write a"<<std::endl;
  if(!(bs & d))std::cout<<"failed to write a"<<std::endl;
  
  std::cout<<"d3.7"<<std::endl;

  for (int i=0; i<80;i++){  
    if(!(e & f))std::cout<<"failed to write f"<<std::endl;
    std::cout<<"d3.8"<<std::endl;
    if(!( e & g))std::cout<<"failed to write g"<<std::endl;
  }
  std::cout<<"d4"<<std::endl;

  if(!(bs & e))std::cout<<"failed to write e"<<std::endl;

  if(!  e.Bclose())std::cout<<"failed to close e"<<std::endl;
  if(!bs.Bclose())std::cout<<"failed to close bs"<<std::endl;
      
  std::cout<<"d5"<<std::endl;

  BinaryStream bs2;
  if(!bs2.Bopen("bstest",READ,UNCOMPRESSED))std::cout<<"failed to open bs2"<<std::endl;  
  bs2.m_write=false;
  bs2.m_pos=0;

  std::cout<<"d6"<<std::endl;

  int a2=0;
  double b2=0;
  std::string c2="";
  std::vector<float> d2;
  BinaryStream e2;
  if(!  e2.Bopen("bstest3",NEW,UNCOMPRESSED))std::cout<<"failed open e2"<<std::endl;
  int f2=0;
  float g2=0;
  
  
  e2.m_write=false;
  //bs.m_pos=0;
  
  //  bs.Bopen("bstest","READ",1);

  if(!(  bs2 & a2))std::cout<<"failed to read a2"<<std::endl;
  if(!(bs2 & b2))std::cout<<"failed to read b2"<<std::endl;
  if(!(bs2 & c2))std::cout<<"failed to read c2"<<std::endl;
  if(!(bs2 & d2))std::cout<<"failed to read d2"<<std::endl;
  
  std::cout<<"a2="<<a2<<std::endl;
  std::cout<<"b2="<<b2<<std::endl;
  std::cout<<"c2="<<c2<<std::endl;
  std::cout<<"d2[0]="<<d2[0]<<std::endl;
  std::cout<<"d2[1]="<<d2[1]<<std::endl;
 
  if(!(  bs2 & e2))std::cout<<"failed to read e2"<<std::endl;
  //  e2.m_write=false;
  //e2.m_pos=0;
  
  if(!(  e2 & f2))std::cout<<"failed to read f2"<<std::endl;
  std::cout<<"f2="<<f2<<std::endl;
  if(!( e2 & g2))std::cout<<"failed to read g2"<<std::endl;
  std::cout<<"g2="<<g2<<std::endl;
  
  if(!(  e2.Bclose()))std::cout<<"failed to close e2"<<std::endl;
  if(!(  bs2.Bclose()))std::cout<<"failed to close bs2"<<std::endl;
  
  return 0;
  
}
