#include <BStore.h>

BStore::BStore(bool type_checking){
  m_serialise=true;
  m_type_checking=type_checking;
}

std::string BStore::GetVersion(){return "BStore:1.0.0";}

bool BStore::Initnew(BinaryStream& bs, unsigned int position){
  m_file_end=position;
  m_open_file_end=m_file_end;
  return true;
}



bool BStore::Initnew(std::string filename, enum_type type, bool type_checking){
  struct stat buffer;   
  if(stat (filename.c_str(), &buffer) == 0){ //if file exists    
    std::cout<<"file exists"<<std::endl;
    output.Bclose();
    output.Bopen(filename, READ, UNCOMPRESSED);
    output.Bseek(0, SEEK_END);
    m_file_end=output.Btell();
    m_open_file_end=output.Btell();
    std::cout<<"flile end="<<m_file_end<<std::endl;
    m_file_name=filename;
    std::cout<<"current pos-2="<<output.Btell()<<std::endl;
    output.Bseek( m_file_end-(sizeof(m_lookup_start)+sizeof(m_lookup_size)+sizeof(m_type_checking)+sizeof(m_type)+sizeof(m_previous_file_end)), SEEK_SET);
    std::cout<<"current pos-11="<<output.Btell()<<std::endl;
    m_flags_start=output.Btell();
    std::cout<<"flast start pos="<<m_flags_start<<std::endl;
    std::cout<<"current pos5="<<output.Btell()<<std::endl;
    output >> m_lookup_start;
    std::cout<<"current pos="<<output.Btell()<<std::endl;
    output >> m_lookup_size;
    std::cout<<"current pos="<<output.Btell()<<std::endl;
    output >> m_type_checking;
    std::cout<<"current pos="<<output.Btell()<<std::endl;
    output >> m_type; 
    std::cout<<"current pos="<<output.Btell()<<std::endl;
    output >> m_previous_file_end;
    std::cout<<"current pos="<<output.Btell()<<std::endl;
    std::cout<<"loading"<<std::endl;
    std::cout<<"m_lookup_start="<< m_lookup_start<<std::endl;
    std::cout<<"m_lookup_size="<< m_lookup_size<<std::endl;
    std::cout<<"m_type_checking="<< m_type_checking<<std::endl;
    std::cout<<"m_type="<< m_type<<std::endl;
    std::cout<<"m_previous_file_end="<< m_previous_file_end<<std::endl;
    output.Bseek(0, SEEK_END);
    std::cout<<"current pos="<<output.Btell()<<std::endl;

    output.Bclose();
    m_entry=m_lookup_size-1;

    if(type==uncompressed) output.Bopen(filename, READ_APPEND, UNCOMPRESSED);
#ifdef ZLIB
    else if(type==compressed) output.Bopen(filename, READ, COMPRESSED);
    else if(type==post_pre_compress){
      std::cout<<"BEN loading file correctly"<<std::endl;
      std::cout<<"open="<<output.Bopen(filename, READ_APPEND, POST_PRE_COMPRESS)<<std::endl;    
      std::cout<<"seek="<<output.Bseek(0, SEEK_END)<<std::endl;
      m_file_end=output.Btell();
      m_open_file_end=output.Btell();
    }
#endif
    else return false;
    
    output.Bseek(m_lookup_start,SEEK_SET);
    output >> m_lookup;
    GetHeader();  
    GetEntry(0);
    
  }
  
  else{ 
    std::cout<<"file doesnt exist"<<std::endl;
    m_file_end=0;
    m_entry=0;
    if(type==uncompressed) output.Bopen(filename, READ_APPEND, UNCOMPRESSED);
#ifdef ZLIB
    else if(type==compressed)  output.Bopen(filename, APPEND, COMPRESSED);
    else if(type==post_pre_compress){
      std::cout<<"BEN opened correctly"<<std::endl;
      output.Bopen(filename, READ_APPEND, POST_PRE_COMPRESS);
    }
#endif
    else return false;
    m_type=type;
    m_type_checking=type_checking;
    m_file_name=filename;    
  }

  m_update=false;
  
}

/*
void BStore::Init(){
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
  m_type_checking=false;
}

void BStore::Init2(){

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
  update=false;
  m_type_checking=false;  
}

*/
bool BStore::Save(unsigned int entry){ //defualt save in next entry so need to do lookup size to find it, overlad with entry number so as to overwrite in lookup table.
  std::cout<<"bob save start="<<output.Btell()<<std::endl;  
  std::cout<<"save m_entry="<<m_entry<<std::endl;

  m_update=true;
  
  std::cout<<"debug1 entry="<<entry<<std::endl;

  entry++;
  if(entry>(m_lookup.size()-1)) entry=0;
  std::cout<<"debug2 entry="<<entry<<std::endl;
  
  if(!entry){
    m_entry++;
    entry=m_entry;
  }

  
  std::cout<<"debug3 entry="<<entry<<std::endl;
  
  std::cout<<"save start2="<<output.Btell()<<std::endl;
  std::cout<<"m_file_end="<<m_file_end<<std::endl;
  if(!output.Bseek(m_file_end,SEEK_SET)) return false;
  std::cout<<"save entry2="<<entry<<std::endl;
  m_lookup[entry]=output.Btell();
  std::cout<<"m_variables.size()="<<m_variables.size()<<std::endl;
  std::cout<<"m_lookup.size()="<<m_lookup.size()<<std::endl;
  std::cout<<"before saving m_variables="<<output.Btell()<<std::endl;
  if(!(output << m_variables)) return false;
  std::cout<<"after saving m_variables="<<output.Btell()<<std::endl;
  if(m_type_checking) if(!(output << m_type_info)) return false;
  m_file_end=output.Btell(); 
 
 
  return true;
}

