#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <exception>

#include <string>
#include <sqlite3.h> 

#include "Field.h"

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
			inline Connection() {};
			inline virtual ~Connection() {}

			Connection(const Connection&) =delete;
			void operator=(const Connection&) =delete;

			typedef std::vector<std::pair<const AField&, const AFieldDefinition&>> Values;
			virtual void select(const Values&)=0;
			virtual void update(const Values&)=0;
			virtual void insert(const Values&)=0;

		protected:
		private:
	};

	class SQLite :public Connection
	{
		public:
			inline SQLite(const std::string& file): fileName(file)
			{
				if(sqlite3_open("test.db", &db)!=0)
				{
					throw ConnectionException("Could not open file");
					// TODO throw exception
				}
			}

			virtual ~SQLite() override 
			{
				sqlite3_close(db);
			};

			virtual void select(const Values&) override {};
			virtual void update(const Values&) override {};
			virtual void insert(const Values&) override {};

			SQLite(const SQLite&) =delete;
			void operator=(const SQLite&)=delete;

		protected:
			sqlite3 *db=nullptr;
			std::string fileName;
	};
}

#endif