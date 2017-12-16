#ifndef WORDQUERY_H
#define WORDQUERY_H

#include<iostream>
#include<string>
#include<functional>
#include<map>
#include<set>
#include<memory>
#include<algorithm>
#include<iterator>

#include"textquery.h"
class Query;
/*
 * 抽象基类，具体的查询类型从中派生，所有成员都是private
*/
class QueryBase{
     friend class Query;
protected:
    using line_no =std::vector<std::string>::size_type;
    virtual ~QueryBase()=default;

private:
    //eval返回与当前的Query匹配的QueryResult
    virtual QueryResult eval(const TextQuery&) const=0;
    //rep表示的查询的一个string
    virtual std::string rep()const =0;
};

class Query{
    friend Query operator~(const Query&);
    friend Query operator | (const Query&lhs,const Query& rhs);
    friend Query operator &(const Query& lhs,const Query& rhs);

public:
    Query(const std::string&);
    QueryResult eval(const TextQuery& t)const
    {
        return q->eval(t);
    }

    std::string rep()const
    {
        return q->rep();
    }

private:
    Query(std::shared_ptr<QueryBase> query):q(query){}
    std::shared_ptr<QueryBase> q;
};

class WordQuery:public QueryBase{
    friend class Query;

    WordQuery(const std::string &s):query_word(s){}

    QueryResult eval(const TextQuery & t) const
    {return t.query(query_word);}

    std::string rep() const
    {return query_word;}

    std::string query_word;
};

class NotQuery:public QueryBase{
    friend Query operator ~(const Query& q);

    NotQuery(const Query &q):query(q){}

    //具体的类将定义所有继承而来的纯虚函数
    QueryResult eval(const TextQuery &) const;

    std::string rep()const
    {return "~("+query.rep()+")";}

    Query query;
};

QueryResult NotQuery::eval(const TextQuery & t) const
{
    auto result = query.eval(t); //找到存在的行
    auto retLines= std::make_shared<std::set<line_no>>(); //初始化返回的结果

//    auto beg = result.begin(),end = result.end();
//    auto sz = result.get_file()->size();
//    for(std::size_t n=0;n!=sz;++n)
//    {
//        if(beg==end || *beg!=n)
//        {
//            retLines->insert(n);
//        }
//        else if(beg !=end)
//        {
//            ++beg;
//        }
//    }
    return QueryResult(rep(),retLines,result.get_file());
}
inline Query operator ~(const Query& q)
{
    return std::shared_ptr<QueryBase>(new NotQuery(q));
}

class BinaryQuery:public QueryBase{
protected:
    BinaryQuery(const Query& l,const Query& r,std::string& s):
        lhs(l),rhs(r),opSym(s){}
    //
    std::string rep() const
    {return "("+lhs.rep()+" "+opSym+" "+rhs.rep()+")";}

    Query lhs,rhs;
    std::string opSym;
};

class AndQuery:public BinaryQuery{
    friend Query operator &(const Query&,const Query&);

    AndQuery(const Query& l,const Query& r):
        BinaryQuery(l,r,std::string("&")){}

    QueryResult eval(const TextQuery &) const;

};
QueryResult AndQuery::eval(const TextQuery &text) const
{
    auto left = lhs.eval(text),right = rhs.eval(text);
    auto retLines = std::make_shared<std::set<line_no>>();
//    std::set_intersection(left.begin(),left.end(),
//                          right.begin(),right.end,
//                          std::inserter(*retLines,retLines->begin()));
    return QueryResult(rep(),retLines,left.get_file());
}
inline Query operator& (const Query& l,const Query& r)
{
    return std::shared_ptr<QueryBase>(new AndQuery(l,r));
}

class OrQuery:public BinaryQuery{
    friend Query operator |(const Query& l,const Query& s);
    OrQuery(const Query& l,const Query& r):
        BinaryQuery(l,r,std::string("|")){}
    QueryResult eval(const TextQuery &) const;
};
QueryResult OrQuery::eval(const TextQuery &text) const
{
    auto left = lhs.eval(text),right = rhs.eval(text);
    auto retLines = std::make_shared<std::set<line_no>>();
//    retLines->insert(left.begin(),left.end());
//    retLines->insert(right.begin(),right.end());
    return QueryResult(rep(),retLines,left.get_file());
}
inline Query operator|(const Query&l,const Query&r)
{
    return std::shared_ptr<QueryBase>(new OrQuery(l,r));
}


#endif // WORDQUERY_H
