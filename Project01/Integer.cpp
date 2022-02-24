#include "Integer.h"









void Integer::operator=(const Integer other) {
	this->value = other.value;
	this->inf = other.inf;
	this->positive = other.positive;

}
Integer Integer::operator+(Integer other) {
    Integer self = *this;
    //-5 + 3
    // 3 - 5
    if (positive == false && other.positive == true) {
        this->positive = true;
        return other - *this;
    }
    //5 + -2
    else if (positive == true && other.positive == false) {
        other.positive = true;
        return *this - other;
    }
    else if (positive == false && other.positive == false) {
        self.positive = false;
    }

    //會有this.value跟other.value做相加
	//假設(this)123456+(other)666666
	//策略 先把string每個位元拆開來存進vector



  
	vector<int> thisDigits;
	vector<int> otherDigits;
	//特殊情況
	
	//
	for (char c : self.value) {
		if (c >= '0' && c <= '9') {
			thisDigits.push_back(c - '0');
		}
		else {
            cout << "error: invalid input at'" << c << "'" << endl;
            return self;
        }
	}
	for (char c : other.value) {
		if (c >= '0' && c <= '9') {
			otherDigits.push_back(c - '0');
		}
		else {
            cout << "error: invalid input at'" << c << "'" << endl;
            return self;
        }
	}


	//幫otherDigits補0 使得位數相同
	while (thisDigits.size() > otherDigits.size())
		otherDigits.insert(otherDigits.begin(), 0);

	//幫thisDigits補0 使得位數相同
	while (thisDigits.size() < otherDigits.size())
		thisDigits.insert(thisDigits.begin(), 0);

	//從最低位元 (vector的尾巴開始加)
	int carry = 0; //進位預設為0

	for (int i = thisDigits.size() - 1; i >= 0; --i) {
		thisDigits[i] += otherDigits[i] + carry;
		carry = 0;
		if (thisDigits[i] >= 10) {
			//進位
			thisDigits[i] -= 10;
			++carry;
		}
	}
	//當已經加到開頭，carry卻非0 = 幫thisDigits頭加1
	//舉例：50000+50000 = 00000 (此時carry = 1)
	//幫00000開頭插入1 變成 100000
	if (carry != 0) {
		thisDigits.insert(thisDigits.begin(), 1);
	}
	
	//轉回string
	string answer = "";
	
	for (int i : thisDigits) {
		answer.push_back(i + '0');
	}
	self.value = answer;
	return self;

}
Integer Integer::operator-(Integer other) {

    Integer self = *this;
    //共有四種可能
    //1.負數 - 負數
    //2.負數 - 正數
    //3.正數 - 正數
    //4.正數 - 負數

    //正數 - 負數 = 正數 + 正數
    //我們優先讓other的positive(號)改為正號 然後call +的operator
    if (self.positive == true && other.positive == false){
        other.positive = true;
        return (*this + other);
    }
	//負數-正數 = -(正數+正數)
	//範例：-1234-4251 = -(1234+4251)
	if (self.positive == false && other.positive == true) {
        this->positive = true;
		Integer tempInteger = *this + other;
		tempInteger.positive = false;
		return tempInteger;
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
	

	//3.正數 - 正數

	//123-023
	//123-321
	//023-123
	
	//位數相同
	//策略與加法相同
	vector<int> thisDigits;
	vector<int> otherDigits;

	for (char c : value) {
		if (c >= '0' && c <= '9') {
			thisDigits.push_back(c - '0');
		}
		else { self.errorCode = INVALIDINPUT; return *this; }
	}
	for (char c : other.value) {
		if (c >= '0' && c <= '9') {
			otherDigits.push_back(c - '0');
		}
		else { self.errorCode = INVALIDINPUT; return *this; }
	}
	//幫otherDigits補0 使得位數相同
	while (thisDigits.size() > otherDigits.size())
		otherDigits.insert(otherDigits.begin(), 0);

	//幫thisDigits補0 使得位數相同
	while (thisDigits.size() < otherDigits.size())
		thisDigits.insert(thisDigits.begin(), 0);

	//從最低位元 (vector的尾巴開始減)
	int borrow = 0; //借位預設為0

	for (int i = thisDigits.size() - 1; i >= 0; --i) {
		thisDigits[i] -= (otherDigits[i] + borrow);
		borrow = 0;
		if (thisDigits[i] < 0) {
			//進位
			thisDigits[i] += 10;
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
		//變成999-123然後結果給負號
		//999-123 = 876 給負號 = -876
		string temp = this->value;
		self.value = other.value;
		other.value = temp;

		thisDigits.clear();
		otherDigits.clear();

		for (char c : self.value) {
			if (c >= '0' && c <= '9') {
				thisDigits.push_back(c - '0');
			}
			else { self.errorCode = INVALIDINPUT; return *this; }
		}
		for (char c : other.value) {
			if (c >= '0' && c <= '9') {
				otherDigits.push_back(c - '0');
			}
			else { self.errorCode = INVALIDINPUT; return *this; }
		}
		//幫otherDigits補0 使得位數相同
		while (thisDigits.size() > otherDigits.size())
			otherDigits.insert(otherDigits.begin(), 0);

		//幫thisDigits補0 使得位數相同
		while (thisDigits.size() < otherDigits.size())
			thisDigits.insert(thisDigits.begin(), 0);

		//從最低位元 (vector的尾巴開始減)
		borrow = 0; //借位預設為0

		for (int i = thisDigits.size() - 1; i >= 0; --i) {
			thisDigits[i] -= (otherDigits[i] + borrow);
			borrow = 0;
			if (thisDigits[i] < 0) {
				//進位
				thisDigits[i] += 10;
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
		self.positive = true; //給結果正號
	}

	//轉回string
	string answer = "";

	for (int i : thisDigits) {
		answer.push_back(i + '0');
	}
	
	
	self.value = answer;

    //trim zero
    while (self.value[0] == '0' && self.value.size() > 1) {
        self.value.erase(self.value.begin());
    }
	return self;
}
Integer Integer::operator*(Integer other) {
    Integer self = *this;
    if (this->value == "0" || other.value == "0") {
        self.value = "0";
        self.positive = true;
        self.inf = false;
        return self;
    }
    //特殊情況

	//策略 先把string每個位元拆開來存進vector
	vector<int> thisDigits;
	vector<int> otherDigits;
	for (char c : this->value) {
		if (c >= '0' && c <= '9') {
			thisDigits.push_back(c - '0');
		}
		else { self.errorCode = INVALIDINPUT; return self; }
	}
	for (char c : other.value) {
		if (c >= '0' && c <= '9') {
			otherDigits.push_back(c - '0');
		}
		else { self.errorCode = INVALIDINPUT; return self; }
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
		if (resultPrevious == ""){
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

	

	self.value = resultPrevious;
	//判斷正負號
	//負 * 負 = 正
	//負 * 正 = 負..依此類推
	if (self.positive == false && other.positive == false)
		self.positive = true;
	else if (self.positive == true && other.positive == true)
		self.positive = true;
	else
		self.positive = false;


    //trim zero
    while (self.value[0] == '0' && self.value.size() > 1) {
        self.value.erase(self.value.begin());
    }

	return self;
}
Integer Integer::operator/(Integer other) {

    Integer ans;

    //分母為0
    if (other.value == "0") {
        cout << "Denominator can't be 0." << endl;
        return ans;		//分母為0的情況下會return ""
    }
    Integer temp = *this;
    temp.positive = true;
    Integer temp2 = other;
    temp2.positive = true;
    if (temp < temp2) {
        ans.value = "0";
        return ans;
    }
    if (this->value.length() == other.value.length()) {
        bool isSameValue = true;
        for (int i = 0; i < this->value.length(); ++i) {
            if (this->value[i] != other.value[i]) {
                isSameValue = false;
                break;
            }
        }
        if (isSameValue) {
            if (this->positive == true && other.positive == true || (this->positive == false && other.positive == false)) {
                ans.value = "1";
                ans.positive = true;
                ans.inf = false;
                return ans;
            }
            else {
                ans.value = "1";
                ans.positive = false;
                ans.inf = false;
                return ans;
            }
        }
    }

    //
    /*
        1260257 / 37
    */
    //

    //do division
    unsigned long long int dlen = other.value.length();//分母長度 = 2
    unsigned long long int mlen = this->value.length();//分子長度 = 7
    Integer base(other.value);
    string beDivide = "";
    string answerStr = "";
    for (int i = 0; i < this->value.length(); ++i) {
       
         beDivide.insert(beDivide.end(), this->value[i]);

#ifdef DEBUG
         cout << "beDivide" << beDivide << endl;
#endif // DEBUG

        Integer bdo(beDivide);
        
        if (bdo > base || bdo == base) {
            //可減
            unsigned long long int ans = 0;
            while (bdo > base || bdo == base) {
                ++ans;
                
                bdo = bdo - base;
            }
            if (ans != 0) {
                if (ans > 10)
                    cout << "error";
                answerStr += to_string(ans);
                
                    int j = i + 1;
                    beDivide = bdo.value;
                    i = bdo.value.length()-1;
                    string newTarget = bdo.value;

                    if (beDivide != "0") {
                        int nt = other.value.length() - newTarget.length() - 1;
                        for (int f = nt; f > 0; --f) {
                            answerStr += "0";
                        }
                    }

                    while (j < this->value.length()) {
                        newTarget.insert(newTarget.end(), this->value[j++]);
                    }
                    bool isAllZero = true;
                    for (char c : newTarget) {
                        if (c != '0') {
                            isAllZero = false;
                            break;

                        }
                    }
                    if (isAllZero) {
                        beDivide = newTarget;
                        break;
                    }

              
                    this->value = newTarget;
                 
#ifdef DEBUG
                    cout << "new: " << newTarget << endl;
#endif // DEBUG
                    //remaining all zero?
             
                    //

                    //beDivide = "";
                    //i = -1;
                
            }
            //cout << ans << " << ";

        }
        else {
            if (beDivide.length() >= other.value.length()) {
                answerStr.insert(answerStr.end(), '0');
            }

        }
    }
  
    ans.value = answerStr;
    if (beDivide.length() > 1 ) {
        bool isAllZero = true;
        for (char c : beDivide) {
            if (c != '0') {
                isAllZero = false;
                break;
            }
        }
        if (isAllZero) {
            beDivide.erase(beDivide.begin());
            ans.value += beDivide;
            
        }
    }
    while (ans.value.length() > 1 && ans.value[0] == '0') {
        ans.value.erase(ans.value.begin());
    }
    //給號
    if (this->positive && other.positive || (!this->positive && !other.positive))
        ans.positive = true;
    else
        ans.positive = false;

    return ans;
}

bool Integer::operator==(Integer other) {
    if (this->positive == true && other.positive == false)
        return false;
    else if (this->positive == false && other.positive == true)
        return false;
    else if (this->value.length() != other.value.length()) {
        return false;
    }else{
        for(int i = 0; i < this->value.length(); ++i){
            if (this->value[i] != other.value[i])
                return false;
        }      
    }
    return true;
}
bool Integer::operator>(Integer other) {
	//未做無窮比大小
    
	if (this->positive == true && other.positive == false)
		return true;
	else if (this->positive == false && other.positive == true)
		return false;
	else if (this->positive == false && other.positive == false){
		Integer tempA(this->value);
		Integer tempB(other.value);
		return (tempA < tempB);
	}else {
		if (this->value.size() > other.value.size())
			return true;
		else if (this->value.size() < other.value.size())
			return false;
		else {
            if (this->value == other.value)
                return false;
			for (int i = 0; i < this->value.size(); ++i) {
                if ((this->value[i] - '0') < (other.value[i] - '0'))
                    return false;
                else if ((this->value[i] - '0') > (other.value[i] - '0'))
                    return true;
			}
            return true;
		}
		
	}

}
bool Integer::operator<(Integer other) {
	//未做無窮比大小

	if (this->positive == true && other.positive == false)
		return true;
	else if (this->positive == false && other.positive == true)
		return false;
	else if (this->positive == false && other.positive == false){
		Integer tempA(this->value);
		Integer tempB(other.value);
		return (tempA < tempB);
	}else {
		if (this->value.size() < other.value.size())
			return true;
		else if (this->value.size() > other.value.size())
			return false;
		else {
            if (this->value == other.value)
                return false;
			for (int i = 0; i < this->value.size(); ++i) {
				if (this->value[i] > other.value[i])
					return false;
                
			}
		}
		return true;
	}

}
ostream& operator<<(std::ostream& os, Integer obj) {
	//trim掉前方多餘的0
	
	while (obj.value[0] == '0' && obj.value.size() > 1) {
			//trim
			obj.value.erase(obj.value.begin());
					
	}
	

	if (obj.inf) {
		os << (obj.positive ? "" : "-") << "Inf";
		return os;
	}
	os << (obj.positive ? "" : "-") << obj.value;
	return os;
}
istream& operator>>(std::istream& is, Integer& obj) {
	string source;
	cin >> source;
	Integer temp = source;
	obj = temp;
	return is;
}
void Integer::operator=(const string statement) {


}
