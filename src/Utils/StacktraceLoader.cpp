//
// Created by Shuhao Zhang on 19/07/2021.
//


#define CALLSTACK_MAX_SIZE 32

#include <Utils/backtrace.hpp>
#include <Utils/Logger.hpp>
#include <Utils/StacktraceLoader.hpp>
/**
 * @brief This methods collects the call stacks and prints is
 */
void collectAndPrintStacktrace() {
  backward::StackTrace stackTrace;
  backward::Printer printer;
  stackTrace.load_here(CALLSTACK_MAX_SIZE);
  std::stringbuf buffer;
  std::ostream os(&buffer);
  printer.print(stackTrace, os);
  SESAME_ERROR("Stacktrace:\n " << buffer.str());
}