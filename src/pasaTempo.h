#ifndef BLOOM_PASATEMPO_H
#define BLOOM_PASATEMPO_H

/*! \file pasaTempo.h
 * \brief Ορισμός της κλάσης pasaTempo.
 */

#include <chrono>
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::time_point;
using timePointMS = time_point<high_resolution_clock>;

/**
 * \class pasaTempo
 * \brief A simple class to measure timings in ns.
 */
class pasaTempo {
public:
  explicit pasaTempo() { start(); }

  void start() {
    mStart = high_resolution_clock::now();
    mDuration = 0;
    isStopped = false;
  }

  long stop() {
    if (!isStopped) {
      mEnd = high_resolution_clock::now();
      mDuration = duration_cast<std::chrono::milliseconds>(mEnd - mStart).count();
      isStopped = true;
    }
    return getMS();
  }

  long getMS() { return duration_cast<std::chrono::milliseconds>(mEnd - mStart).count(); }
  long getNS() { return duration_cast<std::chrono::nanoseconds>(mEnd - mStart).count(); }
  long getSEC() { return duration_cast<std::chrono::seconds>(mEnd - mStart).count(); }

private:
  timePointMS mStart{};
  timePointMS mEnd{};
  long mDuration{};
  bool isStopped{};
};

#endif // BLOOM_PASATEMPO_H
