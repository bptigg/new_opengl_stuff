#include "Log.h"

#include <windows.h>
#include <conio.h>

const std::string Log::critical_message = "CRITICAL";
const std::string Log::error_message = "ERROR";
const std::string Log::warning_message = "WARNING";
const std::string Log::info_message = "INFO";
const std::string Log::debug_message = "DEBUG";
const std::string Log::variable_message = "VAR";
const std::string Log::default_message = "DEFAULT";



Log::Log()
{
	std::string current_time = get_current_time();

	if (std::filesystem::exists("logs") == FALSE)
	{
		std::cout << "Logs directory doesn't exist... creating one\n";
		std::filesystem::create_directory("logs");
		if (std::filesystem::exists("logs"))
			std::cout << "Created directory\n";
	}

	log_file = "logs/" + current_time + ".txt";

	std::stringstream ss;
	ss << "Logging file initialised: " << log_file;
}
void Log::output(const std::string& type, const char* message, const char* filepath, int line, Terminal_colours colour)
{
	log_lock.lock();

#ifdef _DEBUG

	set_terminal_colour(colour);
	std::cout << "[" << type << "] " << message;
	if (line != -1)
	{
		std::cout << ", " << filepath << " : " << line;
	}
	set_terminal_colour(Terminal_colours::Reset);
	std::cout << std::endl;

#endif 
	std::ofstream file;
	file.open(log_file, std::ios_base::app);
	file << get_current_time() << ": [" << type << "] " << message;
	if (line != -1)
	{
		file << " " << filepath << " : " << line;
	}
	file << std::endl;
	file.close();

	log_lock.unlock();
}

void Log::set_terminal_colour(Terminal_colours c)
{
	WORD colour = 7;
	switch (c) {
	case Terminal_colours::Critical:
		colour = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case Terminal_colours::Error:
		colour = FOREGROUND_RED;
		break;
	case Terminal_colours::Warning:
		colour = 14;
		break;
	case Terminal_colours::Debug:
		colour = FOREGROUND_GREEN;
		break;
	case Terminal_colours::Variable:
		colour = FOREGROUND_INTENSITY | 13;
		break;
	default:
		colour = 7;
		break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
}

std::string Log::get_current_time()
{
	time_t CT = time(0);
	struct tm CT_info;
	char string_buffer[80];

	localtime_s(&CT_info, &CT);

	strftime(string_buffer, sizeof(string_buffer), "%d.%m.%Y %H.%M.%S", &CT_info);
	return std::string(string_buffer);
}
#include "log.h"
