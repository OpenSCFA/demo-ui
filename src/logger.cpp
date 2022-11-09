#include <windows.h>
#include <iomanip>
#include <iostream>

#include "gsl/gsl"
#include "logger.hpp"

using namespace std;
using namespace gsl;


Logger::Logger(string filename): output{filename} {}

void Logger::set_output(string filename) {
    this->output = ofstream{filename};
}

Logger& operator<<(Logger& log, void* ptr) {
    // copy & revert state
    std::ios state{nullptr};
    state.copyfmt(log.output);
    log << "0x" << hex << uppercase << setw(8) << setfill('0') << ptr;
    log.output.copyfmt(state);
    return log;
}
