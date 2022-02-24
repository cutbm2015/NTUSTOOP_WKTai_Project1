#include <string>
#include <iostream>
#include <sstream>

//#define DEBUG
#define S_PRECISION 200
#define D_EPSILON "0.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009"
#define BANNER "======================================================================"
using namespace std;

static map<unsigned long long int, string> factorialTable = { {0, "1"} };
static map<char, unsigned int>operCost = { {'!', 1},{'^',2},{'+', 2},{'-', 2},{'*', 2},{'/', 2} };
//static map<string, Integer> IntVariable;
//static map<string, Decimal> DecVariable;
map<string, Integer> IntVariable;
map<string, Decimal> DecVariable;
vector<string> valueQueue;
vector<string> operStack;
vector<string> statementPieces;

bool isVariableExist(string name);




int fLoc;
string sqrtTemp = "";
int openRoot(int len, char* iAddr, int I)
{
    char c;
    char* cAddr = iAddr;
    if (len > 0)
    {
        for (fLoc = 0; cAddr[fLoc]; cAddr[fLoc++] -= 10)
        {
            cAddr[fLoc++] -= 120;
            cAddr[fLoc] -= 110;
            while (!openRoot(0, iAddr, fLoc))
                cAddr[fLoc] += 20;
            sqrtTemp.insert(sqrtTemp.end(), ((cAddr[fLoc] + 1032) / 20));
        }
       
    }
    else
    {
        c = len + (cAddr[I] + 82) % 10 - (I > fLoc / 2) * (cAddr[I - fLoc + I] + 72) / 10 - 9;
        cAddr[I] += I < 0 ? 0 : !(len = openRoot(c / 10, iAddr, I - 1)) * ((c + 999) % 10 - (cAddr[I] + 92) % 10);
        /*
        c = len + (cAddr[I] + 82) % 10 - (I > fLoc / 2) * (cAddr[I - fLoc + I] + 72) / 10 - 9;
        int addNum = 0;
        if (I > 0) {
            int tempA = (c + 999) % 10;
            int tempB = (cAddr[I] + 92) % 10;
            tempA -= tempB;
            int secondRoot = !(len = openRoot(c / 10, iAddr, I - 1));
            cAddr[I] += secondRoot * tempA;
        }*/
     
    }
    return len;
}

string squareRoot(string source) {
    if (source.length() > 0 && source[0] == '-') {
        cout << "square root can't be negative value." << endl;
        return "0";
    }

    //24
    //fill zero
    string sourcetemp = source;
    if (isVariableExist(sourcetemp)) {
        auto iterInt = IntVariable.find(sourcetemp);
        auto iterDec = DecVariable.find(sourcetemp);
        if (iterInt != IntVariable.end()) {
            sourcetemp = iterInt->second.value;
            if (!iterInt->second.positive) {
                cout << "square root can't be negative value." << endl;
                return "0";
            }
        }
        else if (iterDec != DecVariable.end()) {
            sourcetemp = iterDec->second.value;
            if (!iterDec->second.positive) {
                cout << "square root can't be negative value." << endl;
                return "0";
            }
        }
        else {
            cout << BANNER << " syntax error" << endl;
            return "0";
        }
    }
#ifdef DEBUG
    cout << source << endl;
#endif // DEBUG

    unsigned long long int decdig = 0; //this decimal 有效位數       
    bool startCnt = false;
    bool MDP = false;
    for (char c : sourcetemp) {
        if (c == '.') {
            MDP = true;
            break;
        }
    }
    if (MDP) {

        //有效位數計算開始

       
        for (int i = sourcetemp.length() - 1; i >= 0; --i) {
            if (sourcetemp[i] == '.') {
                break;
            }
            if (sourcetemp[i] != '0') {
                startCnt = true;
            }
            if (startCnt) {
                decdig++;
            }
        }

        for (int i = sourcetemp.length() - 1; i >= 0; --i) {
            if (sourcetemp[i] != '0') {
                break;
            }

            sourcetemp.erase(sourcetemp.end() - 1);

        }


        for (int i = 0; i < sourcetemp.length(); ++i) {
            if (sourcetemp[i] == '.') {
                sourcetemp.erase(sourcetemp.begin() + i);
            }
        }

        cout << "decdig: " << decdig << endl;

    }

    //default 400 zero
    for (int i = 0; i < S_PRECISION; ++i) {
        sourcetemp.insert(sourcetemp.end(), '0');
    }
    if (MDP && decdig % 2 != 0) {
        sourcetemp.insert(sourcetemp.end(), '0');
    }

    sqrtTemp = "";
    char charArr[1200];  
    int index = 1;
    for (char c : sourcetemp) {
        charArr[index++] = c;
        if (index == 1200) {
            break;
        }
    }
    charArr[0] = '0';
    charArr[index] = '\0';
    if (strlen(charArr) % 2 == 1){
        openRoot(2, charArr + 1, 0);
    }else {
        openRoot(2, charArr, 0);
    }

    for (int i = 0; i < S_PRECISION; i++) {
        sqrtTemp.insert(sqrtTemp.begin(), '0');
    }
    int decOffset = 0;
    if (MDP) {
        decOffset = decdig / 2;
        if ( decdig % 2 != 0) {
            ++decOffset;
        }
    }
    sqrtTemp.insert(sqrtTemp.end() - 100 - decOffset, '.');

    //trim zero
    while ( sqrtTemp.length() > 1 && sqrtTemp[0] == '0' && sqrtTemp[1] != '.') {
        sqrtTemp.erase(sqrtTemp.begin());
    }
#ifdef DEBUG
    cout << sqrtTemp << endl;
#endif // DEBUG

    return sqrtTemp;
}

