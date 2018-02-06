#define _CRT_SECURE_NO_WARNINGS 

#include "Error.h"

namespace ict {
  Error::Error()
  {
    m_message = nullptr;
  }
  Error::Error(const char * errorMessage)
  {
    m_message = nullptr;
    message(errorMessage);
  }
  Error::~Error()
  {
    delete[]m_message;
  }
  void Error::operator=(const char * errorMessage)
  {
    message(errorMessage);
  }
  void Error::clear()
  {
    delete[] m_message;
    m_message = nullptr;
  }
  bool Error::isClear() const
  {
    return !(m_message);
  }
  void Error::message(const char * value)
  {
    if(!isClear())
    clear();
    m_message = new char[strlen(value) + 1];
    strcpy(m_message, value);

  }
  Error::operator const char*() const
  {
    return m_message;
  }
  Error::operator bool() const
  {
    return isClear();
  }
  std::ostream & operator<<(std::ostream & os, const Error & op)
  {
    if (!op.isClear())
    {
      os << static_cast<const char*>(op);
    }
    return os;
  }
}