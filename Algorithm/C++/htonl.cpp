#include <iostream>
using namespace std;
typedef unsigned long int UINT32;

#define BigLittleSwap32(A) ((((UINT32)(A) & 0xff000000)>>24)| \
                            (((UINT32)(A) & 0x00ff0000)>>8) | \
                            (((UINT32)(A) & 0x0000ff00)<<8) | \
                            (((UINT32)(A) & 0x000000ff)<<24)) 

//返回1，大端；返回0，小端
bool checkCPUEndian()
{
    union{
        short a;
        char b[sizeof(short)];
    }c;

    c.a=0x1234;
    return (0x12)==c.b[0];
}

UINT32 t_htonl(UINT32 h)
{
    return checkCPUEndian()?h:BigLittleSwap32(h);
}

#include <cstdio>
#include <string.h>
int main(){
    // UINT32 h=0x12345678;
    // UINT32 l = t_htonl(h);
    // cout<<showbase;
    // cout<<"hex: "<<hex<<l<<endl;
    // cout<<noshowbase;
    char message[1024];
    //memset(&message,0,sizeof(message));
    message[0] = 'a';
    message[1] = 'b';
    cout<<message<<endl;

    system("pause");
    return 0;
}

