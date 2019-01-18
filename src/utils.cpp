#include "utils.h"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>

using std::ostringstream;
using std::string;
using std::vector;

const char *simpleTokens = " \t\n";
const char *fullTokens = " ,.\n\t\\?/";

std::vector<string> getTokens(const string &input, const string &splitChars, bool skip_first) {
  std::vector<string> tokens;
  boost::algorithm::split(tokens, input, boost::is_any_of(splitChars),
                          boost::algorithm::token_compress_on);
  // Remove empty strings
  tokens.erase(
      std::remove_if(tokens.begin(), tokens.end(), [](const string &s) { return s.empty(); }),
      tokens.end());

  // remove First element
  if (skip_first && !tokens.empty()) {
    tokens.erase(tokens.begin());
  }

  return tokens;
}

string toStringList(const vector<string> &data, const string &separator, const string &quoter,
                    const string &concluder) {
  ostringstream ss;
  auto begin = data.begin();
  auto end = data.end();

  if (begin != end) {
    ss << quoter + (*begin++) + quoter;
  }

  while (begin != end) {
    ss << separator;
    auto quote = string(quoter).append(*begin++).append(quoter);
    ss << quote;
  }

  ss << concluder;
  return ss.str();
}

bool fnameIsReadable(const string &filename) {
  std::ifstream stream(filename);
  return stream.good();
}

bool fnameIsWriteable(const string &filename) {
  std::ofstream stream(filename);
  return stream.good();
}

void reportError(const std::string &error) { std::cout << "Error: " << error << std::endl; }

void reportUsage(const std::string &text) { std::cout << "Usage: " << text << std::endl; }

std::string convertSize(size_t size) {
  static const std::vector<std::string> units{"B", "KB", "MB", "GB", "TB"};
  auto u = units.begin();
  float fsize = size;
  while (fsize >= 1024 && u != units.end()) {
    fsize /= 1024;
    u++;
  }

  std::ostringstream out;
  out << std::setprecision(2) << fsize;
  return out.str() + *u;
}