string trimAS(string source) {
    
    for (int i = 0; i < source.length() - 1; ++i) {
        if (source[i] == '+' && source[i + 1] == '+') {
            source.erase(source.begin() + i);
            --i;
            continue;
        }
        if (source[i] == '+' && source[i + 1] == '-') {
            source.erase(source.begin() + i);
            --i;
            continue;
        }
        if (source[i] == '-' && source[i + 1] == '-') {
            source[i + 1] = '+';
            source.erase(source.begin() + i);
            --i;
            continue;
        }
        if (source[i] == '-' && source[i + 1] == '+') {
            source[i + 1] = '-';
            source.erase(source.begin() + i);
            --i;
            continue;
        }

    }
#ifdef DEBUG
    cout << "after trimAs: " << source << endl;
#endif
    return source;
}

bool isSeparateable(char i) {
    
    switch (i) {
    case '!':       
    case '^':        
    case '+':        
    case '-':        
    case '*':        
    case '/':
    case '(':
    case ')':
        return true;
    default:
        return false;
    }
}

bool isOperChar(char c) {
    switch (c) {
    case '!':
        return true;
    case '^':
        return true;
    case '+':
        return true;
    case '-':
        return true;
    case '*':
        return true;
    case '/':
        return true;
    default:
        return false;
    }
}

bool isOper(string s) {
    if (s.length() > 1)
        return false;
    char i = s[0];
    switch (i) {
    case '!':
        return true;
    case '^':
        return true;
    case '+':
        return true;
    case '-':
        return true;
    case '*':
        return true;
    case '/':
        return true;
    default:
        return false;
    }
}

bool isR(string s) {
    char o = s[0];
    switch (o) {
    case '^':
        return true;
    default:
        return false;
    }
}
bool isL(string s) {
    char o = s[0];
    switch (o) {
    case '!':
    case '*':
    case '/':
    case '+':
    case '-':
        return true;
    default:
        return false;
    }
}


int precedence(string s)
{
    char c = s[0];
    switch (c) {
    case '!':
        return 4;
    case '^':
        return 3;
    case '*':
    case '/':
        return 3;
    case '+':
    case '-':
        return 2;
    case '=':
        return 1;
    }
    //none of above
    return 0;
}

bool isInteger(string s) {
    for (char c : s) {
        if (c == '-') {
            continue;
        }
        if (c > '9' || c < '0') {
            return false;
        }
    }
    return true;
}

