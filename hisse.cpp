#include "hisse.h"
#include <iostream>
#include <string>
#include <list>
#include <utility>



std::ostream & operator<<(std::ostream &os, const hisse &r){
    os<<"( "<<r.hname<<" = ";
    for(auto iter : r.hldval)
        os<<iter<<" ";
    return os<<")";
}
std::istream & operator>>(std::istream &is, hisse &r){
    double dval;
    is>>r.hname>>dval;
    r.hldval.push_back(dval);
    return is;
}
    
hisse &hisse::operator=(const hisse &r){
    this->hname=r.hname;
    this->hldval=r.hldval;
    return *this;
}

bool hisse::operator<(const hisse &r) const{
    return this->hldval<r.hldval;
}

bool hisse::operator>(const hisse &r) const{
    return r<*this;
}

bool hisse::operator<=(const hisse &r) const{
    return *this<r || !(r<*this);
}

bool hisse::operator>=(const hisse &r) const{
    return r<*this || !(*this<r);
}

bool hisse::operator==(const hisse &r) const{
    return !(*this<r) && !(r<*this);
}

bool hisse::operator!=(const hisse &r) const{
    return (*this<r) || (r<*this);
}


std::pair <std::string, std::list<double>> hisse::get_pair() const{
    return std::make_pair(hname,hldval);
}

hisse &hisse::push_back(double dval){
    hldval.push_back(dval);
    return *this;
}