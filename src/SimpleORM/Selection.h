#ifndef DB_SELECTION_H_
#define DB_SELECTION_H_

#include "Connection.h"
#include "WherePart.h"

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
			Select<T> (Connection &c): connection(c)
			{
			}
			Select<T>& where(const Where::Where& W)
			{
				std::cout << W.toStr();
				return *this;
			}

			Set<T> select() { return Set<T>(); };
		protected:
			Connection &connection;

	};
}

#endif