#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <set>
#include <algorithm>

typedef struct
{
    int count;
    int totalWords;
    std::vector<int> wordsNum;
    double pci;
    std::vector<double> pfci;
}TRAINING_T;

//训练结果集
typedef std::map<std::string,TRAINING_T> TRAINING_RESULT;
//样本数据映射
typedef std::pair<std::vector<std::string>,std::string> EXAMPLE_T;


//人工分类好的样本数据
std::vector<EXAMPLE_T>  examples =
{
    { { "周六", "公司", "庆祝", "聚餐", "时间", "订餐" }, "普通邮件" },
    { { "喜欢", "概率论", "考试", "研究", "及格", "补考", "失败" }, "普通邮件" },
    { { "贝叶斯", "理论", "算法", "公式", "困难" }, "普通邮件" },
    { { "上海", "晴朗", "郊游", "青草", "蓝天", "帐篷", "停车场", "拥堵" }, "普通邮件" },
    { { "代码", "走查", "错误", "反馈", "修改", "入库", "编译" }, "普通邮件" },
    { { "公司", "单元测试", "覆盖率", "时间", "用例", "失败", "成功" }, "普通邮件" },
    { { "优惠", "打折", "促销", "返利", "金融", "理财" }, "垃圾邮件" },
    { { "公司", "发票", "税点", "优惠", "增值税", "返利" }, "垃圾邮件" },
    { { "抽奖", "中奖", "点击", "恭喜", "申请", "资格" }, "垃圾邮件" },
    { { "爆款", "秒杀", "打折", "抵用券", "特惠" }, "垃圾邮件" },
    { { "招聘", "兼职", "日薪", "信用", "合作" }, "垃圾邮件" },
    { { "贷款", "资金", "担保", "抵押", "小额", "利息" }, "垃圾邮件" },
    { { "正规", "发票", "税务局", "验证", "咨询", "打折" }, "垃圾邮件" },
    { { "诚意", "合作", "特价", "机票", "欢迎", "咨询" }, "垃圾邮件" }
};


//样本去重
std::vector<std::string> MakeAllWordsList(const std::vector<EXAMPLE_T> &examples)
{
    std::set<std::string> wordsSet;//去除重复
    for(auto &e : examples)//遍历所有样本数据
    {
        wordsSet.insert(e.first.begin(),e.first.end());//将样本数据中的词表加入到集合中
    }

    std::vector<std::string> wordsList;
    //将集合中的词复制到向量中
    std::copy(wordsSet.begin(),wordsSet.end(),std::back_inserter(wordsList));
    return std::move(wordsList);
}

//转换成向量
std::vector<int> MakeWordsVec(const std::vector<std::string>& allWords,const std::vector<std::string> &words)
{
    std::vector<int> wordVec(allWords.size(),0);
    for(auto &word:words)//对于样本中的每个词遍历
    {
        //是否在词汇表中？如果不在词汇表就放弃这个词
        //（一般词汇表准备的话，很少出现这种情况)
        auto it = std::find(allWords.begin(),allWords.end(),word);
        if(it != allWords.end())
        {
            //如果在词汇表中，就计算在词汇表中的索引位置，然后这个词对应的统计向量+1
            wordVec[it - allWords.begin()] += 1;
        }
    }

    return std::move(wordVec);
}


//训练分类器
