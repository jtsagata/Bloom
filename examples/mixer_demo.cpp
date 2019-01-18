#include "Mixer.h"
#include <iostream>
#include <string>

int main() {

  auto text1 = std::string{"Viagra"};
  auto mixer1 = Mixer{text1, 10};
  std::cout << text1 << ": " << mixer1() << " " << mixer1() << " " << mixer1()
            << std::endl;

  auto text2 = std::string{"Viagra"};
  auto mixer2 = Mixer{text1, 10};
  std::cout << text2 << ": " << mixer2() << " " << mixer2() << " " << mixer2()
            << std::endl;

  auto text3 = std::string{"Anarchy"};
  auto mixer3 = Mixer{text3, 10};
  std::cout << text3 << ": " << mixer3() << " " << mixer3() << " " << mixer3()
            << std::endl;
}