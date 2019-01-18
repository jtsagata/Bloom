/*! \file repl_commands.cpp
 * \brief Υλοποιηση των εντολών.
 */

#include <fstream>
#include <iostream>
#include <vector>

#include "BloomFilter.h"
#include "repl_commands.h"
#include "repl_utils.h"

using Replxx = replxx::Replxx;

/**
 * \brief A starting bloom filter.
 */
BloomFilter bloom{100, 10};
std::string prompt{"\x1b[1;32mbloom\x1b[0m> "};
std::string prompt_script{"\x1b[1;32mscript\x1b[0m> "};

bool run_script(const std::basic_string<char> &fname);
/**
 * \brief The read eval print loop.
 */

void repl() {
  // init the repl
  Replxx rx;
  rx.install_window_change_handler();

  std::vector<std::string> commands{"!help", "!history", "!quit",  "!exit",  "!clear",
                                    "!add",  "!init",    "!stats", "!bits",  "!load",
                                    "!save", "!examine", "!train", "!script"};

  rx.set_completion_callback(hook_completion, static_cast<void *>(&commands));
  rx.set_hint_callback(hook_hint, static_cast<void *>(&commands));

  using cl = Replxx::Color;
  std::vector<std::pair<std::string, cl>> regex_color{
      // commands
      {"\\!help", cl::BRIGHTMAGENTA},  {"\\!history", cl::BRIGHTMAGENTA},
      {"\\!quit", cl::BRIGHTMAGENTA},  {"\\!exit", cl::BRIGHTMAGENTA},
      {"\\!clear", cl::BRIGHTMAGENTA}, {"\\!add", cl::BRIGHTBLUE},
      {"\\!init", cl::BRIGHTBLUE},     {"\\!add", cl::BRIGHTBLUE},
      {"\\!stats", cl::BRIGHTBLUE},    {"\\!bits", cl::BRIGHTBLUE},
      {"\\!load", cl::BRIGHTBLUE},     {"\\!save", cl::BRIGHTBLUE},
      {"\\!train", cl::BRIGHTBLUE},    {"\\!examine", cl::BRIGHTBLUE},
      {"\\!script", cl::BRIGHTBLUE},
  };
  rx.set_highlighter_callback(hook_color, static_cast<void *>(&regex_color));

  // the path to the history file
  std::string history_file{".bloom_history.txt"};

  // Configure
  rx.history_load(history_file);
  rx.set_max_history_size(12);
  rx.set_max_line_size(128);
  rx.set_max_hint_rows(8);

  for (;;) {
    // display the prompt and retrieve input from the user
    char const *cinput{nullptr};

    do {
      cinput = rx.input(prompt);
    } while ((cinput == nullptr) && (errno == EAGAIN));

    if (cinput == nullptr)
      break;

    std::string input{cinput};
    if (!input.empty())
      rx.history_add(input);
    if (do_cmd(input) == false)
      break;
  }
  rx.history_save(history_file);
}

bool do_cmd(const std::string &input) {
  if (input.compare(0, 5, "!quit") == 0 || input.compare(0, 5, "!exit") == 0) {
    return false;
  }

  if (input.compare(0, 5, "!help") == 0) {
    help_cmd(input);
    return true;
  }

  if (input.compare(0, 5, "!init") == 0) {
    init_cmd(input);
    return true;
  }

  if (input.compare(0, 4, "!add") == 0) {
    add_cmd(input);
    return true;
  }

  if (input.compare(0, 6, "!stats") == 0) {
    stats_cmd(input);
    return true;
  }

  if (input.compare(0, 5, "!bits") == 0) {
    bits_cmd(input);
    return true;
  }

  if (input.compare(0, 5, "!load") == 0) {
    load_cmd(input);
    return true;
  }

  if (input.compare(0, 5, "!save") == 0) {
    save_cmd(input);
    return true;
  }

  if (input.compare(0, 8, "!examine") == 0) {
    examine_cmd(input);
    return true;
  }

  if (input.compare(0, 6, "!train") == 0) {
    train_cmd(input);
    return true;
  }

  if (input.compare(0, 8, "!verbose") == 0) {
    return verbose_cmd(input);
  }

  if (input.compare(0, 7, "!script") == 0) {
    return script_cmd(input);
  }

  check_cmd(input);
  return true;
}

