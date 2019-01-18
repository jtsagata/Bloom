/*! \file repl.cpp
 * \brief Κλάση Ρepl υλοποίηση.
 */

#include "repl.h"

#include <iomanip>

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "utils.h"

using boost::lexical_cast;
using std::map;
using std::pair;
using std::size_t;
using std::string;
using std::vector;

using Replxx = replxx::Replxx;
using color = Replxx::Color;

class Repl;
typedef bool (Repl::*CMD_PTR)(const string &);
using FcnMap = std::map<string, CMD_PTR>;
using FcnMap_pair = std::pair<std::string, CMD_PTR>;

void Repl::run() {
  char const *cinput{nullptr};
  for (;;) {
    do {
      cinput = rx.input(prompt);
    } while ((cinput == nullptr) && (errno == EAGAIN));

    if (cinput == nullptr)
      break;

    // Exec the line
    if (!eval_line(cinput))
      break;
  } // forever
  rx.history_save(historyFile);
}

Repl::Repl(const string &history, const string &prompt, size_t nBits, size_t nHashes)
    : prompt(prompt), historyFile{history}, bloom{nBits, nHashes} {

  // Configure historyFile
  rx.history_load(history);
  rx.set_max_history_size(12);
  rx.set_max_line_size(128);
  rx.set_max_hint_rows(8);

  // Configure commands
  register_cmd("!help", &Repl::help_cmd, "Help about commands", color::BRIGHTGREEN);
  register_cmd("!history", &Repl::history_cmd, "Show the historyFile", color::BRIGHTGREEN);
  register_cmd("!exit", &Repl::quit_cmd, "Exit program", color::BRIGHTGREEN);
  register_cmd("!quit", &Repl::quit_cmd, "Exit program", color::BRIGHTGREEN);
  register_cmd("!verbose", &Repl::verbose_cmd, "Set verbose mode", color::BRIGHTGREEN);
  register_cmd("!init", &Repl::init_cmd, "(Re)Initialize Bloom filter", color::BRIGHTGREEN);
  register_cmd("!design", &Repl::design_cmd, "Design Bloom filter", color::BRIGHTBLUE);
  register_cmd("!stats", &Repl::stats_cmd, "Bloom filter statistics", color::BRIGHTGREEN);
  register_cmd("!add", &Repl::add_cmd, "\tAdd words to bloom filter", color::BRIGHTGREEN);
  register_cmd("!bits", &Repl::bits_cmd, "Show bloom filter bits", color::BRIGHTGREEN);
  register_cmd("!import", &Repl::import_cmd, "Import words from files", color::BRIGHTGREEN);
  register_cmd("!check", &Repl::check_cmd, "Check words", color::BRIGHTGREEN);
  register_cmd("!nbits", &Repl::nbits_cmd, "Debug mixer", color::BRIGHTGREEN);
  register_cmd("!run", &Repl::run_cmd, "\t Run script", color::BRIGHTGREEN);

  rx.install_window_change_handler();
  rx.set_completion_callback(hook_completion, static_cast<void *>(&commands));
  rx.set_hint_callback(hook_hint, static_cast<void *>(&commands));
  rx.set_highlighter_callback(hook_color, static_cast<void *>(&regex_color));

  // Verbose mode
  verboseMode = set_verbose_mode();
  bloom.setVerbose(verboseMode);
}

void Repl::register_cmd(const string &cmd, CMD_PTR ptr, const string &helpText,
                        const color &color1) {
  // Insert command to function table.
  map_fcn.insert(FcnMap_pair{cmd, ptr});

  commands.emplace_back(cmd);

  // Register help text
  help_text.emplace_back(cmd, helpText);

  // Register regexp color
  regex_color.emplace_back(string("\\") + cmd, color1);
}

bool Repl::eval_line(const string &line, bool doHistory) {
  if (boost::starts_with(line, "#"))
    return true;

  if (boost::starts_with(line, "@")) {
    std::vector<std::string> words = getTokens(line);
    if (words.empty())
      return true;
    auto histLine = words[0];
    histLine.replace(0, 1, "");
    int n = 0;
    try {
      n = lexical_cast<int>(histLine);
    } catch (...) {
      return true;
    }
    if (n - 1 < rx.history_size()) {
      const auto cmdHist = rx.history_line(n - 1);
      std::cout << cmdHist << "\n";
      return eval_line(cmdHist);
    }
  }

  if (boost::starts_with(line, "!"))
    return eval_cmd_line(line, true);

  // fallback to check command
  std::vector<std::string> tokens = getTokens(line, fullTokens, true);
  do_check_cmd(tokens);

  return true;
}

bool Repl::eval_cmd_line(const string &line, bool doHistory) {
  std::vector<std::string> words = getTokens(line, fullTokens);
  if (words.empty())
    return true;

  auto cmd = words[0];
  auto it = map_fcn.find(cmd);
  if (it != map_fcn.end()) {
    if (doHistory) {
      rx.history_add(line);
    }
    CMD_PTR commandFn = it->second;
    return (this->*commandFn)(line);
  } else {
    reportError(string{"Bad Command : "} + cmd);
  }
  // keep going
  return true;
}

bool Repl::help_cmd(const std::string &input) {
  std::cout << "Commands:\n";
  for (const auto &it : help_text) {
    std::cout << "   " << it.first << "\t" << it.second << "\n";
  }
  return true;
}

bool Repl::history_cmd(const std::string &input) {
  for (int i = 0, sz = rx.history_size(); i < sz; ++i) {
    std::cout << std::setw(4) << i << ": " << rx.history_line(i) << "\n";
  }
  return true;
}

bool Repl::quit_cmd(const std::string &input) { return false; }

