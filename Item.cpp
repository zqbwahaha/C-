#include "Item.h"
#define _CRT_SECURE_NO_WARNINGS  
// Lib includes

// inlcude Item and POS header files

using namespace std;
namespace ict{
  // class Item implementaion
  Item::Item()
  {
    m_sku[0] = 0;
    m_name = nullptr;
    m_price = 0;
    m_taxed = false;
    m_quantity = 0;
  }

  Item::Item(const char * sku, const char * name, double price, bool tax)
  {
    strncpy(m_sku, sku, MAX_SKU_LEN);
    m_sku[MAX_SKU_LEN] = '\0';
    m_name = new char[strlen(name) + 1];
    strcpy(m_name, name);
    m_taxed = tax;
    m_price = price;
    m_quantity = 0;
  }

  Item::Item(const Item & item)
  {
    if (this != &item)
    {
      strcpy(m_sku, item.m_sku);
      m_name = new char[strlen(item.m_name) + 1];
      strcpy(m_name, item.m_name);
      m_taxed = item.m_taxed;
      m_quantity = item.m_quantity;
      m_price = item.m_price;
    }
  }

  Item & Item::operator=(const Item & item)
  {
    if (this != &item)
    {
      if (m_name!=nullptr)
      {
        delete[] m_name;
      }
      strcpy(m_sku, item.m_sku);
      m_name = new char[strlen(item.m_name) + 1];
      strcpy(m_name, item.m_name);
      m_taxed = item.m_taxed;
      m_quantity = item.m_quantity;
      m_price = item.m_price;
    }
    return *this;
  }

  Item::~Item()
  {
    delete[] m_name;
  }

  void Item::sku(const char * sku)
  {
    strncpy(m_sku, sku,MAX_SKU_LEN);
    m_sku[MAX_SKU_LEN] = '\0';
  }

  void Item::price(const double price)
  {
    m_price = price;
  }

  void Item::name(const char * name)
  {
    m_name = new char[strlen(name) + 1];
    strcpy(m_name,name);
  }

  void Item::taxed(bool taxed)
  {
    m_taxed = taxed;
  }

  void Item::quantity(int quantity)
  {
    m_quantity = quantity;
  }

  const char * Item::sku() const
  {
    return m_sku;
  }

  const char * Item::name() const
  {
    return m_name;
  }

  const double Item::price() const
  {
    return m_price;
  }

  const bool Item::taxed() const
  {
    return m_taxed;
  }

  const int Item::quantity() const
  {
    return m_quantity;
  }

  const double Item::cost()const
  {
    return m_taxed ? (m_price*1.13) : m_price;
  }

  const bool Item::isEmpty()
  {
    return (!m_name && !m_sku[0] && !m_quantity && !m_price && !m_taxed);
  }

  const bool Item::operator==(const char * sku) const
  {
    return !(strcmp(sku, this->sku()));
  }

  int Item::operator-=(const int oneIn)
  {
    this->m_quantity -= oneIn;
    return m_quantity;
  }

  int Item::operator+=(const int oneIn)
  {
    this->m_quantity += oneIn;
    return m_quantity;
  }

 /* std::ostream & Item::write(std::ostream & os, bool linear)const
  {
      os << sku() << ": " << name() << endl
      << "Quantity: " << quantity() << endl
      << "Total Cost:" << fixed << std::setprecision(2) << cost();
      return os;
  }*/

  //std::istream & Item::read(std::istream & is)
  //{
  //  char buf[2000];
  //  double dbuf;
  //  int ibuf;
  //  cout << "sku: ";
  //  is >> buf;
  //  sku(buf);
  //  is.ignore();
  //  cout << "name: ";
  //  is.getline(buf, 2000);
  //  name(buf);
  //  cout << "qty: ";
  //  is >> ibuf;
  //  quantity(ibuf);
  //  cout << "is taxed? (1/0): ";
  //  is >> ibuf;
  //  taxed(!!ibuf);
  //  cout << "price: ";
  //  is >> dbuf;
  //  price(dbuf);
  //  return is;
  //}

  double operator+=(double & d, const Item & I)
  {
    return (d += I.cost()*I.quantity());
  }

  std::ostream & operator<<(std::ostream & os, const Item & item)
  {
    item.write(os, true);
    return os;
  }

  std::istream & operator >> (std::istream & is, Item & item)
  {
    item.read(is);
    return is;
  }

}
