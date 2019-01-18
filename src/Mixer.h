#ifndef BLOOM_MIXER_H
#define BLOOM_MIXER_H

/*! \file Mixer.h
 * \brief Ορισμός της κλάσης Mixer.
 */

#include <random>
#include <string>

/*!  \class Mixer
 * \brief Ένα συναρτησιακό για τον υπολογισμό των θέσεων
 *        σε ένα φίλτρο Bloom.
 *
 * Η κλάση Mixer είναι μια  συναρτησιακή κλάση που παράγει  uniformly
 * distributed random numbers αρχικοποιημένουςμε την τιμή Hash ενός κλειδιού.
 * Χρήσιμη για τον υπολογισμό των θέσεων σε φίλτρα Bloom.
 *
 * Παράδειγμα χρήσης:
 * \code
 *    std::size_t index = Mixer{"Viagra",10}();
 * \endcode
 *
 * H κατάσταση που αποθηκεύει είναι κατάσταση της γενήτριας των ψευδοτυχαίων
 * αριθμών. Η γενήτρια αυτή είναι μια κλασσική  linear_congruential_engine, η
 * απολούστερη δυνατή της τυπικής βιβλιοθήκης, με εξαιρετική απόδοση ώς προς τον
 * απαιτούμενο αποθηκευτικό χώρο για την αποθήκευση της κατάτσασης και τον
 * υπολογιστικό χρόνο. \f[ x = (\alpha \cdot x + c) \, \% \, m \f]
 *
 *
 * \example examples/mixer_demo.cpp
 *
 */
class Mixer {
public:
  /**
   * \brief Δημιούργησε το συναρτησιακό για μιά δεδομένη συμβολοσειρά κλειδί.
   *
   * Αλγόριθμος: Πρώτα υπολογισε την τιμή κερματισμού του κλειδίου με την
   * std::hash<std::string> και δημιουργησε μια γενήτρια τυχαίων αριθμών
   * αρχικοποιημένη με αυτή την τιμή.Σ
   *
   * @param val  Το κλειδί εισόδου
   * @param size Το μέγεθός του φίλτρου Bloom
   */
  Mixer(const std::string &val, size_t size) : mSize(size) {
    // Generate the hash value of the given key
    auto hashVal = mHashFn(val);
    // Initialize random number generator.
    mRandomFn = std::minstd_rand{hashVal};
  }

  /**
   * Επιστρέφει κάθε φορά που καλείτε μια θέση στον πίνακα bitvector.
   * Συνεχείς κλησεις της θα επιστρέψουν διαφορετικές τιμές, αλλά πάντα
   * με την ίδια σειρά για το ίδιο κείμενο.
   *
   *
   * @return Mια θέση στον πίνακα bitvector.
   */
  std::size_t operator()() { return mRandomFn() % mSize; }

private:
  size_t mSize{};                   // The max size
  std::minstd_rand mRandomFn{};     // The random number generator
  std::hash<std::string> mHashFn{}; // The hash function
};

#endif // BLOOM_MIXER_H
