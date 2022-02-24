#include <string>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <vector>

#include "Decimal.h"
#include "Calculate.h"



//#define DEBUG
#define BANNER "======================================================================"

using namespace std;

string filter(string s) {


    if (isInteger(s)) {
        return s;
    }
    else {
        string follow = "";
        bool startCnt = false;
        int cnt = 0;
        for (int i = 0; i < s.length(); ++i) {
            
            if (startCnt) {
                ++cnt;
            }
            if (cnt > 100) {
                break;
            }
            follow.insert(follow.end(), s[i]);
            if (s[i] == '.') {
                startCnt = true;
                continue;
            }
        }
        return follow;
    }
  
}
void selfTestingModule() {
    //TEST 1
    cout << BANNER << endl;
    cout << "self test starting..." << endl;
    Integer debugA("997998789789");
    Integer debugB("966698789789");
    Integer debugC;
    debugC = (debugA + debugB);
    if (debugC.value == "1964697579578" && debugC.positive == true) { cout << "large number addition OK" << endl; }
    else { cout << "large number addition NO!!" << endl; }
    debugC = (debugA - debugB);
    if (debugC.value == "31300000000" && debugC.positive == true) { cout << "large number subtraction OK" << endl; }
    else { cout << "large number subtraction NO!!" << endl; }
    debugC = debugA * debugB;
    if (debugC.value == "964764222299912910664521" && debugC.positive == true) { cout << "large number multiply OK" << endl; }
    else { cout << "large number multiply NO!!" << endl; }
    debugC = debugA / debugB;
    if (debugC.value == "1" && debugC.positive == true) { cout << "large number division OK" << endl; }
    else { cout << "large number division NO!!" << endl; }
    string fc;
    fc = factorial(100);
    if (fc == "93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000") { cout << "factorial OK" << endl; }
    else { cout << "factorial NO!!" << endl; }
    fc = factorial(71);
    if (fc == "850478588567862317521167644239926010288584608120796235886430763388588680378079017697280000000000000000") { cout << "factorial OK" << endl; }
    else { cout << "factorial NO!!" << endl; }
    cout << "self test done!" << endl;
    cout << BANNER << endl;
    cout << ">> ";
}
void listModule() {
    cout << BANNER << endl;
    cout << " ALL VARIABLE LIST " << endl;
    cout << BANNER << endl;
    int index = 1;
    if (IntVariable.size() == 0) {
        cout << " There are no integer variables " << endl;
    }
    else {
        cout << " Interger: " << endl;
        for (auto iter = IntVariable.begin(); iter != IntVariable.end(); ++iter) {
            cout << " " << index++  << " - " << iter->first << endl;
        }
    }
    if (DecVariable.size() == 0) {
        cout << " There are no decimal variables " << endl;
    }
    else {
        index = 1;
        cout << " Decimal: " << endl;
        for (auto iter2 = DecVariable.begin(); iter2 != DecVariable.end(); ++iter2) {
            cout << " " << index++ << " - " << iter2->first << endl;
        }
    }
    cout << BANNER << endl;
    cout << ">> ";
}
void helpModule() {
    cout << BANNER << endl;
    cout << " Welcome to use high precision calculator" << endl;
    cout << " You can input any validate expression" << endl;
    cout << " Such as: " << endl;
    cout << " - 1 / 7 * 3 * -(6 + 1)" << endl;
    cout << " - 123456^80" << endl;
    cout << " etc..." << endl;
    cout << " And we support variable setup" << endl;
    cout << " - Set [Integer/Decimal] [Name] = [expression]" << endl;
    cout << " All the variable need validate expression" << endl;
    cout << " Such as : Set Decimal myDec = 2.0" << endl;
    cout << " E N J O Y " << endl;
    cout << BANNER << endl;
    cout << ">> ";
}