bool BStore::GetHeader(){
  
    if(!m_lookup.count(0)) return false;
  //if(!m_lookup.size()) return false;
  output.Bseek(m_lookup[0], SEEK_SET);
  if(!(output >> Header)) return false;
  
  return true;
}

bool BStore::GetEntry(unsigned int entry_request){
  std::cout<<"mode="<<output.m_mode<<std::endl;
  entry_request++;
  
  std::cout<<"gettentry called="<<entry_request<<std::endl;

  if(!entry_request) return false;  
  std::cout<<"passed non zero check"<<std::endl;
  //if((m_lookup.size()-1)<entry_request) return false;
  if(!m_lookup.count(entry_request)) return false;
  std::cout<<"mode="<<output.m_mode<<std::endl;
  std::cout<<"passed get entry checks"<<std::endl;

  Delete();
  std::cout<<"getting entry data: entry="<<entry_request<<", location is="<<m_lookup[entry_request]<<std::endl;  
  std::cout<<"mode="<<output.m_mode<<std::endl;
  if(!output.Bseek(m_lookup[entry_request], SEEK_SET)) return false;
  std::cout<<"mode="<<output.m_mode<<std::endl;  
  if(!(output >> m_variables))return false;
  std::cout<<"mode="<<output.m_mode<<std::endl;
  if(m_type_checking)  if(!(output >>  m_type_info)) return false;
  std::cout<<"mode="<<output.m_mode<<std::endl;
  return true;
  
  
}

bool BStore::Close(){
  
  if(!m_update) return output.Bclose(true);
  else{

    std::cout<<"in close"<<std::endl;
    std::cout<<"btell="<<output.Btell()<<std::endl;
    std::cout<<"m_file_end"<<m_file_end<<std::endl;
    if(!output.Bseek(m_file_end,SEEK_SET))return false;
    m_lookup[0]=output.Btell();
    std::cout<<"btell="<<output.Btell()<<std::endl;
    std::cout<<"saving header"<<std::endl;
    if(!(output << Header)) return false; 
    std::cout<<"header saved"<<std::endl;
    std::cout<<"btell="<<output.Btell()<<std::endl;
    m_lookup_start= output.Btell();
    m_lookup_size=m_lookup.size();
    m_previous_file_end=m_open_file_end;
    std::cout<<"saving lookup"<<std::endl;
    if(!(output << m_lookup)) return false;
    std::cout<<"saved lookup"<<std::endl;
    std::cout<<"btell="<<output.Btell()<<std::endl;
    m_file_end=output.Btell();

    if(m_type!=uncompressed){

      if(!output.Bclose())return false;
      std::cout<<"k1"<<std::endl;
      if(!output.Bopen(m_file_name,APPEND,UNCOMPRESSED))return false;
      std::cout<<"k2 ="<<output.pfile<<std::endl;
    }
    

    std::cout<<"k3 saving flag start pos="<<output.Btell()<<std::endl;
    if(!output.Bseek(0,SEEK_END))return false;
    std::cout<<"k3 saving flag start pos2="<<output.Btell()<<std::endl;
    std::cout<<"m_lookup_start="<< m_lookup_start<<std::endl;
    std::cout<<"m_lookup_size="<< m_lookup_size<<std::endl;
    std::cout<<"m_type_checking="<< m_type_checking<<std::endl;
    std::cout<<"m_type="<< m_type<<std::endl;
    std::cout<<"m_previous_file_end="<< m_previous_file_end<<std::endl;
    if(!(output << m_lookup_start)) return false;
    if(!(output << m_lookup_size)) return false;
    if(!(output << m_type_checking)) return false;
    if(!(output << m_type)) return false;
    if(!(output << m_previous_file_end)) return false;
    std::cout<<"k4 "<<output.Btell()<<std::endl;
    if(!output.Bclose()) return false;
    std::cout<<"k5"<<std::endl;
    Delete();    
    std::cout<<"k6"<<std::endl;
    return true;
  }

}
//importing
  
bool BStore::Print(){ Print(false);}

