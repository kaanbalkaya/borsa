#pragma once
#include <utility>
#include <string>
#include <list>

class hisse{
    std::string hname;
    std::list<double> hldval;
public:
    hisse(std::string name, double dval=0.0): hname{name}, hldval{dval} {}

    friend std::ostream &operator<<(std::ostream &os, const hisse &r);
    friend std::istream &operator>>(std::istream &is, hisse &r);
    
    hisse &operator=(const hisse &r);
    hisse &push_back(double dval);
    

    bool operator<(const hisse &r) const;
    bool operator>(const hisse &r) const;
    bool operator<=(const hisse &r) const;
    bool operator>=(const hisse &r) const;
    bool operator==(const hisse &r) const;
    bool operator!=(const hisse &r) const;

    std::pair <std::string, std::list<double>> get_pair() const;


};