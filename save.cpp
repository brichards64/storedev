#include <iostream>
#include <stdio.h>
#include <map>
#include <string>
#include "zlib.h"
#include <unistd.h> //for lseek
#include <sstream>
#include <assert.h>

class Store{

#define CHUNK 16384

public:


  /* Compress from file source to file dest until EOF on source.
   def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_STREAM_ERROR if an invalid compression
   level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
   version of the library linked do not match, or Z_ERRNO if there is
   an error reading or writing the files. */
  int def(FILE *source, FILE *dest, int level)
  {
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
      return ret;

    /* compress until end of file */
    do {
      strm.avail_in = fread(in, 1, CHUNK, source);
      if (ferror(source)) {
	(void)deflateEnd(&strm);
	return Z_ERRNO;
      }
      flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
      strm.next_in = in;

      /* run deflate() on input until output buffer not full, finish
	 compression if all of source has been read in */
      do {
	strm.avail_out = CHUNK;
	strm.next_out = out;
	ret = deflate(&strm, flush);    /* no bad return value */
	assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
	have = CHUNK - strm.avail_out;
	if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
	  (void)deflateEnd(&strm);
	  return Z_ERRNO;
	}
      } while (strm.avail_out == 0);
      assert(strm.avail_in == 0);     /* all input will be used */

      /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
  }

  int inf(FILE *source, FILE *dest)
  {
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
      return ret;
   
 /* decompress until deflate stream ends or end of file */
    do {
      strm.avail_in = fread(in, 1, CHUNK, source);
      if (ferror(source)) {
	(void)inflateEnd(&strm);
	return Z_ERRNO;
      }
      if (strm.avail_in == 0)
	break;
      strm.next_in = in;

      /* run inflate() on input until output buffer not full */
      do {
	strm.avail_out = CHUNK;
	strm.next_out = out;
	ret = inflate(&strm, Z_NO_FLUSH);
	assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
	switch (ret) {
	case Z_NEED_DICT:
	  ret = Z_DATA_ERROR;     /* and fall through */
	case Z_DATA_ERROR:
	case Z_MEM_ERROR:
	  (void)inflateEnd(&strm);
	  return ret;
	}
	have = CHUNK - strm.avail_out;
	if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
	  (void)inflateEnd(&strm);
	  return Z_ERRNO;
	}
      } while (strm.avail_out == 0);

      /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
  }

  /* report a zlib or i/o error */
  void zerr(int ret)
  {
    fputs("zpipe: ", stderr);
    switch (ret) {
    case Z_ERRNO:
      if (ferror(stdin))
	fputs("error reading stdin\n", stderr);
      if (ferror(stdout))
	fputs("error writing stdout\n", stderr);
      break;
    case Z_STREAM_ERROR:
      fputs("invalid compression level\n", stderr);
      break;
    case Z_DATA_ERROR:
      fputs("invalid or incomplete deflate data\n", stderr);
      break;
    case Z_MEM_ERROR:
      fputs("out of memory\n", stderr);
      break;
    case Z_VERSION_ERROR:
      fputs("zlib version mismatch!\n", stderr);
    }
  }

  void Init(){
    //    file = gzopen ("myfile.bin", "ab");
    FILE * pFile;
    pFile = fopen ("myfile.bin", "rb");
    lseek(fileno(pFile), 0,  SEEK_END);
    //std::cout<<ftell(pFile)<<std::endl;
    file_end=ftell(pFile);
    lseek(fileno(pFile), 0-(sizeof(lookup_start)+sizeof(lookup_size)+sizeof(type)+sizeof(previous_file_end)),  SEEK_CUR);
    flags_start=ftell(pFile);
    fread(&lookup_start, sizeof(lookup_start),1, pFile);
    fread(&lookup_size, sizeof(lookup_size),1, pFile);
    fread(&type, sizeof(type),1, pFile);
    fread(&previous_file_end, sizeof(previous_file_end),1, pFile);
    fclose(pFile); 

    entry=0;
    file = gzopen ("myfile.bin", "wb");
    update=false;
    previous_file_end=0;
 }

