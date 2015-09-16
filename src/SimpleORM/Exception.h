#ifndef SIMPLEORM_EXCEPTION_H_
#define SIMPLEORM_EXCEPTION_H_

#include <string>
#include <exception>

namespace SimpleORM
{
	class SelectException:public std::exception
	{
		public:
			SelectException(const std::string & _str) : str(_str.c_str()) {}
			SelectException(const char* _str) : str(_str) {}

			SelectException(const SelectException& r): str(r.str) {}

			SelectException& operator=(const SelectException& r){ str=r.str; return *this; }

			virtual ~SelectException() {}

			virtual const char* what() const noexcept override { return str; }
		protected:
			const char* str;
	};
}

#endif