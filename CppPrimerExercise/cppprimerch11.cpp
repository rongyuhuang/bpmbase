#include<iostream>

#include<vector>
#include<list>
#include<map>
#include<set>
#include<unordered_map>
#include<functional>
#include<algorithm>
#include<numeric>

void ch11_1()
{
    std::map<std::string,size_t> wordCount;
    std::set<std::string> excludes={"the","The","THE","a","A","an","An","AN"};
    std::string word;
    while(std::cin>>word)
    {
        if(excludes.find(word)==excludes.end())
        {
            ++wordCount[word];
        }
    }
    for(const auto &w:wordCount)
    {
        std::cout<<w.first<<" occurs "<<w.second<<(w.second>1? " times.":" time.")<<std::endl;
    }
}

void ch11_2()
{
    std::vector<int> ivec;
    for(std::vector<int>::size_type i=0;i<10;++i)
    {
        ivec.push_back(i);
        ivec.push_back(i);
    }
    std::set<int> iset(ivec.cbegin(),ivec.cend());
    std::multiset<int> miset(ivec.cbegin(),ivec.cend());
    std::cout<<"ivec.size()="<<ivec.size()<<",iset.size()="<<iset.size()
            <<",miset.size()="<<miset.size()<<std::endl;

    std::pair<std::string,int> ann;
    std::pair<std::string,std::vector<int>> lines;
    std::pair<std::string,std::string> author{"Jame","Jone"};
    std::cout<<"first name:"<<author.first<<",last name:"<<author.second<<std::endl;
}


std::map<std::string,std::string> buildMap(std::ifstream &mapFile)
{
    std::map<std::string,std::string> transMap;
    std::string key,value;
    while(mapFile>>key &&getline(mapFile,value))
    {
        if(value.size()>1)
        {
            transMap[key]=value;
        }
        else
        {
            std::cout<<"no rule for "<<key<<std::endl;
        }
    }
    return transMap;
}

const std::string transform(const std::string &s,const std::map<std::string,std::string>& m)
{
    auto mapIter = m.find(s);
    if(mapIter !=m.end())
    {
        return mapIter->second;
    }
    else
    {
        return s;
    }
}

void wordTransform(std::ifstream& mapFile,std::ifstream& input)
{
    auto tranMap = buildMap(mapFile);
    std::string text;
    while(getline(input,text))
    {
        std::istringstream stream(text);
        std::string word;
        bool firstWord=true;
        while(stream>>word)
        {
            if(firstWord)
            {
                firstWord=false;
            }
            else
            {
                std::cout<<" ";
            }
            std::cout<<transform(word,tranMap);
        }
        std::cout<<std::endl;
    }
}
void ch11_3()
{
    std::set<std::string>::value_type v1;
    std::set<std::string>::key_type v2;
    std::map<std::string,int>::key_type v3;
    std::map<std::string,int>::value_type v4;
    std::map<std::string,int>::mapped_type v5;

    std::map<std::string,int> wordCount;
    std::string word;
    while(std::cin>>word)
    {
        ++wordCount[word];
    }
    auto mapIter = wordCount.cbegin();
    while(mapIter != wordCount.cend())
    {
        std::cout<<mapIter->first<<" occrus "<<mapIter->second<<std::endl;
        ++mapIter;
    }

    std::vector<int> ivec={2,4,6,8,10};
    std::set<int> set2;
    set2.insert(ivec.cbegin(),ivec.cend());
    set2.insert({1,3,5,7,9});
    for(auto x:set2)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

    wordCount.insert({word,1});
    wordCount.insert(std::make_pair("C++",1));
    mapIter = wordCount.cbegin();
    while(mapIter != wordCount.cend())
    {
        std::cout<<mapIter->first<<" occrus "<<mapIter->second<<std::endl;
        ++mapIter;
    }
    if(wordCount.erase("C++"))
    {
        std::cout<<"ok:"<<"C++"<<" removed."<<std::endl;
    }
    else
    {
        std::cout<<"fail:can\'t find \"C++\""<<std::endl;
    }

    wordCount["Anna"]=1;
    //auto temp=wordCount.at(1);
    //std::cout<<wordCount.at(1)<<std::endl;

    //访问元素
    std::cout<<"set2.find(2)="<<*set2.find(2)<<std::endl;
    std::cout<<"set2.find(15)="<<*set2.find(15)<<std::endl;
    std::cout<<"set2.count(2)="<<set2.count(2)<<std::endl;
    std::cout<<"set2.count(15)="<<set2.count(15)<<std::endl;

    //wordTransform();
}

void ch11_4()
{
   std::unordered_map<std::string,int> wordCount;
   std::string word;
   while(std::cin>>word)
   {
       ++wordCount[word];
   }
   for(const auto &x:wordCount)
   {
       std::cout<<x.first<<" ocurrs "<<x.second<<std::endl;
   }
}
void testCh11()
{
    std::cout<<"Start test chapter 11"<<std::endl;
    //ch11_1();
    //ch11_2();
    ch11_3();
    ch11_4();
}
