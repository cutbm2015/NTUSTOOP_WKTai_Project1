#include "Decimal.h"

#define DEBUG

Decimal Decimal::operator+(Decimal other) {
    Decimal self;

    if (positive == false && other.positive == true) {
        Decimal temp = *this;
        temp.positive = true;
        Decimal temp2 = other;
        temp2.positive = true;
        temp =  temp2 - temp;
        return temp;
    }
    else if (positive == true && other.positive == false) {
        // 3 + -2
        Decimal temp = *this;
        Decimal temp2 = other;
        
        temp2.positive = true;
        temp = temp - temp2;
        return temp;
    }
    else if (positive == false && other.positive == false) {
        self.positive = false;
    }
    vector<int> thisIntegerPart;
    vector<int> thisDecimalPart;
    vector<int> otherIntegerPart;
    vector<int> otherDecimalPart;

#ifdef DEBUG
    cout << "this dec:" << endl;
    for (int i : thisDecimalPart) {
        cout << i << " ";
    }
    cout << endl << "other dec: " << endl;
    for (int i : otherDecimalPart) {
        cout << i << " ";
    }
    cout << endl;
#endif // DEBUG

  

    bool MDP = false; //meet decimal point
    for (int i = 0; i < value.size(); ++i) {
        if (value[i] == '.'){
            MDP = true;
            continue;
        }
        if (value[i] >= '0' && value[i] <= '9') {
            if (MDP) {
                thisDecimalPart.push_back(value[i] - '0');
            }
            else {
                thisIntegerPart.push_back(value[i] - '0');
            }
        }
        else {
            cout << "error: invalid input at'" << value[i] << "'" << endl;
            return self;
        }
    }
    if (!MDP) {
        cout << "warning: left value missing decimal parts" << endl;
        while (thisDecimalPart.size() < 100) {
            thisDecimalPart.push_back(0);
        }
    }
    MDP = false;
    for (int i = 0; i < other.value.size(); ++i) {
        char c = other.value[i];
        if (c == '.') {
            MDP = true;
            continue;
        }
        if (c >= '0' && c <= '9') {
            if (MDP) {
                otherDecimalPart.push_back(c - '0');
            }
            else {
                otherIntegerPart.push_back(c - '0');
            }
        }
        else {
            cout << "error: invalid input at'" << c << "'" << endl;
            return self;
        }
    }
    if (!MDP) {
        cout << "warning: right value missing decimal parts" << endl;
        while (otherDecimalPart.size() < 100) {
            otherDecimalPart.push_back(0);
        }
    }
    while (thisDecimalPart.size() < otherDecimalPart.size()) {
        thisDecimalPart.push_back(0);
    }
    while (otherDecimalPart.size() < thisDecimalPart.size()) {
        otherDecimalPart.push_back(0);
    }
    //幫otherDigits補0 使得位數相同
    while (thisIntegerPart.size() > otherIntegerPart.size())
        otherIntegerPart.insert(otherIntegerPart.begin(), 0);
    //幫thisDigits補0 使得位數相同
    while (thisIntegerPart.size() < otherIntegerPart.size())
        thisIntegerPart.insert(thisIntegerPart.begin(), 0);


    int carry = 0; //進位預設為0

    //decimal 相加
    carry = 0; //進位預設為0
    for (int i = thisDecimalPart.size() - 1; i >= 0; --i) {
        thisDecimalPart[i] += otherDecimalPart[i] + carry;
        carry = 0;
        if (thisDecimalPart[i] >= 10) {
            //進位
            thisDecimalPart[i] -= 10;
            ++carry;
        }
    }

    for (int i = thisIntegerPart.size() - 1; i >= 0; --i) {
        thisIntegerPart[i] += otherIntegerPart[i] + carry;
        carry = 0;
        if (thisIntegerPart[i] >= 10) {
            //進位
            thisIntegerPart[i] -= 10;
            ++carry;
        }
    }
    //當已經加到開頭，carry卻非0 = 幫thisDigits頭加1
    //舉例：50000+50000 = 00000 (此時carry = 1)
    //幫00000開頭插入1 變成 100000
    if (carry != 0) {
        thisIntegerPart.insert(thisIntegerPart.begin(), 1);
    }

    //轉回string
    string integerAnswer = "";
    string decimalAnswer = "";
    for (int i : thisIntegerPart) {
        integerAnswer.push_back(i + '0');
    }
    for (int i : thisDecimalPart) {
        decimalAnswer.push_back(i + '0');
    }



    self.value = integerAnswer + "." + decimalAnswer;

    //trim zero
    while (self.value.size() > 1 && self.value[1] != '.' && self.value[0] == '0') {
        self.value.erase(self.value.begin());
    }

    return self;

}
Decimal Decimal::operator-(Decimal other) {
    Decimal self = *this;

    if (this->positive == false && other.positive == true) {
        Decimal temp;
        this->positive = true;
        temp = (*this + other);
        temp.positive = false;
        return temp;
    }
    if (this->positive == true && other.positive == false) {
        //3 - (-2) = 3 + 2
        Decimal temp = *this;
        Decimal temp2 = other;
        temp2.positive = true;
        temp = temp + temp2;
        return temp;
    }


    string TIP = ""; //THIS INT PART
    string OIP = ""; //OTHER INT PART
    string TDP = ""; //THIS DEC PART
    string ODP = ""; //OTHER DEC PART
    bool MDP = false; //MEET DEC POINT?
    for (char c : this->value) {
        if (c == '.') {
            MDP = true;
            continue;
        }
        if (!MDP) {
            TIP.push_back(c);
        }
        else {
            TDP.push_back(c);
        }
    }

    MDP = false;
    for (char c : other.value) {
        if (c == '.') {
            MDP = true;
            continue;
        }
        if (!MDP) {
            OIP.push_back(c);
        }
        else {
            ODP.push_back(c);
        }
    }

    //有效位數計算開始
    unsigned long long int thisDecDig = 0; //this decimal 有效位數
    unsigned long long int otherDecDig = 0; //other decimal 有效位數
    unsigned long long int thisIntDig = TIP.length(); //this Int 有效位數
    unsigned long long int otherIntDig = OIP.length(); //other decimal 有效位數

    bool startCnt = false;
    for(int i = TDP.length() -1; i >=0;--i) {
        if (TDP[i] != '0') {
            startCnt = true;
        }
        if (startCnt) {
            thisDecDig++;
        }
    }
    startCnt = false;
    for (int i = ODP.length() - 1; i >= 0; --i) {
        if (ODP[i] != '0') {
            startCnt = true;
        }
        if (startCnt) {
            otherDecDig++;
        }
    }
    //有效位數計算結束
#ifdef DEBUG
    cout << endl << TIP << " " << TDP << endl;
    cout << OIP << " " << ODP << endl;
    cout << "validate this dec: " << thisDecDig << endl;
    cout << "validate other dec: " << otherDecDig << endl;
#endif // DEBUG

    string TDPTemp = "";
    string ODPTemp = "";
    for (int i = 0; i < (thisDecDig > otherDecDig?thisDecDig:otherDecDig); ++i) {
        TDPTemp.insert(TDPTemp.end(), TDP[i]);
        ODPTemp.insert(ODPTemp.end(), ODP[i]);
    }
    TDP = TDPTemp;
    ODP = ODPTemp;

    Integer TDPI(TDP);
    Integer ODPI(ODP);
    TDPI = TDPI - ODPI;

    Integer TIPI(TIP);
    Integer OIPI(OIP);
    TIPI = TIPI - OIPI;

    while (TDPI.value.length() > 100) {
        //cout << "len : " << TDPI.value.length() << endl;
        TDPI.value.erase(TDPI.value.end() - 1);
    }
    while (TDPI.value.length() < 100) {
        TDPI.value.insert(TDPI.value.end(), '0');
    }

    //999.999 - 111.111
    if (TDPI.positive && TIPI.positive) {
        //正 - 正 且前者 > 後者
    
        self.value = TIPI.value + "." + TDPI.value;
        self.positive = true;
        
        return self;
    }
    //1.789 - 3.456
    else {

   

        //新版
#ifdef DEBUG
        cout << endl << "this int validate: " << thisIntDig << endl;
        cout << "other int validate: " << otherIntDig << endl;
#endif // DEBUG
        for (int i = thisIntDig - otherIntDig; i > 0; --i) {
            other.value.insert(other.value.begin(), '0');
        }
        for (int i = otherIntDig - thisIntDig; i > 0; --i) {
            this->value.insert(this->value.begin(), '0');
        }
#ifdef DEBUG
        cout << this->value << endl;
        cout << other.value << endl;
#endif // DEBUG

        
        if (TIPI.positive == false) {
            //變成反過來減 給結果negative sign
            string temp = this->value;
            this->value = other.value;
            other.value = temp;
            self.positive = false;
        }
        else { self.positive = true; }

        while (this->value.length() < other.value.length()) {
            this->value.insert(this->value.end(), '0');
        }
        while (this->value.length() > other.value.length()) {
            other.value.insert(other.value.end(), '0');
        }
#ifdef DEBUG
        cout << this->value << endl;
        cout << other.value << endl;
#endif // DEBUG

        int borrow = 0;
        for (long long int i = this->value.length() - 1; i >= 0; --i) {
            if (this->value[i] == '.')
                continue;
            int thistmp, othertmp;
            thistmp = this->value[i] - '0';
            othertmp = other.value[i] - '0';
            thistmp -= (othertmp + borrow);
            borrow = 0;
            if (thistmp < 0) {
                thistmp += 10;
                ++borrow;
            }
            this->value[i] = thistmp + '0';
        }
        self.value = this->value;
        //trim zero
        while (self.value.size() > 1 && self.value[1] != '.' && self.value[0] == '0') {
            self.value.erase(self.value.begin());
        }
        return self;
    }
      
}


