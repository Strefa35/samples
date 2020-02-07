// Example program
#include <iostream>
#include <string>

int main()
{
  unsigned int m_value, d_value;
  std::string b_value = "";
  
  std::cout << "Enter DEC value: ";
  std::cin >> d_value;
  
  std::cout << "VALUE: " << d_value << "\n\r";
  
  m_value = d_value;
  
  while(1)
  {
    unsigned int next = d_value / 2;
    
    std::cout << "NEXT: " << next << "\n\r";
    if (d_value % 2)
    {
        //std::cout << "1" << "\n\r";
        b_value.insert(0, "1");
    }
    else
    {
        //std::cout << "0" << "\n\r";
        b_value.insert(0, "0");
    }
    
    d_value = next;
    
    if (!d_value)
    {
       std::cout << "=== STOP ===" << "\n\r";
       break;
    }
  }
  
  std::cout << "Convert DEC=" << m_value << " to BIN=" << b_value.c_str() << "\n\r";
  
  return 0;
  
}
