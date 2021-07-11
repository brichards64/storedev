#include <iostream>
#include <string>
#include <vector>
#include <BinaryStream2.h>
#include <sstream>
#include <SerialisableObject.h>

class myclass: public SerialisableObject{

public:

  int a;
  float b;
  std::string c;
  std::vector<int> d;
  std::map<std::string, float> e;

  bool Print(){return true;}
  std::string GetVersion(){ return "test";}
  void Clear(){
    a=0;
    b=0;
    c="";
    d.clear();
    e.clear();
  }
  bool Serialise(BinaryStream &bs){

    if(!(bs & a)) std::cout<<"fa false"<<std::endl;// return false;
    if(!(bs & b)) std::cout<<"fb false"<<std::endl;//return false;
    if(!(bs & c)) std::cout<<"fc false"<<std::endl;//return false;
    if(!(bs & d)) std::cout<<"fd false"<<std::endl;//return false;
    if(!(bs & e)) std::cout<<"fe false"<<std::endl;//return false;

    return true;    
  }

};


int main(){

  int a=1;
  float b=2.3;
  std::string c="hello";
  std::vector<int> d;
  std::map<std::string, float> e;
  for (int i=0;i<5; i++){
    d.push_back(i);
    std::stringstream tmp;
    tmp<<i;
    e[tmp.str()]=i*1.1;
  }
  myclass f;
  f.a=4;
  f.b=5.6;
  f.c="world";
  for (int i=0;i<5; i++){
    f.d.push_back(i*10);
    std::stringstream tmp;
    tmp<<(i*10);
    f.e[tmp.str()]=i*2.2;
  }


  int a2=0;
  float b2=0;
  std::string c2="";
  std::vector<int> d2;
  std::map<std::string, float> e2;
  myclass f2;

  std::cout<<"Naked test"<<std::endl;

  BinaryStream bs1;

  if(!(bs1<<a)) std::cout<<"naked in a error"<<std::endl;
  if(!(bs1<<b)) std::cout<<"naked in b error"<<std::endl;
  if(!(bs1<<c)) std::cout<<"naked in c error"<<std::endl;
  if(!(bs1<<d)) std::cout<<"naked in d error"<<std::endl;
  if(!(bs1<<e)) std::cout<<"naked in e error"<<std::endl;
  if(!(bs1<<f)) std::cout<<"naked in f error"<<std::endl;

  bs1.m_pos=0;
  
  if(!(bs1>>a2)) std::cout<<"naked out a error"<<std::endl;
  if(!(bs1>>b2)) std::cout<<"naked out b error"<<std::endl;
  if(!(bs1>>c2)) std::cout<<"naked out c error"<<std::endl;
  if(!(bs1>>d2)) std::cout<<"naked out d error"<<std::endl;
  if(!(bs1>>e2)) std::cout<<"naked out e error"<<std::endl;
  if(!(bs1>>f2)) std::cout<<"naked out f error"<<std::endl;

  if(a!=a2)  std::cout<<"naked a value error"<<std::endl;
  if(b!=b2)  std::cout<<"naked b value error"<<std::endl;
  if(c!=c2)  std::cout<<"naked c value error"<<std::endl;
  for (int i=0;i<5; i++){
    if(d[i]!=d2[i])  std::cout<<"naked d value error"<<std::endl;
    std::stringstream tmp;
    tmp<<i;
    if(e[tmp.str()]!=e2[tmp.str()])  std::cout<<"naked e value error"<<std::endl;
  }
  if(f.a!=f2.a)  std::cout<<"naked f.a value error"<<std::endl;
  if(f.b!=f2.b)  std::cout<<"naked f.b value error"<<std::endl;
  if(f.c!=f2.c)  std::cout<<"naked f.c value error"<<std::endl;
  for (int i=0;i<5; i++){
    if(f.d[i]!=f2.d[i])  std::cout<<"naked f.d value error"<<std::endl;
    std::stringstream tmp;
    tmp<<i;
    if(f.e[tmp.str()]!=f2.e[tmp.str()])  std::cout<<"naked f.e value error"<<std::endl;
  }


  a2=0;
  b2=0;
  c2="";
  d2.clear();
  e2.clear();
  f2.Clear();

  std::cout<<"Naked test finnished"<<std::endl<<std::endl;

  ///////////////////////////////////////////////////////////////////////////

  std::cout<<"Uncompressed test"<<std::endl;

  BinaryStream bs2in;
  if(!bs2in.Bopen("bstest2",NEW,UNCOMPRESSED))std::cout<<"failed to open bs2in"<<std::endl;

  for(int i=0;i<1;i++){
    if(!(bs2in<<a)) std::cout<<"uncompressed in a error"<<std::endl;
    if(!(bs2in<<b)) std::cout<<"uncompressed in b error"<<std::endl;
    if(!(bs2in<<c)) std::cout<<"uncompressed in c error"<<std::endl;
    if(!(bs2in<<d)) std::cout<<"uncompressed in d error"<<std::endl;
    if(!(bs2in<<e)) std::cout<<"uncompressed in e error"<<std::endl;
    if(!(bs2in<<f)) std::cout<<"uncompressed in f error"<<std::endl;
  }
  if(!bs2in.Bclose())std::cout<<"failed to close bs2in"<<std::endl;
  
  BinaryStream bs2out;
  if(!bs2out.Bopen("bstest2",READ,UNCOMPRESSED))std::cout<<"failed to open bs2out"<<std::endl;
  
  if(!(bs2out>>a2)) std::cout<<"uncompressed out a error"<<std::endl;
  if(!(bs2out>>b2)) std::cout<<"uncompressed out b error"<<std::endl;
  if(!(bs2out>>c2)) std::cout<<"uncompressed out c error"<<std::endl;
  if(!(bs2out>>d2)) std::cout<<"uncompressed out d error"<<std::endl;
  if(!(bs2out>>e2)) std::cout<<"uncompressed out e error"<<std::endl;
  if(!(bs2out>>f2)) std::cout<<"uncompressed out f error"<<std::endl;

  if(!bs2out.Bclose())std::cout<<"failed to close bs2out"<<std::endl;


  if(a!=a2)  std::cout<<"uncompressed a value error"<<std::endl;
  if(b!=b2)  std::cout<<"uncompressed b value error"<<std::endl;
  if(c!=c2)  std::cout<<"uncompressed c value error"<<std::endl;
  for (int i=0;i<5; i++){
    if(d[i]!=d2[i])  std::cout<<"uncompressed d value error"<<std::endl;
    std::stringstream tmp;
    tmp<<i;
    if(e[tmp.str()]!=e2[tmp.str()])  std::cout<<"uncompressed e value error"<<std::endl;
  }
  if(f.a!=f2.a)  std::cout<<"naked f.a value error"<<std::endl;
  if(f.b!=f2.b)  std::cout<<"naked f.b value error"<<std::endl;
  if(f.c!=f2.c)  std::cout<<"naked f.c value error"<<std::endl;
  for (int i=0;i<5; i++){
    if(f.d[i]!=f2.d[i])  std::cout<<"naked f.d value error"<<std::endl;
    std::stringstream tmp;
    tmp<<i;
    if(f.e[tmp.str()]!=f2.e[tmp.str()])  std::cout<<"naked f.e value error"<<std::endl;
  }

  a2=0;
  b2=0;
  c2="";
  d2.clear();
  e2.clear();
  f2.Clear();

  std::cout<<"Uncompressed test finnished"<<std::endl<<std::endl;

  /////////////////////////////////////////////////////////////////////////////////////

  std::cout<<"Post_Pre_Compress test"<<std::endl;

  BinaryStream bs3in;
  if(!bs3in.Bopen("bstest3",NEW,POST_PRE_COMPRESS))std::cout<<"failed to open bs3in"<<std::endl;

  for(int i=0;i<1;i++){
    if(!(bs3in<<a)) std::cout<<"post_pre_compress in a error"<<std::endl;
    if(!(bs3in<<b)) std::cout<<"post_pre_compress in b error"<<std::endl;
    if(!(bs3in<<c)) std::cout<<"post_pre_compress in c error"<<std::endl;
    if(!(bs3in<<d)) std::cout<<"post_pre_compress in d error"<<std::endl;
    if(!(bs3in<<e)) std::cout<<"post_pre_compress in e error"<<std::endl;
    if(!(bs3in<<f)) std::cout<<"post_pre_compress in f error"<<std::endl;
  }

  if(!bs3in.Bclose())std::cout<<"failed to close bs3in"<<std::endl;

  BinaryStream bs3out;
  if(!bs3out.Bopen("bstest3",READ,POST_PRE_COMPRESS))std::cout<<"failed to open bs3out"<<std::endl;
  
  if(!(bs3out>>a2)) std::cout<<"post_pre_compress out a error"<<std::endl;
  if(!(bs3out>>b2)) std::cout<<"post_pre_compress out b error"<<std::endl;
  if(!(bs3out>>c2)) std::cout<<"post_pre_compress out c error"<<std::endl;
  if(!(bs3out>>d2)) std::cout<<"post_pre_compress out d error"<<std::endl;
  if(!(bs3out>>e2)) std::cout<<"post_pre_compress out e error"<<std::endl;
  if(!(bs3out>>f2)) std::cout<<"post_pre_compress out f error"<<std::endl;

  if(!bs3out.Bclose())std::cout<<"failed to close bs3out"<<std::endl;


  if(a!=a2)  std::cout<<"post_pre_compress a value error"<<std::endl;
  if(b!=b2)  std::cout<<"post_pre_compress b value error"<<std::endl;
  if(c!=c2)  std::cout<<"post_pre_compress c value error"<<std::endl;
  for (int i=0;i<5; i++){
    if(d[i]!=d2[i])  std::cout<<"post_pre_compress d value error"<<std::endl;
    std::stringstream tmp;
    tmp<<i;
    if(e[tmp.str()]!=e2[tmp.str()])  std::cout<<"post_pre_compress e value error"<<std::endl;
  }
  if(f.a!=f2.a)  std::cout<<"naked f.a value error"<<std::endl;
  if(f.b!=f2.b)  std::cout<<"naked f.b value error"<<std::endl;
  if(f.c!=f2.c)  std::cout<<"naked f.c value error"<<std::endl;
  for (int i=0;i<5; i++){
    if(f.d[i]!=f2.d[i])  std::cout<<"naked f.d value error"<<std::endl;
    std::stringstream tmp;
    tmp<<i;
    if(f.e[tmp.str()]!=f2.e[tmp.str()])  std::cout<<"naked f.e value error"<<std::endl;
  }

  a2=0;
  b2=0;
  c2="";
  d2.clear();
  e2.clear();
  f2.Clear();

  std::cout<<"Post_Pre_Compress test finnished"<<std::endl<<std::endl;

  /////////////////////////////////////////////////////////////////////////////////////

  std::cout<<"Compressed test"<<std::endl;

  BinaryStream bs4in;
  if(!bs4in.Bopen("bstest4",NEW,COMPRESSED))std::cout<<"failed to open bs4in"<<std::endl;

  for(int i=0;i<1;i++){
    if(!(bs4in<<a)) std::cout<<"compressed in a error"<<std::endl;
    if(!(bs4in<<b)) std::cout<<"compressed in b error"<<std::endl;
    if(!(bs4in<<c)) std::cout<<"compressed in c error"<<std::endl;
    if(!(bs4in<<d)) std::cout<<"compressed in d error"<<std::endl;
    if(!(bs4in<<e)) std::cout<<"compressed in e error"<<std::endl;
    if(!(bs4in<<f)) std::cout<<"compressed in f error"<<std::endl;
  }

  if(!bs4in.Bclose())std::cout<<"failed to close bs4in"<<std::endl;

  BinaryStream bs4out;
  if(!bs4out.Bopen("bstest4",READ,COMPRESSED))std::cout<<"failed to open bs4out"<<std::endl;
  
  if(!(bs4out>>a2)) std::cout<<"compressed out a error"<<std::endl;
  if(!(bs4out>>b2)) std::cout<<"compressed out b error"<<std::endl;
  if(!(bs4out>>c2)) std::cout<<"compressed out c error"<<std::endl;
  if(!(bs4out>>d2)) std::cout<<"compressed out d error"<<std::endl;
  if(!(bs4out>>e2)) std::cout<<"compressed out e error"<<std::endl;
  if(!(bs4out>>f2)) std::cout<<"compressed out f error"<<std::endl;

  if(!bs4out.Bclose())std::cout<<"failed to close bs4out"<<std::endl;


  if(a!=a2)  std::cout<<"compressed a value error"<<std::endl;
  if(b!=b2)  std::cout<<"compressed b value error"<<std::endl;
  if(c!=c2)  std::cout<<"compressed c value error"<<std::endl;
  for (int i=0;i<5; i++){
    if(d[i]!=d2[i])  std::cout<<"compressed d value error"<<std::endl;
    std::stringstream tmp;
    tmp<<i;
    if(e[tmp.str()]!=e2[tmp.str()])  std::cout<<"compressed e value error"<<std::endl;
  }
  if(f.a!=f2.a)  std::cout<<"naked f.a value error"<<std::endl;
  if(f.b!=f2.b)  std::cout<<"naked f.b value error"<<std::endl;
  if(f.c!=f2.c)  std::cout<<"naked f.c value error"<<std::endl;
  for (int i=0;i<5; i++){
    if(f.d[i]!=f2.d[i])  std::cout<<"naked f.d value error"<<std::endl;
    std::stringstream tmp;
    tmp<<i;
    if(f.e[tmp.str()]!=f2.e[tmp.str()])  std::cout<<"naked f.e value error"<<std::endl;
  }

  a2=0;
  b2=0;
  c2="";
  d2.clear();
  e2.clear();
  f2.Clear();

  std::cout<<"Compressed test finnished"<<std::endl<<std::endl;

  /////////////////////////////////////////////////////////////////////////////////////

  std::cout<<"multiple open test"<<std::endl;

  BinaryStream aa;
  if(!aa.Bopen("bstest",READ,RAM))std::cout<<"failed to open aa read doesnt exist"<<std::endl;
  if(!aa.Bopen("bstest",NEW,RAM))std::cout<<"failed to open aa1"<<std::endl;
  if(!aa.Bopen("bstest",NEW,RAM))std::cout<<"failed to open aa2"<<std::endl;
  if(!aa.Bopen("bstest",READ,RAM))std::cout<<"failed to open aa3"<<std::endl;
  if(!aa.Bopen("bstest",READ,RAM))std::cout<<"failed to open aa4"<<std::endl;

  BinaryStream bb;
  if(!bb.Bopen("bstest",READ,UNCOMPRESSED))std::cout<<"failed to open bb read doesnt exist"<<std::endl;
  if(!bb.Bopen("bstest",NEW,UNCOMPRESSED))std::cout<<"failed to open bb1"<<std::endl;
  if(!bb.Bopen("bstest",NEW,UNCOMPRESSED))std::cout<<"failed to open bb2"<<std::endl;
  if(!bb.Bopen("bstest",READ,UNCOMPRESSED))std::cout<<"failed to open bb3"<<std::endl;
  if(!bb.Bopen("bstest",READ,UNCOMPRESSED))std::cout<<"failed to open bb4"<<std::endl;

  BinaryStream cc;
  if(!cc.Bopen("bstest",READ,POST_PRE_COMPRESS))std::cout<<"failed to open cc read doesnt exist"<<std::endl;
  if(!cc.Bopen("bstest",NEW,POST_PRE_COMPRESS))std::cout<<"failed to open cc1"<<std::endl;
  if(!cc.Bopen("bstest",NEW,POST_PRE_COMPRESS))std::cout<<"failed to open cc2"<<std::endl;
  if(!cc.Bopen("bstest",READ,POST_PRE_COMPRESS))std::cout<<"failed to open cc3"<<std::endl;
  if(!cc.Bopen("bstest",READ,POST_PRE_COMPRESS))std::cout<<"failed to open cc4"<<std::endl;
 
  BinaryStream dd;
  if(!dd.Bopen("bstest",READ,COMPRESSED))std::cout<<"failed to open dd read doesnt exist"<<std::endl;
  if(!dd.Bopen("bstest",NEW,COMPRESSED))std::cout<<"failed to open dd1"<<std::endl;
  if(!dd.Bopen("bstest",NEW,COMPRESSED))std::cout<<"failed to open dd2"<<std::endl;
  if(!dd.Bopen("bstest",READ,COMPRESSED))std::cout<<"failed to open dd3"<<std::endl;
  if(!dd.Bopen("bstest",READ,COMPRESSED))std::cout<<"failed to open dd4"<<std::endl;

  std::cout<<"multiple open test finnished"<<std::endl<<std::endl;

  //////////////////////////////////////////////////////////////////////////////

  std::cout<<"multiple close test"<<std::endl;

  if(!aa.Bclose())std::cout<<"failed to close aa read doesnt exist"<<std::endl;
  if(!aa.Bclose())std::cout<<"failed to close aa1"<<std::endl;
  if(!aa.Bclose())std::cout<<"failed to close aa2"<<std::endl;
  if(!aa.Bclose())std::cout<<"failed to close aa3"<<std::endl;
  if(!aa.Bclose())std::cout<<"failed to close aa4"<<std::endl;

  if(!bb.Bclose())std::cout<<"failed to close bb read doesnt exist"<<std::endl;
  if(!bb.Bclose())std::cout<<"failed to close bb1"<<std::endl;
  if(!bb.Bclose())std::cout<<"failed to close bb2"<<std::endl;
  if(!bb.Bclose())std::cout<<"failed to close bb3"<<std::endl;
  if(!bb.Bclose())std::cout<<"failed to close bb4"<<std::endl;

  if(!cc.Bclose())std::cout<<"failed to close cc read doesnt exist"<<std::endl;
  if(!cc.Bclose())std::cout<<"failed to close cc1"<<std::endl;
  if(!cc.Bclose())std::cout<<"failed to close cc2"<<std::endl;
  if(!cc.Bclose())std::cout<<"failed to close cc3"<<std::endl;
  if(!cc.Bclose())std::cout<<"failed to close cc4"<<std::endl;

  if(!dd.Bclose())std::cout<<"failed to close dd read doesnt exist"<<std::endl;
  if(!dd.Bclose())std::cout<<"failed to close dd1"<<std::endl;
  if(!dd.Bclose())std::cout<<"failed to close dd2"<<std::endl;
  if(!dd.Bclose())std::cout<<"failed to close dd3"<<std::endl;
  if(!dd.Bclose())std::cout<<"failed to close dd4"<<std::endl;

  std::cout<<"multiple close test finnished"<<std::endl<<std::endl;

  //////////////////////////////////////////////////////////////////////////

  /*
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
  */  
  return 0;
  
}