Decimal Decimal::operator*(Decimal other) {

    bool allZero = true;
    for (char c : this->value) {
        if (c == '.') { continue; }
        if (c != '0') {
            allZero = false;
            break;
        }
    }
    if (allZero) {
        Decimal temp("0");
        temp.positive = true;
        temp.inf = false;
        return temp;
    }
    allZero = true;
    for (char c : other.value) {
        if (c == '.') { continue; }
        if (c != '0') {
            allZero = false;
            break;
        }
    }
    if (allZero) {
        Decimal temp("0");
        temp.positive = true;
        temp.inf = false;
        return temp;
    }


    Decimal self;
    //有效位數計算開始
    long long int thisDecDigAll = 0; //this decimal 全部位數
    long long int otherDecDigAll = 0; //other decimal 全部位數

    long long int thisDecDig = 0; //this decimal 有效位數
    long long int otherDecDig = 0; //other decimal 有效位數
    long long int thisIntDig = 0; //this decimal 有效位數
    long long int otherIntDig = 0; //other decimal 有效位數

    bool startCntAll = true;
    bool startCnt = false;
    bool startInt = false;
    for (int i = this->value.length() - 1; i >= 0; --i) {
        if (startCntAll) {
            ++thisDecDigAll;
        }
        if (startInt) {
            thisIntDig++;
            continue;
        }
        if (this->value[i] == '.') {
            startCntAll = false;
            startInt = true;
            continue;
        }
        if (this->value[i] != '0') {
            startCnt = true;
        }
        if (startCnt) {
            thisDecDig++;
        }
    }
    startCntAll = true;
    startCnt = false;
    startInt = false;
    for (int i = other.value.length() - 1; i >= 0; --i) {
        if (startCntAll) {
            ++otherDecDigAll;
        }
        if (startInt) {
            otherIntDig++;
            continue;
        }
        if (other.value[i] == '.') {
            startCntAll = false;
            startInt = true;
            continue;
        }
        if (other.value[i] != '0') {
            startCnt = true;
        }
        if (startCnt) {
            otherDecDig++;
        }
    }
#ifdef DEBUG
    cout << "this dec dig all " << thisDecDigAll << endl;
    cout << "other dec dig all " << otherDecDigAll << endl;
    cout << "this dec dig " << thisDecDig << endl;
    cout << "other dec dig " << otherDecDig << endl;
    cout << "this int dig " << thisIntDig << endl;
    cout << "other int dig " << otherIntDig << endl;
#endif
    //有效位數計算結束
    for (int i = thisDecDigAll - otherDecDigAll; i > 0; --i) {
        other.value.insert(other.value.end(), '0');
    }
    for (int i = otherDecDigAll - thisDecDigAll; i > 0; --i) {
        this->value.insert(this->value.end(), '0');
    }
    for (int i = thisIntDig - otherIntDig; i > 0; --i) {
        other.value.insert(other.value.begin(), '0');
    }
    for (int i = otherIntDig - thisIntDig; i > 0; --i) {
        this->value.insert(this->value.begin(), '0');
    }
#ifdef DEBUG



    cout << this->value << endl;
    cout << other.value << endl;
#endif // DEBUG
    //策略 先把string每個位元拆開來存進vector
    vector<int> thisDigits;
    vector<int> otherDigits;
    for (char c : this->value) {
        if (c >= '0' && c <= '9') {
            thisDigits.push_back(c - '0');
        }        
    }
    for (char c : other.value) {
        if (c >= '0' && c <= '9') {
            otherDigits.push_back(c - '0');
        }       
    }

    int carry = 0;
    int offset = 0;
    string resultPrevious = "";
    string resultCurrent = "";
    Integer tempInteger;
    for (int i = otherDigits.size() - 1; i >= 0; --i) {
        resultCurrent = "";
        for (int j = thisDigits.size() - 1; j >= 0; --j) {
            int digitResult = (thisDigits[j] * otherDigits[i]) + carry;
            carry = 0;
            if (digitResult >= 10) {
                carry = digitResult / 10;
                digitResult %= 10;
            }
            resultCurrent.insert(resultCurrent.begin(), digitResult + '0');
        }
        if (resultPrevious == "") {
            if (carry != 0) {
                resultCurrent.insert(resultCurrent.begin(), carry + '0');
                carry = 0;
            }
            resultPrevious = resultCurrent;
            continue;
        }
        else {
            offset++;
            //補carry
            if (carry != 0) {
                resultCurrent.insert(resultCurrent.begin(), carry + '0');
                carry = 0;
            }
            //補0
            for (int os = 0; os < offset; ++os) {
                resultCurrent.insert(resultCurrent.end(), '0');
            }
            //previous + current
            Integer brokerA(resultPrevious);
            Integer brokerB(resultCurrent);
            brokerA = brokerA + brokerB;
            resultPrevious = brokerA.value;

        }
    }
    for (int i = 0; i < 200; ++i) {
        resultPrevious.insert(resultPrevious.begin(), '0');
    }
    unsigned int pointLoc = (thisDecDigAll > otherDecDigAll ? thisDecDigAll * 2 : otherDecDigAll * 2) -2;
    resultPrevious.insert(resultPrevious.end()-pointLoc, '.');
    if (resultPrevious[0] == '.') {
        resultPrevious.insert(resultPrevious.begin(), '0');
    }

#ifdef DEBUG
    cout << resultPrevious << endl;
    #endif // DEBUG

    self.value = resultPrevious;

    //判斷正負號
    //負 * 負 = 正
    //負 * 正 = 負..依此類推
    if (this->positive == false && other.positive == false)
        self.positive = true;
    else if (this->positive == true && other.positive == true)
        self.positive = true;
    else
        self.positive = false;

    //trim zero
   
    while (self.value.size() > 1 && self.value[1] != '.' && self.value[0] == '0') {
        self.value.erase(self.value.begin());
    }
    bool MDP = false;
    int cnt = 0;
    string temp = self.value;
    for (int i = 0; i < temp.length(); ++i) {
        if (MDP) {
            ++cnt;
        }
        if (cnt > 200) {
            temp.erase(temp.begin() + i , temp.end());
        }
        if (temp[i] == '.') {
            MDP = true;
            continue;
        }
    }
    self.value = temp;
#ifdef DEBUG


    cout << "**" << endl;
    cout << self.value << endl;
    cout << "**" << endl;

#endif // DEBUG
    return self;

}
/*
Decimal Decimal::operator*(Decimal other) {

    //假設 123.45678 * 1234.5678987 
    //從乘數的最小外數開始，從被乘數的最小位數開始依序乘

    Decimal ans("0");

    bool positiveTmp = positive;
    bool otherPositiveTmp = other.positive;

    positive = other.positive = true;   //都視為正數做運算

    int muledSize = 0;  //被乘數有效數字位數(含小數點)   ex.9
    int mulSize = 0;    //乘數有效數字位數(含小數點)    ex.12
    int muledPos = 0;   //被乘數最小有效位數位置    ex.5
    int mulPos = 0;     //乘數最小有效位數位置    ex.7

    int cnt = 0;
    bool MDF = false;   //meet dot flag
    for (int i = 0; i < value.size(); i++) { //find muledPos

        if (MDF) {
            cnt++;
            if (value[i] != '0' && value[i] != '.') {  //尋找最小有效位數位置
                muledPos = cnt;
            }
        }
        else
            muledSize++;

        if (value[i] == '.')
            MDF = true;

    }
    if (muledPos == 0)   //小數點後沒數值
        muledSize--;
    else {  //小數點後有數值
        muledSize += muledPos;
    }

    cnt = 0;
    MDF = false;
    for (int i = 0; i < other.value.size(); i++) { //find mulPos

        if (MDF) {
            cnt++;
            if (other.value[i] != '0' && other.value[i] != '.') {  //尋找最小有效位數位置
                mulPos = cnt;
            }
        }
        else
            mulSize++;

        if (other.value[i] == '.')
            MDF = true;
    }
    if (mulPos == 0)   //小數點後沒數值
        mulSize--;
    else {  //小數點後有數值
        mulSize += mulPos;
    }

    // cout << muledPos << " " << mulPos << endl;    //debugMine

     //開始做乘法運算
     //cout << muledSize << " " << mulSize << endl;    //debugMine
    for (int i = muledSize - 1; i >= 0; i--) { //i為目前指到被乘數字元的位置
        //cout << muledPos << " " << mulPosCopy << endl;    //debugMine
        if (value[i] == '.')
            continue;

        int mulPosCopy = mulPos;
        for (int j = mulSize - 1; j >= 0; j--) { //j為目前指到乘數字元的位置

            if (other.value[j] == '.')
                continue;

            int ansTmpI;
            ansTmpI = ((other.value[j]) - '0') * ((value[i]) - '0');
            // if (ansTmpI == 0)
            //     continue;
                 //cout << ansTmpI << endl;  //debugMine

            string ansTmpS = to_string(ansTmpI);

            int decimalAmount;  //小數點後應有的小數數量   ex. 0.08*0.5 => 3   /  25.3*0.6 => 2
            int integerAmount;  //應有的整數數量   ex. 0.8*9 => 1      /   900*0.3 => 3    / 300*0.2 => 2
            if (muledPos > 0 || mulPosCopy > 0) {    //有小數
                if (muledPos > 0 && mulPosCopy > 0) //乘數與被乘數都有小數
                    decimalAmount = muledPos + mulPosCopy;
                else if (muledPos > 0 && mulPosCopy <= 0)   //只有被乘數有小數
                    decimalAmount = muledPos;
                else    //只有乘數有小數
                    decimalAmount = mulPosCopy;

                if (ansTmpS.size() == 2) {     //答案兩位數
                    if (muledPos + mulPosCopy >= 2) //整數部分為0
                        integerAmount = 0;
                    else   //整數部分不為0
                        integerAmount = 2 - muledPos - mulPosCopy;
                }
                else {  //答案一位數
                    if (muledPos + mulPosCopy >= 1) //整數部分為0
                        integerAmount = 0;
                    else   //整數部分不為0
                        integerAmount = 1 - muledPos - mulPosCopy;
                }

                if (integerAmount == 0) {    //整數部分為0

                    if (decimalAmount == 1) {   //只有一位小數
                        if (ansTmpS.size() == 2) {  //答案有兩位數   則將小數點插在兩數中間
                            ansTmpS.insert(ansTmpS.begin() + 1, '.');
                        }
                        else {  //答案只有一位數   則直接在答案前補 "0."
                            ansTmpS.insert(ansTmpS.begin(), '.');
                            ansTmpS.insert(ansTmpS.begin(), '0');
                        }
                    }
                    else {  //小數有兩位數(含)以上

                        if (ansTmpS.size() == 1) {
                            for (int i = 0; i < muledPos + mulPosCopy - 1; i++) {   //=1不補0，=2補一個0
                                ansTmpS.insert(ansTmpS.begin(), '0');   //補0補在ans前
                            }
                            while (ansTmpS.size() < decimalAmount)
                                ansTmpS += "0"; //補0補在ans後

                            ansTmpS.insert(ansTmpS.begin(), '.');
                            ansTmpS.insert(ansTmpS.begin(), '0');
                        }
                        else if (ansTmpS.size() == 2) {
                            for (int i = 0; i < muledPos + mulPosCopy - 2; i++) {   //=2不補0，=3補一個0
                                ansTmpS.insert(ansTmpS.begin(), '0');   //補0補在ans前
                            }
                            while (ansTmpS.size() < decimalAmount)
                                ansTmpS += "0"; //補0補在ans後

                            ansTmpS.insert(ansTmpS.begin(), '.');
                            ansTmpS.insert(ansTmpS.begin(), '0');
                        }

                    }

                }
                else {  //整數部分不為0   (X.Y or X.0)

                    if (ansTmpS.size() == 2) {  //答案為兩位數
                        if (integerAmount == 1) {
                            ansTmpS.insert(ansTmpS.begin() + 1, '.');
                        }
                        else {
                            for (int i = 0; i < integerAmount - 2; i++) {
                                ansTmpS += "0";
                            }
                        }
                    }
                    else {  //答案為一位數
                        for (int i = 0; i < integerAmount - 1; i++) {
                            ansTmpS += "0";
                        }

                    }

                }

            }
            else {  //只有整數

                //cout << muledPos << " " << mulPosCopy << endl;    //debugMine
                if (ansTmpS.size() == 1) {  //答案只有一位
                    integerAmount = 1 - muledPos - mulPosCopy;
                }
                else {  //答案有兩位
                    integerAmount = 2 - muledPos - mulPosCopy;
                }


                if (ansTmpS.size() == 1) {  //答案只有一位
                    for (int i = 0; i < integerAmount - 1; i++) {
                        ansTmpS += "0";
                    }
                }
                else {  //答案有兩位
                    for (int i = 0; i < integerAmount - 2; i++) {
                        ansTmpS += "0";
                    }
                }

            }

            Decimal ansTmpD(ansTmpS);
            ans = ans + ansTmpD;

            //cout << ansTmpD.value << endl;  //debugMine
            //return *this;


            mulPosCopy--;

        }
        muledPos--;



    }

    if (positiveTmp == otherPositiveTmp)
        ans.positive = true;
    else
        ans.positive = false;

    //numerator、denominator功能
    ans.numerator = ans.value;
    ans.denominator = "1";

    int dotPos; //小數點位置
    for (int i = 0; i < ans.numerator.size(); i++) {
        if (ans.numerator[i] == '.') {
            dotPos = i;
            break;
        }
    }

    bool numeratorInt = false;  //檢測分子是否為整數
    while (!numeratorInt) {

        numeratorInt = true;

        for (int i = dotPos; i < ans.numerator.size(); i++) {
            if (ans.numerator[i] != '0' && ans.numerator[i] != '.') {
                Decimal tmp(ans.numerator);
                Decimal ten("0.1");

                tmp = tmp / ten;
                ans.numerator = tmp.value;

                ans.denominator += "0";

                numeratorInt = false;
                dotPos++;
                // cout << "i:" << i << " " << ans.numerator[i] << endl;
                // cout << "ans: " << ans.numerator << endl;
                break;
            }

        }
    }

   // cout << "nu:" << ans.numerator << " " << "de:" << ans.denominator << endl;

    return ans;

}

*/
Decimal Decimal::operator/(Decimal other) {
    Decimal self = *this;

    bool allZero = true;
    for (char c : other.value) {
        if (c == '.')
            continue;
        if (c != '0') {
            allZero = false;
            break;
        }
    }
    if (allZero) {
        cout << "denominator can't be zero" << endl;
        return self;
    }
    allZero = true;
    for (char c : this->value) {
        if (c == '.')
            continue;
        if (c != '0') {
            allZero = false;
            break;
        }
    }
    if (allZero) {
        Decimal temp("0");
        return temp;
    }


    //有效位數計算開始
    long long int thisDecDig = 0; //this decimal 有效位數
    long long int otherDecDig = 0; //other decimal 有效位數
    long long int thisIntDig = 0; //this decimal 有效位數
    long long int otherIntDig = 0; //other decimal 有效位數

    bool startCnt = false;
    bool startInt = false;
    for (int i = this->value.length() - 1; i >= 0; --i) {
        if (startInt) {
            thisIntDig++;
            continue;
        }
        if (this->value[i] == '.') {
            startInt = true;
            continue;
        }
        if (this->value[i] != '0') {
            startCnt = true;
        }
        if (startCnt) {
            thisDecDig++;
        }
    }
    startCnt = false;
    startInt = false;
    for (int i = other.value.length() - 1; i >= 0; --i) {
        if (startInt) {
            otherIntDig++;
            continue;
        }
        if (other.value[i] == '.') {
            startInt = true;
            continue;
        }
        if (other.value[i] != '0') {
            startCnt = true;
        }
        if (startCnt) {
            otherDecDig++;
        }
    }
#ifdef DEBUG


    cout << "this dec dig " << thisDecDig << endl;
    cout << "other dec dig " << otherDecDig << endl;
    cout << "this int dig " << thisIntDig << endl;
    cout << "other int dig " << otherIntDig << endl;


    //有效位數計算結束
#endif // DEBUG

    //remove decimal point
    for (int i = 0; i < self.value.size(); ++i) {
        if (self.value[i] == '.') {
            self.value.erase(self.value.begin() + i);
            break;
        }
    }
    for (int i = 0; i < other.value.size(); ++i) {
        if (other.value[i] == '.') {
            other.value.erase(other.value.begin() + i);
            break;
        }
    }
    //
    string thisStr = "";
    string otherStr = "";
    for (int i = 0; i < thisDecDig + thisIntDig; ++i) {
        thisStr.insert(thisStr.end(), self.value[i]);
    }
    for (int i = 0; i < otherDecDig + otherIntDig; ++i) {
        otherStr.insert(otherStr.end(), other.value[i]);
    }
   /* bool start = false;
    for (int i = self.value.size() - 1; i >= 0; --i) {
        char c = self.value[i];
        if (c != '0')
            start = true;
        if (start) 
            thisStr.insert(thisStr.begin(), c);
        
    }
    start = false;
    for (int i = other.value.size() - 1; i >= 0; --i) {
        char c = other.value[i];
        if(c != '0')
            start = true;
        if (start) 
            otherStr.insert(otherStr.begin(), c);
        
    }*/
#ifdef DEBUG



    cout << thisStr << endl;
    cout << otherStr << endl;
#endif // DEBUG
    //fill zero
    for (long long int i = (thisDecDig - otherDecDig); i > 0; --i) {
        otherStr.push_back('0');
    }
    for (long long int i = (otherDecDig - thisDecDig); i > 0; --i) {
        thisStr.push_back('0');
    }
#ifdef DEBUG
    cout << thisStr << endl;
    cout << otherStr << endl;
#endif // DEBUG
    Integer thisInt(thisStr);
    Integer otherInt(otherStr);
    thisInt = thisInt / otherInt;
    int ansDig = thisInt.value.length();
    if (thisInt.value == "0")
        ansDig = 0;
    for (int i = 0; i < 200; ++i) {
        thisStr.push_back('0');
    }
    thisInt.value = thisStr;

//    cout << thisInt.value << " " << otherInt.value << endl;
    thisInt = thisInt / otherInt;
  //  cout << endl << thisInt.value << endl;

    string answerStrNew = thisInt.value;
    for(int i = 0; i < 200; ++i)
        answerStrNew.insert(answerStrNew.begin(), '0');
    answerStrNew.insert(answerStrNew.end() - 200, '.');
    while (answerStrNew.length() > 1 && answerStrNew[0] == '0' && answerStrNew[1] != '.') {
        answerStrNew.erase(answerStrNew.begin());
    }

#ifdef DEBUG
    cout << "****" << endl;
    cout << answerStrNew << endl;
#endif // DEBUG

   

  /*  string answerStr = thisInt.value;
    
    if (ansDig == 0) {
        answerStr.insert(answerStr.begin(), '.');
        answerStr.insert(answerStr.begin(), '0');
    }
    else {
        answerStr.insert(answerStr.begin() + ansDig, '.');
    }
    cout << answerStr << endl;*/
    self.value = answerStrNew;
    if (this->positive == true && other.positive == true || (this->positive == false && other.positive == false)) {
        self.positive = true;
    }
    else {
        self.positive = false;
    }
#ifdef DEBUG
    cout << "numer: " << numerator << " / " << denominator << endl;
#endif // DEBUG

    

    return self;
}
void Decimal::operator=(const Decimal other) {
    this->value = other.value;
    this->inf = other.inf;
    this->positive = other.positive;
    return ;
}
ostream& operator<<(std::ostream& os, Decimal obj) {
    if (obj.inf) {
        os << (obj.positive ? "" : "-") << "Inf";
        return os;
    }
    os << (obj.positive ? "" : "-") << obj.value;
    return os;
}
istream& operator>>(std::istream& is, Decimal& obj) {
    string source;
    cin >> source;
    Decimal temp = source;
    obj = temp;
    return is;
}
void Decimal::operator=(const string statement) {
   

}


