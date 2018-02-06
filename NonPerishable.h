#ifndef ICT_NONPERISHABLE_H__
#define ICT_NONPERISHABLE_H__
#include "Error.h"
#include "Item.h"
namespace ict
{
  class NonPerishable:public Item
  {
  private:
    Error m_err;
  protected:
    bool ok()const;
    void error(const char* message);
    virtual char signature()const;
  public:
    virtual std::fstream& save(std::fstream& file)const;
    virtual std::fstream& load(std::fstream& file);
    virtual std::ostream& write(std::ostream& os, bool linear)const;
    virtual std::istream& read(std::istream& is);
  };

}
#endif