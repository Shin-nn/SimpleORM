#ifndef DB_WHERE_PART_H_
#define DB_WHERE_PART_H_

#include <string>

namespace SimpleORM
{
namespace Where
{
		class Where {
			public:
				virtual std::string toStr() const = 0;
				virtual ~Where() {}
		};

		class Is : public Where
		{
			public:
				Is(const std::string& _field, const std::string& _value): field(_field),value(_value) {};
				virtual std::string toStr() const override{
					return "S";
				}
			protected:
				std::string field;
				std::string value;
		};
}
}

#endif