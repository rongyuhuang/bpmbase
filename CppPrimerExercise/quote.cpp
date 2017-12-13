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
