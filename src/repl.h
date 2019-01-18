/*! \file repl.h
 * \brief Κλάση Ρepl ορισμοί.
 */
#ifndef BLOOM_REPL_H
#define BLOOM_REPL_H

#include <string>
#include <vector>

#include "BloomFilter.h"
#include "repl_hooks.h"

using Replxx = replxx::Replxx;
using color = Replxx::Color;

class Repl;
typedef bool (Repl::*CMD_PTR)(const std::string &);
using FcnMap = std::map<std::string, CMD_PTR>;
using FcnMap_pair = std::pair<std::string, CMD_PTR>;

class Repl {
private:
  std::string prompt;
  std::string historyFile;
  bool verboseMode;

  BloomFilter bloom;

  // Repl UI
  Replxx rx;
  FcnMap map_fcn;

  std::vector<std::string> commands;
  std::vector<std::pair<std::string, color>> regex_color;
  std::vector<std::pair<std::string, std::string>> help_text;

public:
  /**
   * \brief Construct the REPL object.
   * @param history The history file
   * @param prompt  The prompt string
   * @param nBits   Number of bit for the initial bloom filter
   * @param nHashes Number of hashed for the initial bloom filter
   */
  explicit Repl(const std::string &history, const std::string &prompt, size_t nBits = 60,
                size_t nHashes = 10);

  /**
   * \brief Do the read, eval, print loop.
   */
  void run();

  /**
   * \brief Register a command
   *
   * A lookup table with pointers to member functions as keys. This is tricky
   * C++ Also register a color and a help text.
   *
   * @param cmd      The name of the command (without '!')
   * @param ptr      The internal pointer to a mamber function of signature
   * CMD_PTR
   * @param helpText The help text
   * @param color1   The command color
   */
  void register_cmd(const std::string &cmd, CMD_PTR ptr, const std::string &helpText,
                    const color &color1 = color::WHITE);

  /**
   * \brief Evaluate a line
   *
   * Ignore comments. If the line contains a command, then run the command
   * otherwise analyze text as if command 'check' is given
   *
   * @param line
   * @return
   */
  bool eval_line(const std::string &line, bool doHistory = true);

  /**
   * \brief Evaluate a command line
   *
   * Find and execute the command. return the exit status to caller.
   *
   * @param line The input line
   * @return false on quit commands
   */
  bool eval_cmd_line(const std::string &line, bool doHistory = true);

  /**
   * \brief Implementaion of '!help' command
   */
  bool help_cmd(const std::string &input);

  /**
   * \brief Implementaion of '!history' command
   */
  bool history_cmd(const std::string &input);

  /**
   * \brief Implementaion of '!quit' command
   */
  bool quit_cmd(const std::string &input);

  /**
   * \brief Implementaion of 'init' command
   */
  bool init_cmd(const std::string &input);

  /**
   * \brief Implementaion of 'design' command
   */
  bool design_cmd(const std::string &input);

  /**
   * \brief Implementaion of 'stats' command
   */
  bool stats_cmd(const std::string &input);

  /**
   * \brief Implementaion of 'add' command
   */
  bool add_cmd(const std::string &input);

  /**
   * \brief Implementaion of 'verbose' command
   */
  bool verbose_cmd(const std::string &input);

  /**
   * \brief Implementaion of 'bits' command
   */
  bool bits_cmd(const std::string &input);

  /**
   * \brief Implementaion of 'import' command
   */
  bool import_cmd(const std::string &input);

  /**
   * \brief Implementaion of 'check' command
   */
  bool check_cmd(const std::string &input);

  /**
   * \brief Implementaion of 'nbits' command
   */
  bool nbits_cmd(const std::string &input);

  /**
   * \brief Implementaion of 'run' command
   */
  bool run_cmd(const std::string &input);

  bool set_verbose_mode() {
#ifndef NDEBUG
    return true;
#else
    return false;
#endif
  }

  const std::string &getPrompt() const { return prompt; }
  void setPrompt(const std::string &prompt) { Repl::prompt = prompt; }

  bool do_check_cmd(const std::vector<std::string> &tokens);
  bool do_run_cmd(const std::vector<std::string> &tokens);

  bool run_script(const std::string &filename);
};

#endif // BLOOM_REPL_H
