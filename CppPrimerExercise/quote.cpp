#include"quote.h"

double BulkQuote::net_price(size_t n) const
{
    if(n<minQty)
    {
        return n*price;
    }
    else
    {
        return (1-discount)*n*price;
    }
}


double print_total(std::ostream& os,const Quote& item,std::size_t qty)
{
    double ret = item.net_price(qty);
    os<<"ISBN:"<<item.isbn()<<" sold:"<<qty<<" total due:"<<ret<<std::endl;
    return ret;
}


double Basket::totalReceipt(std::ostream &os) const
{
    double sum=0;
    //iter指向items中相同ISBN的第一个元素
    //upper_bound返回一个迭代器，该迭代器指向这批元素的尾后位置
    for(auto iter = items.cbegin();iter != items.cend(); iter =items.upper_bound(*iter))
    {
        sum+=print_total(os,**iter,items.count(*iter));
    }
    os<<"Total Sale:"<<sum<<std::endl;
    return sum;
}
