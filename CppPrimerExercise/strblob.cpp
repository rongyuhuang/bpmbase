#include"strblob.h"

StrBlob::StrBlob():data(std::make_shared<std::vector<std::string>>)
{

}

StrBlob::StrBlob(std::initializer_list<std::string> il):data(std::make_shared<std::vector<std::string>>(il))
{

}

void StrBlob::check(size_type i, const std::string &msg) const
{
    if(i>=data->size())
    {
        throw std::out_of_range(msg);
    }
}

void StrBlob::push_back(const std::string &t)
{
    data->push_back(t);
}

void StrBlob::pop_back()
{
    check(0,"pop_back on empty StrBlob");
    return data->pop_back();
}

std::string& StrBlob::front()
{
    check(0,"front on empty StrBlob");
    return data->front();
}

std::string& StrBlob::back()
{
    check(0,"back on empty StrBlob");
    return data->back();
}

StrBlobPtr  StrBlob::begin()
{
    return StrBlobPtr(*this);
}
StrBlobPtr  StrBlob::end()
{
    auto ret= StrBlobPtr(*this,data->size());
    return ret;
}
std::string& StrBlobPtr::deref() const
{
    auto p = check(curr,"dereference past end");
    return (*p)[curr];
}

StrBlobPtr& StrBlobPtr::incr()
{
    check(curr,"increment past end of StrBlobPtr");
    ++curr;
    return *this;
}

std::shared_ptr<std::vector<std::string>> StrBlobPtr::check(size_t t, const std::string &s) const
{
    auto ret = wptr.lock(); //vector还存在么？
    if(!ret)
    {
        throw std::runtime_error("unbound StrBolbPtr");
    }
    if(t>=ret->size())
    {
        throw std::out_of_range(s);
    }
    return ret;
}