bool Repl::init_cmd(const std::string &input) {
  std::vector<std::string> tokens = getTokens(input, fullTokens);
  bool proceed = true;
  if (tokens.size() < 3)
    proceed = false;
  size_t n = 0, m = 0;
  try {
    n = lexical_cast<size_t>(tokens[1]);
    m = lexical_cast<size_t>(tokens[2]);
  } catch (...) {
    proceed = false;
  }
  if (!proceed) {
    reportUsage("!init <number of bits> <number of hashes>");
    return true;
  }

  bloom = BloomFilter(n, m);
  if (verboseMode) {
    eval_cmd_line("!stats", false);
  }
  return true;
}

bool Repl::design_cmd(const std::string &input) {
  std::vector<std::string> tokens = getTokens(input, simpleTokens);
  size_t n = 0;
  double P = 0.001;

  bool proceed = true;
  if (tokens.size() <= 1)
    proceed = false;

  if (tokens.size() >= 2) {
    try {
      n = lexical_cast<size_t>(tokens[1]);
    } catch (...) {
      proceed = false;
    }
  }

  if (tokens.size() >= 3) {
    try {
      auto s = tokens[2];
      P = stod(s);
    } catch (...) {
      proceed = false;
    }
  }

  if (!proceed) {
    reportUsage("!design <word capacity> [Propability]");
    return true;
  }

  if (P <= 0)
    P = 0.001;

  bloom = BloomFilter(n, P);
  if (verboseMode) {
    eval_cmd_line("!stats", false);
  }
  return true;
}

bool Repl::stats_cmd(const std::string &input) {
  std::cout << "   Bits   : " << bloom.getSize() << std::endl;
  std::cout << "   Memory : " << convertSize(bloom.getSize()) << std::endl;
  std::cout << "   Hashes : " << bloom.getNumHashes() << std::endl;
  std::cout << "Num items : " << bloom.getCount() << std::endl;
  std::cout << " Fullness : " << bloom.fullness() * 100 << "%" << std::endl;
  std::cout << "False Pos ~ " << bloom.estimateP() << std::endl;
  return true;
}

bool Repl::add_cmd(const std::string &input) {
  std::vector<std::string> tokens = getTokens(input, fullTokens, true);

  for (auto &token : tokens) {
    bloom.add(token);
  }
  return true;
}

bool Repl::verbose_cmd(const std::string &input) {
  std::vector<std::string> tokens = getTokens(input, fullTokens);
  if (tokens.size() > 1) {
    auto v = tokens[1];
    boost::to_upper(v);
    verboseMode = (v == "1") || (v == "ON") || (v == "TRUE");
    bloom.setVerbose(verboseMode);
  }
  std::cout << "Verbose mode: " << (verboseMode ? "ON" : "OFF") << "\n";

  return true;
}

bool Repl::bits_cmd(const std::string &input) {
  const auto &bits = bloom.getMBits();
  std::ostringstream ss;
  for (auto i = 0; i < bits.size(); i++) {
    ss << (bits[i] ? "1" : "0");
  }
  std::cout << ss.str() << "\n";
  return true;
}

bool Repl::import_cmd(const std::string &input) {
  std::vector<std::string> tokens = getTokens(input, simpleTokens, true);

  for (auto &token : tokens) {
    if (fnameIsReadable(token)) {
      if (verboseMode)
        std::cout << (string{"Importing words form "} + token + ".\n");
      bloom.populate(token);
    } else {
      if (verboseMode) {
        reportError(string{"Filename :"} + token + " is not readable");
      }
    }
  }
  if (verboseMode) {
    eval_cmd_line("!stats", false);
  }
  return true;
}

bool Repl::check_cmd(const std::string &input) {
  std::vector<std::string> tokens = getTokens(input, fullTokens, true);
  return do_check_cmd(tokens);
}

bool Repl::do_check_cmd(const vector<string> &tokens) {
  vector<string> bad_words;

  for (auto &word : tokens) {
    if (!bloom.notHave(word)) {
      bad_words.emplace_back(word);
    }
  }

  if (!bad_words.empty()) {
    auto txt = toStringList(bad_words, ",", "'", "\n");
    std::cout << "Positives: ";
    std::cout << txt;
  } else {
    if (verboseMode) {
      std::cout << "OK\n";
    }
  }
  return true;
}

bool Repl::nbits_cmd(const std::string &input) {
  std::vector<std::string> tokens = getTokens(input, fullTokens, true);

  for (const auto &token : tokens) {
    auto m = Mixer{token, bloom.getSize()};
    std::ostringstream ss;
    ss << token << "\t: ";
    for (auto i = 0; i < bloom.getNumHashes(); i++) {
      ss << m() << " ";
    }
    std::cout << ss.str() << "\n";
  }

  return true;
}

bool Repl::run_cmd(const std::string &input) {
  std::vector<std::string> tokens = getTokens(input, simpleTokens, true);
  return do_run_cmd(tokens);
}

bool Repl::do_run_cmd(const std::vector<std::string> &tokens) {
  for (const auto &token : tokens) {
    auto res = run_script(token);
    if (res == false)
      return false;
  }
  return true;
}

bool Repl::run_script(const string &filename) {
  if (fnameIsReadable(filename)) {
    std::ifstream in(filename);
    if (verboseMode) {
      std::cout << (string{"Execute script from :"} + filename + ".\n");
    }
    string line;
    while (getline(in, line)) {
      if (verboseMode) {
        // std::cout << prompt << " ";
        std::cout << line << "\n";
      }
      auto res = eval_line(line, false);
      // handle quit
      if (res == false)
        return false;
    }
  } else {
    if (verboseMode) {
      reportError(string{"Filename "} + filename + " is not readable");
    }
  }
  return true;
}