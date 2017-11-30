#include"sales_data.h"
#include<iostream>

Sales_data::Sales_data(std::istream &is)
{
    read(is,*this);
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
