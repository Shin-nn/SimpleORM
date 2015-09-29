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
			Model(Connection& c):connection(c) {};
			virtual ~Model() {}

			inline virtual bool isInDatabase() { return isInDB; }

			virtual void getAllUpdates(std::vector<std::string>& cols,std::vector<std::shared_ptr<ValueHandler>>& values)=0;

			virtual void getFromDB(const Row&)=0;

			virtual SimpleORM::Expression::Is<int> getPrimaryWhere()=0;

			inline virtual void save()=0;

			inline virtual void remove() final
			{
				auto primaryWhere= getPrimaryWhere();
				connection.remove(T::TableName, primaryWhere.sql(), primaryWhere.values());
			}

		protected:
			bool isInDB = false;
			Connection& connection;
	};

	template <typename T>
	class CachedModel: public Model<T>
	{
		public:
			CachedModel(Connection& c): Model<T>(c) {}
			virtual ~CachedModel() {}
	};
}

#endif