void toPieces(string statement) {
    
    statementPieces.clear();
    //statement

    //1+++++1--+-+---1
    //1.5 + 3 * ( -(-5) )


    //策略 先把空格去除
    //1.5+3*(-(-5))
    //把運算子前後增加空格

    statement.erase(remove_if(statement.begin(), statement.end(), isspace), statement.end());
    string af = "";
    for (int i = 0; i < statement.size(); ++i) {
        if (i == 0 && statement[0] == '-') {
            af.push_back(statement[i]);
            continue;
        }
        if (statement[i] == '-') {
            //if (i > 0 && statement[i - 1] == '(') {
                
            if (i > 0 && (statement[i - 1] == '(' || statement[i - 1] == '/' || statement[i - 1] == '*')) {
                af.push_back(statement[i]);
                continue;
            }
        }
        if (isSeparateable(statement[i])) {
            af.push_back(' ');
            af.push_back(statement[i]);
            af.push_back(' ');
        }
        else {
            af.push_back(statement[i]);
        }
    }

#ifdef DEBUG
    cout << "after sperate" << af << endl;
    cout << endl;
#endif // DEBUG


    istringstream in(af);

    // M 1
    string t;
    while (in >> t) {
        statementPieces.push_back(t);
    }
    for (unsigned int i = 0; i < statementPieces.size()-1; ++i) {
        string temp = statementPieces[i];
        if(temp.length() > 0 && temp[0] == '-'){
            if (statementPieces[i + 1] == "!") {
                temp.erase(temp.begin());
                statementPieces[i] = temp;
                statementPieces.insert(statementPieces.begin() + i, "(");
                statementPieces.insert(statementPieces.begin() + i, "*");
                statementPieces.insert(statementPieces.begin() + i, "-1");
                statementPieces.insert(statementPieces.begin() + i + 5, ")");
            }
        }
    }
    for (unsigned int i = 0; i < statementPieces.size() ; ++i) {
        if (isVariableExist(statementPieces[i])) {
            statementPieces.insert(statementPieces.begin() + i, "*");
            statementPieces.insert(statementPieces.begin() + i, "1");
            statementPieces.insert(statementPieces.begin() + i, "(");
            statementPieces.insert(statementPieces.begin() +i+ 4, ")");
            i+=3;
        }
    }
    for (unsigned int i = 0; i < statementPieces.size() - 1; ++i) {
        if (statementPieces[i] == "(" && statementPieces[i + 1] == "(") {
            statementPieces.insert(statementPieces.begin() + i + 1, "*");
            statementPieces.insert(statementPieces.begin() + i + 1, "1");
            --i;
        }
    }
    for (unsigned int i = 0; i < statementPieces.size()-1; ++i) {
        //原始         if (statementPieces[i] == "-" && statementPieces[i + 1] == "("
        if (statementPieces[i] == "-" && statementPieces[i + 1] == "("){
            statementPieces[i] = "-1";
            statementPieces.insert(statementPieces.begin() + i + 1 , "*");
            --i;
        }
    }
    for (unsigned int i = 0; i < statementPieces.size() - 1; ++i) {
        if (!isOper( statementPieces[i] ) && statementPieces[i + 1] == "(") {
           //3(4+5) = 3 * (4+5)
            statementPieces.insert(statementPieces.begin() + i + 1, "*");
            --i;
        }
    }

    for (unsigned int i = 0; i < statementPieces.size() - 1; ++i) {
        if (statementPieces[i] == ")" && !isOper(statementPieces[i+1]) && statementPieces[i + 1] != ")") {
            //(2+3) 3 = (2+3) * 3
            statementPieces.insert(statementPieces.begin() + i + 1, "*");
            --i;
        }
    }

    for (unsigned int i = 0; i < statementPieces.size() - 1; ++i) {
        string temp = statementPieces[i];
        if (temp.length() > 0 && temp[0] == '-' && statementPieces[i + 1] == "^") {
            statementPieces[i].erase(statementPieces[i].begin());
            statementPieces.insert(statementPieces.begin() + i, "(");
            statementPieces.insert(statementPieces.begin() + i, "-1");
            statementPieces.insert(statementPieces.begin() + i, "*");
            statementPieces.insert(statementPieces.begin() + i+6, ")");
        }
    }


#ifdef DEBUG
    cout << "statementPieces: " << endl;
    for (string sp : statementPieces) {
  
        cout << sp << endl;
    }
    cout << endl;
#endif // DEBUG
}