void stringToValue(string source, Integer& obj) {
    if (source == "") {
        obj.positive = true;
        obj.value = "0";
        obj.inf = false;
        return;
    }
    source.erase(remove_if(source.begin(), source.end(), isspace), source.end()); //remove white space
    if (source == "Inf") {
        //特例
        obj.positive = true;
        obj.value = "";
        obj.inf = true;
        return;
    }
    else if (source == "-Inf") {
        //特例
        obj.positive = false;
        obj.value = "";
        obj.inf = true;
        return;
    }

    for (char c : source) {
        //僅允許 . - 0-9
        if (c == '.' || c == '-' || (c >= '0' && c <= '9')) {
            //ACCEPT
            //DO NOTHING, 方便邏輯查看
        }
        else {
            //ERROR
            obj.errorCode = obj.ErrorCode::INVALIDINPUT;
            return;
        }
    }
    if (source.size() > 0) {
        //先針對有無負號做分類
        if (source[0] == '-') {
            source.erase(source.begin());
            obj.positive = false;
        }
        else { obj.positive = true; }
        while (source.length() > 1 && source[0] == '0' && source[1] != '.') {
            source.erase(source.begin());
        }
        //接下來有兩種可能 
        //1.有小數點
        //2.無小數點
        //不管如何 碰到小數點就直接停止
        string afterSource = ""; //無小數點的source
        for (int i = 0; i < source.size(); ++i) {
            if (source[i] == '.')
                break;
            afterSource.push_back(source[i]);
        }
        obj.value = afterSource;
    }
}
void stringToDecimal(string source, Decimal& obj) {
    if (source == "") {
        obj.positive = true;
        obj.value = "0";
        obj.inf = false;
        return;
    }
    source.erase(remove_if(source.begin(), source.end(), isspace), source.end()); //remove white space
    if (source == "Inf") {
        //特例
        obj.positive = true;
        obj.value = "";
        obj.inf = true;
        obj.numerator = "1";
        obj.denominator = "1";
        return;
    }
    else if (source == "-Inf") {
        //特例
        obj.positive = false;
        obj.value = "";
        obj.inf = true;
        obj.numerator = "1";
        obj.denominator = "1";
        return;
    }
    bool point = false;
    bool negative = false;
    for (char c : source) {
        //僅允許 . - 0-9
        if (!(c == '.' || c == '-' || (c >= '0' && c <= '9'))) {
            //ERROR
            std::cout << "error: Invalid input at '" << c << "'" << endl;
            return;
        }
        if (c == '-' && negative) {
            std::cout << "error: duplicate '-' (Negative sign)" << endl;
            return;
        }
        if (c == '.' && point) {
            std::cout << "error: duplicate '.' (Decimal point)" << endl;
            return;
        }
        if (c == '-')
            negative = true;
        if (c == '.')
            point = true;
    }
    if (source.size() > 0) {
        //先針對有無負號做分類
        if (source[0] == '-') {
            source.erase(source.begin());
            obj.positive = false;
        }
        else { obj.positive = true; }
        //meet decimal point?
        bool MDP = false;
        for (char c : source) {
            if (c == '.')
                MDP = true;
        }
        if (!MDP) {
            //data only have integer part, we need fill 100 zero
            source.insert(source.end(), '.');
            for (int i = 0; i < 100; i++) {
                source.insert(source.end(), '0');
            }
            obj.value = source;
            obj.numerator = source;
            obj.denominator = "1";
            return;
        }
        int cnt = 0;
        MDP = false;
        for (char c : source) {
            if (MDP)
                ++cnt;
            if (c == '.')
                MDP = true;
        }
        while (cnt < 100) {
            ++cnt;
            source.insert(source.end(), '0');
        }
        obj.value = source;
        obj.numerator = source;
        obj.denominator = "1";
    }
}


bool isVariableExist(string name) {

    auto iter = IntVariable.find(name);
    if (iter != IntVariable.end()) {
        return true;
    }
    auto iter2 = DecVariable.find(name);
    if (iter2 != DecVariable.end()) {
        return true;
    }
    return false;


}

