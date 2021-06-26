#include <iostream>
#include <string>
#include <vector>
#include <BinaryStream.h>


int main(){


  BinaryStream bs;
  bs.Bopen("bstest","WRTIE",1);

  int a=1;
  double b=2.3;
  std::string c="hello";
  std::vector<float> d;
  d.push_back(1.1);
  d.push_back(2.2);
  d.push_back(3.3);
  d.push_back(4.4);
  BinaryStream e;
  e.Bopen("bstest2","WRTIE",1);
  int f=56;
  float g=55;

  bs & a;
  bs & b;
  bs & c;
  bs & d;

  e & f;
  e & g;

  bs & e;

  //e.Bclose();
  bs.Bclose();

  BinaryStream bs2;
  bs2.Bopen("bstest","READ",1);  
  bs2.m_write=false;
  bs2.m_pos=0;

  int a2=0;
  double b2=0;
  std::string c2="";
  std::vector<float> d2;
  BinaryStream e2;
  e2.Bopen("bstest3","WRTIE",1);
  int f2=0;
  float g2=0;

  
  e2.m_write=false;
  //bs.m_pos=0;
  
  //  bs.Bopen("bstest","READ",1);

  bs2 & a2;
  bs2 & b2;
  bs2 & c2;
  bs2 & d2;

  std::cout<<"a2="<<a2<<std::endl;
  std::cout<<"b2="<<b2<<std::endl;
  std::cout<<"c2="<<c2<<std::endl;
  std::cout<<"d2[0]="<<d2[0]<<std::endl;
  std::cout<<"d2[1]="<<d2[1]<<std::endl;
 
  bs2 & e2;
  //  e2.m_write=false;
  //e2.m_pos=0;
  
  e2 & f2;
  std::cout<<"f2="<<f2<<std::endl;
  e2 & g2;
  std::cout<<"g2="<<g2<<std::endl;
  
  return 0;
  
}
