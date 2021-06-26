#include <iostream>
#include <stdio.h>
#include <string.h>

int main(){

  int a=4;
  double b=5.3;
  std::string c="hello world";
  
  char* tmp;
  
  std::cout<<"a "<<sizeof(a)<<std::endl;
  std::cout<<"b "<<sizeof(b)<<std::endl;
  std::cout<<"c "<<sizeof(c)<<std::endl;

  tmp=new char[sizeof(a)];
  tmp=reinterpret_cast<char*>(&a);
  
  std::cout<<"tmp "<<sizeof(tmp)<<" "<<tmp<<std::endl;
  a=40;
  // a=*reinterpret_cast<int>(tmp);
  memcpy(&a,tmp,sizeof(a));
 
 std::cout<<"a "<<sizeof(a)<<std::endl;
  return 0;

}
