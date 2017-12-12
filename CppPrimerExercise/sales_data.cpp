#include"sales_data.h"
#include<iostream>

Sales_data::Sales_data(std::istream &is)
{
    read(is,*this);
}

Sales_data::Sales_data(const Sales_data & rhs):
    bookNo(rhs.bookNo),
    units_sold(rhs.units_sold),
     revenue(rhs.revenue)
{

}

Sales_data& Sales_data::operator =(const Sales_data& rhs)
{
    bookNo= rhs.bookNo;
    units_sold = rhs.units_sold;
    revenue = rhs.revenue;
    return *this;
}

Sales_data& Sales_data::operator +=(const Sales_data& rhs)
{
    units_sold += rhs.units_sold;
    revenue +=rhs.revenue;
    return *this;
}
double Sales_data::avg_price()const
{
    if(units_sold)
    {
        return revenue/units_sold;
    }
    else
    {
        return 0;
    }
}

Sales_data& Sales_data::combine(const Sales_data & data)
{
    units_sold +=data.revenue;
    revenue +=data.revenue;
    return *this;
}

//定义类相关的非成员函数

Sales_data add(const Sales_data &lhs, const Sales_data &rhs)
{
    Sales_data sum= lhs;
    sum.combine(rhs);
    return sum;
}

std::istream& read(std::istream& is,Sales_data& item)
{
    double price=0;
    is>>item.bookNo>>item.units_sold>>price;
    item.revenue = price*item.units_sold;
    return is;
}

std::ostream& print(std::ostream& os,const Sales_data& item)
{
    os<<item.isbn()<<" "<<item.units_sold<<" "<<item.revenue<<" "<<item.avg_price();
    return os;
}

std::istream& operator >>(std::istream& is,Sales_data& rhs)
{
    double price;
    is>>rhs.bookNo>>rhs.units_sold>>price;
    if(is)
    {
        rhs.revenue = rhs.units_sold*price;
    }
    else
    {
        rhs = Sales_data();
    }
    return is;
}

std::ostream& operator<<(std::ostream& os,const Sales_data& item)
{
    os<<item.bookNo<<" "<<item.revenue<<" "<<item.avg_price();
    return os;
}

bool operator ==(const Sales_data& lhs,const Sales_data& rhs)
{
    return lhs.bookNo == rhs.bookNo &&
            lhs.units_sold == rhs.units_sold &&
            lhs.revenue == rhs.revenue;
}
bool operator != (const Sales_data& lhs,const Sales_data& rhs)
{
    return !(lhs == rhs);
}
