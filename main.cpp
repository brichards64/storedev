#include <iostream>
#include <fstream>

int main(){

int a[20];

 for (int i=0;i<20; i++){
   a[i]=i;
 }
 
 std::ofstream outfile ("save.bin", std::ios::binary);
 outfile.write(reinterpret_cast<char*>(&a), sizeof(a));
 outfile.close(); 

 int b[20];

 std::ifstream infile ("save.bin", std::ios::binary);
 infile.read(reinterpret_cast<char*>(&b), sizeof(b));

 for (int i=0;i<20; i++){
   std::cout<<b[i]<<std::endl;
 }


 return 0;

}
