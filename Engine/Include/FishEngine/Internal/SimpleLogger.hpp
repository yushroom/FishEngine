#pragma once

#include <deque>
#include <string>

#include "LogType.hpp"
#include "../ReflectClass.hpp"

class LogViewModel;

namespace FishEngine
{
	struct LogData
	{
		LogType 	type;
		std::string message;
		std::string filePath;
		int 		line;
		std::string functionName;
	};

	class FE_EXPORT SimpleLogger
	{
	public:
		SimpleLogger( SimpleLogger const & ) = delete;
		SimpleLogger & operator=( SimpleLogger const & ) = delete;

		static SimpleLogger & GetInstance()
		{
			static SimpleLogger instance;
			return instance;
		}

		void Log(LogData && data)
		{
			m_data.emplace_back(data);
		}

	private:
		friend class ::LogViewModel;

		SimpleLogger() = default;

		std::deque<LogData> m_data;
	};
}
