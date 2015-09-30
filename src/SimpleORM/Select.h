#ifndef SIMPLEORM_SELECT_H_
#define SIMPLEORM_SELECT_H_

#include "Connection.h"
#include "Expression.h"

#include <iostream>

namespace SimpleORM
{
	template <typename T>
	class Set : public std::vector<T>
	{
	};

	template <typename T>
	class Select
	{
		public:
			Select<T> (Connection& c,const Expression::Expression& where): values(where.values()),sql(where.sql()), tableName(T::TableName),connection(c)
			{
			}

			Set<T> select()
			{
				Set<T> tmp;
				connection.select(T::collumns,T::TableName,sql,values,[&tmp,this](const Row&r)
				{
					T tt(connection);
					tt.getFromDB(r);
					tmp.push_back(tt);
				});
				return tmp;
			};
			inline T first() { return select().at(0); } // TODO: rewrite
			inline size_t count() { return select().size(); } // TODO: rewrite

			inline const std::string& getSQL() const { return sql; }
			inline const std::vector<std::shared_ptr<ValueHandler>>& getValues() const { return values; }
		protected:
			std::vector<std::shared_ptr<ValueHandler>> values;
			std::string sql;
			const std::string& tableName;
			Connection& connection;

	};
}

#endif

