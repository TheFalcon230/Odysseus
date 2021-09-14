#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Odysseus
{
	class ODYSSEUS_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log Macros
#define ODC_CORE_TRACE(...) ::Odysseus::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ODC_CORE_INFO(...) ::Odysseus::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ODC_CORE_WARN(...) ::Odysseus::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ODC_CORE_ERROR(...) ::Odysseus::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ODC_CORE_FATAL(...) ::Odysseus::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log Macros
#define ODC_TRACE(...) ::Odysseus::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ODC_INFO(...) ::Odysseus::Log::GetClientLogger()->info(__VA_ARGS__)
#define ODC_WARN(...) ::Odysseus::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ODC_ERROR(...) ::Odysseus::Log::GetClientLogger()->error(__VA_ARGS__)
#define ODC_FATAL(...) ::Odysseus::Log::GetClientLogger()->fatal(__VA_ARGS__)

