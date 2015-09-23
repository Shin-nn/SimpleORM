#ifndef SIMPLEORM_MODEL_H_
#define SIMPLEORM_MODEL_H_

#include <vector>

#include "Connection.h"
#include "Field.h"

namespace SimpleORM
{
	template <typename T>
	class Model
	{
		public:
			Model(Connection &c):connection(c) {};
			virtual ~Model() {}

			inline virtual bool isInDatabase() { return isInDB; }

			//virtual Connection::Values getAllUpdates() = 0;

			virtual void getFromDB(const Row&)=0;

			inline virtual void save() final
			{
/*				auto updates = getAllUpdates();
				if(isInDatabase())
				{
					connection.update(updates);
				}else
				{
					connection.insert(updates);
				}
*/			};

		protected:
			bool isInDB = false;
			Connection &connection;
	};
}

#endif