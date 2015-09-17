#include "SimpleORM/Expression.h"
#include "SimpleORM/Field.h"

#include <cassert>
#include <string>
#include <iostream>

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
/*		std::shared<SimpleORM::ValueHandler> h=((a||b).values().at(0);
		assert(dynamic_cast<SimpleORM::Value<std::string>*>(h) !=nullptr);
		std::cout <<dynamic_cast<SimpleORM::Value<std::string>*>(h)->value() << "\n";
		assert(dynamic_cast<SimpleORM::Value<std::string>*>(h)->value()=="5");
*/	}
	{
		const auto& a=SimpleORM::Expression::Is<std::string>("name","5");
		const auto& b=SimpleORM::Expression::Is<int>("id",5);
		assert((a && b).sql() =="(name=?) AND (id=?)");
		assert((a && b).values().size() ==2);
/*
 *W		SimpleORM::ValueHandler* h=(a||b).values().at(1).get();
		assert(dynamic_cast<SimpleORM::Value<int>*>(h) !=nullptr);
		assert(dynamic_cast<SimpleORM::Value<int>*>(h)->value()==5);
*/	}
	{
		assert((SimpleORM::Expression::Is<std::string>("name","5")||SimpleORM::Expression::Is<std::string>("id","5")).sql()== "(name=?) OR (id=?)");
	}
}