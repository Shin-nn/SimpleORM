#include "SimpleORM/Table.h"
#include "SimpleORM/Field.h"
#include "SimpleORM/Selection.h"
#include "SimpleORM/Row.h"

#include <iostream>
#include <string>

class User: public SimpleORM::Table<User>
{
	public:
		User(SimpleORM::Connection& c) : SimpleORM::Table<User>(c) {}

		const static std::string TableName;

		class Where
		{
			public:
				const static SimpleORM::ParametricFieldDefinition<std::string> name;
				const static SimpleORM::ParametricFieldDefinition<std::string> username;
				const static SimpleORM::ParametricFieldDefinition<int> id;
		};

		SimpleORM::StringField name =  SimpleORM::StringField();
		SimpleORM::StringField username =  SimpleORM::StringField();
		SimpleORM::IntField id = SimpleORM::IntField();

		virtual SimpleORM::Expression::Is<std::string> getPrimaryWhere()
		{
			return Where::name == "TODO";//id.value();
		};

		const static std::vector<std::string> rows;

		inline virtual void getFromDB(const SimpleORM::Row& row) override
		{
			id=row.getInt(1);
			username=row.getString(2);
		}

		virtual SimpleORM::Connection::Values getAllUpdates() override
		{
			SimpleORM::Connection::Values changes;
			if(name.isChanged())
			{
//				changes.push_back(std::make_pair<const SimpleORM::Field&,const SimpleORM::FieldDefinition&>(name,Where::name));
			}
			return changes;
		};
};

const std::vector<std::string> User::rows = std::vector<std::string>({"id","username"});
const std::string User::TableName="Users";
const SimpleORM::ParametricFieldDefinition<std::string> User::Where::name("name",TableName);
const SimpleORM::ParametricFieldDefinition<std::string> User::Where::username("username",TableName);
const SimpleORM::ParametricFieldDefinition<int> User::Where::id("id",TableName);

int main()
{
	SimpleORM::Connection *c= new SimpleORM::SQLite("./db-001.db");

/*
 *	User newUser(*c);
	newUser.name=std::string("newUser");
	newUser.save();
*/

	//SimpleORM::Result<User> queryResult=SimpleORM::Select<User> userSelection(*c,User::Where::name=="5");
	SimpleORM::Select<User> userSelection(*c,User::Where::username=="default");
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