#ifndef SIMPLEORM_CONNECTION_H_
#define SIMPLEORM_CONNECTION_H_

#include "Exception.h"
#include "Row.h"
#include "Value.h"

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
			inline Connection() {};
			inline virtual ~Connection() {}

			Connection(const Connection&) =delete;
			void operator=(const Connection&) =delete;

			virtual void select(const std::vector<std::string>& cols, const std::string& table,const std::string &sql, const std::vector<std::shared_ptr<ValueHandler>>& values,
				std::function<void(const Row&)>
			) =0;

		protected:
		private:
	};

	class SQLite :public Connection
	{
		public:
			class SQLiteRow : public Row
			{
				public:
					SQLiteRow(sqlite3_stmt *st) :statement(st) {};

					SQLiteRow(const SQLiteRow&) = delete;
					SQLiteRow& operator=(const SimpleORM::SQLite::SQLiteRow&) = delete;

					virtual ~SQLiteRow() {}
					virtual int getInt(size_t row) const override
					{
						return sqlite3_column_int64(statement,row);
					}
					virtual std::string getString(size_t row) const override
					{
						return std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement,row)));
					}
				protected:
					sqlite3_stmt *statement;
			};

			inline SQLite(const std::string& file): fileName(file)
			{
				if(sqlite3_open(fileName.c_str(), &db)!=0)
				{
					throw ConnectionException("Could not open file");
					// TODO throw exception
				}
			}

			virtual ~SQLite() override 
			{
				sqlite3_close(db);
			};

			void bind(SimpleORM::ValueHandler* value, int where, sqlite3_stmt* statement)
			{
				if(dynamic_cast<SimpleORM::Value<int>*>(value)!=nullptr)
				{
					sqlite3_bind_int64(statement,where,dynamic_cast<SimpleORM::Value<int>*>(value)->value());
				}else if (dynamic_cast<SimpleORM::Value<std::string>*>(value)!=nullptr)
				{
					const std::string& tmp =dynamic_cast<SimpleORM::Value<std::string>*>(value)->value();
					sqlite3_bind_text(statement,where,tmp.c_str(),tmp.length(),nullptr);
				}else
				{
					throw ConnectionException("ERR");
				}
			}
			virtual void select(const std::vector<std::string>& cols, const std::string& table,const std::string &sql, const std::vector<std::shared_ptr<ValueHandler>>& values,std::function<void(const Row&)> r) override
			{
				std::string colsStr;
				for(auto a= cols.begin();a<cols.end();++a)
				{
					colsStr+=*a;
					if(a+1 != cols.end())
						colsStr+=", ";
				}
				std::string query ="SELECT "+colsStr+" FROM " + table + " WHERE "+sql;
				sqlite3_stmt *statement;
				if(sqlite3_prepare(db,query.c_str(),query.length(),&statement,0) != SQLITE_OK)
				{
					throw SelectException(sqlite3_errmsg(db));
				}

				int counter=1;
				for(const auto& val: values)
				{
					bind(val.get(),counter,statement);
					counter++;
				}

				SQLiteRow row(statement);
				while(sqlite3_step(statement)==SQLITE_ROW)
				{
					r(row);
					//TODO fetch
				}

				sqlite3_finalize(statement);
			}

			SQLite(const SQLite&) =delete;
			void operator=(const SQLite&)=delete;

		protected:
			sqlite3 *db=nullptr;
			std::string fileName;
	};
}

#endif