#ifndef STRBLOB_H
#define STRBLOB_H

#include<vector>
#include<memory>

class StrBlobPtr;

typedef std::vector<std::string>::size_type size_type;
class StrBlob
{
    friend class StrBlobPtr;

public:
    StrBlob();
    StrBlob(std::initializer_list<std::string> il);

    size_type size()const{return data->size();}
    bool empty()const{return data->empty();}
    void push_back(const std::string& t);
    void pop_back();
    std::string & front();
    std::string & back();

    StrBlobPtr begin();
    StrBlobPtr end();
private:
    std::shared_ptr<std::vector<std::string>> data;
    void check(size_type i,const std::string &msg)const;

};

class StrBlobPtr
{
public:
    StrBlobPtr():curr(0){}
    StrBlobPtr(StrBlob& s,size_type sz=0):wptr(s.data),curr(sz){}
    std::string & deref()const;
    StrBlobPtr& incr();

private:
    std::shared_ptr<std::vector<std::string>> check(std::size_t t,const std::string& s)const;
    std::weak_ptr<std::vector<std::string>> wptr;
    std::size_t curr;
};


#endif // STRBLOB_H