/*
sub old


 //12.34 - 45.67
    //共有四種可能
    //1.負數 - 負數
    //2.負數 - 正數
    //3.正數 - 正數
    //4.正數 - 負數

    //正數 - 負數 = 正數 + 正數
    //我們優先讓other的positive(號)改為正號 然後call +的operator
    if (self.positive == true && other.positive == false) {
        other.positive = true;
        return (*this + other);
    }
    //負數-正數 = -(正數+正數)
    //範例：-1234-4251 = -(1234+4251)
    if (self.positive == false && other.positive == true) {
        this->positive = true;
        Decimal tempDecimal = *this + other;
        tempDecimal.positive = false;
        return tempDecimal;
    }
    //排除上述情況，目前還有
    //1.負數 - 負數
    //3.正數 - 正數

    //case1.負數-負數 = 負數 + 正數 = 正數 - 正數
    //範例：-1234-(-4251) = -1234+4251 = 4251-1234 => 等於Case3.
    //我們swap兩組obj的value, 然後重新給定正負號
    if (self.positive == false && other.positive == false) {
        string temp = self.value;
        self.value = other.value;
        other.value = temp;
        self.positive = true;
        other.positive = true;
    }

    //剩下正數 - 正數

    //先做浮點 - 浮點
    string TIP = "";
    string OIP = "";
    string TDP = "";
    string ODP = "";
    vector<int> thisIntegerPart;
    vector<int> thisDecimalPart;
    vector<int> otherIntegerPart;
    vector<int> otherDecimalPart;
    bool MDP = false; //meet decimal point
    for (int i = 0; i < value.size(); ++i) {
        if (value[i] == '.') {
            MDP = true;
            continue;
        }
        if (value[i] >= '0' && value[i] <= '9') {
            if (MDP) {
                TDP.push_back(value[i]);
                thisDecimalPart.push_back(value[i] - '0');
            }
            else {
                TIP.push_back(value[i]);
                thisIntegerPart.push_back(value[i] - '0');
            }
        }
        else {
            cout << "error: invalid input at'" << value[i] << "'" << endl;
            return self;
        }
    }
    if (!MDP) {
        cout << "warning: left value missing decimal parts" << endl;
        for (int i = 0; i < 100; i++) {
            thisDecimalPart.push_back(0);
        }
    }
    MDP = false;
    for (int i = 0; i < other.value.size(); ++i) {
        char c = other.value[i];
        if (c == '.') {
            MDP = true;
            continue;
        }
        if (c >= '0' && c <= '9') {
            if (MDP) {
                ODP.push_back(c);
                otherDecimalPart.push_back(c - '0');
            }
            else {
                OIP.push_back(c);
                otherIntegerPart.push_back(c - '0');
            }
        }
        else {
            cout << "error: invalid input at'" << c << "'" << endl;
            return self;
        }
    }
    if (!MDP) {
        cout << "warning: right value missing decimal parts" << endl;
        for (int i = 0; i < 100; i++) {
            thisDecimalPart.push_back(0);
        }
    }
    while (thisDecimalPart.size() < 100) {
        thisDecimalPart.push_back(0);
    }
    while (otherDecimalPart.size() < 100) {
        otherDecimalPart.push_back(0);
    }

    //從最低位元 (vector的尾巴開始減)
    int borrow = 0; //借位預設為0
    bool decimalBorrow = true;

    for (int i = 0; i < thisDecimalPart.size(); ++i) {
        if (thisDecimalPart[i] != otherDecimalPart[i])
            decimalBorrow = false;
    }

    for (int i = thisDecimalPart.size() - 1; i >= 0; --i) {
        thisDecimalPart[i] -= (otherDecimalPart[i] + borrow);
        borrow = 0;
        if (thisDecimalPart[i] < 0) {
            //進位
            thisDecimalPart[i] += 10;
            ++borrow;
        }
    }

    //當已經到開頭，borrow卻非0
    //舉例：123-999
    // 3 - 9 = -6+10 = 4 (borrow = 1)
    // 2 - 9 - 1(前級borrow) = -8+10 = 2 (borrow = 1)
    // 1 - 9 - 1 = -9+10 = 1 (borrow = 1)
    // 現在是 -124
    if (borrow != 0) {
        //變成反過來減 然後給結果負號
        decimalBorrow = true;
        thisDecimalPart.clear();
        otherDecimalPart.clear();

        for (char c : TDP) {
            if (c >= '0' && c <= '9') {
                thisDecimalPart.push_back(c - '0');
            }
        }
        for (char c : ODP) {
            if (c >= '0' && c <= '9') {
                otherDecimalPart.push_back(c - '0');
            }
        }

        //幫otherDigits補0 使得位數相同
        while (thisDecimalPart.size() > otherDecimalPart.size())
            otherDecimalPart.insert(otherDecimalPart.end(), 0);

        //幫thisDigits補0 使得位數相同
        while (thisDecimalPart.size() < otherDecimalPart.size())
            thisDecimalPart.insert(thisDecimalPart.end(), 0);


        thisDecimalPart.swap(otherDecimalPart);

        //從最低位元 (vector的尾巴開始減)
        borrow = 0; //借位預設為0

        for (int i = thisDecimalPart.size() - 1; i >= 0; --i) {
            thisDecimalPart[i] -= (otherDecimalPart[i] + borrow);
            borrow = 0;
            if (thisDecimalPart[i] < 0) {
                //進位
                thisDecimalPart[i] += 10;
                ++borrow;
            }
        }
        self.positive = false; //給結果負號
    }
    else {
        //如果borrow = 0
        //舉例：999-123
        // 9 - 3 = 6
        // 9 - 2 = 7
        // 9 - 1 = 8
        // 答案則為678 結果為正號

       //cout << "error";
        self.positive = true; //給結果正號
    }

    if (self.positive == false) {
        //代表小數部分有界位

        borrow = 1;
    }
    string decimalAnswer = "";
    for (int i : thisDecimalPart) {
        decimalAnswer.push_back(i + '0');
    }

    //做整數-

    for (int i = thisIntegerPart.size() - 1; i >= 0; --i) {
        thisIntegerPart[i] -= (otherIntegerPart[i] + borrow);
        borrow = 0;
        if (thisIntegerPart[i] < 0) {
            //進位
            thisIntegerPart[i] += 10;
            ++borrow;
        }
    }
    //當已經到開頭，borrow卻非0
    if (borrow != 0) {
        //變成反過來減 然後給結果負號

        thisIntegerPart.clear();
        otherIntegerPart.clear();

        for (char c : TIP) {
            if (c >= '0' && c <= '9') {
                thisIntegerPart.push_back(c - '0');
            }

        }
        for (char c : OIP) {
            if (c >= '0' && c <= '9') {
                otherIntegerPart.push_back(c - '0');
            }

        }

        //幫otherDigits補0 使得位數相同
        while (thisIntegerPart.size() > otherIntegerPart.size())
            otherIntegerPart.insert(otherIntegerPart.begin(), 0);

        //幫thisDigits補0 使得位數相同
        while (thisIntegerPart.size() < otherIntegerPart.size())
            thisIntegerPart.insert(thisIntegerPart.begin(), 0);


        thisIntegerPart.swap(otherIntegerPart);

        //從最低位元 (vector的尾巴開始減)
        borrow = 0; //借位預設為0
        if (!decimalBorrow)
            borrow = 1;

        for (int i = thisIntegerPart.size() - 1; i >= 0; --i) {
            thisIntegerPart[i] -= (otherIntegerPart[i] + borrow);
            borrow = 0;
            if (thisIntegerPart[i] < 0) {
                //進位
                thisIntegerPart[i] += 10;
                ++borrow;
            }
        }
        self.positive = false; //給結果負號
    }
    else {
        self.positive = true; //給結果正號
    }

    string integerAnswer = "";
    for (int i : thisIntegerPart) {
        integerAnswer.push_back(i + '0');
    }
    while (decimalAnswer.length() < 100)
        decimalAnswer.insert(decimalAnswer.end(), '0');
    self.value = integerAnswer + "." + decimalAnswer;
    //trim zero
    while (self.value[0] == '0' && self.value.size() > 1) {
        self.value.erase(self.value.begin());
    }







*/
