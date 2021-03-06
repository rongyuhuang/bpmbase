#ifndef SALES_DATA_H
#define SALES_DATA_H

#include<string>
#include<iostream>
typedef double wages;
typedef wages base,*p;


class Sales_data{
    friend Sales_data add(const Sales_data&,const Sales_data&);
    friend std::ostream &print(std::ostream&,const Sales_data&);
    friend std::istream &read(std::istream&,Sales_data&);
    friend std::istream& operator>>(std::istream&,Sales_data&);
    friend std::ostream& operator<<(std::ostream&,const Sales_data&);
    friend bool operator ==(const Sales_data&,const Sales_data&);
    friend bool operator !=(const Sales_data&,const Sales_data&);
public:
    //只有当类没有声明任何构造函数时，便一起才会自动的生成默认构造函数
    //如果类包含有内置类型或者复合类型的成员，只有当这些成员全都被赋予了类内的初始值时，这个类才适合于使用合成的默认构造函数Sales_data()=default;
    Sales_data()=default;
    Sales_data(const std::string&s):bookNo(s){}
    Sales_data(const std::string&s,unsigned n,double p):bookNo(s),units_sold(n),revenue(p*n){}
    Sales_data(std::istream&);

    //拷贝构造函数
    Sales_data(const Sales_data&);

    //重载赋值运算符 =》 合成拷贝赋值运算符
    Sales_data& operator=(const Sales_data& rhs);

    //复合赋值运算符
    Sales_data& operator +=(const Sales_data&);
    ~Sales_data(){}
    //委托构造函数
//    Sales_data():Sales_data("",0,0){}
//    Sales_data(std::string &s):Sales_data(s,0,0){}

    //阻止 隐式转换
    //explicit Sales_data(std::istream &);
    //默认情况下，this的类型是指向类类型非常量版本的常量指针。
    std::string isbn()const
    {
        return bookNo;
    }
    Sales_data& combine(const Sales_data&);
    double avg_price()const;
private:
    std::string bookNo;
    unsigned units_sold=0;
    double revenue=0.0;

};
//Sales_data的非成员接口函数(当 Sales_item为struct时)
Sales_data add(const Sales_data&,const Sales_data&);
std::ostream &print(std::ostream&,const Sales_data&);
std::istream &read(std::istream&,Sales_data&);

std::istream& operator>>(std::istream&,Sales_data&);
std::ostream& operator<<(std::ostream&,const Sales_data&);
bool operator ==(const Sales_data&,const Sales_data&);
bool operator != (const Sales_data&,const Sales_data&);

#endif // SALES_DATA_H