/**
 * \brief Display initial welcome message
 */
void welcome() {
  std::cout << "Welcome to BloomFilter demo\n"
            << "Press 'tab' to view autocompletions\n"
            << "Type '!help' for help\n"
            << "Type '!quit' or '!exit' to exit\n\n";
}

/**
 * \brief Repl command ''
 */
bool help_cmd(const std::string &input) {
  std::cout << "Help" << input << std::endl;
  return true;
}

/**
 * \brief Repl command ''
 */
bool init_cmd(const std::string &input) {
  std::cout << "Init" << input << std::endl;
  return true;
}

/**
 * \brief Repl command ''
 */
bool add_cmd(const std::string &input) {
  std::vector<std::string> words = getWordsVector(input);
  for (auto word : words) {
    bloom.add(word);
  }
  return true;
}

/**
 * \brief Repl command ''
 */
bool stats_cmd(const std::string &input) {
  std::cout << "Fullness: " << bloom.fullness() << std::endl;
  return true;
}

/**
 * \brief Repl command 'bits'
 *
 * Show bloom filter bits
 */
bool bits_cmd(const std::string &input) {
  std::cout << "Bits" << input << std::endl;
  return true;
}

/**
 * \brief Repl command 'load'
 *
 * Load a bloom filter from disk
 */
bool load_cmd(const std::string &input) {
  std::cout << "Load" << input << std::endl;
  return true;
}

/**
 * \brief Repl command 'save'
 *
 * REPL command to save a bloom filter to disk.
 */
bool save_cmd(const std::string &input) {
  std::cout << "Save" << input << std::endl;
  return true;
}

/**
 * \brief Repl command 'examine'
 *
 * REPL command to examine the contents of a file
 *
 * REPL usage:
 *   examine ../data/text1.txt
 */
bool examine_cmd(const std::string &input) {
  std::cout << "examine" << input << std::endl;
  return true;
}

/**
 * \brief Repl command ''
 */
bool train_cmd(const std::string &input) {
  std::cout << "train" << input << std::endl;
  return true;
}

/**
 * \brief Repl command ''
 */
bool verbose_cmd(const std::string &input) {
  std::cout << "Verbose" << input << std::endl;
  return true;
}

bool check_cmd(const std::string &input) {
  std::vector<std::string> words = getWordsVector(input, false);
  std::vector<std::string> bad_words;

  for (auto word : words)
    if (bloom.maybeHave(word))
      bad_words.emplace_back(word);

  if (!bad_words.empty()) {
    auto res = getStringList(bad_words, ", ");
    std::cout << "Possibly bad words in text: " << res << "." << std::endl;
    std::cout << "Check it again with a better but slower tool.\n";
  }
  return true;
}

bool script_cmd(const std::string &input) {
  std::vector<std::string> words;
  std::istringstream stream(input);
  std::copy(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>(),
            std::back_inserter(words));

  if (words.empty())
    return true;

  std::reverse(words.begin(), words.end());
  words.pop_back();
  auto fname = words.back();

  return run_script(fname);
}

bool run_script(const std::basic_string<char> &fname) {
  std::ifstream in(fname);
  if (!in.is_open()) {
    std::cout << "Can't read file contents from '" << fname << "'.\n";
    return true;
  }

  std::cout << "Running script :" << fname << "\n";
  std::__cxx11::string line;
  while (getline(in, line)) {
    std::cout << prompt_script << " " << line << std::endl;
    bool res = do_cmd(line);
    if (res == false)
      return false;
  }

  return true;
}
