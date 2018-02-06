#ifndef ICT_ITEM_H__
#define ICT_ITEM_H__
// inlcude PosIO and POS header files
#include "PosIO.h"
#include "POS.h"
#include <iomanip>
namespace ict{
  // class Item
  class Item:public PosIO
  {
  private:
    char m_sku[MAX_SKU_LEN + 1];
    char* m_name;
    double m_price;
    bool m_taxed;
    int m_quantity;
  public:
    Item();
    Item(const char* sku, const char* name, double price, bool tax = true);
    Item(const Item& item);
    Item& operator = (const Item& item);
    virtual ~Item();
    void sku(const char* sku);
    void price(const double price);
    void name(const char* name);
    void taxed(bool taxed);
    void quantity(int quantity);
    const char* sku()const;
    const char* name()const;
    const double price()const;
    const bool taxed()const;
    const int quantity()const;
    const double cost()const;
    const bool isEmpty();
    const bool operator==(const char* sku)const;
    int operator-=(const int oneIn);
    int operator+=(const int oneIn);
    //std::ostream& write(std::ostream& os, bool linear)const;
    //std::istream& read(std::istream& is);
  };

  // end class Item
  // operator += 
  double operator+=(double& d, const Item& I);
  // operator << and operator >>
  std::ostream& operator<<(std::ostream& os, const Item& item);
  std::istream&operator >> (std::istream& is, Item& item);
}


#endif