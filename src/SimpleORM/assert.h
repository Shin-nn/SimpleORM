#ifndef _SIMPLEORM_ASSERT_
#define _SIMPLEORM_ASSERT_

#include <iostream>
#include <chrono>

int __assertions=0;
int __assertions_OK=0;
int __assertions_ERR=0;

#define massert_text(EXPR,TEXT)\
	{\
		__assertions++; if(EXPR) {__assertions_OK++;} else {__assertions_ERR++;}\
		std::cout << "\033[1;12m" << __FILE__ << ":"<< __LINE__ << "\033[0m " << TEXT << " " << (EXPR ?"\033[1;32mOK\033[0m" : "\033[1;31mFAILED\033[0m") << std::endl;\
	}

#define _ASSERT_BACK_ massert_text

#define assert(EXPR) _ASSERT_BACK_(EXPR,__STRING(EXPR))

#define assert_eq(X,Y) assert(X==Y)

#define assert_ne(X,Y) assert(X!=Y)

#define assert_throws(expr,except) { bool ok=0; try {expr;} catch(except&e) { ok=1; } catch(...) {}; _ASSERT_BACK_(ok==true,__STRING(expr THROWS except))}

//100 loops

#define measure(X) {auto start = std::chrono::system_clock::now();\
	for(int i=0;i<100;i++) { X; } auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();\
	std::cout << "\033[1;12mexecution of\033[0m " << __STRING(X) << " took " <<  (double)duration/1000.0/10.0 << " sec" << std::endl; }

inline int assert_status()
{
	if(__assertions==__assertions_OK && !__assertions_ERR)
	{
		std::cout << "\033[1;32m"<< __assertions_OK << "/" << __assertions <<" passed\033[0m" << std::endl;
		return 0;
	} else {
		std::cout << "\033[1;31m"<<__assertions_ERR<<"/"<< __assertions <<" failed\033[0m" << std::endl;
		return 1;
	}
}

#endif