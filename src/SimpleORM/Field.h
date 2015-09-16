#ifndef DB_FIELD_H_
#define DB_FIELD_H_

#include "WherePart.h"

#include <ostream>
#include <iostream>

namespace SimpleORM
{
	class AField
	{
	};

	template <typename T>
	class Field : public AField
	{
		public:
			inline Field& operator=(const T& _v) {
				if(val!=_v)
					changed=1;

				val=_v;
				return *this;
			}

			inline const T& value() const { return val; }

			inline bool isChanged() { return changed; }
		protected:
			T val = T();
			bool changed=0;
		private:
	};
	template <typename T>
	std::ostream & operator<<(std::ostream& os, const Field<T> &o) { os<< o.value(); return os; }

	class StringField: public Field<std::string>
	{
	};

	class IntField: public Field<int>
	{
	};

	class ReferenceField: public IntField
	{
	};

	class AFieldDefinition
	{
		public:
			AFieldDefinition(const std::string& tn, const std::string &fn) : tableName(tn), fieldName(fn) {}

		protected:
			std::string tableName;
			std::string fieldName;
	};

	template <typename T>
	class FieldDefinition: public AFieldDefinition
	{
		public:
			inline FieldDefinition<T>(const std::string& _fieldName, const std::string& _tableName) : AFieldDefinition(_tableName,_fieldName) {}
			Where::Is operator==(const T is) const { return Where::Is(this->fieldName,is); }
		private:
	};
}

#endif