#include <BinaryStream.h>

BinaryStream::BinaryStream(int endpoint){

  m_endpoint=endpoint;
  m_pos=0;
  m_write=true;
  m_file_name="";
}

//Todo:

// could add seperate >> and << operators for in and out that & then use wraps
// add append functionality?
// shore up by adding error checks to everything

bool BinaryStream::Bopen(std::string filename, std::string method, int endpoint){//make methods auto from READ WRITE strings (maybe append in future)
  //auto to highest endpoint unless specified;

  int prev_endpoint=m_endpoint;
  m_endpoint=endpoint;
  m_file_name=filename;

  if(method=="READ") m_write=false;
  else m_write=true;

#ifndef ZLIB
  if(m_endpoint==2) m_endpoint=1; // or give warning havent decided
#endif
  if(m_endpoint==0){
    //      endpoint=reinterpret_cast<int>(method);//how to check file type? maybe end control variable/ /or default to highest endpoint // nope read from serialise you dummy... but how think maybe go back to end of file control variable... wait could pass it on the method variable!?!?! good idea
    if(prev_endpoint!=0) m_endpoint=prev_endpoint;
    else{
      m_endpoint=1;
#ifdef ZLIB
      m_endpoint=2;
#endif
    }
    Bopen(filename,"READ",m_endpoint); 
    Bseek(0,SEEK_END);
    unsigned int end= Btell();
    Bseek(0,SEEK_SET);    
    buffer.resize(end);
    Bread(&(buffer[0]),end);
    Bclose();
    m_endpoint=0;
    return true;
  }
  if(m_endpoint==1){
    if(method=="READ") pfile = fopen(m_file_name.c_str(), "rb+");
    else  pfile = fopen(m_file_name.c_str(), "wb+");
    return true;
  }
#ifdef ZLIB
  if(m_endpoint==2){
    if(method=="READ") gzfile = gzopen(m_file_name.c_str(), "rb");
    else gzfile = gzopen(m_file_name.c_str(), "wb");
    return true;
  }
#endif
  m_file_name="";
  return false;
}


bool BinaryStream::Bclose(){
  
  if(m_endpoint==0){
    buffer.clear();
    m_pos=0;
    return true;
  }
  if(m_endpoint==1){
    fclose(pfile);
    return true;
  }
#ifdef ZLIB
  if(m_endpoint==2){
    gzclose(gzfile);
    return true;
  }
#endif
  return false;
    
}

bool BinaryStream::Bwrite(void* in, unsigned int size){

  if(m_endpoint==0){
    buffer.resize(buffer.length()+size);
    memcpy(&(buffer[m_pos]), in, size);
    m_pos+=size;
    return true;
  }
  if(m_endpoint==1){
    fwrite(in , size, 1, pfile);
    return true;
  }
#ifdef ZLIB
  if(m_endpoint==2){
    gzwrite(gzfile, in, size);
    return true;
  }
#endif
  return false;
  
}

bool BinaryStream::Bread(void* out, unsigned int size){
  
  if(m_endpoint==0){
    memcpy(out, &(buffer[m_pos]), size);
    m_pos+=size;
    return true;
  }
  if(m_endpoint==1){
    fread(out , size, 1, pfile);
    return true;
  }
#ifdef ZLIB
  if(m_endpoint==2){
    gzread(gzfile, out, size);
    return true;
  }
#endif
  return false;
  
}


long int BinaryStream::Btell(){
  
  if(m_endpoint==0){
    return m_pos;
  }
  if(m_endpoint==1){
    return ftell(pfile);
  }
#ifdef ZLIB
  if(m_endpoint==2){
    return gztell(gzfile);
  }
#endif
  return false;
}

bool BinaryStream::Bseek(unsigned int pos, int whence){

  if(m_endpoint==0){
    if(whence==SEEK_SET) m_pos=pos;
    else if(whence==SEEK_CUR) m_pos+=pos;
    else if(whence==SEEK_END) m_pos=buffer.length()+pos;
    return true;
  }
  if(m_endpoint==1){
    if(whence==SEEK_END) lseek(fileno(pfile), pos, whence);
    else fseek (pfile, pos, whence);
    return true;
  }
#ifdef ZLIB
  if(m_endpoint==2){
    if(whence==SEEK_END) return false; // could rewrite to get end pos with fopen and lseek
    gzseek(gzfile, pos, whence);
    return true;
  }
#endif
  return false;
}

bool BinaryStream::Print(){

  std::cout<<"endpoint="<<m_endpoint<<std::endl;

}

void BinaryStream::Serialise(BinaryStream &bs){
  
  if(m_endpoint==0){
    bs & buffer;
    m_pos=0;
  }
  else{
    if(bs.m_write){
      Bclose();
      Bopen(m_file_name,"READ",0);
      bs & buffer;
      Bclose();
    }
    else{
      int tmp=m_endpoint;
      Bclose();
      Bopen(m_file_name,"WRITE",tmp);
      bs & buffer;
      Bwrite(&(buffer[0]), buffer.length());
      Bclose();
      Bopen(m_file_name,"READ",tmp);
    }

  }
  
}

