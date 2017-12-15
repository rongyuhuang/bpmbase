#ifndef QUOTE_H
#define QUOTE_H
#include<iostream>
#include<string>
#include<memory>
#include<set>

class Quote{
public:
    Quote()=default;
    Quote(const Quote&)=default; //对成员依次拷贝
    Quote(Quote&&)=default; //对成员依次拷贝
    Quote& operator =(const Quote&)=default;  //拷贝赋值
    Quote& operator =(Quote &&)=default; //移动赋值
    Quote(const std::string& id,double sales_price):
        bookNo(id),price(sales_price){}
    virtual ~Quote()=default;

    std::string isbn()const{return bookNo;}
    virtual double net_price(std::size_t n)const{return n*price;}

    virtual Quote* clone()const&{return new Quote(*this);}
    virtual Quote* clone()const &&{return new Quote(std::move(*this));}
private:
    std::string bookNo;
protected:
    double price;
};

class QuoteDisc:public Quote{
    //抽象基类：含有纯虚函数的类

public:
    QuoteDisc()=default;
    QuoteDisc(std::string id,double sales_price,std::size_t qty,double disc):
        Quote(id,sales_price),quantity(qty),discount(disc){}

    std::pair<std::size_t,double> dicount_policy() const{return {quantity,discount};}

    double net_price(size_t n) const = 0;//纯虚函数

protected:
    std::size_t quantity=0;  //折扣适用的购买量
    double discount=0.0;    //折扣的小数值
};

/*
 * 当同一数据的销售量超过某个值时启用折扣
*/
class BulkQuote:public QuoteDisc
{
public :
    using QuoteDisc::QuoteDisc; // 继承QuoteDisc的构造函数
    BulkQuote()=default;
    //先初始化基类，再根据声明顺序依次初始化成员

    BulkQuote(std::string& id,double sales_price,std::size_t n,double dis):
        QuoteDisc(id,sales_price,n,dis){}
    double net_price(size_t n) const override;
    BulkQuote* clone()const &{return new BulkQuote(*this);}
    BulkQuote* clone() const &&{return new BulkQuote(std::move(*this));}
private:
    std::size_t minQty=0;
    double discount=0.0;
};

double print_total(std::ostream&,const Quote&,std::size_t);

class Basket{
public:
    void addItem(const std::shared_ptr<Quote>& sale)
    {
        items.insert(sale);
    }

    void addItem(const Quote& sale)
    {
        items.insert(std::shared_ptr<Quote>(sale.clone()));
    }
    void addItem(Quote&& sale)
    {
        items.insert(std::shared_ptr<Quote>(std::move(sale).clone()));
    }
    double totalReceipt(std::ostream& os)const;

private:
    static bool compare(const std::shared_ptr<Quote>& lhs,const std::shared_ptr<Quote>& rhs)
    {
        return lhs->isbn() < rhs->isbn();
    }
    std::multiset<std::shared_ptr<Quote>,decltype(compare)*> items{compare};
};
#endif // QUOTE_H
