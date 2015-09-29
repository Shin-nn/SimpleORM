#ifndef SIMPLEROM_LOGGER_H_
#define SIMPLEROM_LOGGER_H_

#include <string>
#include <iostream>

namespace SimpleORM
{
	class Logger
	{
		public:
			virtual ~Logger() {}
			virtual void logInsert(const std::string& sql)=0;
			virtual void logRemove(const std::string& sql)=0;
			virtual void logSelect(const std::string& sql)=0;
			virtual void logUpdate(const std::string& sql)=0;
	};

	class SimpleLogger: public Logger
	{
		public:
			virtual void logInsert(const std::string& sql) override
			{
				std::cerr << sql << std::endl;
			}
			virtual void logRemove(const std::string& sql) override
			{
				std::cerr << sql << std::endl;
			}
			virtual void logSelect(const std::string& sql) override
			{
				std::cerr << sql << std::endl;
			}
			virtual void logUpdate(const std::string& sql) override
			{
				std::cerr << sql << std::endl;
			}
	};

}
#endif