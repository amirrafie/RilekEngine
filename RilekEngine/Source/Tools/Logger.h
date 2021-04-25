#pragma once
#include "spdlog/spdlog.h"
#include <memory>


// Logging levels
#define DO_NOT_DISPLAY_FILE_AND_LINE 0
#define ONLY_SAVE_FILE_AND_LINE_TO_TXT 1
#define DISPLAYT_FILE_AND_LINE 2


#define LOGGING_MODE ONLY_SAVE_FILE_AND_LINE_TO_TXT


#ifdef _DEBUG

#if (LOGGING_MODE == DO_NOT_DISPLAY_FILE_AND_LINE)
#define RLK_TRACE(...) ::Rilek::Tools::Logger::GetConsoleLogger()->trace(__VA_ARGS__); ::Rilek::Tools::Logger::GetFileLogger()->trace(__VA_ARGS__)
#define RLK_INFO(...) ::Rilek::Tools::Logger::GetConsoleLogger()->info(__VA_ARGS__); ::Rilek::Tools::Logger::GetFileLogger()->info(__VA_ARGS__)
#define RLK_WARN(...) ::Rilek::Tools::Logger::GetConsoleLogger()->warn(__VA_ARGS__); ::Rilek::Tools::Logger::GetFileLogger()->warn(__VA_ARGS__)
#define RLK_ERROR(...) ::Rilek::Tools::Logger::GetConsoleLogger()->error(__VA_ARGS__); ::Rilek::Tools::Logger::GetFileLogger()->error(__VA_ARGS__)

#elif (LOGGING_MODE == ONLY_SAVE_FILE_AND_LINE_TO_TXT)
#define RLK_TRACE(...) ::Rilek::Tools::Logger::GetConsoleLogger()->trace(__VA_ARGS__); ::Rilek::Tools::Logger::GetFileLogger()->trace("[TRACE] " __FILE__ " Line {0}", __LINE__); ::Rilek::Tools::Logger::GetFileLogger()->trace(__VA_ARGS__)
#define RLK_INFO(...) ::Rilek::Tools::Logger::GetConsoleLogger()->info(__VA_ARGS__); ::Rilek::Tools::Logger::GetFileLogger()->info("[INFO] " __FILE__ " Line {0}", __LINE__); ::Rilek::Tools::Logger::GetFileLogger()->info(__VA_ARGS__)
#define RLK_WARN(...) ::Rilek::Tools::Logger::GetConsoleLogger()->warn(__VA_ARGS__); ::Rilek::Tools::Logger::GetFileLogger()->warn("[WARNING] " __FILE__ " Line {0}", __LINE__); ::Rilek::Tools::Logger::GetFileLogger()->warn(__VA_ARGS__)
#define RLK_ERROR(...) ::Rilek::Tools::Logger::GetConsoleLogger()->error(__VA_ARGS__); ::Rilek::Tools::Logger::GetFileLogger()->error("[ERROR] " __FILE__ " Line {0}", __LINE__); ::Rilek::Tools::Logger::GetFileLogger()->error(__VA_ARGS__)

#elif (LOGGING_MODE == DISPLAY_FILE_AND_LINE)
#define RLK_TRACE(...) ::Rilek::Tools::Logger::GetConsoleLogger()->trace(__FILE__ " Line {0}", __LINE__); ::Rilek::Tools::Logger::GetConsoleLogger()->trace(__VA_ARGS__); ::Rilek::Tools::Logger::GetFileLogger()->trace("[TRACE] " __FILE__ " Line {0}", __LINE__); ::Rilek::Tools::Logger::GetFileLogger()->trace(__VA_ARGS__)
#define RLK_INFO(...) ::Rilek::Tools::Logger::GetConsoleLogger()->info(__FILE__ " Line {0}", __LINE__); ::Rilek::Tools::Logger::GetConsoleLogger()->info(__VA_ARGS__); ::Rilek::Tools::Logger::GetFileLogger()->info("[INFO] " __FILE__ " Line {0}", __LINE__); ::Rilek::Tools::Logger::GetFileLogger()->info(__VA_ARGS__)
#define RLK_WARN(...) ::Rilek::Tools::Logger::GetConsoleLogger()->warn(__FILE__ " Line {0}", __LINE__); ::Rilek::Tools::Logger::GetConsoleLogger()->warn(__VA_ARGS__); ::Rilek::Tools::Logger::GetFileLogger()->warn("[WARNING] " __FILE__ " Line {0}", __LINE__); ::Rilek::Tools::Logger::GetFileLogger()->warn(__VA_ARGS__)
#define RLK_ERROR(...) ::Rilek::Tools::Logger::GetConsoleLogger()->error(__FILE__ " Line {0}", __LINE__); ::Rilek::Tools::Logger::GetConsoleLogger()->error(__VA_ARGS__); ::Rilek::Tools::Logger::GetFileLogger()->error("[ERROR] " __FILE__ " Line {0}", __LINE__); ::Rilek::Tools::Logger::GetFileLogger()->error(__VA_ARGS__)

#endif

#else // Release
#define RLK_TRACE(...)
#define RLK_WARN(...)
#define RLK_INFO(...)
#define RLK_ERROR(...)

#endif


namespace Rilek::Tools
{

	class Logger
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetConsoleLogger();
		static std::shared_ptr<spdlog::logger>& GetFileLogger();

	private:
		static std::shared_ptr<spdlog::logger> m_consoleLogger;
		static std::shared_ptr<spdlog::logger> m_fileLogger;

	};
	
}