#ifndef DB_SELECTION_H_
#define DB_SELECTION_H_

#include "Connection.h"
#include "Expression.h"

#include <iostream>

namespace SimpleORM
{
	template <typename T>
	class Set
	{
	};

	template <typename T>
	class Select
	{
		public:
			Select<T> (Connection &c,const Expression::Expression &where): connection(c)
			{
				connection.select(T::rows,T::TableName,where);
			}

			Set<T> select() { return Set<T>(); };
		protected:
			Connection &connection;

	};
}

#endif