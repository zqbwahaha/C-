#define _CRT_SECURE_NO_WARNINGS
// lib includes
#include "Perishable.h"

char ict::Perishable::signature() const
{
  return 'P';
}
ict::Perishable::Perishable()
{
  m_expiry.dateOnly(true);
}

std::fstream & ict::Perishable::save(std::fstream & file) const
{
  NonPerishable::save(file);
  file << ",";
  file << m_expiry;
//  file << std::endl;
  return file;
}

std::fstream & ict::Perishable::load(std::fstream & file)
{
  NonPerishable::load(file);
  int m_year, m_mon, m_day;
  file >> m_year;
  file.ignore(1);
  file >> m_mon;
  file.ignore(1);
  file >> m_day;
  Date tmp(m_year, m_mon, m_day);
  m_expiry = tmp;
  return file;
}

std::ostream & ict::Perishable::write(std::ostream & os, bool linear) const
{
  if (ok())
  {
    NonPerishable::write(os, linear);
    if (!linear)
    { 
      os << "Expiry date: ";
    os << m_expiry << std::endl;
    }
  }
  else   NonPerishable::write(os,linear);
  return os;
}

std::istream & ict::Perishable::read(std::istream & istr)
{
  NonPerishable::read(istr);
  if (!istr.fail())
  {
    std::cout << "Expiry date (YYYY/MM/DD): ";
    istr >> m_expiry;
    switch (m_expiry.errCode())
    {
    case CIN_FAILED:
      error("Invalid Date Entry");
      istr.setstate(std::ios::failbit);
      break;
    case YEAR_ERROR:
      error("Invalid Year in Date Entry");
      istr.setstate(std::ios::failbit);
      break;
    case MON_ERROR:
      error("Invalid Month in Date Entry");
      istr.setstate(std::ios::failbit);
      break;
    case DAY_ERROR:
      error("Invalid Day in Date Entry");
      istr.setstate(std::ios::failbit);
      break;
    default:
      break;
    }
  }
  return istr;
}
