def getCType(stype):
	if stype == "String":
		return "std::string"
	elif stype == "Int":
		return "int"
	else:
		# throw exception
		print("ERROR: unknown dtype")
	

def generateHeaders():
	ret = """#include \"SimpleORM/Table.h\"
#include \"SimpleORM/Field.h\"
#include \"SimpleORM/Selection.h\"
#include \"SimpleORM/Row.h\"

#include <iostream>
#include <string>

"""
	return ret

def generateClassHeadAndConstructor(className):
	ret = "class "+className+": public SimpleORM::Table<"+className+""">
{
	public:
		"""+className+"(SimpleORM::Connection& c) : SimpleORM::Table<"+className+">(c) {}\n\n"
	return ret

def generateTableName():
	return "		const static std::string tableName;\n\n"

def generateFields(fields):
	where = """		class Where
		{
			public:
"""
	var = ""
	
	for field in fields:
		stype = field['type']['dtype']
		ctype = getCType(stype)
		
		where += "				const static SimpleORM::ParametricFieldDefinition<"+ctype+"> "+field['varName']+";\n"
		var += "		SimpleORM::"+stype+"Field "+field['varName']+" = SimpleORM::"+stype+"Field();\n"

	where += "		};\n\n"
	var += "\n"
	
	return where + var

def generateGetFromDB(fields):
	ret = """		inline virtual void getFromDB(const SimpleORM::Row& row) override
		{
"""
	
	cnt = 0
	for field in fields:
		stype = field['type']['dtype']
		ret += "			"
		ret += field['varName']+" = "+"row.get"+stype+"("+str(cnt)+");\n"
		cnt += 1
	
	ret += "		}\n\n"
	return ret

def generateStaticClassContent():
	ret = """		virtual SimpleORM::Expression::Is<std::string> getPrimaryWhere()
		{
			return Where::name == "TODO";//id.value();
		};

		const static std::vector<std::string> collumns;

		virtual SimpleORM::Connection::Values getAllUpdates() override
		{
			SimpleORM::Connection::Values changes;
			if(name.isChanged())
			{
//				changes.push_back(std::make_pair<const SimpleORM::Field&,const SimpleORM::FieldDefinition&>(name,Where::name));
			}
			return changes;
		};
		
"""
	return ret

def generateClassEnd():
	return "};\n\n"

def generateFieldDefinitions(fields, tableName):
	ret = "const std::vector<std::string> User::collumns = std::vector<std::string>({"
	first = True
	for field in fields:
		if first: 
			first = False 
		else: 
			ret += ", "
		ret += "\""+field['DBName']+"\"";
	ret += "});\n";
	
	ret += "const std::string User::TableName = \""+tableName+"\";\n\n"
	
	for field in fields:
		ctype = getCType(field['type']['dtype'])
		
		ret += """//                                                                     fieldname, tablename
const SimpleORM::ParametricFieldDefinition<"""+ctype+"> User::Where::name(\""+field['DBName']+"\",\""+tableName+"\");\n"
		
	return ret+"\n\n"