string factorial(unsigned long long int i) {
    auto iter = factorialTable.find(i);
    if (iter != factorialTable.end())
        return iter->second;


    Integer Base("1");
    Integer ratio;
    for (long long int end = i; end >= 0; --end) {
        iter = factorialTable.find(end);
        if (iter != factorialTable.end()) {
            Integer ratio((iter->second));
            //找到先前算的 直接拿來用
            ratio.value = iter->second;
            Base = Base * ratio;
            factorialTable[i] = Base.value;
            return Base.value;
        }
        ratio.value = to_string(end);
        Base = Base * ratio;
       
    }
    factorialTable[i] = Base.value;
    return Base.value;
}

string power(string a, string b) {
    //a^b
    if (b == "0") {
        return "1";
    }
    if (b == "1.0") {
        return a;
    }
    if (!isInteger(b)) {
        int zeroCnt = 0;
        bool zeroStart = false;
        for (char c : b) {
            if (zeroStart && c == '0') {
                ++zeroCnt;
            }
            if (c == '.') {
                zeroStart = true;
            }
        }
        if (zeroCnt >= 100) {
            while (b[b.length() - 1] != '.') {
                b.erase(b.end() - 1);
            }
            b.erase(b.end() - 1);
        }
        else {
            return "error: power can't be decimal";
        }
    }
    if (isInteger(a) && isInteger(b)) {
        //整數power
        bool postiveSign = true;
        Integer ans(a);
        Integer base(a);
        char* endptr = NULL;
        unsigned long long int times = strtoull(b.c_str(), &endptr, 10);
        if (b.c_str() == endptr) {
            cout << "error at power(" << a << "," << b << ")" << endl;
            return "0";
        }
        if (times == 0)
            return "1";
        for (unsigned long long int i = 0; i < times-1; ++i) {
            ans = ans * base;
        }
        if (times % 2 == 1) {
            //基數
            if (a.length() > 0 && a[0] == '-') {
                postiveSign = false;
            }
        }
        return (postiveSign ? "":"-")+ ans.value;
    }
    else if(isInteger(b)){
        bool postiveSign = true;
        Decimal ans(a);
        Decimal base(a);
        char* endptr = NULL;
        unsigned long long int times = strtoull(b.c_str(), &endptr, 10);
        if (b.c_str() == endptr) {
            cout << "error at power(" << a << "," << b << ")" << endl;
            return "0";
        }
        if (times == 0)
            return "1";
        for (unsigned long long int i = 0; i < times - 1; ++i) {
            ans = ans * base;
        }
        if (times % 2 == 1) {
            //基數
            if (a.length() > 0 && a[0] == '-') {
                postiveSign = false;
            }
        }
        return (postiveSign ? "" : "-") + ans.value;
    }
    return "";


}

