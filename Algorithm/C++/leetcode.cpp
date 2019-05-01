#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;
 int countPrimes(int n) {
        if(n < 3)return 0;
       vector<int> ans(n,1);
        //0和1不是质数，所以列表的前两个位置赋值为0
        for(int i = 0;i < n;i++){
            cout<<ans[i]<<" ";
        }
        ans[0] = 0;
        ans[1] = 0;
        
        for(int i = 2;i < sqrt(n)+1;i++){
            if(ans[i] == 1){
                for(int j = i*i; j < n;j+=i){
                    ans[j] = 0;
                }
            }
        }
        
        int count = 0;
        for(int i = 2;i <= n;i++){
            if(ans[i] == 1)count++;
        }
        
        return count;
    }

bool containsDuplicate(vector<int>& nums) {
        unordered_map<int,int> hash;
        int len = nums.size();
        
        for(int i = 0; i < len;i++)
        {
            if(hash[nums[i]]){
                return true;
            }
            hash[nums[i]] = 1;
        }
        
        return false;
    }
int main(void)
{
    vector<int> vec = {1,1,1,3,3,4,3,2,4,2};
    bool flag = containsDuplicate(vec);
    string str = flag?"yes":"no";
    cout<<str<<endl;
    system("pause");
    return 0;
}


