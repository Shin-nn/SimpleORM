#include "SimpleORM/Table.h"
#include "SimpleORM/Field.h"
#include "SimpleORM/Selection.h"
#include "SimpleORM/WherePart.h"

#include <iostream>
#include <string>

class User: public SimpleORM::Table<User>
{
	public:
		User(SimpleORM::Connection& c) : SimpleORM::Table<User>(c) {}

		static const std::string TableName;

		class Where
		{
			public:
				const static SimpleORM::FieldDefinition<std::string> name;
				const static SimpleORM::FieldDefinition<int> id;
		};

		SimpleORM::Field<std::string> name = SimpleORM::Field<std::string>();
		SimpleORM::Field<int> id = SimpleORM::Field<int>();

		virtual SimpleORM::Where::Is getPrimaryWhere() override
		{
			return Where::name == "TODO";//id.value();
		};

		virtual SimpleORM::Connection::Values getAllUpdates() override
		{
			SimpleORM::Connection::Values changes;
			if(name.isChanged())
			{
				changes.push_back(std::make_pair<const SimpleORM::AField&,const SimpleORM::AFieldDefinition&>(name,Where::name));
			}
			return changes;
		};
};
const std::string User::TableName="Users";
const SimpleORM::FieldDefinition<std::string> User::Where::name("name",TableName);
const SimpleORM::FieldDefinition<int> User::Where::id("id",TableName);

int main()
{
	SimpleORM::Connection *c= new SimpleORM::SQLite("./test.db");
	User newUser(*c);
	newUser.name="newUser";
	newUser.save();

	SimpleORM::Select<User> userSelection(*c);
	userSelection.where(User::Where::name=="5");
	userSelection.select();

//	std::cout << s.toStr();

	//userSelection.where();
	//std::vector<User> users =userSelection.get();

	
//.w	here(U::name=="5");

	User test(*c);
	test.name ="newName";
	std::cout << test.name;
	test.save();

	delete c;
}