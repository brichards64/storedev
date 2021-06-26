#ifndef BINARYSTREAM_H
#define BINARYSTREAM_H

#include <string.h>
#include <string>
#include <iostream>
#include <unistd.h> //for lseek
#include <vector>
#include <map>
#include <deque>
#include <SerialisableObject.h>
#include <stdio.h>
#ifdef ZLIB
#include <zlib.h>
#endif


class BinaryStream : public SerialisableObject{ 
  
 public:
  
  BinaryStream(int endpoint=0);
  bool Bopen(std::string filename, std::string method="READ", int endpoint=2);
  
  bool Bclose();
  bool Bwrite(void* in, unsigned int size);
  bool Bread(void* out, unsigned int size);
  long int Btell();
  bool Bseek(unsigned int pos, int whence);
  bool Print();
  void Serialise(BinaryStream &bs);  
    
  int m_endpoint;
  FILE* pfile;
#ifdef ZLIB
  gzFile gzfile;
#endif
  std::string buffer;
  bool m_write;
  std::string m_file_name;
  long int m_pos;

  //operator overloads

  void operator&(std::string& rhs){
    
    if(m_write){
      unsigned int tmp=rhs.length();
      (*this) & tmp;
      Bwrite(&(rhs[0]), tmp);
    }
    else{
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      Bread(&(rhs[0]), tmp);
    }
    
  }
  
  template<typename T> void operator&(T& rhs){

      if(check_base<SerialisableObject,T>::value){
	SerialisableObject* tmp=reinterpret_cast<SerialisableObject*>(&rhs);
	tmp->Serialise(*this);
      }
      else if(m_write){
	Bwrite(&rhs, sizeof(T));
      }
      else {
	Bread(&rhs, sizeof(T));
      }
  }

  
  template<typename T> void operator&(const T& rhs){

    if(m_write) Bwrite(&rhs, sizeof(T));

  }

  template<typename T> void operator&(std::vector<T>& rhs){

    if(m_write){
      unsigned int tmp=rhs.size(); 
      (*this) & tmp;
      if(check_base<SerialisableObject,T>::value){
	for(typename std::vector<T>::iterator it=rhs.begin(); it!=rhs.end(); it++) (*this) & (*it);	
      }
      else Bwrite(&(rhs[0]), tmp*sizeof(T));
    }
    else{
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      if(check_base<SerialisableObject,T>::value){
	for(typename std::vector<T>::iterator it=rhs.begin(); it!=rhs.end(); it++) (*this) & (*it);
      }
      else Bread(&(rhs[0]), tmp*sizeof(T));
      
    }
    
  }

  void operator&(std::vector<std::string>& rhs){
   
    if(m_write){
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

    if(m_write){
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

  
  template<typename T> void operator&(std::deque<T>& rhs){
    
    if(m_write){
      unsigned int tmp=rhs.size(); 
      (*this) & tmp;
      if(check_base<SerialisableObject,T>::value){
	for(typename std::deque<T>::iterator it=rhs.begin(); it!=rhs.end(); it++) (*this) & (*it);	
      }
      else Bwrite(&(rhs[0]), tmp*sizeof(T));
    }
    else{
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      if(check_base<SerialisableObject,T>::value){
	for(typename std::deque<T>::iterator it=rhs.begin(); it!=rhs.end(); it++) (*this) & (*it);
      }
      else Bread(&(rhs[0]), tmp*sizeof(T));
      
    }
    
  }

  void operator&(std::deque<std::string>& rhs){
       
    if(m_write){
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

 private:
  
  template <typename B, typename D> struct Host{
    
    operator B*() const;
    operator D*();
    
  };
  
 
  template <typename B, typename D> struct check_base {
    template <typename T> 
    static short check(D*, T);
    static char check(B*, int);
    
    static const bool value = sizeof(check(Host<B,D>(), int())) == sizeof(short);
  };
  
  
  
  /*
derived:

  yes D*(Host<B,D>(),T)   = D*(B* const, T);  not allowed
                          = D*(D*, T); ----------------------------------------------------------------------------------->   preferred as D*->D* better than D*->B* : answer yes

  no B*(Host<B,D>(), int) =  B*(B* const, int); 
                          =  B*(D*, int);  preferred as not const and object called on is not const----------------------->


not derrived:

  yes D*(Host<B,D>(),T)   =  D*(B* const, T); not allowed
                          =  D*(D*, T); ------------------------------>

  no B*(Host<B,D>(), int) =  B*(B* const, int);----------------------->  preffered as not templated : answer no
                          =  B*(D*, int); not allowed


   */


};


#endif
