/*! \file repl_hooks.cpp
 * \brief Υποσύστημα repl.
 */
#include "repl_hooks.h"
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
  std::string prefix{context.substr(static_cast<unsigned long>(index))};
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