#pragma once
#include <cstdarg>
#include <string>

typedef enum loglevel_t
{
	SUCCESS,
	INFO,
	WARNING,
	ERR,
	LUAA
} LogLevel;

/*FOREGROUND COLORS*/
typedef enum logcolors_t
{
	FG_BLACK = 0,
	FG_BLUE = 1,
	FG_GREEN = 2,
	FG_CYAN = 3,
	FG_RED = 4,
	FG_MAGENTA = 5,
	FG_BROWN = 6,
	FG_LIGHTGRAY = 7,
	FG_GRAY = 8,
	FG_LIGHTBLUE = 9,
	FG_LIGHTGREEN = 10,
	FG_LIGHTCYAN = 11,
	FG_LIGHTRED = 12,
	FG_LIGHTMAGENTA = 13,
	FG_YELLOW = 14,
	FG_WHITE = 15
} LogColors;

class c_logger
{
public:
	c_logger()
	{
	}

	~c_logger()
	{
	}

	void Succes(const char* format, ...);
	void Error(const char* format, ...);
	void Warning(const char* format, ...);
	void Info(const char* format, ...);
	void Lua(const char* format, ...);

private:
	/* Internal API to output information with color on console + write to file.*/
	void output(LogLevel level, const char* format, va_list args);

	/*https://learn.microsoft.com/en-us/answers/questions/813614/how-to-printf-to-std-string-by-using-of-a-function*/
	/*format -> Args to string.*/
	template <typename... Args>
	std::string string_sprintf(const char* format, Args... args);
};
