#ifndef TEXTQUERY_H
#define TEXTQUERY_H
#include<string>
#include<vector>
#include<map>
#include<set>
#include<memory>
#include<sstream>


using line_no =std::vector<std::string>::size_type;

class QueryResult;
class TextQuery
{

public:

    TextQuery(std::ifstream&);
    QueryResult query(const std::string&) const;

private:
    std::shared_ptr<std::vector<std::string>> file;
    std::map<std::string,std::shared_ptr<std::set<line_no>>> wm;
};


class QueryResult{
    friend std::ostream& print(std::ostream&,const QueryResult&);

public:
    QueryResult(std::string s,std::shared_ptr<std::set<line_no>> p,
                std::shared_ptr<std::vector<std::string>> f):sought(s),lines(p),file(f){}
private:
    std::string sought;
    std::shared_ptr<std::set<line_no>> lines; //出现的行号
    std::shared_ptr<std::vector<std::string>> file;//输入文件
};


TextQuery::TextQuery(std::ifstream & is):file(new std::vector<std::string>)
{
    std::string text;
    while(std::getline(is,text))
    {
        file->push_back(text);
        int n = file->size()-1; //行号

        std::istringstream line(text);
        std::string word;
        while(line>>word) //对行中每个单词
        {
            //如果单词不在wm中以此为下标在wm中添加一项
            auto &lines = wm[word];
            if(!lines)//liens是一个shared_ptr，当第一次遇到某个单词时，此指针为空，需要分配一个新的set
            {
                lines.reset(new std::set<line_no>);
            }
            lines->insert(n);
        }
    }
}

QueryResult TextQuery::query(const std::string &sought) const
{
    static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>);
    auto loc= wm.find(sought);
    if(loc ==wm.end())
    {
        return QueryResult(sought,nodata,file);
    }
    else
    {
        return QueryResult(sought,loc->second,file);
    }
}

std::ostream& print(std::ostream& os,const QueryResult& qr)
{
    os<<qr.sought<<" ocurrs "<<qr.lines->size()<<" time(s)"<<std::endl;
    //打印单词出现的每一行信息
    for(auto num:*qr.lines)
    {
        os<<"\t(line "<<num+1<<" )"<<*(qr.file->begin()+num)<<std::endl;
    }
    return os;
}

#endif // TEXTQUERY_H
