#include "stdafx.h"
#include "Logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Rilek
{
	std::shared_ptr<spdlog::logger> Rilek::Logger::m_consoleLogger;
	std::shared_ptr<spdlog::logger> Rilek::Logger::m_fileLogger;

	void Rilek::Logger::Init()
	{
		spdlog::set_pattern("%^%D:[%T]: %v%$");
		m_consoleLogger = spdlog::stdout_color_mt("console");
		m_consoleLogger->set_level(spdlog::level::trace);

		m_fileLogger = spdlog::basic_logger_mt("basic_logger", "logs/" __DATE__ ".txt");
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

