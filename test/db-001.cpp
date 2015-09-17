#include "SimpleORM/Table.h"
#include "SimpleORM/Field.h"
#include "SimpleORM/Selection.h"
#include "SimpleORM/Row.h"

#include <iostream>
#include <string>

class User;

class Login: public SimpleORM::Table<Login>
{
	public:
		Login(SimpleORM::Connection& c) : SimpleORM::Table<Login>(c),user(c) {}

		const static std::string TableName;

		class Where
		{
			public:
				const static SimpleORM::ParametricFieldDefinition<int> id;
				const static SimpleORM::ReferenceFieldDefinition<User> user;
				const static SimpleORM::ParametricFieldDefinition<std::string> login;
				const static SimpleORM::ParametricFieldDefinition<std::string> password;
		};

		SimpleORM::IntField id = SimpleORM::IntField();
		SimpleORM::ReferenceField<User> user;
		SimpleORM::StringField login =  SimpleORM::StringField();
		SimpleORM::StringField password =  SimpleORM::StringField();

		virtual SimpleORM::Expression::Is<int> getPrimaryWhere()
		{
			return Where::id == id.value();
		};

		const static std::vector<std::string> rows;

		inline virtual void getFromDB(const SimpleORM::Row& row) override
		{
			id=row.getInt(0);
			user=row.getInt(1);
			login=row.getString(2);
			password=row.getString(3);
		}

};

const std::vector<std::string> Login::rows = std::vector<std::string>({"id","user","login","password"});
const std::string Login::TableName="Logins";
const SimpleORM::ParametricFieldDefinition<int> Login::Where::id("id",TableName);
const SimpleORM::ReferenceFieldDefinition<User> Login::Where::user("user",TableName);
const SimpleORM::ParametricFieldDefinition<std::string> Login::Where::login("login",TableName);
const SimpleORM::ParametricFieldDefinition<std::string> Login::Where::password("password",TableName);

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
	std::cout << "id: " << ret.id << "\n"; 

	SimpleORM::Select<Login> loginSelection(*c,Login::Where::user==ret);
	Login login = loginSelection.first();
	std::cout << "user: " << login.user.value().username.value() << "\n";
	std::cout << "id: " << login.id<< "\n"; 
	

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