#define _CRT_SECURE_NO_WARNINGS
#include "PosApp.h"

ict::PosApp::PosApp(const char * filename, const char * billfname)
{
  strncpy(m_filename, filename, 128);
  m_filename[127] = '\0';
  strncpy(m_billfname, billfname, 128);
  m_billfname[127] = '\0';
}

void ict::PosApp::run()
{
  loadRecs();
  int select = -1;
  while (select != 0)
  {
    std::cout << "The Sene-Store" << std::endl
      << "1- List items" << std::endl
      << "2- Add Perishable item" << std::endl
      << "3- Add Non-Perishable item" << std::endl
      << "4- Update item quantity" << std::endl
      << "5- Show Item" << std::endl
      << "6- POS" << std::endl
      << "0- exit program" << std::endl
      << "> ";
     select = menu();
     while (getchar() != '\n');
    if (select == 1)
    {
      listItems();
    }
    else if (select == 2)
    {
      addItem(true);
    }
    else if (select == 3)
    {
      addItem(false);
    }
    else if (select == 4)
    {
      updateQty();
    }
    else if (select == 5)
    {
      char sku[MAX_SKU_LEN + 1];
      std::cout << "Please enter the SKU: ";
      std::cin >> sku;
      int match = searchItems(sku);
      if (match == -1)
        std::cout << "Not found!" << std::endl;
      else
      {
        std::cout << "v-----------------------------------v" << std::endl;
        m_items[match]->write(std::cout, false);
        std::cout << "^-----------------------------------^" << std::endl;
      }
    }
    else if (select == 6)
    {
      POS();
    }
    else if (select == 0)
    {
      std::cout << "Goodbye!";
    }
    else
    {
      std::cout << "===Invalid Selection, try again===" << std::endl;
    }
    std::cout << std::endl;
  }
}

int ict::PosApp::menu()
{
  int select = -1;
  std::cin >> select;
  std::cout<< std::endl;
  if (std::cin.fail() || select < 0 || select>6)
  {
    std::cin.clear();
    std::cin.ignore(100, '\n');
    select = -1;
  }
  return select;
}

void ict::PosApp::deallocateItems()
{
  //delete[] m_items;
  m_noOfItems = 0;
}

void ict::PosApp::loadRecs()
{
  deallocateItems();
  std::fstream file;
  file.open(m_filename, std::ios::in);
  if (file.is_open())
  {
    while (!file.eof())
    {
      char sp;
      file >> sp;
      file.ignore(1);
      if (!file.eof())
      {
        if (sp == 'N')
        {
          m_items[m_noOfItems] = new NonPerishable;
          m_items[m_noOfItems]->load(file);
          m_noOfItems++;
        }
        else if (sp == 'P')
        {
          m_items[m_noOfItems] = new Perishable;
          m_items[m_noOfItems]->load(file);
          m_noOfItems++;
        }
        else
        {
          std::cout << "read Rec error" << std::endl;
        }
      }
    }
  }
  else
  {
    std::cerr << "Fail to open the file >" << m_filename << std::endl;
  } 
  file.clear();
  file.close();
}

void ict::PosApp::saveRecs()
{
  std::fstream file;
  file.open(m_filename, std::ios::out);
  for (int i =0;i< m_noOfItems;i++)
  {
    if (m_items[i] != nullptr)
    {
      m_items[i]->save(file);
      file << std::endl;
    }
  }
  m_noOfItems = 0;
  file.close();
  loadRecs();

}

int ict::PosApp::searchItems(const char * sku) const
{
  int match = 0;
  while (1)
  {
    if (match >= m_noOfItems)
    {
     
      match = -1;
      break;
    }
    if (*m_items[match] == sku)
    {
      break;
    }

    match++;
  }
  return match;
}

void ict::PosApp::updateQty()
{
  char sku[MAX_SKU_LEN + 1];
  std::cout << "Please enter the SKU: ";
  std::cin >> sku;
  int match = searchItems(sku);
  int add;
  if (match == -1)
  {
    std::cout << "Not found!" << std::endl;
  }
  else
  {
    m_items[match]->write(std::cout, false);
    std::cout << std::endl;
    std::cout << "Please enter the number of purchased items: ";
    std::cin >> add;
    std::cin.ignore(100, '\n');
    *m_items[match] += add;
    saveRecs();
    std::cout << "Updated!" << std::endl;
  }
}

