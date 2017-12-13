#ifndef QUOTE_H
#define QUOTE_H
#include<iostream>
#include<string>

class Quote{
public:
    Quote()=default;
    Quote(const std::string& id,double sales_price):
        bookNo(id),price(sales_price){}
    virtual ~Quote()=default;

    std::string isbn()const{return bookNo;}
    virtual double net_price(std::size_t n)const{return n*price;}
private:
    std::string bookNo;
protected:
    double price;
};


class BulkQuote:public Quote
{
public :
    BulkQuote()=default;
    //先初始化基类，再根据声明顺序依次初始化成员

    BulkQuote(std::string& id,double sales_price,std::size_t n,double dis):
        Quote(id,sales_price),minQty(n),discount(dis){}
    double net_price(size_t n) const override;

private:
    std::size_t minQty=0;
    double discount=0.0;
};

double print_total(std::ostream&,const Quote&,std::size_t);
#endif // QUOTE_H