  void Init2(){

    FILE * pFile;
    pFile = fopen ("myfile.bin", "rb");
    lseek(fileno(pFile), 0,  SEEK_END);
    //std::cout<<ftell(pFile)<<std::endl;
    file_end=ftell(pFile);
    lseek(fileno(pFile), 0-(sizeof(lookup_start)+sizeof(lookup_size)+sizeof(type)+sizeof(previous_file_end)),  SEEK_CUR);
    flags_start=ftell(pFile);
    fread(&lookup_start, sizeof(lookup_start),1, pFile);
    fread(&lookup_size, sizeof(lookup_size),1, pFile);
    fread(&type, sizeof(type),1, pFile);
    fread(&previous_file_end, sizeof(previous_file_end),1, pFile);
    //    std::cout<<"lookup_start="<<lookup_start<<std::endl;
    //std::cout<<"lookup_size="<<lookup_size<<std::endl;
    fclose(pFile);

    entry=0;
    file = gzopen ("myfile.bin", "rb");
    //    gzbuffer(file, 128000);
    
    gzseek(file,lookup_start,SEEK_SET);

    for(unsigned int i=0;i<lookup_size; i++){
      unsigned int key;
      if(!gzread(file, &key, sizeof(key))) break;
      unsigned int value;
      if(!gzread(file, &value, sizeof(value))) break;
      lookup[key]=value;

    }
    
    
  }

  std::map<std::string,std::string> test;
  std::map<std::string,std::string> test2;
  std::map<std::string,std::string> Header;

  bool save(){

    update=true;

    entry++;

    if(!entry) return false;
    lookup[entry]=gztell(file);
    unsigned int mapsize=test.size();
    gzwrite (file, &mapsize , sizeof(mapsize));
    for (std::map<std::string,std::string>::iterator it=test.begin(); it!=test.end(); ++it){
      unsigned int a=it->first.length() * sizeof(it->first[0]);
      unsigned int b=it->second.length() *  sizeof(it->second[0]);
      gzwrite (file, &a , sizeof(a));
      gzwrite (file, &b , sizeof(b));
      gzwrite (file, &(it->first[0]) , a);
      gzwrite (file, &(it->second[0]) , b);
      if(false){
	
	unsigned int c=test2[it->first].length() *  sizeof(test2[it->first][0]);
	gzwrite (file, &c , sizeof(c));
	gzwrite (file, &(test2[it->first][0]) , c);

      }
    }
    return true;
  }

  bool GetHeader(){

 if(!lookup.count(0)) return false;

    unsigned int end=lookup_start;
    gzseek(file,lookup[0], SEEK_SET);
    
    while (gztell(file)<end){
      
      unsigned int a=0;
      unsigned int b=0;
      
      if(!gzread(file, &a, sizeof(a))) break;    
      if(!gzread(file, &b, sizeof(b))) break;
      
      //std::cout<<"keysize="<<a<<std::endl;
      //std::cout<<"valuesize="<<b<<std::endl;
      
      std::string key;
      key.resize(a);
      // gzread(file, &(key.data()), a);
      if(!gzread(file, &(key[0]), a)) break;
      //std::cout<<"key="<<key<<" test="<<(key=="a")<<std::endl;
      std::string value;
      value.resize(b);
      //gzread(file, &(value.data()), b);
      if(!gzread(file, &(value[0]), b)) break;
      //std::cout<<"value="<<value<<std::endl;
      Header[key]=value;	 
   
   
    }
    
    
    return true;
  }

  bool GetEntry(unsigned int entry_request){
    
    entry_request++;

    if(!entry_request) return false;

    if(!lookup.count(entry_request)) return false;

    gzseek(file,lookup[entry_request], SEEK_SET);

    unsigned int mapsize=0;
    if(!gzread(file, &mapsize, sizeof(mapsize))) return false;
    
    for(unsigned int i=0; i<mapsize; i++){
      
      unsigned int a=0;
      unsigned int b=0;
      
      if(!gzread(file, &a, sizeof(a))) break;    
      if(!gzread(file, &b, sizeof(b))) break;
      
      //std::cout<<"keysize="<<a<<std::endl;
      //std::cout<<"valuesize="<<b<<std::endl;
      
      std::string key;
      key.resize(a);
      // gzread(file, &(key.data()), a);
      if(!gzread(file, &(key[0]), a)) break;
      //std::cout<<"key="<<key<<" test="<<(key=="a")<<std::endl;
      std::string value;
      value.resize(b);
      //gzread(file, &(value.data()), b);
      if(!gzread(file, &(value[0]), b)) break;
      //std::cout<<"value="<<value<<std::endl;
      test[key]=value;	 
       if(false){
	unsigned int c=0;
	std::string var_type;
	if(!gzread(file, &c, sizeof(c))) break;
	if(!gzread(file, &(var_type[0]), c)) break;

	test2[key]=var_type;
      }
    
    }
    return true;
    
    
  }
  
