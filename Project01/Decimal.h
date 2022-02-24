#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

#include "Integer.h"

using namespace std;

class Decimal : public Integer{
public:


   

	typedef enum {
		NONE,
		INVALIDINPUT,
	}ErrorCode;

	
    Decimal();
    Decimal(string);
	Decimal(const char*);
     
    string numerator;
    string denominator;

	string value;
	bool inf;
	bool positive; //true = postive

	Decimal operator+(const Decimal);
	Decimal operator-(Decimal);
	Decimal operator*(Decimal);
	Decimal operator/(Decimal);

	void operator=(const Decimal);

    void operator=(const string);




    friend ostream& operator<<(std::ostream& os, const Decimal obj);
	friend istream& operator>>(std::istream& is, Decimal& obj);

};

