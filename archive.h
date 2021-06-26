#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <SerialisableObject.h>
#include <stdio.h>
#ifdef ZLIB
#include <zlib.h>
#endif

class archive: public std::string, public SerialisableObject {

  /////////////////ben need to add vector and deque serialisable object

public: 

 archive(bool in_loop=true, bool in_out=false) : pos(0), loop(in_loop), out(in_out){};
  
  bool out;
  bool loop;
  
  void operator&(std::string& rhs){
    
    
    if(out && length()>0){
      if(loop && pos==length()) pos=0;
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      memcpy(&(rhs[0]), &((*this)[pos]), tmp);
      if(loop) pos+=tmp;
      else erase(0,tmp);
    }
    
    else{
      unsigned int tmp=rhs.length();
      (*this) & tmp;
      resize(length()+tmp);
      memcpy(&((*this)[length()-tmp]), &rhs[0], tmp);
    }
    
  }
  
  template<typename T> void operator&(T& rhs){
    
    
    if((out && length()>0) || !out ){

      if(check_base<SerialisableObject,T>::value){
	SerialisableObject* tmp=reinterpret_cast<SerialisableObject*>(&rhs);
	tmp->Serialise(*this);
      }

      else{

	if(out){    
	  if(loop && pos==length()) pos=0;
	  memcpy(&rhs, &((*this)[pos]), sizeof(T));
	  if(loop) pos+=sizeof(T);
	  else erase(0,sizeof(T));
	}

	else{
	  
	  unsigned int tmp=length();
	  resize(tmp+sizeof(T));
	  memcpy(&((*this)[tmp]), &rhs, sizeof(T));
	  
	}
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

    if(out && length()>0){
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      if(check_base<SerialisableObject,T>::value){
	for(typename std::vector<T>::iterator it=rhs.begin(); it!=rhs.end(); it++) (*this) & (*it);
      }
      else memcpy(&(rhs[0]), &((*this)[pos]), sizeof(T)*tmp);
      if(loop) pos+=sizeof(T)*tmp;
      else erase(0,sizeof(T)*tmp);
    }
    
    else{
      unsigned int tmp=rhs.size();
      (*this) & tmp;
      if(check_base<SerialisableObject,T>::value){
	for(typename std::vector<T>::iterator it=rhs.begin(); it!=rhs.end(); it++) (*this) & (*it);
      }
      else{
	resize(length()+(tmp*sizeof(T)));
	memcpy(&((*this)[length()-(tmp*sizeof(T))]), &(rhs[0]), tmp*sizeof(T));
      }
    }

  }

  void operator&(std::vector<std::string>& rhs){
    
    if(out && length()>0){
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      for(int i=0; i<tmp; i++){
        (*this) & rhs.at(i);
      }
      
    }
    
    else{
      unsigned int tmp=rhs.size();
      (*this) & tmp;
      for(int i=0; i<tmp; i++){
	(*this) & rhs.at(i);
      }
    }
    
  }
  
  template<typename T, typename U> void operator&(std::map<T,U>& rhs){

    if(out && length()>0){
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
    
    else{
      
      unsigned int tmp=rhs.size();
      (*this) & tmp;
      for (typename std::map<T,U>::iterator it=rhs.begin(); it!=rhs.end(); ++it){
	(*this) & it->first;
	(*this) & it->second;
      }
      
    }
    
  }

  template<typename T> void operator&(std::deque<T>& rhs){

    if(out && length()>0){
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      if(check_base<SerialisableObject,T>::value){
	for(typename std::deque<T>::iterator it=rhs.begin(); it!=rhs.end(); it++) (*this) & (*it);
      }
      else memcpy(&(rhs[0]), &((*this)[pos]), sizeof(T)*tmp);
      if(loop) pos+=sizeof(T)*tmp;
      else erase(0,sizeof(T)*tmp);
    }

    else{
      unsigned int tmp=rhs.size();
      (*this) & tmp;
      if(check_base<SerialisableObject,T>::value){
	for(typename std::deque<T>::iterator it=rhs.begin(); it!=rhs.end(); it++) (*this) & (*it);
      }
      else{
      resize(length()+(tmp*sizeof(T)));
      memcpy(&((*this)[length()-(tmp*sizeof(T))]), &(rhs[0]), tmp*sizeof(T));
      }
    }
  }


  void operator&(std::deque<std::string>& rhs){
    
    if(out && length()>0){
      unsigned int tmp=0;
      (*this) & tmp;
      rhs.resize(tmp);
      for(int i=0; i<tmp; i++){
        (*this) & rhs.at(i);
      }
    }
    
    else{
      unsigned int tmp=rhs.size();
      (*this) & tmp;
      for(int i=0; i<tmp; i++){
	(*this) & rhs.at(i);
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

  bool Print(){ std::cout<<(*this)<<std::endl;}
  void Serialise(archive &ar){
    
    std::string* tmp=this;
    ar & tmp;

  }

  bool Save(std::string file_name, bool compress=true){

#ifdef ZLIB
    if(compress){
      gzFile gzfile;
      gzfile = gzopen(file_name.c_str(), "wb");
      Save(gzfile);
      gzclose(gzfile);      
      return true;
    }
#endif   
    
    FILE* pfile;
    pfile = fopen(file_name.c_str(), "wb+");
    Save(pfile);
    fclose(pfile);
    return true;
  }

  bool Save(FILE* pfile){

    unsigned int size= length();
    fwrite(&size , sizeof(size), 1, pfile);
    fwrite(&((*this)[0]) , sizeof(char), size, pfile);

    return true;
  }

#ifdef ZLIB
  bool Save(gzFile &gzfile){

    unsigned int size= length();
    gzwrite(gzfile, &size , sizeof(size));
    gzwrite(gzfile, &((*this)[0]) , sizeof(char)* size);   
    return true;
 
 }
#endif


  bool Load(std::string file_name, bool compress=true){

#ifdef ZLIB
    if(compress){
      gzFile gzfile; 
      gzfile = gzopen (file_name.c_str(), "rb");
      Load(gzfile);
      gzclose(gzfile);      
      return true;
    }
#endif  
    FILE* pfile;
    pfile = fopen(file_name.c_str(), "rb+");
    Load(pfile);
    fclose(pfile);

    return true;
  }

  bool Load(FILE* pfile){
  
    unsigned int size=0;
    fread(&size, sizeof(size), 1, pfile);
    resize(size);
    fread(&((*this)[0]) , sizeof(char), size, pfile); 
    out=true;

    return true;
  }

#ifdef ZLIB
  bool Load(gzFile &gzfile){
  
    unsigned int size=0;
    gzread(gzfile, &size, sizeof(size));   
    resize(size);
    gzread(gzfile, &((*this)[0]) , sizeof(char)*size); 
    out=true;

    return true;
  }
#endif

 private:

  unsigned int pos;

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
