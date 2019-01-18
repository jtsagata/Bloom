#include <iostream>
#include <locale>

#include "pasaTempo.h"
#include "repl.h"

const char *history_file = ".bloomsh.hist";

/**
 * A clock to measure program execution time
 */
pasaTempo *program_clock;

void welcome();

/**
 * \brief an exit handler to show total run time.
 */
void exit_handler_time() {
  // Better formatting.
  std::locale::global(std::locale("en_GB.UTF-8"));
  std::cout.imbue(std::locale());
  program_clock->stop();
  std::cout << "Total run time " << std::fixed << program_clock->getMS() << " ms." << std::endl;
}

int main(int argc, char **argv) {
  program_clock = new pasaTempo{};
  program_clock->start();

  std::atexit(exit_handler_time);

  std::locale::global(std::locale(""));
  std::cout.imbue(std::locale());

  Repl eval_loop{history_file, "$ "};

  for (char **a = argv; *a; ++a) {
    char *arg = *a;
    if (strcmp(argv[0], arg) != 0) {
      auto res = eval_loop.run_script(arg);
      // Some script called !quit
      if (res == false) {
        return EXIT_SUCCESS;
      }
    }
  }
  welcome();

  eval_loop.run();

  return EXIT_SUCCESS;
}

void welcome() {
  std::cout << "Welcome to bshell, an interactive scripted bloom filter demo\n";
  std::cout << "Type !help to see the available connands.\n\n";
}
