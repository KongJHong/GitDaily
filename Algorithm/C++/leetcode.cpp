#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <set>
#include <algorithm>
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


int longestPalindrome(string s) {
        unordered_map<char,int> sMap;
        int ans = 0;
        int sLen = s.size();
        for(auto ch:s)
        {
            if(sMap.find(ch) == sMap.end())sMap[ch] = 1;
            else sMap[ch]++;
        }
        
        
        auto iter = sMap.begin();
        
        for(;iter != sMap.end();iter++)
        {
            if(iter->second % 2)
            {
                ans += (int)(iter->second / 2) * 2;
                continue;
            }
            ans += iter->second;
        }
        
        return ans == sLen?ans:ans+1;
    }

string frequencySort(string s) {
        unordered_map<char,int> sMap;
        
        
        for(auto ch:s)
        {
            if(sMap.find(ch) == sMap.end())sMap[ch] = 1;
            else sMap[ch]++;
        }
        
        
        vector<pair<char,int>> vec(sMap.begin(),sMap.end());    

        sort(vec.begin(),vec.end(),[=](pair<char,int> &a,pair<char,int> &b){
            return a.second > b.second;
        });

        for(auto item:vec)
        {
            cout<<item.second<<":"<<item.first<<endl;
        }

        return "";
}
int main(void)
{
    
    string s = "aaaaaabbbbbeeedsdwqqrerewa";
    frequencySort(s);
    system("pause");
    return 0;
}


