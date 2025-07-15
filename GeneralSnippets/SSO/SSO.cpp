// =====================================================================================
// SSO.cpp // SSO (Small String Optimization)
// =====================================================================================

module modern_cpp:sso;

void main_sso()
{
  //  std::string s{"ABC"};

    auto size{ sizeof(std::string) };
    auto capacity{ std::string{}.capacity() };
    auto small { std::string(capacity, '*') };
    auto big{ std::string(capacity + 1, '#') };

    std::cout << "sizeof  : " << size << std::endl;
    std::cout << "Capacity: " << capacity << std::endl;
    std::cout << "Small   : " << small.capacity() << ": " << small << std::endl;
    std::cout << "Big     : " << big.capacity() << ": " << big << std::endl;
}

void main_fragen()
{
    auto s1        = "111111111111111111111111111111111111111111111111";

    const char* s2 = "111111111111111111111111111111111111111111111111";

   //  s2[0] = '!';

    std::string s3 = "111111111111111111111111111111111111111111111111";
}

// =====================================================================================
// End-of-File
// =====================================================================================
