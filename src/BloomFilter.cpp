/*! \file BloomFilter.cpp
 * \brief Υλοποίηση της κλάσης BloomFilter.
 */

#include "BloomFilter.h"

void BloomFilter::populate(const std::string &fname) {
  std::ifstream in(fname);
  if (!in.is_open())
    throw std::invalid_argument("Can't read file contents.");

  std::string line;
  while (getline(in, line)) {
    Tokeniizer tok(line);
    for (auto const &word : tok) {
      if (notHave(word)) {
        add(word);
        count++;
      }
    }
  }
}

double BloomFilter::estimateP(double N) {
  double k = mNumHashes;
  double m = mSize;
  double ek = -1.0 * k * N / m;
  double b = (1 - exp(ek));
  double res = pow(b, k);
  return res;
}

double BloomFilter::estimateP() { return BloomFilter::estimateP(count); }

const dynamic_bitset<> &BloomFilter::getMBits() const { return mBits; }

BloomFilter::BloomFilter(size_t nwords, double P) : count{0} {
  double m = ceil(-1.0 * nwords * log(P) / pow(log(2), 2));
  double k = ceil((m / nwords) * log(2));
  mBits = boost::dynamic_bitset<>{static_cast<size_t>(m)};
  mSize = static_cast<size_t>(m);
  mNumHashes = static_cast<size_t>(k);
}

void BloomFilter::add(const std::string &key) {
  auto mixer = Mixer(key, mSize);
  if (notHave(key)) {
    for (auto i = 0; i < mNumHashes; i++) {
      mBits.set(mixer());
    }
    count++;
    if (verbose) {
      std::cout << "added '" << key << "'\n";
    }
  } else {
    if (verbose) {
      std::cout << "skipped '" << key << "'\n";
    }
  }
}

bool BloomFilter::notHave(const std::string &key) {
  auto mixer = Mixer(key, mSize);
  bool res = true;
  for (auto i = 0; i < mNumHashes; i++) {
    res &= mBits[mixer()];
  }
  return !res;
}
