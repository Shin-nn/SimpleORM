#ifndef DB_TABLE_H_
#define DB_TABLE_H_

#include <vector>

#include "Connection.h"
#include "Field.h"

namespace SimpleORM
{
	template <typename T>
	class Table
	{
		public:
			Table(Connection &c):connection(c) {};
			virtual ~Table() {}

			inline virtual bool isInDatabase() { return 0; }

			virtual Connection::Values getAllUpdates() = 0;

			virtual Where::Is getPrimaryWhere()=0;
			
			inline virtual void save() final
			{
				auto updates = getAllUpdates();
				if(isInDatabase())
				{
					connection.update(updates);
				}else
				{
					connection.insert(updates);
				}
			};

		protected:
			Connection &connection;
	};
}

#endif