/// <summary>
/// 用於計算任意陳述句並回傳結果。
/// </summary>
/// <param name="statement">陳述句，如2+2或2^2^2或2! + 3 (4+5)等</param>
/// <returns>回傳結果的字串</returns>
string calculate(string statement) {
    if (statement == "") {
        return "error: the expression can't be null";
    }
    
    valueQueue.clear();
    operStack.clear();
    toPieces(trimAS(statement));
    //return "";

    for (int len = 0; len < statementPieces.size(); ++len) {
        if (isOper(statementPieces[len])) {
            string operTemp1 = statementPieces[len]; //currentOper
            if (operStack.size() >= 1) {
              
                string operTemp2 = operStack.back(); //Stack Oper           
                while ((isL(operTemp1) && (precedence(operTemp1) <= precedence(operTemp2))) || (isR(operTemp1) && (precedence(operTemp1) < precedence(operTemp2)))) {
                    valueQueue.push_back(operTemp2);
                    operStack.pop_back();
                    if (operStack.size() == 0)
                        break;
                    operTemp2 = operStack.back(); //Stack Oper
                   // valueQueue.push_back(operTemp2);
                }
                
            }
            //o1 into stack
            operStack.push_back(operTemp1);
        
        }
        else if (statementPieces[len] == "(") {
            //into stack
            operStack.push_back(statementPieces[len]);
        }
        else if (statementPieces[len] ==  ")") {
            if (operStack.size() > 0) {
                string val = operStack.back();
                while (val != "(") {
                    valueQueue.push_back(val);
                    operStack.pop_back();
                    if (operStack.empty()) {
                        cout << BANNER << endl << "syntax error" << endl;
                        return "";
                    }
                    val = operStack.back();
                 
                }

                operStack.pop_back();
            }
            if (operStack.size() > 0) {
                string val = operStack.back();
                val = operStack.back();
                auto isIntegerVariable = IntVariable.find(val);
                auto isDecimalVeriable = DecVariable.find(val);
                if (isIntegerVariable != IntVariable.end() || isDecimalVeriable != DecVariable.end()) {
                    valueQueue.push_back(val);
                    operStack.pop_back();
                }
            }
        }
        else {
            valueQueue.push_back(statementPieces[len]);
            if (false) {
                auto isIntegerVariable = IntVariable.find(statementPieces[len]);
                auto isDecimalVeriable = DecVariable.find(statementPieces[len]);
                if (isIntegerVariable != IntVariable.end()) {
                    //found

                    operStack.push_back(statementPieces[len]);
                    continue;
                }
                else if (isDecimalVeriable != DecVariable.end()) {
                    //found


                    operStack.push_back(statementPieces[len]);
                    continue;
                }
                else {
                    //is value
                    valueQueue.push_back(statementPieces[len]);
                    //cout << " located at " << len << " , detected invalid input '" << statementPieces[len] << "' (Invalid Input)" << endl;
                    //return "";
                }
            }
        }
    }
    ///if == ) or ( => error
    while (operStack.size() > 0) {
        string operTemp = operStack.back();
        if (operTemp == ")" || operTemp == "(") {
            cout << "Error: There are mismatched symbol." << endl;
            return "";
        }
        valueQueue.push_back(operTemp);
        operStack.pop_back();
    }
#ifdef DEBUG


    cout << "os" << endl;
    for (string o : operStack) {
        cout << o << endl;
    }
    cout << endl;
    cout << "vq" << endl;
    for (string o : valueQueue) {
        cout << o << endl;
    }
    cout << endl;
#endif // DEBUG

    
    //開始計算
    for(int o = 0; o < valueQueue.size(); o++){
        if (isOper(valueQueue[o])) {
            if (valueQueue[o] == "!") {
                //階層

               //is answer decimal?

                bool isAnswerDecimal = false;
                
                string temp = valueQueue[o - 1];
                if (temp.length() > 0 && temp[0] == '-') {
                //    cout << "factorial can't be negative value" << endl;
                    return "error factorial can't be negative value";
                }
         
                if (!isInteger(temp)) {
                    int zeroCnt = 0;
                    bool zeroStart = false;
                    for (char c : temp) {
                        if (zeroStart && c == '0') {
                            ++zeroCnt;
                        }
                        if (c == '.') {
                            zeroStart = true;
                        }
                    }
                    if (zeroCnt >= 100) {
                        while (temp[temp.length() - 1] != '.') {
                            temp.erase(temp.end() - 1);
                        }
                        isAnswerDecimal = true;
                    }
                    else {
                        return "error: factorial can't be decimal";
                    }
                }
                if (isVariableExist(temp)) {
                    auto intIter = IntVariable.find(temp);
                    auto decIter = DecVariable.find(temp);
                    if (intIter != IntVariable.end()) {
                        //is variable
                        temp = intIter->second.value;
                        if (!intIter->second.positive) {
                           // cout << "factorial can't be negative value" << endl;
                            return "error factorial can't be negative value";
                        }                       
                    }
                    else if (decIter != DecVariable.end()) {
                        //is variable
                        temp = decIter->second.value;
                        for (int i = temp.length() - 1; i >= 0; --i){
                            if (temp[i] == '.') {
                                temp.erase(temp.end());
                                break;
                            }
                            temp.erase(temp.end());
                        }
                        if (!decIter->second.positive) {
                            cout << "factorial can't be negative value" << endl;
                            return "error";
                        }
                    }
                    else {
                        //
                        cout << "error" << endl;
                    }
                }

                
                //epsilon

               /* if (!isInteger(temp)) {
                    if (temp.length() > 0) {
                        Decimal target(temp);
                        Decimal epsilon(D_EPSILON);
                        if (temp[0] == '-') {
                            epsilon.positive = false;
                        }
                        cout << target.value << endl;
                        cout << epsilon.value << endl;
                        target = target + epsilon;
                        valueQueue[o-1] = (target.positive ? "" : "-") + target.value;
                        temp = target.value;
                    }
                }
                //*/
                


                char* endptr = NULL;
                unsigned long long int f = strtoull(temp.c_str(), &endptr, 10);
                if (temp.c_str() == endptr) {
                    cout << "error factorial input" << endl;
                    return "";
                }
                string result = factorial(f);
                if (isAnswerDecimal) {
                    Decimal casting(result);
                    result = casting.value;
                    // cout << "i expect answer is decimal" << endl;
                }
                valueQueue[o] = result;
                valueQueue.erase(valueQueue.begin() + o - 1);
                o = 0;
                continue;
            }
            if (o < 2) { continue; }
            string A = valueQueue[o - 2];
            string B = valueQueue[o - 1];

            //次方？

            if (valueQueue[o] == "^") {
                //次方
              
                bool helfRoot = false;

#ifdef DEBUG
                bool debug = isInteger(B);
                if (debug)
                    cout << "isInteger(B) : true" << endl;
                else
                    cout << "isInteger(B) : false" << endl;
                
#endif // DEBUG

                if (!isInteger(B)) {
                    while (B.length() > 2 && B[B.length() - 1] == '0' && B[B.length() - 2] != '.') {
                        B.erase(B.end() - 1);
                    }
                    if (B.length() > 2) {
                        if (B[B.length() - 1] == '5' && B[B.length() - 2] == '.') {
                            helfRoot = true;

                            B.erase(B.end() - 1);
                            B.erase(B.end() - 1);

                            
                        }
                    }
                }
                    
                
             //2.5^1.5 = 2.5^1 * 2.5^0.5
                string answer = power(A, B);

                if (helfRoot) {
                    Decimal answerDec(answer);

                    //addon = half root
#ifdef DEBUG
                    cout << squareRoot(A);
#endif // DEBUG


                    Decimal addOn(squareRoot(A));
                    answerDec = answerDec * addOn;
                    answer = answerDec.value;
                }
                
                valueQueue[o] = answer;
                valueQueue.erase(valueQueue.begin() + o - 2);
                valueQueue.erase(valueQueue.begin() + o - 2);
                o = 0;
                continue;
            }


            Integer AI;
            Integer BI;
            Decimal AD;
            Decimal BD;
            bool modeInteger = true;
            bool Atakeover = false;
            bool Btakeover = false;

            auto intIterA = IntVariable.find(A);
            auto decIterA = DecVariable.find(A);
          
            if (intIterA != IntVariable.end()) {
                //A is int variable
                AI.value = intIterA->second.value;
                AI.positive = intIterA->second.positive;
                A = intIterA->second.value;
                if (!intIterA->second.positive) {
                    A.insert(A.begin(), '-');
                }
                Atakeover = true;
            }
            else if (decIterA != DecVariable.end() ){
                //A is dec variable
                AD.value = decIterA->second.value;
                AD.positive = decIterA->second.positive;
                A = decIterA->second.value;
                if (!decIterA->second.positive) {
                    A.insert(A.begin(), '-');
                }
                Atakeover = true;
            }
            auto intIterB = IntVariable.find(B);
            auto decIterB = DecVariable.find(B);
            if (intIterB != IntVariable.end()) {
                //b is variable
                BI.value = intIterB->second.value;
                BI.positive = intIterB->second.positive;
                B = intIterB->second.value;
                if (!intIterB->second.positive) {
                    B.insert(B.begin(), '-');
                }
                Btakeover = true;
            }
            else if (decIterB != DecVariable.end()) {
                //b is variable
                BD.value = decIterB->second.value;
                BD.positive = decIterB->second.positive;
                B = decIterB->second.value;
                if (!decIterB->second.positive) {
                    B.insert(B.begin(), '-');
                }
                Btakeover = true;
            }
            if (!Atakeover && !Btakeover) {
                bool isAlphablet = false;
                for (char c : A) {
                    if (!(c == '-' || c == '.' || (c >= '0' && c <= '9'))) {
                        isAlphablet = true;
                        break;
                    }
                }
                if (isAlphablet) {
                    return "variable doesn't exist.";

                }
                isAlphablet = false;
                for (char c : B) {
                    if (!(c == '-' || c == '.' || (c >= '0' && c <= '9'))) {
                        isAlphablet = true;
                        break;
                    }
                }
                if (isAlphablet) {
                    return "variable doesn't exist.";

                }
            }
            if (!Atakeover) {
                if (isInteger(A)) {
                    AI = Integer(A);
                }
                else {
                    AD = Decimal(A);
                    modeInteger = false;
                }
            }
            if (!Btakeover) {
                if (isInteger(B)) {
                    BI = Integer(B);
                }
                else {
                    BD = Decimal(B);
                    modeInteger = false;
                }
            }
            if (AD.value != "" && BD.value == "") {
                //強制轉型
                Decimal temp(BI.value);
                temp.positive = BI.positive;
                temp.inf = BI.inf;
                BD = temp;

                modeInteger = false;
            }
            else if (BD.value != "" && AD.value == "") {
                //強制轉型
                Decimal temp(AI.value);
                temp.positive = AI.positive;
                temp.inf = AI.inf;
                AD = temp;
                modeInteger = false;
            }
            if (modeInteger) {
                Integer temp;
                if (valueQueue[o] == "+") {
                    temp = AI + BI;
                }
                else if (valueQueue[o] == "-") {
                    temp = AI - BI;
                }
                else if (valueQueue[o] == "*") {
                    temp = AI * BI;


             

                }
                else if (valueQueue[o] == "/") {
                    temp = AI / BI;
                }
                valueQueue[o] = (!temp.positive ? "-" + temp.value : temp.value);

                valueQueue.erase(valueQueue.begin() + o - 2);
                valueQueue.erase(valueQueue.begin() + o - 2);
                o = 0;
            }
            else {
                Decimal temp;
                if (valueQueue[o] == "+") {
                    temp = AD + BD;
                }
                else if (valueQueue[o] == "-") {
                    temp = AD - BD;
                }
                else if (valueQueue[o] == "*") {
                    temp = AD * BD;
                    //epsilon
                    Decimal epsilon(D_EPSILON);
                    epsilon.positive = temp.positive;
                    temp = temp + epsilon;
                    //
                }
                else if (valueQueue[o] == "/") {
                    temp = AD / BD;
                }
                valueQueue[o] = (!temp.positive ? "-" + temp.value : temp.value);


                valueQueue.erase(valueQueue.begin() + o - 2);
                valueQueue.erase(valueQueue.begin() + o - 2);
                o = 0;
            }
            //
            

            //
        }
    }
    if (valueQueue.size() > 1 || valueQueue.size() == 0) {

        cout << BANNER << endl << "syntax error" << endl;

        return "";
    }
    // cout << "test";
    // cout << "[debug] answer: " << endl << valueQueue[0] << endl;

    bool noOper = true;
    for (char c : valueQueue[0]) {
        if (c == '.' || (c >= '0' && c <= '9')) {
            noOper = false;
            break;
        }
    }
    if (noOper) {
       // cout << "error: syntax error" << endl;
        return "variable doesn't exist.";
    }
    if (!isInteger(valueQueue[0])) {
        string temp = valueQueue[0];
        if (temp.length() > 0) {
            Decimal target(temp);
           Decimal epsilon(D_EPSILON);
            //Decimal epsilon("0.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009");
            if (temp[0] == '-') {
                epsilon.positive = false;
            }
            cout << target.value << endl;
            cout << epsilon.value << endl;
            target = target + epsilon;
            valueQueue[0] = (target.positive?"":"-") +  target.value;
        }       
        //cout << "rounding off";
    }
    //cout << "doo";
    return valueQueue[0];
}