void BStore::Print(bool values){
  
  for (std::map<std::string,BinaryStream>::iterator it=m_variables.begin(); it!=m_variables.end(); ++it){
    
    std::cout<< it->first << " => ";
    if(values) std::cout << it->second.buffer <<" :";
    std::cout<<" "<<m_type_info[it->first]<<std::endl;

  }
  
  for (std::map<std::string,PointerWrapperBase*>::iterator it=m_ptrs.begin(); it!=m_ptrs.end(); ++it){
    if(m_variables.count(it->first)==0){
      std::cout<< it->first << " => ";
      if(values) std::cout << it->second <<" :";
      std::cout<<" Pointer "<<std::endl;
    }
  }
  
}


void BStore::Delete(){ 
  
  m_variables.clear();
  m_type_info.clear();
 
  for (std::map<std::string,PointerWrapperBase*>::iterator it=m_ptrs.begin(); it!=m_ptrs.end(); ++it){

    delete it->second;
    it->second=0;

  }
  m_ptrs.clear();
  
  
}


void BStore::Remove(std::string key){

  for (std::map<std::string,BinaryStream>::iterator it=m_variables.begin(); it!=m_variables.end(); ++it){

    if(it->first==key){
      m_variables.erase(it);
      break;
    }
  }

    for (std::map<std::string,PointerWrapperBase*>::iterator it=m_ptrs.begin(); it!=m_ptrs.end(); ++it){

    if(it->first==key){
      delete it->second;
      it->second=0;
      m_ptrs.erase(it);
      break;
    }
  }
  

  if(m_type_checking){
    for (std::map<std::string,std::string>::iterator it=m_type_info.begin(); it!=m_type_info.end(); ++it){
      
      if(it->first==key){
	m_type_info.erase(it);
	break;
      }
    }
  }
  

}

void BStore::JsonParser(std::string input){
  
  int type=0;
  std::string key;
  std::string value;
  
  for(std::string::size_type i = 0; i < input.size(); ++i) {
    
    if(input[i]=='\"')type++;
    else if(type==1)key+=input[i];
    else if(type==3)value+=input[i];
    else if(type==4){
      type=0;
      m_variables[key] << value;
      key="";
      value="";
    }
  }
}


std::string BStore::Type(std::string key){

  if(m_type_info.count(key)>0){
    if(m_type_checking) return m_type_info[key];
    else return "?";
  }
  else return "Not in Store";

}

bool BStore::Has(std::string key){

  if(m_variables.count(key)>0) return true;
  else return false;

}

bool BStore::DeleteEntry(unsigned int entry_request){
 
  //Impliment this when you ahve changed it to a vector will be much easier

 /*
  entry_request++;

  if(!entry_request) return false;

  if(entry_request>=m_lookup.size()) return false;

  std::map<unsigned int, unsigned int>::iterator itbuff=m_lookup.end()

  for(std::map<unsigned int, unsigned int>::iterator it=m_lookup.end(); it!=m_lookup.begin(); it--){
    if(it->first==entry_request){
      itbuff=it;
      break;
    }   
    else {
      std::map<unsigned int, unsigned int>::iterator ittmp=it;
      it->second=itbuff->second;
      itbuff=ittmp;
    }
  }
  */

  return true;
  }


bool BStore::Rollback(){

  //ned to check if compressed or prepost to get location right sigh....

  Delete();
  m_lookup.clear();

  output.Bseek( m_previous_file_end-(sizeof(m_lookup_start)+sizeof(m_lookup_size)+sizeof(m_type_checking)+sizeof(m_type)+sizeof(m_previous_file_end)), SEEK_SET);
  std::cout<<"current pos-11="<<output.Btell()<<std::endl;
  m_flags_start=output.Btell();
  std::cout<<"flast start pos="<<m_flags_start<<std::endl;
  std::cout<<"current pos5="<<output.Btell()<<std::endl;
  output >> m_lookup_start;
  std::cout<<"current pos="<<output.Btell()<<std::endl;
  output >> m_lookup_size;
  std::cout<<"current pos="<<output.Btell()<<std::endl;
  output >> m_type_checking;
  std::cout<<"current pos="<<output.Btell()<<std::endl;
  output >> m_type; 
  std::cout<<"current pos="<<output.Btell()<<std::endl;
  output >> m_previous_file_end;
  std::cout<<"current pos="<<output.Btell()<<std::endl;
  std::cout<<"loading"<<std::endl;
  std::cout<<"m_lookup_start="<< m_lookup_start<<std::endl;
  std::cout<<"m_lookup_size="<< m_lookup_size<<std::endl;
  std::cout<<"m_type_checking="<< m_type_checking<<std::endl;
  std::cout<<"m_type="<< m_type<<std::endl;
  std::cout<<"m_previous_file_end="<< m_previous_file_end<<std::endl;
  output.Bseek(0, SEEK_END);
  std::cout<<"current pos="<<output.Btell()<<std::endl;
  
  m_entry=m_lookup_size-1;    
  output.Bseek(m_lookup_start,SEEK_SET);
  output >> m_lookup;
  GetHeader();  
  GetEntry(0);
  

  return true;
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


