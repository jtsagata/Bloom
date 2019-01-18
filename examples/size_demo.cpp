/**
 * \brief BloomFilter class demo
 *
 * Παράδειγμα χρήσης της κλάσης BloomFilter.
 */

#include "BloomFilter.h"
#include <iostream>

void test_word(const BloomFilter &bloom, const std::string &key) {
  std::cout << "Testing " << key << " : ";
  auto res = (bloom.maybeHave(key) ? "maybe yes" : "NO");
  std::cout << res << std::endl;
}

int main() {

  auto bloom1 = BloomFilter{100, 5};
  bloom1.populate("../data/cia_words.txt");

  test_word(bloom1, "Linux");
  test_word(bloom1, "Amiga");
  test_word(bloom1, "Windows");

  auto bloom2 = BloomFilter{100, 5};
  bloom1.populate("../data/cia_words.txt");

  test_word(bloom2, "Linux");
  test_word(bloom2, "Amiga");
  test_word(bloom2, "Windows");
}