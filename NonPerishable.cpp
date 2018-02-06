#define _CRT_SECURE_NO_WARNINGS
// lib includes
#include "NonPerishable.h"

bool ict::NonPerishable::ok() const
{
  return m_err.isClear();
}

void ict::NonPerishable::error(const char * message)
{
  m_err = message;
}

char ict::NonPerishable::signature() const
{
  return 'N';
}

std::fstream & ict::NonPerishable::save(std::fstream & file) const
{

   file << signature()<<","<< sku() << "," << name() << "," << price() << "," << taxed() << "," << quantity();

//  if (signature() != 'P') {
//    std::cout << std::endl;
//  }
  return file;
}

std::fstream & ict::NonPerishable::load(std::fstream & file)
{
  char sk[20];     // read the data to from txt file
  char nam[200];
  double pric;
  int taxe;
  int qtt;
  //file.ignore(1, ',');
  file.getline(sk, MAX_SKU_LEN, ',');
  file.getline(nam, 200, ',');
  file >> pric;
  file.ignore(100, ',');
  file >> taxe;
  file.ignore(100, ',');
  file >> qtt;
  file.ignore(1);
  sku(sk);
  name(nam);
  price(pric);
  taxed(taxe);
  quantity(qtt);

  return file;
}

std::ostream & ict::NonPerishable::write(std::ostream & os, bool linear) const
{
  if (ok())
  {
    if (linear)
    {
      os << std::setw(MAX_SKU_LEN) << std::left << std::setfill(' ') << sku()
        << "|" << std::setw(20) << std::left << std::setfill(' ') << name()
        << "|" << std::setw(7) << std::right << std::setfill(' ')
        << std::fixed << std::setprecision(2) << price()
        << "| ";
      if (taxed())
        os << "T" << signature();
      else
        os << " " << signature();
      os << "|" << std::setw(4) << std::right << std::setfill(' ') << quantity() << "|"
        << std::setw(9) << std::right << std::setfill(' ')
        << std::fixed << std::setprecision(2) << quantity()*cost()
        << "|";
    }
    else
    {
      os << "Name:" << std::endl
        << name() << std::endl
        << "Sku: " << sku() << std::endl
        << "Price: " << std::setprecision(2) << price() << std::endl;
      if (taxed())
        os << "Price after tax: " << cost() << std::endl;
      else
        os << "Price after tax: N/A" << std::endl;
      os << "Quantity: " << quantity() << std::endl;
      os << "Total Cost: " << cost()*quantity() << std::endl;
    }
  }
  else
  {
    os << m_err;
  }
  return os;
}

std::istream & ict::NonPerishable::read(std::istream & is)
{
  char taxe;
  char buf[2000];
  double dbuf;
  int ibuf;
  while (1)
 {
    if(signature() == 'N')

    std::cout << "Item Entry:"<<std::endl;
    else
    std::cout <<"Perishable Item Entry:"<<std::endl;

    std::cout << "Sku: ";
    is >> buf;
    sku(buf);
    is.ignore();
    std::cout << "Name:" << std::endl;
    is.getline(buf, 2000);
    name(buf);
    std::cout << "Price: ";
    is >> dbuf;
    if (is.fail())
    {
      is.setstate(std::ios::failbit);
      m_err = "Invalid Price Entry";
      break;
    }
    price(dbuf);
    is.ignore();
    std::cout << "Taxed: ";
    is >> taxe;
    if (taxe == 'y' || taxe == 'Y')
      taxed(1);
    else if (taxe == 'N' || taxe == 'n')
      taxed(0);
    else
    {
      is.setstate(std::ios::failbit);
      m_err = "Invalid Taxed Entry, (y)es or (n)o";
      break;
    }
    is.ignore();
    std::cout << "Quantity: ";

    is >> ibuf;
    if (is.fail())
    {
      is.setstate(std::ios::failbit);
      m_err = "Invalid Quantity Entry";
      break;
    }
    else
    quantity(ibuf);
    m_err.clear();
    break;
  }
  return is;
}
