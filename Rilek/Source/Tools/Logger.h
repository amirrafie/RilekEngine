#pragma once
#include "spdlog/spdlog.h"
#include <memory>


#ifdef _DEBUG
#define RLK_TRACE(...) ::Rilek::Logger::GetConsoleLogger()->trace(__FILE__ " Line {0}", __LINE__); ::Rilek::Logger::GetConsoleLogger()->trace(__VA_ARGS__); ::Rilek::Logger::GetFileLogger()->trace("[TRACE] " __FILE__ " Line {0}", __LINE__); ::Rilek::Logger::GetFileLogger()->trace(__VA_ARGS__)

#define RLK_INFO(...) ::Rilek::Logger::GetConsoleLogger()->info(__FILE__ " Line {0}", __LINE__); ::Rilek::Logger::GetConsoleLogger()->info(__VA_ARGS__); ::Rilek::Logger::GetFileLogger()->info("[INFO] " __FILE__ " Line {0}", __LINE__); ::Rilek::Logger::GetFileLogger()->info(__VA_ARGS__)

#define RLK_WARN(...) ::Rilek::Logger::GetConsoleLogger()->warn(__FILE__ " Line {0}", __LINE__); ::Rilek::Logger::GetConsoleLogger()->warn(__VA_ARGS__); ::Rilek::Logger::GetFileLogger()->warn("[WARNING] " __FILE__ " Line {0}", __LINE__); ::Rilek::Logger::GetFileLogger()->warn(__VA_ARGS__)

#define RLK_ERROR(...) ::Rilek::Logger::GetConsoleLogger()->error(__FILE__ " Line {0}", __LINE__); ::Rilek::Logger::GetConsoleLogger()->error(__VA_ARGS__); ::Rilek::Logger::GetFileLogger()->error("[ERROR] " __FILE__ " Line {0}", __LINE__); ::Rilek::Logger::GetFileLogger()->error(__VA_ARGS__)

#else // Release
#define RLK_TRACE(...)
#define RLK_WARN(...)
#define RLK_INFO(...)
#define RLK_ERROR(...)

#endif


namespace Rilek
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