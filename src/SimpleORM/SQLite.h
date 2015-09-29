#ifndef SIMPLEORM_SQLITE_H_
#define SIMPLEORM_SQLITE_H_

#include "Connection.h"

#include <string>
#include <sqlite3.h>
#include <functional>
#include <memory>
#include <vector>
#include <sqlite3.h>

namespace SimpleORM
{
	class SQLite :public Connection
	{
		public:
			class SQLiteRow : public Row
			{
				public:
					SQLiteRow(sqlite3_stmt* st) :statement(st) {};

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
					sqlite3_stmt* statement;
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
				}
				else if(dynamic_cast<SimpleORM::Value<std::string>*>(value)!=nullptr)
				{
					const std::string& tmp =dynamic_cast<SimpleORM::Value<std::string>*>(value)->value();
					sqlite3_bind_text(statement,where,tmp.c_str(),tmp.length(),nullptr);
				}
				else
				{
					throw ConnectionException("ERR");
				}
			}
			virtual void select(const std::vector<std::string>& cols, const std::string& table,const std::string& sql,
								const std::vector<std::shared_ptr<ValueHandler>>& values,std::function<void(const Row&)> r) override
			{
				std::string colsStr;

				for(auto a= cols.begin(); a<cols.end(); ++a)
				{
					colsStr+=*a;

					if(a+1 != cols.end())
						colsStr+=", ";
				}

				std::string query ="SELECT "+colsStr+" FROM " + table + " WHERE "+sql;
				logger->logSelect(query);

				sqlite3_stmt* statement;

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
				}

				sqlite3_finalize(statement);
			}

			virtual long int insert(const std::string& table, const std::vector<std::string>& cols, const std::vector<std::shared_ptr<ValueHandler>>& values) override
			{
				std::string query ="INSERT INTO "+table+" (";
				std::string queryTmp="";

				for(auto a= cols.begin(); a<cols.end(); ++a)
					for(auto c=cols.begin(); c<cols.end(); ++c)
					{
						query+=*c;
						queryTmp+="?";

						if(c+1 !=cols.end())
						{
							query+=",";
							queryTmp+=",";
						}
					}

				query+=") VALUES ("+queryTmp+")";

				logger->logInsert(query);
				sqlite3_stmt* statement;

				if(sqlite3_prepare(db,query.c_str(),query.length(),&statement,0) != SQLITE_OK)
				{
					throw SelectException(query+=": error "+std::string(sqlite3_errmsg(db)));
				}

				int counter=1;

				for(const auto& val: values)
				{
					bind(val.get(),counter,statement);
					counter++;
				}

				if(sqlite3_step(statement) != SQLITE_DONE)
				{
					sqlite3_finalize(statement);
					throw SelectException(query+=": error "+std::string(sqlite3_errmsg(db)));
				}

				auto ret =sqlite3_last_insert_rowid(db);
				sqlite3_finalize(statement);
				return ret;

			}

			virtual void update(const std::string& table, const std::vector<std::string>& cols, const std::vector<std::shared_ptr<ValueHandler>>& values,
								const std::string& where, const std::vector<std::shared_ptr<ValueHandler>>& whereValues) override
			{
				std::string query ="UPDATE "+table+" SET ";

				for(const auto& c: cols)
					query+=c+"=?";

				query+=" WHERE " + where;


				logger->logUpdate(query);
				sqlite3_stmt* statement;

				if(sqlite3_prepare(db,query.c_str(),query.length(),&statement,0) != SQLITE_OK)
				{
					throw SelectException(query+=": error "+std::string(sqlite3_errmsg(db)));
				}

				int counter=1;

				for(const auto& val: values)
				{
					bind(val.get(),counter,statement);
					counter++;
				}

				for(const auto& val: whereValues)
				{
					bind(val.get(),counter,statement);
					counter++;
				}

				if(sqlite3_step(statement) != SQLITE_DONE)
				{
					sqlite3_finalize(statement);
					throw SelectException(query+=": error "+std::string(sqlite3_errmsg(db)));
				}

				sqlite3_finalize(statement);
			}

			inline virtual void remove(const std::string& table, const std::string& where, const std::vector<std::shared_ptr<ValueHandler>>& values)
			{
				std::string query = "DELETE FROM "+table+" WHERE "+where;

				logger->logRemove(query);
				sqlite3_stmt* statement;

				if(sqlite3_prepare(db,query.c_str(),query.length(),&statement,0) != SQLITE_OK)
				{
					throw SelectException(query+=": error "+std::string(sqlite3_errmsg(db)));
				}

				int counter=1;

				for(const auto& val: values)
				{
					bind(val.get(),counter,statement);
					counter++;
				}

				if(sqlite3_step(statement) != SQLITE_DONE)
				{
					sqlite3_finalize(statement);
					throw SelectException(query+=": error "+std::string(sqlite3_errmsg(db)));
				}

				sqlite3_finalize(statement);
			}

			SQLite(const SQLite&) =delete;
			void operator=(const SQLite&)=delete;

		protected:
			sqlite3* db=nullptr;
			std::string fileName;
	};
}

#endif