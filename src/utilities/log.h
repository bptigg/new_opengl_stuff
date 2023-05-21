#pragma once

#include <iostream>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include <chrono>
#include <ctime>

//Synchronisation
#include <thread>
#include <mutex>

class Log
{
public:
	enum class Terminal_colours
	{
		Reset = 0,
		Critical,
		Error,
		Warning,
		Info,
		Debug,
		Variable,
	};

private:
	Log();
	static const std::string critical_message;
	static const std::string error_message;
	static const std::string warning_message;
	static const std::string info_message;
	static const std::string debug_message;
	static const std::string variable_message;
	static const std::string default_message;

	std::string log_file;

	std::mutex log_lock;

	void output(const std::string& type, const char* message, const char* filepath, int line, Terminal_colours colour);
	void set_terminal_colour(Terminal_colours colour);
	static std::string get_current_time();

public:
	Log(const Log&) = delete;

	static Log& get()
	{
		static Log instance;
		return instance;
	}

	static void crit(const char* message, const char* file, int line)
	{
		get().output(critical_message, message, file, line, Terminal_colours::Critical);

#ifdef _DEBUG
		__debugbreak();
#endif
	}

	static void error(const char* message, const char* file, int line)
	{
		get().output(error_message, message, file, line, Terminal_colours::Error);
	}
	static void warning(const char* message)
	{
		get().output(warning_message, message, "", -1, Terminal_colours::Warning);
	}
	static void info(const char* message)
	{
		get().output(info_message, message, "", -1, Terminal_colours::Info);
	}
	static void debug(const char* message)
	{
		get().output(debug_message, message, "", -1, Terminal_colours::Debug);
	}

	template<typename T>
	static void variable(const char* name, T& var)
	{
		std::stringstream ss;
		ss << name << ": " << var;
		get().output(variable_message, ss.str().c_str(), "", -1, Terminal_colours::Variable);
	}
	template<typename T>
	static void variable(const char* name, T* var)
	{
		std::stringstream ss;
		ss << name << ": " << var;
		get().output(variable_message, ss.str().c_str(), "", -1, Terminal_colours::Variable);
	}

	static std::string current_time()
	{
		return get().get_current_time();
	}

};


