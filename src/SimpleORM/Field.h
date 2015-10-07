#ifndef SIMPLEORM_FIELD_H_
#define SIMPLEORM_FIELD_H_

#include "Expression.h"
#include "Connection.h"
#include "Select.h"

#include <ostream>
#include <iostream>

namespace SimpleORM
{
	class Field
	{
		public:
			inline virtual bool isChanged() final { return changed; }
			virtual ~Field() {}

		protected:
			bool changed=0;
	};

	template <typename T>
	class ParametricField : public Field
	{
		public:
			inline ParametricField& operator=(const T& _v)
			{
				if(val!=_v)
					changed=1;

				val=_v;
				return *this;
			}

			inline const T& value() const { return val; }

		protected:
			T val = T();
		private:
	};

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const ParametricField<T>& o) { os<< o.value(); return os; }

	class StringField: public ParametricField<std::string>
	{
		public:
			using ParametricField::operator=;
		protected:
		private:
	};

	class IntField: public ParametricField<int>
	{
		public:
			using ParametricField::operator=;
	};

	template <typename T>
	class ReferenceField: public Field
	{
		public:
			ReferenceField<T>(Connection& c):connection(c) {}
			inline ReferenceField<T>& operator=(int _v)
			{
				if(val!=_v)
					changed=1;

				val=_v;
				return *this;
			}
			inline ReferenceField<T>& operator=(const T& _v)
			{
				if(val!=_v.id.value())
					changed=1;

				val=_v.id.value();
				return *this;
			}

			inline T value() const
			{
				return SimpleORM::Select<T>(connection,T::Attribute::id==val).first();
			}
		protected:
			int val=0;
			Connection& connection;
	};

	class FieldDefinition
	{
		public:
			FieldDefinition(const std::string& tn, const std::string& fn) : tableName(tn), fieldName(fn) {}

			Expression::IsNull operator==(std::nullptr_t) const { return Expression::IsNull(this->fieldName); }
		protected:
			std::string tableName;
			std::string fieldName;
	};

	template <typename T>
	class ParametricFieldDefinition: public FieldDefinition
	{
		public:
			inline ParametricFieldDefinition<T>(const std::string& _fieldName, const std::string& _tableName) : FieldDefinition(_tableName,_fieldName) {}

			using FieldDefinition::operator==;
			Expression::Is<T> operator==(const T& is) const { return Expression::Is<T>(this->fieldName,is); }
		private:
	};

	template <typename T>
	class ReferenceFieldDefinition: public FieldDefinition
	{
		public:
			inline ReferenceFieldDefinition<T>(const std::string& _fieldName, const std::string& _tableName) : FieldDefinition(_tableName,_fieldName) {}

			using FieldDefinition::operator==;
			Expression::Is<int> operator==(const T& is) const { return Expression::Is<int>(this->fieldName,is.id.value()); }
			Expression::InQuery<T> in(const Select<T>& s) const { return Expression::InQuery<T>(this->fieldName,s.getSQL(),s.getValues()); }
		private:
	};
}

#endif