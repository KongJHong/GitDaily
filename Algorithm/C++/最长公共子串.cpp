#include <iostream>
#include <string>


using namespace std;

const int MAX_STRING_LEN = 100;

int dpLCS(string &str1,string &str2,int s[MAX_STRING_LEN][MAX_STRING_LEN])
{
    string::size_type i,j;
    for(i = 1;i <= str1.size();i++)
        s[i][0] = 0;
    for(j = 1;j <= str2.size();j++)
        s[0][j] = 0;

    for(i = 1;i <= str1.size();i++){
        for(j = 1;j <= str2.size();j++){
            if(str1[i] == str2[j])
                s[i][j] = s[i-1][j-1] + 1;
            else{
                s[i][j] = std::max(s[i-1][j],s[i][j-1]);
            }
        }
    }

    return s[str1.size()][str2.size()];
}


int main(void){
    string str1 = "abcdefgh";
    string str2 = "fasdwgeqweh";
    int s[MAX_STRING_LEN][MAX_STRING_LEN] = {0};
    
    int sz = dpLCS(str1,str2,s);
    cout<<sz<<endl;

    system("pause");
    return 0;
    
}