void ict::PosApp::addItem(bool isPerishable)
{
  if (isPerishable)
  {
    m_items[m_noOfItems] = new Perishable;
    if (m_items[m_noOfItems]->read(std::cin).fail())
    {
      std::cin.clear();
      std::cin.ignore(100, '\n');
      std::cout << *m_items[m_noOfItems] << std::endl;
      delete m_items[m_noOfItems];
    }
    else
    {
      m_noOfItems++;
      std::cout << "Item added." << std::endl;
    }
  }
  else if (!isPerishable)
  {
    m_items[m_noOfItems] = new NonPerishable;
    if (m_items[m_noOfItems]->read(std::cin).fail())
    {
      std::cin.clear();
      std::cin.ignore(100, '\n');
      std::cout << *m_items[m_noOfItems] << std::endl;
      delete m_items[m_noOfItems];
    }
    else
    {
      m_noOfItems++;
      std::cout << "Item added." << std::endl;
    }
  }
}
void ict::PosApp::listItems() const
{
  double total = 0;
  std::cout << " Row | SKU    | Item Name          | Price |TX |Qty |   Total |" << std::endl;
  std::cout << "-----|--------|--------------------|-------|---|----|---------|" << std::endl;
  if (m_noOfItems)
  {
    for (int dsp = 0; dsp < m_noOfItems; dsp++)
    {
      std::cout <<std::setw(4)<<std::right<<std::setfill(' ')<< dsp + 1 << " | ";
      m_items[dsp]->write(std::cout, true);
      std::cout << std::endl;
      total += m_items[dsp]->cost()*m_items[dsp]->quantity();
    }
  }
  std::cout << "-----^--------^--------------------^-------^---^----^---------^" << std::endl;
  std::cout << "                               Total Asset: $  |       " <<std::setw(7)<<std::right
    <<std::setfill(' ')
    <<std::fixed<<std::setprecision(2)<< total << "|" << std::endl;
  std::cout << "-----------------------------------------------^--------------^" << std::endl;
}

void ict::PosApp::truncateBillFile()
{
  std::fstream file(m_billfname, std::fstream::out | std::fstream::trunc);
}

void ict::PosApp::showBill()
{
  std::fstream file;
  file.open(m_billfname, std::ios::in);
  Item* items[MAX_NO_ITEMS];
  int no = 0;
  double total = 0;
  if (file.is_open())
  {
       char sp;
      file >> sp;
      file.ignore(1);
    while (!file.eof())
    {
      if (sp == 'N')
      {
        items[no] = new NonPerishable;
        items[no]->load(file);
        no++;
      }
      else if (sp == 'P')
      {
        items[no] = new Perishable;
        items[no]->load(file);
        no++;
      }
      else
      {
        std::cout << "read Rec error" << std::endl;
      }
      file >> sp;
      file.ignore(1,'\n');
    }
  }
  else
  {
    std::cerr << "Fail to open the file >" << m_filename << std::endl;
  }
  std::cout << "v--------------------------------------------------------v" << std::endl;
  std::cout << "| 2017/04/02, 12:42                                      |" << std::endl;
  std::cout << "| SKU    | Item Name          | Price |TX |Qty |   Total |" << std::endl;
  std::cout << "|--------|--------------------|-------|---|----|---------|" << std::endl;

  for (int dsp = 0; dsp < no; dsp++)
  {
    std::cout << "| ";
    items[dsp]->write(std::cout, true);
    std::cout << std::endl;
    total += items[dsp]->cost();
  }
  std::cout << "^--------^--------------------^-------^---^----^---------^" << std::endl;
  std::cout << "|                               Total: $  |         "<<std::setw(5)
    <<std::right<<std::setfill(' ')<<std::fixed<<std::setprecision(2)<<total<<"|" << std::endl;
  std::cout << "^-----------------------------------------^--------------^" << std::endl;
  file.close();
  truncateBillFile();
}

void ict::PosApp::addToBill(Item& i)
{
  int tem = i.quantity();
  i.quantity(1);
  std::fstream file;
  file.open(m_billfname, std::ios_base::out|std::ios_base::app);
  if(file.is_open())
  i.save(file);
  file << std::endl;
  i.quantity(tem - 1);
  file.close();
}

void ict::PosApp::POS()
{
  while (1)
  {
    char sku[MAX_SKU_LEN + 1];
    sku[MAX_SKU_LEN] = '\0';
    std::cout << "Sku: ";
    int i = 0;
    while (i<=MAX_SKU_LEN)
    {
      std::cin.get(sku[i]);
      if (sku[i] == '\n')
      {
        if (i != 0)
        {
          sku[i] = '\0';
        }
        break;
      }
      i++;
    }
    if (sku[0]=='\n')
    {
      showBill();
      break;
    }
    else
    {
      int match = searchItems(sku);
      if (match == -1)
      {
        std::cout << "Not found!" << std::endl;
      }
      else
      {
        std::cout << "v------------------->" << std::endl;
        std::cout << "| " << m_items[match]->name() << std::endl;
        std::cout << "^------------------->" << std::endl;
        addToBill(*m_items[match]);
        if (m_items[match]->quantity() == 0)
        {
          delete m_items[match];
          m_items[match] = nullptr;
        }
        saveRecs();

      }
    }
  }
}
