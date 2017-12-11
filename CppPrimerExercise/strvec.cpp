#include"strvec.h"
#include<utility>
#include <iterator>     // std::make_move_iterator
std::allocator<std::string> StrVec::alloc;
StrVec::StrVec(const StrVec & rhs)
{
    auto data = alloc_n_copy(rhs.begin(),rhs.end());
    elements = data.first;
    first_free = data.second;
}

StrVec& StrVec::operator =(const StrVec& rhs)
{
    auto data = alloc_n_copy(rhs.begin(),rhs.end());
    free();
    elements = data.first;
    first_free = data.second;
    return *this;
}

StrVec::~StrVec()
{
    free();
}

StrVec::StrVec( StrVec && rhs)noexcept
    :elements(rhs.elements),first_free(rhs.first_free),cap(rhs.cap)
{
    rhs.elements=rhs.first_free=rhs.cap=nullptr;
}

StrVec& StrVec::operator =( StrVec&& rhs) noexcept
{
    if(this !=&rhs)
    {
        free(); //释放已有元素
        //接管rhs元素
        elements = rhs.elements;
        first_free = rhs.first_free;
        cap = rhs.cap;
        //将rhs置于可析构状态
        rhs.elements = rhs.first_free = rhs.cap = nullptr;
    }
    return *this;
}
void StrVec::push_back(const std::string &s)
{
    chk_n_alloc();  //确保有空间容纳新元素
    alloc.construct(first_free++,s);
}

void StrVec::push_back(std::string && s)
{
    chk_n_alloc();
    alloc.construct(first_free++,std::move(s));
}
std::pair<std::string*,std::string*> StrVec::alloc_n_copy(const std::string *b, const std::string *e)
{
    auto data = alloc.allocate(e-b);
    return {data,std::uninitialized_copy(b,e,data)};
}

void StrVec::free()
{
    if(elements)
    {
        //逆袭销毁旧元素
        for(auto p = first_free;p!=elements;)
        {
            alloc.destroy(--p);
        }
        alloc.deallocate(elements,cap-elements);
    }
}

void StrVec::reallocate()
{
    //重新分配内存
    auto newCapacity = size()? 2*size():1;
    auto data = alloc.allocate(newCapacity);
    //移动原来数据到新内存中
    auto dest = data;
    auto elem = elements;
    for(size_t i=0;i!=size();++i)
    {
        alloc.construct(dest++,std::move(*elem++));
    }
    //移动元素
//    auto last = std::uninitialized_copy(std::make_move_iterator(begin()),
//                                        std::make_move_iterator(end()),
//                                        data);
    //释放旧内存
    free();
    //更新数据结构
    elements=data;
    first_free = dest;
    //
//    first_free = last;
    cap = elements+newCapacity;
}
