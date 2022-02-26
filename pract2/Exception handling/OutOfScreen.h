#pragma once

#include <exception>
#include <string>

class OutOfScreen : public std::exception {
public:
	OutOfScreen() : std::exception() {};
	OutOfScreen(char* message) : std::exception(message) {};
	OutOfScreen(std::string message) : std::exception(message.c_str()) {};
};