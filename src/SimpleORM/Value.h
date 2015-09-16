#ifndef SIMPLEORM_VALUE_H_
#define SIMPLEORM_VALUE_H_

namespace SimpleORM
{
	class ValueHandler
	{
		public:
			virtual ~ValueHandler() {}
	};

	template<typename T>
	class Value : public ValueHandler
	{
		public:
			Value<T> (const T& _val) : val(_val) {}
			inline const T& value() { return val; }
			virtual ~Value() {}
		protected:
			T val;
	};
}

#endif