  void close(){
    
    if(update){

      entry=0;
      lookup[entry]=gztell(file);
      for (std::map<std::string,std::string>::iterator it=Header.begin(); it!=Header.end(); ++it){
	unsigned int a=it->first.length() * sizeof(it->first[0]);
	unsigned int b=it->second.length() *  sizeof(it->second[0]);
	gzwrite (file, &a , sizeof(a));
	gzwrite (file, &b , sizeof(b));
	gzwrite (file, &(it->first[0]) , a);
	gzwrite (file, &(it->second[0]) , b);
      }


      lookup_start=gztell(file);
      lookup_size=lookup.size();
      
      for (std::map<unsigned int,unsigned int>::iterator it=lookup.begin(); it!=lookup.end(); ++it){
	gzwrite (file, &(it->first) , sizeof(unsigned int));
	gzwrite (file, &(it->second) , sizeof(unsigned int));
      }
    }
    
    gzclose(file);
    

    if (update){
      FILE* pFile = fopen ("myfile.bin", "ab");
      fwrite ( &lookup_start, sizeof(lookup_start), 1, pFile);
      fwrite ( &lookup_size, sizeof(lookup_size), 1, pFile);
      fwrite ( &type, sizeof(type), 1, pFile);
      fwrite ( &previous_file_end, sizeof(previous_file_end), 1, pFile);
      fclose(pFile);
    }
  }
    
private:

  gzFile file;
  unsigned int file_end;
  unsigned int previous_file_end;
  unsigned int type;

  unsigned int flags_start;


  unsigned int lookup_start;
  unsigned int lookup_size;
  
  std::map<unsigned int, unsigned int> lookup;
  unsigned int entry;
  bool update;

};


int main ()
{
  
  
  Store store;
  store.Init();

  for(unsigned int i=0; i<10; i++){
    //    store.test["a"]="a";
    //  std::cout<<"a="<<store.test["a"]<<std::endl;
    // store.test["b"]="b";
    std::stringstream tmp;
    tmp<<i;
    store.test["c"]=tmp.str().c_str();
    store.test2["c"]=tmp.str().c_str();
    store.Header[tmp.str().c_str()]=tmp.str().c_str();
    store.save();
  }

  store.close();
  
  Store store2;
  store2.Init2();


  for(unsigned int i=0; i<10; i+=2){
    store2.GetEntry(i);
    //std::cout<<"testsize="<<store2.test.size()<<std::endl;  
    //std::cout<<"a="<<store2.test["a"]<<std::endl;
    //std::cout<<"b="<<store2.test["b"]<<std::endl;
    std::cout<<"c="<<store2.test["c"]<<std::endl;
  //  FILE * pFile;
  // char buffer[] = { 'x' , 'y' , 'z' };
  //pFile = fopen ("myfile.bin", "wb+");
  //fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
  //fclose (pFile);
  }

   store2.GetHeader();
    for(unsigned int i=0; i<10; i+=1){

    std::stringstream tmp;
    tmp<<i;
    std::cout<<store2.Header[tmp.str().c_str()]<<std::endl;

    }

  return 0;
}










  //////////////////////////////////////////////////////////////
  /// entry 1 size
  /// entry 1 
  /// entry 2 size
  /// entry 2 
  /// entry 2 extra data for nesting 
  /// entry 3 size
  /// entry 3
  /// ..
  /// ..
  /// Header / entry 0
  /// lookup 0                                        :  lookup_start
  /// lookup 1 
  /// ..
  /// ..
  /// lookup_start                                    :  flags_start
  /// lookup_size
  /// type   //
  /// previous_file_end
  //////////////////////////////////////////////////  :  file_end


