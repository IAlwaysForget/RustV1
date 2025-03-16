#include "Logging.h"

#include <ctime>
#include <iostream>
#include <assert.h>

#include "context.h"
#include <Windows.h>

template <typename... Args>
std::string c_logger::string_sprintf(const char* format, Args... args)
{
	int length = std::snprintf(nullptr, 0, format, args...);
	assert(length >= 0);

	char* buf = new char[length + 1];
	std::snprintf(buf, length + 1, format, args...);

	std::string str(buf);
	delete[] buf;
	return str;
}

void c_logger::output(LogLevel level, const char* format, va_list args)
{
	std::time_t t = std::time(nullptr);
	std::tm tm;
	std::string log = "";

	localtime_s(&tm, &t);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 3);
	std::ostringstream oss;
	oss << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "]";
	log = oss.str();
	std::cout << "[" << log << "] ";

	switch (level)
	{
	case LogLevel::SUCCESS:
		SetConsoleTextAttribute(hConsole, LogColors::FG_GREEN);
		std::printf("[SUCCESS] ");
		log += "[SUCCESS] ";
		break;
	case LogLevel::INFO:
		SetConsoleTextAttribute(hConsole, LogColors::FG_LIGHTGREEN);
		std::printf("[INFO] ");
		log += "[INFO] ";
		break;
	case LogLevel::WARNING:
		SetConsoleTextAttribute(hConsole, LogColors::FG_BROWN);
		std::printf("[WARNING] ");
		log += "[WARNING] ";
		break;
	case LogLevel::ERR:
		SetConsoleTextAttribute(hConsole, LogColors::FG_RED);
		std::printf("[ERROR] ");
		log += "[ERROR] ";
		break;
	case LogLevel::LUAA:
		SetConsoleTextAttribute(hConsole, LogColors::FG_LIGHTCYAN);
		std::printf("[LUA] ");
		log += "[LUA] ";
		break;
	}

	SetConsoleTextAttribute(hConsole, 15);
	std::vprintf(format, args);
	log += this->string_sprintf(format, args);
}

//Fuck yourself Geisha it's Succes(s) silent S
void c_logger::Succes(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	output(LogLevel::SUCCESS, format, args);
	va_end(args);
}

void c_logger::Error(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	output(LogLevel::ERR, format, args);
	va_end(args);
}

void c_logger::Warning(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	output(LogLevel::WARNING, format, args);
	va_end(args);
}

void c_logger::Info(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	output(LogLevel::INFO, format, args);
	va_end(args);
}

void c_logger::Lua(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	output(LogLevel::LUAA, format, args);
	va_end(args);
}
