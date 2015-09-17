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
				const static SimpleORM::ParametricFieldDefinition<std::string> username;
				const static SimpleORM::ParametricFieldDefinition<int> id;
		};


		SimpleORM::StringField username =  SimpleORM::StringField();

		SimpleORM::IntField id = SimpleORM::IntField();

		virtual SimpleORM::Expression::Is<int> getPrimaryWhere()
		{
			return Where::id == id.value();
		};

		const static std::vector<std::string> collumns;

		inline virtual void getFromDB(const SimpleORM::Row& row) override
		{
			id=row.getInt(0);
			username=row.getString(1);
		}

		virtual SimpleORM::Connection::Values getAllUpdates() override
		{
			SimpleORM::Connection::Values changes;
			if(username.isChanged())
			{
//				changes.push_back(std::make_pair<const SimpleORM::Field&,const SimpleORM::FieldDefinition&>(name,Where::name));
			}
			return changes;
		};
};

const std::vector<std::string> User::collumns = std::vector<std::string>({"id","username"});
const std::string User::TableName="Users";

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
	User ret = userSelection.first();
	std::cout << "name: " << ret.username << "\n";
	std::cout << "id: " << ret.id; 
	

//	std::cout << s.toStr();

	//userSelection.where();
	//std::vector<User> users =userSelection.get();

	
//.w	here(U::name=="5");

	User test(*c);
	test.username ="newName";
	std::cout << test.username;
	test.save();

	delete c;
}