void clsModule() {
       system("cls");
    cout << BANNER << endl;
    cout << "input 'selftest' to do self-testing." << endl;
    cout << "input 'help' to open help pages." << endl;
    cout << "input 'exit' to leave calculator." << endl;
    cout << "input 'list' to show all variable." << endl;
    cout << "input 'cls' to clear current output." << endl;
    cout << BANNER << endl;
    cout << ">> ";
}

int main() {

/*
    while (cin >> at >> bt) {

       /*

        cout << "+:";
        temp = at + bt;
        cout << "next: " << endl;
        cout << temp << endl;

        cout << "-:";
        temp = at - bt;
        cout << "next: " << endl;
        cout << temp << endl;

        cout << "*:";
        temp = at * bt;
        cout << "next: " << endl;
        cout << temp << endl;

        cout << "/:";
        temp = at / bt;
        cout << "next: " << endl;
        cout << temp << endl;
    }
    */
  
    system("cls");
    cout << BANNER << endl;
    cout << "input 'selftest' to do self-testing." << endl;
    cout << "input 'help' to open help pages." << endl;
    cout << "input 'exit' to leave calculator." << endl;
    cout << "input 'cls' to clear current output." << endl;
    cout << "input 'exit' to leave calculator." << endl;
    cout << BANNER << endl;
    cout << ">> ";
    string statement;
    while (getline(cin, statement)) {
        if (statement == "cls")
            clsModule();
        else if (statement == "selftest")
            selfTestingModule();
        else if (statement == "exit")
            return 0;
        else if (statement == "help")
            helpModule();
        else if (statement == "list")
            listModule();
        else if (statement.size() >= 3) {
            bool assignmentSign = false;
            bool setInteger = false;
            bool setDecimal = false;
       
            for (char c : statement) {
                if (c == '=') {
                    assignmentSign = true;
                }
            }

            if (statement.substr(0, 3) == "Set") {
                //設定變數
                if (!assignmentSign) {
                    cout << "missing assignment sign (=)" << endl;
                    cout << ">> ";
                    continue;
                }
                size_t found = statement.find("Integer");
                if (found != std::string::npos)
                    setInteger = true;
                found = statement.find("Decimal");
                if (found != std::string::npos)
                    setDecimal = true;
                if (setInteger && setDecimal) {
                    cout << BANNER << endl << "syntax error" << endl;
                    cout << ">> ";
                    continue;
                }
                else if (!setInteger && !setDecimal) {
                    cout << "missing data type (Integer or Decimal)" << endl;
                    cout << ">> ";
                    continue;
                }
                auto n = statement.find("Set");
                if (n != std::string::npos)
                {
                    statement.erase(n, 3);
                }
                n = statement.find("Integer");
                if (n != std::string::npos)
                {
                    statement.erase(n, 7);
                }
                n = statement.find("Decimal");
                if (n != std::string::npos)
                {
                    statement.erase(n, 7);
                }
                string variableName = "";
                for (int i = 0; i < statement.length();) {
                    if (statement[i] == '=') {
                        statement.erase(statement.begin());
                        break;
                    }
                    variableName.insert(variableName.end(), statement[i]);
                    statement.erase(statement.begin());
                }
                string s = calculate(statement);
                variableName.erase(remove_if(variableName.begin(), variableName.end(), isspace), variableName.end());
                if (isVariableExist(variableName)) {
                    cout << "error: Variable '" << variableName << "' already exist." << endl;
                    cout << ">> ";
                    continue;
                }
                cout << "Setting variable '" << variableName << "'" << endl;               
                if (setInteger) {
                    Integer temp(s);
                    IntVariable[variableName] = temp;
                }
                else {
                    Decimal temp(s);
                    DecVariable[variableName] = temp;
                }
                cout << ">> ";
                continue;
            }
            if (assignmentSign) {
                string variableName = "";
                for (int i = 0; i < statement.length();) {
                    char c = statement[i];
                    if (c == '='){
                        statement.erase(statement.begin());
                        break;
                    }
                    variableName.insert(variableName.end(), c);
                    statement.erase(statement.begin());
                    
                }
                variableName.erase(remove_if(variableName.begin(), variableName.end(), isspace), variableName.end());
                if (!isVariableExist(variableName)) {
                    cout << "error: Variable '" << variableName << "' doesn't exist." << endl;
                    cout << ">> ";
                    continue;
                }
                cout << variableName << "  =   " << statement << endl;
                string s = calculate(statement);
                auto iter1 = IntVariable.find(variableName);
                auto iter2 = DecVariable.find(variableName);
                if (iter1 != IntVariable.end()) {
                    Integer temp(s);
                    IntVariable[variableName] = temp;
                    cout << ">> ";
                    continue;
                }
                else if (iter2 != DecVariable.end()) {
                    Decimal  temp(s);
                    DecVariable[variableName] = temp;
                    cout << ">> ";
                    continue;
                }
                continue;
            }
            //基本運算
            string s = calculate(statement);
            s = filter(s);
            cout << BANNER << endl;
            cout << "calculate: " << statement << endl;
            cout << BANNER << endl;
            cout << "result: " << s << endl;
            cout << BANNER << endl;
            cout << ">> ";            
        }
        else {
            //length < 3
            string s = calculate(statement);
            s = filter(s);
            cout << BANNER << endl;
            cout << "calculate: " << statement << endl;
            cout << BANNER << endl;
            cout << "result: " << s << endl;
            cout << BANNER << endl;
            cout << ">> ";
        }


    }


}
Integer::Integer() {
    this->value = "";
    this->inf = false;
    this->positive = true;
}
Integer::Integer(string source) {
    this->inf = false;
    this->positive = true;
    bool needCal = false;
    if (source.length() > 0 && source[0] != '-') {
        for (char c : source) {
            if (isOperChar(c))
                needCal = true;
        }
    }
    if (needCal) {
        source = calculate(source);
    }
    else {
        stringToValue(source, *this);
        return;
    }
    if (source.length() > 0 && source[0] == '-') {
        source.erase(source.begin());
        this->positive = false;
    }
    this->value = source;
}
Integer::Integer(const char* s) {
    this->inf = false;
    this->positive = true;
    string source(s);
    bool needCal = false;
    for (char c : source) {
        if (isOperChar(c))
            needCal = true;
    }
    if (needCal) {
        source = calculate(source);
    }
    else {
        stringToValue(source, *this);
        return;
    }
    if (source.length() > 0 && source[0] == '-') {
        source.erase(source.begin());
        this->positive = false;
    }
    this->value = source;
    
}

Decimal::Decimal() {
    this->denominator = "1";
    this->numerator = "1";
    this->value = "";
    this->inf = false;
    this->positive = true;
}
Decimal::Decimal(string source) {
    this->denominator = "";
    this->numerator = "";
    this->inf = false;
    bool needCal = false;
    if (source.length() > 0 && source[0] != '-') {
        for (char c : source) {
            if (isOperChar(c))
                needCal = true;
        }
    }
    if (needCal) {
        source = calculate(source);
    }
    else {
        stringToDecimal(source, *this);
        return;
    }
    if (source.length() > 0 && source[0] == '-') {
        source.erase(source.begin());
        this->positive = false;
    }
    this->value = source;
}
Decimal::Decimal(const char* s) {    
    this->positive = true;
    this->inf = false;
    string source(s);
    bool needCal = false;
    for (char c : source) {
        if (isOperChar(c))
            needCal = true;
    }
    if (needCal) {
        source = calculate(source);
    }
    else {
        stringToDecimal(source, *this);
        return;
    }
    if (source.length() > 0 && source[0] == '-') {
        source.erase(source.begin());
        this->positive = false;
    }
    this->value = source;
}