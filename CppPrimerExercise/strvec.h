#ifndef STRVEC_H
#define STRVEC_H

#include<iostream>
#include<string>
#include<vector>
#include<set>
#include<memory>

class StrVec
{
public:
    StrVec():elements(nullptr),first_free(nullptr),cap(nullptr){}
    StrVec(const StrVec&);
    StrVec& operator =(const StrVec&);
    ~StrVec();

    //移动操作
    StrVec(StrVec &&) noexcept;
    StrVec& operator =(StrVec&&) noexcept;

    void push_back(const std::string& s);
    void push_back(std::string&&);//移动版
    size_t size()const {return first_free-elements;}
    size_t capacity ()const{ return cap-elements;}
    std::string* begin()const{return elements;}
    std::string* end()const {return first_free;}
private:
    static std::allocator<std::string> alloc;
    void chk_n_alloc()
    {
        if(size()==capacity())
        {
            reallocate();
        }
    }
    std::pair<std::string*,std::string*> alloc_n_copy(const std::string*,const std::string*);
    void free();
    void reallocate();
    std::string * elements;
    std::string* first_free;
    std::string* cap;
};
#endif // STRVEC_H
