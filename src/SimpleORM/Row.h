#ifndef SIMPLEORM_ROW_H_
#define SIMPLEORM_ROW_H_

#include <string>

namespace SimpleORM
{
	class Row
	{
		public:
			inline virtual ~Row() {}
			virtual int getInt(size_t row) const=0;
			virtual std::string getString(size_t row) const=0;
	};
}

#endif
