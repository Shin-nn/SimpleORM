#ifndef SIMPLEORM_TERMINAL_LOGGER_H_
#define SIMPLEORM_TERMINAL_LOGGER_H_

#include "Logger.h"

namespace SimpleORM
{
	class TerminalLogger: public Logger
	{
		public:
			virtual void logInsert(const std::string& sql) override
			{
				std::cerr << "\033[1;31m" << sql << "\033[0m" << std::endl;
			}
			virtual void logRemove(const std::string& sql) override
			{
				std::cerr << "\033[1;31m" << sql << "\033[0m" << std::endl;
			}
			virtual void logSelect(const std::string& sql) override
			{
				std::cerr << "\033[1;31m" << sql << "\033[0m" << std::endl;
			}
			virtual void logUpdate(const std::string& sql) override
			{
				std::cerr << "\033[1;31m" << sql << "\033[0m" << std::endl;
			}
	};

}
#endif

