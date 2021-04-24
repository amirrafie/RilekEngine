#include "stdafx.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "Logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Rilek::Tools
{
	std::shared_ptr<spdlog::logger> Logger::m_consoleLogger;
	std::shared_ptr<spdlog::logger> Logger::m_fileLogger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^%D:[%T]: %v%$");
		m_consoleLogger = spdlog::stdout_color_mt("console");
		m_consoleLogger->set_level(spdlog::level::trace);

		// getting current date
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::basic_stringstream<char> date;
		date << std::put_time(std::localtime(&now), "%F");
		std::string dateString(date.str());

		std::string filepath = std::string("logs/") + dateString + std::string(".txt");

		m_fileLogger = spdlog::basic_logger_mt("file logger", filepath);
		m_fileLogger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger>& Logger::GetConsoleLogger()
	{
		return m_consoleLogger;
	}
	std::shared_ptr<spdlog::logger>& Logger::GetFileLogger()
	{
		return m_fileLogger;
	}
}

