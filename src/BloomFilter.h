#ifndef BLOOM_BLOOMFILTER_H
#define BLOOM_BLOOMFILTER_H

/*! \file BloomFilter.h
 * \brief Ορισμός της κλάσης BloomFilter.
 */

#include "Mixer.h"
#include <boost/dynamic_bitset.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>

using Tokeniizer = boost::tokenizer<>;
using boost::dynamic_bitset;

/*!
 * \class BloomFilter
 * \brief Υλοποιηση ενός απλού φίλτρου Bloom
 *
 * Η κλάση Mixer είναι μια  συναρτησιακή κλάση που παράγει  uniformly
 * distributed random numbers αρχικοποιημένουςμε την τιμή Hash ενός κλειδιού.
 * Χρήσιμη για τον υπολογισμό των θέσεων σε φίλτρα Bloom.
 *
 * \example examples/size_demo.cpp
 *
 */
class BloomFilter {
public:
  /**
   * \brief Bloom filter by size
   * @param size Number of bits
   * @param numHashes Number of hashes
   */
  explicit BloomFilter(size_t size = 80, size_t numHashes = 10)
      : mBits{size}, mSize{size}, mNumHashes{numHashes}, count{0} {}

  /**
   * \brief Design a bloom filter
   * @param nwords Capacity in words
   * @param P Propability of false possitives
   */
  explicit BloomFilter(size_t nwords = 1000, double P = 0.01);

  /**
   * Add a wprd to the floom filter
   * @param key The word to add
   */
  void add(const std::string &key);

  /**
   * \bried check NOT mebership
   * @param key
   * @return true if not have
   */
  bool notHave(const std::string &key);

  /**
   * \brief Get the percentage of free bits
   *
   * If a bloom filter have no zero bits its almost useless.
   *
   * @return The percentage of the free bits.
   */
  float fullness() const {
    return static_cast<float>(mBits.count()) / static_cast<float>(mBits.size());
  }

  /**
   * \brief Populate bloom filter from filename.
   *
   * Smart split the finame contents to words and add each word to the bloom
   * filter
   *
   * @param fname
   */
  void populate(const std::string &fname);

  /**
   * \brief Get the number of bit in the hash table.
   * @return The number of bits
   */
  size_t getSize() const { return mSize; }

  /**
   * \brief Get the number of hashes
   * @return The number of hashes
   */
  size_t getNumHashes() const { return mNumHashes; }

  /**
   * \brief Get an estimate of the number of words in the bloom filter
   * @return Estimate number of words
   */
  size_t getCount() const { return count; }

  /**
   * \brief Set the verbose flag
   * @param verbose
   */
  void setVerbose(bool verbose) { BloomFilter::verbose = verbose; }

  /**
   * \brief Get the false possitive propability of the filter
   * @return The false positives probability.
   */
  double estimateP();

  /**
   *\brief Get the false possitive propability of the filter
   * @param N The number of stored elements
   * @return The false positives probability.
   */
  double estimateP(double N);

  const dynamic_bitset<> &getMBits() const;

private:
  dynamic_bitset<> mBits;
  size_t mSize;
  size_t mNumHashes;
  size_t count;
#ifndef NDEBUG
  bool verbose = true;
#else
  bool verbose = false;
#endif
};

#endif // BLOOM_BLOOMFILTER_H
