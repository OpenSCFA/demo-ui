#ifndef LOGGER__H
#define LOGGER__H

#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>


class Logger;

Logger& operator<<(Logger& log, void* ptr);
template<typename T>
Logger& operator<<(Logger& log, T arg);

class Logger {
    friend Logger& operator<<(Logger&, void*);
    template<typename T>
    friend Logger& operator<<(Logger& log, T arg);
public:
    Logger(std::string filename);

    void set_output(std::string filename);

    template<typename... Ts>
    void log(Ts... args)
    {
        if (this->output.is_open()) {
            ((*this) << ... << args);
        }
    }

private:
    std::ofstream output;
};

template<typename T>
Logger& operator<<(Logger& log, T arg) {
    if (log.output.is_open()) {
        log.output << arg;
    }
    return log;
}


#endif