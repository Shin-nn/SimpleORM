#include "SimpleORM/Expression.h"
#include "SimpleORM/Field.h"

#include <cassert>
#include <string>
#include <iostream>
#include <memory>
#include <vector>

int main()
{
	{
		const auto& a=SimpleORM::Expression::Is<std::string>("name","5");
		assert(a.sql()=="name=?");
		assert(a.values().size() ==1);
	}
	{
		const auto& a=SimpleORM::Expression::Is<int>("id",5);
		assert(a.sql()=="id=?");
		assert(a.values().size() ==1);
	}
	{
		const auto& a=SimpleORM::Expression::Is<int>("id",5);
		assert(a.sql()=="id=?");
		assert(a.values().size() ==1);
	}
	{
		const auto& a=SimpleORM::Expression::Is<std::string>("name","5");
		const auto& b=SimpleORM::Expression::Is<int>("id",5);
		assert((a||b).sql() =="(name=?) OR (id=?)");
		assert((a||b).values().size() ==2);
		std::shared_ptr<SimpleORM::ValueHandler> h=(a||b).values().at(0);
		auto value = h.get();
		assert(dynamic_cast<SimpleORM::Value<std::string>*>(value) !=nullptr);
		assert(dynamic_cast<SimpleORM::Value<std::string>*>(value)->value()=="5");
	}
	{
		const auto& a=SimpleORM::Expression::Is<std::string>("name","5");
		const auto& b=SimpleORM::Expression::Is<int>("id",5);
		assert((a && b).sql() =="(name=?) AND (id=?)");
		assert((a && b).values().size() ==2);
		std::shared_ptr<SimpleORM::ValueHandler> h=(a||b).values().at(1);
		auto tmp=h.get();
		assert(dynamic_cast<SimpleORM::Value<int>*>(tmp) !=nullptr);
		assert(dynamic_cast<SimpleORM::Value<int>*>(tmp)->value()==5);
	}
	{
		assert((SimpleORM::Expression::Is<std::string>("name","5")||SimpleORM::Expression::Is<std::string>("id","5")).sql()== "(name=?) OR (id=?)");
	}
	{
		assert(SimpleORM::Expression::In<std::string>("name",std::vector<std::string>({"5","6"})).sql() == "name IN (?,?)");
	}
	{
		assert(SimpleORM::Expression::In<std::string>("name",{"5","6","7"}).sql() == "name IN (?,?,?)");
	}
}