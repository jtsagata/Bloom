#ifndef BLOOM_UTILS_H
#define BLOOM_UTILS_H

/*! \file utils.h
 * \brief Βοηθητικές συναρτήσεις.
 */
#include <string>
#include <vector>

extern const char *fullTokens;
extern const char *simpleTokens;

std::vector<std::string> getTokens(const std::string &input,
                                   const std::string &splitChars = fullTokens,
                                   bool skip_first = false);

std::string toStringList(const std::vector<std::string> &data, const std::string &separator = ", ",
                         const std::string &quoter = "'", const std::string &concluder = "");

bool fnameIsReadable(const std::string &filename);
bool fnameIsWriteable(const std::string &filename);

void reportError(const std::string &error);
void reportUsage(const std::string &text);

std::string convertSize(size_t size);
#endif // BLOOM_UTILS_H