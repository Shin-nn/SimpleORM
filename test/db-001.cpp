#include "SimpleORM/Model.h"
#include "SimpleORM/Field.h"
#include "SimpleORM/Select.h"
#include "SimpleORM/Row.h"
#include "SimpleORM/SQLite.h"

#include <iostream>
#include <string>

class User: public SimpleORM::Model<User>
{
	public:
		User(SimpleORM::Connection& c) : SimpleORM::Model<User>(c) {}

		const static std::string TableName;

		class Attribute
		{
			public:
				const static SimpleORM::ParametricFieldDefinition<std::string> username;
				const static SimpleORM::ParametricFieldDefinition<int> id;
		};


		SimpleORM::StringField username =  SimpleORM::StringField();

		SimpleORM::IntField id = SimpleORM::IntField();

		virtual SimpleORM::Expression::Is<int> getPrimaryWhere()
		{
			return Attribute::id == id.value();
		};

		const static std::vector<std::string> collumns;

		inline virtual void getFromDB(const SimpleORM::Row& row) override
		{
			id=row.getInt(0);
			username=row.getString(1);
		}

		inline virtual void getAllUpdates(std::vector<std::string>& cols,std::vector<std::shared_ptr<SimpleORM::ValueHandler>>& values) override
		{
			if(username.isChanged())
			{
				cols.push_back("username");
				values.push_back(std::shared_ptr<SimpleORM::ValueHandler>(new SimpleORM::Value<std::string>(username.value())));
			}
		}

		inline virtual void save() override
		{
			std::vector<std::string> cols;
			std::vector<std::shared_ptr<SimpleORM::ValueHandler>> values;

			getAllUpdates(cols,values);

			if(isInDatabase())
			{
				auto tmp =this->getPrimaryWhere();
				connection.update(TableName,cols,values,tmp.sql(),tmp.values());
			}else
			{
				auto _id= connection.insert(TableName,cols,values);
				this->id=_id;
				isInDB=true;
			}
		};

};

const std::vector<std::string> User::collumns = std::vector<std::string>({"id","username"});
const std::string User::TableName="Users";

const SimpleORM::ParametricFieldDefinition<std::string> User::Attribute::username("username",TableName);
const SimpleORM::ParametricFieldDefinition<int> User::Attribute::id("id",TableName);

class Login: public SimpleORM::CachedModel<Login>
{
	public:
		Login(SimpleORM::Connection& c) : SimpleORM::CachedModel<Login>(c),user(c) {}

		const static std::string TableName;

		class Attribute
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
			return Attribute::id == id.value();
		};

		const static std::vector<std::string> collumns;

		inline virtual void getFromDB(const SimpleORM::Row& row) override
		{
			id=row.getInt(0);
			user=row.getInt(1);
			login=row.getString(2);
			password=row.getString(3);
			isInDB=true;
		}

		inline virtual void getAllUpdates(std::vector<std::string>& cols,std::vector<std::shared_ptr<SimpleORM::ValueHandler>>& values) override
		{
			if(user.isChanged())
			{
				cols.push_back("user");
				values.push_back(std::shared_ptr<SimpleORM::ValueHandler>(new SimpleORM::Value<int>(user.value().id.value())));
			}
			if(login.isChanged())
			{
				cols.push_back("login");
				values.push_back(std::shared_ptr<SimpleORM::ValueHandler>(new SimpleORM::Value<std::string>(login.value())));
			}
			if(password.isChanged())
			{
				cols.push_back("password");
				values.push_back(std::shared_ptr<SimpleORM::ValueHandler>(new SimpleORM::Value<std::string>(password.value())));
			}
		}

		inline virtual void save() override
		{
			std::vector<std::string> cols;
			std::vector<std::shared_ptr<SimpleORM::ValueHandler>> values;

			if(isInDatabase())
			{
				auto tmp =this->getPrimaryWhere();
				connection.update(TableName,cols,values,tmp.sql(),tmp.values());
			}else
			{
				auto _id= connection.insert(TableName,cols,values);
				this->id=_id;
				isInDB=true;
			}
		};
};

const std::vector<std::string> Login::collumns = std::vector<std::string>({"id","user","login","password"});
const std::string Login::TableName="Logins";
const SimpleORM::ParametricFieldDefinition<int> Login::Attribute::id("id",TableName);
const SimpleORM::ReferenceFieldDefinition<User> Login::Attribute::user("user",TableName);
const SimpleORM::ParametricFieldDefinition<std::string> Login::Attribute::login("login",TableName);
const SimpleORM::ParametricFieldDefinition<std::string> Login::Attribute::password("password",TableName);


int main()
{
	SimpleORM::Connection *c= new SimpleORM::SQLite("./db-001.db");

/*
 *	User newUser(*c);
	newUser.name=std::string("newUser");
	newUser.save();
*/

	//SimpleORM::Result<User> queryResult=SimpleORM::Select<User> userSelection(*c,User::Where::name=="5");
	SimpleORM::Select<User> userSelection(*c,User::Attribute::username=="default");
	User ret = userSelection.first();
	std::cout << "name: " << ret.username << "\n";
	std::cout << "id: " << ret.id << "\n"; 

	SimpleORM::Select<Login> loginSelection(*c,Login::Attribute::user==ret);
	Login login = loginSelection.first();
	std::cout << "user: " << login.user.value().username.value() << "\n";
	std::cout << "id: " << login.id<< "\n"; 
	

//	std::cout << s.toStr();

	//userSelection.where();
	//std::vector<User> users =userSelection.get();

	
//.w	here(U::name=="5");

	{
		SimpleORM::Select<User> userSelection2(*c,User::Attribute::username=="newName2");
		if(userSelection2.count() > 0)
		{
			userSelection2.first().remove();
		}
	}
	{
		SimpleORM::Select<User> userSelection2(*c,User::Attribute::username=="newName");
		if(userSelection2.count() > 0)
		{
			userSelection2.first().remove();
		}
	}

	User test(*c);
	test.username ="newName2";
	std::cout << test.username;
	test.save();

	test.username="newName";
	test.save();

	{
	SimpleORM::Select<User> userSelection2(*c,User::Attribute::username=="newName");
	std::cout << "\n count: " << userSelection2.count() << std::endl;
	}
	{
	SimpleORM::Select<User> userSelection2(*c,User::Attribute::username=="newName2");
	std::cout << "\n count: " << userSelection2.count() << std::endl;
	}
	delete c;
}