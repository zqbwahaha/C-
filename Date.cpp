#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <iostream>
#include <ctime>
using namespace std;
#include "Date.h"
#include "POS.h"


namespace ict {
  void Date::set() {
    time_t t = time(NULL);
    tm lt = *localtime(&t);
    m_day = lt.tm_mday;
    m_mon = lt.tm_mon + 1;
    m_year = lt.tm_year + 1900;
    if (dateOnly()) {
      m_hour = m_min = 0;
    }
    else {
      m_hour = lt.tm_hour;
      m_min = lt.tm_min;
    }
  }

  int Date::value()const {
    return m_year * 535680 + m_mon * 44640 + m_day * 1440 + m_hour * 60 + m_min;
  }

  int Date::mdays()const {
    int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1 };
    int mon = m_mon >= 1 && m_mon <= 12 ? m_mon : 13;
    mon--;
    return days[mon] + int((mon == 1)*((m_year % 4 == 0) && (m_year % 100 != 0)) || (m_year % 400 == 0));
  }
  Date::Date()
  {
    m_dateOnly = false;
    set();
  }
  Date::Date(int year, int mon, int day)
  {
    m_dateOnly = true;
    m_year = year;
    m_mon = mon;
    m_day = day;
    m_hour = 0;
    m_min = 0;
    m_readErrorCode = NO_ERROR;
  }
  Date::Date(int year, int mon, int day, int hour, int min)
  {
    m_dateOnly = false;
    m_year = year;
    m_mon = mon;
    m_day = day;
    m_hour = hour;
    m_min = min;
    m_readErrorCode = NO_ERROR;
  }
  void Date::set(int year, int mon, int day, int hour, int min)
  {
    m_year = year;
    m_mon = mon;
    m_day = day;
    m_hour = hour;
    m_min = min;
    m_readErrorCode = NO_ERROR;
  }
  void Date::errCode(int errorCode)
  {
    m_readErrorCode = errorCode;
  }
  int Date::errCode()const
  {
    return m_readErrorCode;
  }
  bool Date::bad()const
  {
    return m_readErrorCode;
  }
  void Date::dateOnly(bool value)
  {
    m_dateOnly = value;
    if (value)
    {
      m_min = 0;
      m_hour = 0;
    }
  }
  bool Date::dateOnly()const
  {
    return m_dateOnly;
  }
  bool Date::operator==(const Date& d)const
  {
    bool cmp = false;
    if (this->value() == d.value())
    {
      cmp = true;
    }
    return cmp;
  }
  bool Date::operator!=(const Date& d)const
  {
    bool cmp = false;
    if (this->value() != d.value())
    {
      cmp = true;
    }
    return cmp;
  }
  bool Date::operator<(const Date& d)const
  {
    bool cmp = false;
    if (this->value() < d.value())
    {
      cmp = true;
    }
    return cmp;
  }
  bool Date::operator>(const Date& d)const
  {
    bool cmp = false;
    if (this->value() > d.value())
    {
      cmp = true;
    }
    return cmp;
  }
  bool Date::operator<=(const Date& d)const
  {
    bool cmp = false;
    if (this->value() <= d.value())
    {
      cmp = true;
    }
    return cmp;
  }
  bool Date::operator>=(const Date& d)const
  {
    bool cmp = false;
    if (this->value() >= d.value())
    {
      cmp = true;
    }
    return cmp;
  }
  std::istream& Date::read(std::istream& is)
  {
    if (m_dateOnly)
    {
      is >> m_year;
      is.ignore(1);
      is >> m_mon;
      is.ignore(1);
      is >> m_day;
    }
    else
    {
      is >> m_year;
      is.ignore(1);
      is >> m_mon;
      is.ignore(1);
      is >> m_day;
      is.ignore(2);
      is >> m_hour;
      is.ignore(1);
      is >> m_min;
    }
    if (is.fail())
    {
      m_readErrorCode = CIN_FAILED;
    }
    else if (m_year > MAX_YEAR || m_year < MIN_YEAR)
    {
      m_readErrorCode = YEAR_ERROR;
    }
    else if (m_mon > 12 || m_mon < 1)
    {
      m_readErrorCode = MON_ERROR;
    }
    else if (m_day < 1 || m_day > mdays())
    {
      m_readErrorCode = DAY_ERROR;
    }
    else if (m_hour < 0 || m_hour > 23)
    {
      m_readErrorCode = HOUR_ERROR;
    }
    else if (m_min < 0 || m_min >59)
    {
      m_readErrorCode = MIN_ERROR;
    }
    else
    {
      m_readErrorCode = NO_ERROR;
    }
    return is;
  }
  std::ostream& Date::write(std::ostream& ostr )const
  {
    ostr << m_year << "/" << setw(2) << right << setfill('0') << m_mon << "/" << setw(2) << right << setfill('0')<<m_day;
    if (!m_dateOnly)
    {
      ostr << ", " << setw(2) << right << setfill('0') << m_hour << ":" << setw(2) << right << setfill('0') << m_min;
    }
    return ostr;
  }
  std::ostream&operator<<(std::ostream& os, const Date& op)
  {
    op.write(os);
    return os;
  }
  std::istream&operator >> (std::istream& is, Date& ip)
  {
    ip.read(is);
    return is;
  }
}