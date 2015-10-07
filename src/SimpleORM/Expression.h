#ifndef SIMPLEORM_EXPRESSION_H_
#define SIMPLEORM_EXPRESSION_H_

#include "Value.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace SimpleORM
{

	namespace Expression
	{
		class Expression
		{
			public:
				virtual std::string sql() const { return "1=1"; }
				virtual std::vector<std::shared_ptr<ValueHandler>> values() const { return std::vector<std::shared_ptr<ValueHandler>>(); };
				virtual ~Expression() {}
		};

		template<typename T>
		class Is: public Expression
		{
			public:
				inline Is(const std::string& _field, const T& _value): field(_field),val(_value) {}
				inline virtual std::string sql() const override { return field+"=?"; }
				inline virtual std::vector<std::shared_ptr<ValueHandler>> values() const override
				{
					std::vector<std::shared_ptr<ValueHandler>> a;
					a.push_back(std::shared_ptr<ValueHandler>(new Value<T>(val)));
					return a;
				}

			protected:
				std::string field;
				Value<T> val;
		};

		class IsNull: public Expression
		{
			public:
				inline IsNull(const std::string& _field): field(_field) {}
				inline virtual std::string sql() const override { return field+" IS NULL"; }
				inline virtual std::vector<std::shared_ptr<ValueHandler>> values() const override
				{
					return std::vector<std::shared_ptr<ValueHandler>>();
				}

			protected:
				std::string field;
		};

		template<typename T>
		class In: public Expression
		{
			public:
				inline In(const std::string& _field, const std::vector<T>& _v): field(_field),_values(_v) {}
				inline In(const std::string& _field, const std::initializer_list<T>& _v): field(_field),_values(_v) {}

				inline virtual std::string sql() const override
				{
					std::string tmp=field+" IN (";

					for(size_t counter=0; counter<_values.size(); counter++)
					{
						if(counter > 0)
							tmp+=",?";
						else
							tmp+="?";

					}

					return tmp+")";
				}
				inline virtual std::vector<std::shared_ptr<ValueHandler>> values() const override
				{
					std::vector<std::shared_ptr<ValueHandler>> a;

					for(const T& val: _values)
						a.push_back(std::shared_ptr<ValueHandler>(new Value<T>(val)));

					return a;
				}

			protected:
				std::string field;
				std::vector<T> _values;
		};

		template<typename T>
		class InQuery: public Expression
		{
			public:
				inline InQuery(const std::string& _field, const std::string& _expr, const std::vector<std::shared_ptr<ValueHandler>>& _val): field(_field), expr(_expr),
					val(_val) {}

				inline virtual std::string sql() const override
				{
					return field+" IN ( SELECT id FROM "+T::TableName+" WHERE " +expr+")";
				}

				inline virtual std::vector<std::shared_ptr<ValueHandler>> values() const override
				{
					return val;
				}

			protected:
				std::string field;
				std::string expr;
				std::vector<std::shared_ptr<ValueHandler>> val;
		};

		class TwoExpression: public Expression
		{
			public:
				inline TwoExpression(const std::string& _op,const Expression& _l, const Expression& _r):Expression(), op(_op), l(_l), r(_r) {}
				inline virtual std::string sql() const override { return "("+l.sql()+") "+op+" ("+(&r)->sql()+")"; }
				inline virtual std::vector<std::shared_ptr<ValueHandler>> values() const override
				{
					std::vector<std::shared_ptr<ValueHandler>> tmp =l.values();

					for(const auto& a: r.values())
					{
						tmp.push_back(a);
					}

					return tmp;
				}
			protected:
				const std::string op;
				const Expression& l;
				const Expression& r;
		};

		class OR: public TwoExpression
		{
			public:
				inline OR(const Expression& _l, const Expression& _r): TwoExpression("OR",_l,_r) {}
			protected:
		};

		class AND: public TwoExpression
		{
			public:
				inline AND(const Expression& _l, const Expression& _r): TwoExpression("AND",_l,_r) {}
		};

#pragma GCC diagnostic ignored "-Weffc++"
		inline OR operator||(const Expression& l, const Expression& r) { return SimpleORM::Expression::OR(l,r); }

		inline AND operator&&(const Expression& l, const Expression& r) { return SimpleORM::Expression::AND(l,r); }

		inline std::ostream& operator<<(std::ostream& os, const Expression& l) { os<< l.sql(); return os; }
	}
}

#endif

