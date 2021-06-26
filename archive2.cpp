#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
//#include <SerialisableObject.h>
//#include <type_traits>
//#include <typeinfo>  

//typedef char (&yes)[1];
//typedef char (&no)[2];

template <typename B, typename D>
struct Host
{
  operator B*() const;
  operator D*();
};


template <typename B, typename D>
struct check_base
{
  template <typename T> 
  static short check(D*, T);
  static char check(B*, int);

  static const bool value = sizeof(check(Host<B,D>(), int())) == sizeof(short);
};




class archive;

class SerialisableObject{

public:

  virtual void Serialise(archive &ar)=0;

};



class archive: public std::string {


public: 

  archive() : pos(0){};
  bool out;


  void operator&(std::string& rhs){

    if(!out){
      unsigned int tmp=rhs.length();
      (*this) & tmp;
      resize(length()+tmp);
      memcpy(&((*this)[length()-tmp]), &rhs[0], tmp);
    }

    else{
      if(pos==length()) pos=0;
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      memcpy(&(rhs[0]), &((*this)[pos]), tmp);
      pos+=tmp;
    }

  }

  template<typename T> void operator&(T& rhs){

    if(check_base<SerialisableObject,T>::value){
      SerialisableObject* tmp=reinterpret_cast<SerialisableObject*>(&rhs);
      tmp->Serialise(*this);
    }
    else{
      if(!out){    
	unsigned int tmp=length();
	resize(tmp+sizeof(T));
	memcpy(&((*this)[tmp]), &rhs, sizeof(T));
      }
      
      else{
	if(pos==length()) pos=0;
	memcpy(&rhs, &((*this)[pos]), sizeof(T));
	pos+=sizeof(T);
      }
    }
  }
 
  template<typename T> void operator&(const T& rhs){

    if(!out){
      unsigned int tmp=length();
      resize(tmp+sizeof(T));
      memcpy(&((*this)[tmp]), &rhs, sizeof(T));
    }

  }

  template<typename T> void operator&(std::vector<T>& rhs){

    if(!out){
      unsigned int tmp=rhs.size();
      (*this) & tmp;
      resize(length()+(tmp*sizeof(T)));
      memcpy(&((*this)[length()-(tmp*sizeof(T))]), &(rhs[0]), tmp*sizeof(T));
    }

    else{
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      memcpy(&(rhs[0]), &((*this)[pos]), sizeof(T)*tmp);
      pos+=sizeof(T)*tmp;
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

private:

  unsigned int pos;

};

class mydata: public SerialisableObject{

public:

  mydata(int b=65):a(b),b(345.345),c("fuck yeh"){};

  int a;
  std::string c;
  double b;
  virtual void Serialise(archive &ar){

    ar & a;
    ar & b;
    ar & c;
  }

};


int main(){

  mydata test3;

  SerialisableObject* check=0;
  int test2;

  //std::cout<<"check "<<(std::is_base_of<SerialisableObject,mydata>::value)<<std::endl;
  //std::cout<<"check "<<(std::is_base_of<SerialisableObject,int>::value)<<std::endl;

  //  std::cout<<"check2 "<<typeid(SerialisableObject).name()<<std::endl;
  //std::cout<<"check2 "<<typeid(mydata).name()<<std::endl;
  //std::cout<<"check2 "<<typeid(check).name()<<std::endl;
  //std::cout<<"check2 "<<typeid(test3).name()<<std::endl;
  std::cout<<"check2 "<<sizeof(short)<<std::endl;
  std::cout<<"check2 "<<sizeof(char)<<std::endl;

   std::cout<<"check3 "<<(check_base<SerialisableObject,mydata>::value)<<std::endl;
   std::cout<<"check3 "<<(check_base<SerialisableObject,int>::value)<<std::endl;

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
  mydata m;


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

  test & m;

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
  m.a=0;
  m.b=0;
  m.c="";
 
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
  std::cout<<"i="<<i<<" not supposed to work"<<std::endl;

  test & j;
  std::cout<<"*j="<<*j<<" not supposed to work"<<std::endl;

  test & k;

  for (std::map<int,float>::iterator it=k.begin(); it!=k.end(); ++it){
    std::cout<<it->first<<":"<<it->second<<std::endl;
  }

  test & l;

  for (std::map<int,std::string>::iterator it=l.begin(); it!=l.end(); ++it){
    std::cout<<it->first<<":"<<it->second<<std::endl;
  }

  test & m;
  std::cout<<"m.a="<<m.a<<std::endl;
  std::cout<<"m.b="<<m.b<<std::endl;
  std::cout<<"m.c="<<m.c<<std::endl;

  a=0;
  test & a;
  std::cout<<"a="<<a<<std::endl;



}
