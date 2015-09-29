#ifndef SIMPLEORM_CONNECTION_H_
#define SIMPLEORM_CONNECTION_H_

#include "Exception.h"
#include "Row.h"
#include "Value.h"
#include "Logger.h"

#include <string>
#include <sqlite3.h> 
#include <functional>
#include <memory>
#include <vector>

namespace SimpleORM
{
	class ConnectionException : public std::exception
	{
		public:
			inline ConnectionException(const std::string &_except): except(_except) {}
			virtual const char* what() const noexcept override { return except.c_str(); }
		protected:
			std::string except;
	};

	class Connection
	{
		public:
			inline Connection(std::shared_ptr<Logger> l = std::make_shared<SimpleLogger>()):logger(l) {};
			inline virtual ~Connection() {}

			Connection(const Connection&) =delete;
			void operator=(const Connection&) =delete;

			virtual void select(const std::vector<std::string>& cols, const std::string& table,const std::string &sql, const std::vector<std::shared_ptr<ValueHandler>>& values,
				std::function<void(const Row&)>
			) =0;
			virtual long insert(const std::string& table, const std::vector<std::string>& cols,const std::vector<std::shared_ptr<ValueHandler>>& values) =0;
			virtual void update(const std::string& table, const std::vector<std::string>& cols,const std::vector<std::shared_ptr<ValueHandler>>& values, const std::string &where,const std::vector<std::shared_ptr<ValueHandler>>& whereValues) =0;

			virtual void remove(const std::string& table, const std::string &where,const std::vector<std::shared_ptr<ValueHandler>>& whereValues) =0;
		protected:
			std::shared_ptr<Logger> logger;
		private:
	};
}

#endif