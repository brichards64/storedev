#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>

class archive: public std::string {


public: 

  bool out;

  void operator&(std::string& rhs){

    if(!out){
      unsigned int tmp=rhs.length();
      (*this) & tmp;
      this->resize(this->length()+tmp);
      memcpy(&((*this)[this->length()-tmp]), &rhs[0], tmp);
    }

    else{
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      memcpy(&(rhs[0]), &((*this)[0]), tmp);
      this->erase(0,tmp);
    }

  }

  template<typename T> void operator&(T& rhs){
    
    if(!out){    
      unsigned int tmp=this->length();
      this->resize(tmp+sizeof(T));
      memcpy(&((*this)[tmp]), &rhs, sizeof(T));
    }
    
    else{
      memcpy(&rhs, &((*this)[0]), sizeof(T));
      this->erase(0,sizeof(T));
    }
  }

  template<typename T> void operator&(const T& rhs){

    if(!out){
      unsigned int tmp=this->length();
      this->resize(tmp+sizeof(T));
      memcpy(&((*this)[tmp]), &rhs, sizeof(T));
    }

  }

  template<typename T> void operator&(std::vector<T>& rhs){

    if(!out){
      unsigned int tmp=rhs.size();
      (*this) & tmp;
      this->resize(this->length()+(tmp*sizeof(T)));
      memcpy(&((*this)[this->length()-(tmp*sizeof(T))]), &(rhs[0]), tmp*sizeof(T));
    }

    else{
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      memcpy(&(rhs[0]), &((*this)[0]), sizeof(T)*tmp);
      this->erase(0,sizeof(T)*tmp);
    }
  }

  void operator&(std::vector<std::string>& rhs){

    if(!out){
      unsigned int tmp=rhs.size();
      (*this) & tmp;
      for(int i=0; i<tmp; i++){
	(*this) & rhs.at(i);
      }
    }

    else{
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      for(int i=0; i<tmp; i++){
        (*this) & rhs.at(i);
      }
    }
  }
 
  template<typename T, typename U> void operator&(std::map<T,U>& rhs){

    if(!out){
      unsigned int tmp=rhs.size();
      (*this) & tmp;
      for (typename std::map<T,U>::iterator it=rhs.begin(); it!=rhs.end(); ++it){
	(*this) & it->first;
	(*this) & it->second;
      }

     }

    else{
      unsigned int tmp=0;
      (*this) & tmp;
      for (int i=0; i<tmp; i++){
	T key;
	U value;
        (*this) & key;
        (*this) & value;
	rhs[key]=value;
      }

    }
  }


  /*
  // friends defined inside class body are inline and are hidden from non-ADL lookup
  friend X operator&(X lhs,        // passing lhs by value helps optimize chained a+b+c
                     const X& rhs) // otherwise, both parameters may be const references
  {
    lhs += rhs; // reuse compound assignment
    return lhs; // return the result by value (uses move constructor)
  }
  */

};


int main(){
  
  int a=56;
  double b= 435543;
  std::string c="hello";
  std::vector<int> d;
  std::vector<std::string> e;
  char f='h';
  char g[5]= {'h','e','l','l','o'};
  int h[5]= {2,4,6,8,10};
  char* i= new char[5];
  i[0]='w';
  i[1]='o';
  i[2]='r';
  i[3]='l';
  i[4]='d';
  int *j= new int; //wont work
  *j=42; //wont work
  std::map<int,float> k;
  std::map<int,std::string> l;

  for(int i=0; i<10; i++){
    d.push_back(i);
    std::stringstream tmp;
    tmp<<i;
    e.push_back(tmp.str());
    k[i]=(double)(i*100);
    l[i]=tmp.str();
  }


  archive test;
  test.out=false;

  
  test & a;

  std::cout<<test.length()<<std::endl;

  test & b;

  std::cout<<test.length()<<std::endl;

  test & c;

  std::cout<<test.length()<<std::endl;

  test & d;

  std::cout<<test.length()<<std::endl;

  test & e;

  std::cout<<test.length()<<std::endl;

  test & f;

  std::cout<<test.length()<<std::endl;

  test & g;

  std::cout<<test.length()<<std::endl;

  test & h;

  std::cout<<test.length()<<std::endl;

  test & i;

  std::cout<<test.length()<<std::endl;

  test & j;

  std::cout<<test.length()<<std::endl;

  test & k;

  std::cout<<test.length()<<std::endl;

  test & l;

  std::cout<<test.length()<<std::endl;

  test.out=true;

  a=0;
  b=0;
  c="";
  d.clear();
  e.clear();
  f=' ';
  g[0]=' ';
  g[1]=' ';
  g[2]=' ';
  g[3]=' ';
  g[4]=' ';
  h[0]=0;
  h[1]=0;
  h[2]=0;
  h[3]=0;
  h[4]=0;
  i[0]=' ';
  i[1]=' ';
  i[2]=' ';
  i[3]=' ';
  i[4]=' ';
  *j=0;
  k.clear();
  l.clear();

  test & a;
  std::cout<<"a="<<a<<std::endl;

  test & b;
  std::cout<<"b="<<b<<std::endl;

  test & c;
  std::cout<<"c="<<c<<std::endl;

  test & d;

  for(int i=0; i<10; i++){
    std::cout<<"d["<<i<<"]="<<d.at(i)<<std::endl;
  }

  test & e;

  for(int i=0; i<10; i++){
    std::cout<<"e["<<i<<"]="<<e.at(i)<<std::endl;
  }

  test & f;
  std::cout<<"f="<<f<<std::endl;

  test & g;
  std::cout<<"g="<<g<<std::endl;

  test & h;
  std::cout<<"h[0]="<<h[0]<<std::endl;
  std::cout<<"h[1]="<<h[1]<<std::endl;
  std::cout<<"h[2]="<<h[2]<<std::endl;
  std::cout<<"h[3]="<<h[3]<<std::endl;
  std::cout<<"h[4]="<<h[4]<<std::endl;

  test & i;
  std::cout<<"i="<<i<<std::endl;

  test & j;
  std::cout<<"*j="<<*j<<std::endl;

  test & k;

  for (std::map<int,float>::iterator it=k.begin(); it!=k.end(); ++it){
    std::cout<<it->first<<":"<<it->second<<std::endl;
  }

  test & l;

  for (std::map<int,std::string>::iterator it=l.begin(); it!=l.end(); ++it){
    std::cout<<it->first<<":"<<it->second<<std::endl;
  }

}
