/*! \file repl_utils.cpp
 * \brief Υποσύστημα repl.
 */
#include "repl_utils.h"
#include <boost/tokenizer.hpp>

using Tokeniizer = boost::tokenizer<>;

Replxx::completions_t hook_completion(std::string const &context, int index, void *user_data) {
  auto *examples = static_cast<std::vector<std::string> *>(user_data);
  Replxx::completions_t completions;

  std::string prefix{context.substr(index)};
  for (auto const &e : *examples) {
    if (e.compare(0, prefix.size(), prefix) == 0) {
      completions.emplace_back(e.c_str());
    }
  }

  return completions;
}

Replxx::hints_t hook_hint(std::string const &context, int index, Replxx::Color &color,
                          void *user_data) {
  auto *examples = static_cast<std::vector<std::string> *>(user_data);
  Replxx::hints_t hints;

  // only show hint if prefix is at least 'n' chars long
  // or if prefix begins with a specific character
  std::string prefix{context.substr(index)};
  if (prefix.size() >= 2 || (!prefix.empty() && prefix.at(0) == '.')) {
    for (auto const &e : *examples) {
      if (e.compare(0, prefix.size(), prefix) == 0) {
        hints.emplace_back(e.substr(prefix.size()).c_str());
      }
    }
  }

  // set hint color to green if single match found
  if (hints.size() == 1) {
    color = Replxx::Color::GREEN;
  }

  return hints;
}

void hook_color(std::string const &context, Replxx::colors_t &colors, void *user_data) {
  auto *regex_color = static_cast<std::vector<std::pair<std::string, Replxx::Color>> *>(user_data);

  // highlight matching regex sequences
  for (auto const &e : *regex_color) {
    size_t pos{0};
    std::string str = context;
    std::smatch match;

    while (std::regex_search(str, match, std::regex(e.first))) {
      std::string c{match[0]};
      pos += std::string(match.prefix()).size();

      for (size_t i = 0; i < c.size(); ++i) {
        colors.at(pos + i) = e.second;
      }

      pos += c.size();
      str = match.suffix();
    }
  }
}

std::string getStringList(const std::vector<std::string> &bad_words,
                          const std::string &sep = std::string{","}) {
  std::stringstream ss;
  bool first = true;
  for (const auto &s : bad_words) {
    if (!first)
      ss << sep;
    ss << s;
    first = false;
  }
  std::__cxx11::string res = ss.str();
  return res;
}

/**
 * \brief Split a string to words.
 * @param input
 * @return a vector of words.
 */
std::vector<std::string> getWordsVector(const std::string &input, bool skip_first) {
  Tokeniizer tok(input);
  auto words = std::vector<std::__cxx11::string>{};
  for (auto const &word : tok) {
    if (!skip_first)
      words.emplace_back(word);
    skip_first = false;
  }
  return words;
}
