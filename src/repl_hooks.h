/*! \file repl_hooks.h
 * \brief Υποσύστημα repl.
 */

#ifndef BLOOM_REPL_HOOKS_H
#define BLOOM_REPL_HOOKS_H

#include <regex>
#include <replxx.hxx>
using Replxx = replxx::Replxx;

// prototypes
Replxx::completions_t hook_completion(std::string const &context, int index, void *user_data);
Replxx::hints_t hook_hint(std::string const &context, int index, Replxx::Color &color,
                          void *user_data);
void hook_color(std::string const &str, Replxx::colors_t &colors, void *user_data);

#endif // BLOOM_REPL_HOOKS_H
