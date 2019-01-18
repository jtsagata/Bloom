/*! \file repl_commands.h
 * \brief Υλοποιηση των εντολών.
 */
#ifndef BLOOM_REPL_COMMANDS_H
#define BLOOM_REPL_COMMANDS_H
#include <string>
#include <vector>

void welcome();
void repl();

bool help_cmd(const std::string &input);
bool init_cmd(const std::string &input);
bool add_cmd(const std::string &input);
bool stats_cmd(const std::string &input);
bool bits_cmd(const std::string &input);
bool load_cmd(const std::string &input);
bool save_cmd(const std::string &input);
bool examine_cmd(const std::string &input);
bool train_cmd(const std::string &input);
bool verbose_cmd(const std::string &input);
bool script_cmd(const std::string &input);

bool check_cmd(const std::string &input);

bool do_cmd(const std::string &input);

bool run_script(const std::basic_string<char> &fname);

#endif // BLOOM_REPL_COMMANDS_H
