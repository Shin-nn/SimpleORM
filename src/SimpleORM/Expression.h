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
		class Is : public Expression
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

		class OR : public TwoExpression
		{
			public:
				inline OR(const Expression& _l, const Expression& _r): TwoExpression("OR",_l,_r) {}
			protected:
		};

		class AND : public TwoExpression
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
