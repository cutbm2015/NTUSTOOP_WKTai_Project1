#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

class Integer {
public:

	

	typedef enum {
		NONE,
		INVALIDINPUT,
	}ErrorCode;

	ErrorCode errorCode = NONE;

	string value;
	bool inf;
	bool positive; //true = positive

	Integer();
	Integer(string);
	Integer(const char*);


	Integer operator+(Integer);
	Integer operator-(Integer);
	Integer operator*(Integer);
	Integer operator/(Integer);
	bool operator>(Integer other);
	bool operator<(Integer other);
    bool operator==(Integer other);
    void operator=(const Integer);

    void operator=(const string);

	friend ostream& operator<<(std::ostream& os, Integer obj);
	friend istream& operator>>(std::istream& is, Integer& obj);

	
};
