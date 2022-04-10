#include <iostream>
#include <vector>
#include <string> 
#include <sstream>
#include <algorithm>

class BigInteger {
private:

    static const int logBase = 4;
    static const int base = 10000; //размерость системы счисления
    bool sign;
    std::vector <int> data;
    BigInteger substrNum(size_t start, size_t count) const;

    void swap(BigInteger& other) {
        std::swap(sign, other.sign);
        std::swap(data, other.data);
    }

    void addNoSign(const BigInteger& x);

    void subNoSign(const BigInteger& x);

public:

    BigInteger();
    BigInteger(const BigInteger& x);
    BigInteger(int x);
    
    int binSearchAns(const BigInteger& x, int l, int r);

    void removeZeros() {
        int64_t j = data.size() - 1;
        while (!data[j] && j) {
            data.pop_back();
            --j;
        }
    }

    BigInteger operator-() const;
    BigInteger& operator--();
    BigInteger& operator++();
    BigInteger operator--(int);
    BigInteger operator++(int);

    friend bool operator==(const BigInteger& x1, const BigInteger& x2);
    friend bool operator>(const BigInteger& x1, const BigInteger& x2);

    BigInteger& operator+=(const BigInteger& x);
    BigInteger& operator-=(const BigInteger& x);
    BigInteger& operator*=(const BigInteger& x);
    BigInteger& operator/=(const BigInteger& x);
    BigInteger& operator%=(const BigInteger& x);
    std::string toString() const;
    
    explicit operator bool() const{
        return !data.back() == 0;
    }

    friend std::ostream& operator<< (std::ostream& out, const BigInteger& s);
    friend std::istream& operator>> (std::istream& out, BigInteger& s);

    ~BigInteger() = default;
};

BigInteger operator+(const BigInteger& x1, const BigInteger& x2);
BigInteger operator-(const BigInteger& x1, const BigInteger& x2);
BigInteger operator*(const BigInteger& x1, const BigInteger& x2);
BigInteger operator/(const BigInteger& x1, const BigInteger& x2);
BigInteger operator%(const BigInteger& x1, const BigInteger& x2);

BigInteger gcd(const BigInteger& x1, const BigInteger& x2);

bool operator!=(const BigInteger& x1, const BigInteger& x2);
bool operator<(const BigInteger& x1, const BigInteger& x2);
bool operator>=(const BigInteger& x1, const BigInteger& x2);
bool operator<=(const BigInteger& x1, const BigInteger& x2);

class Rational {
private:

    BigInteger numerator = 0;
    BigInteger denominator = 1;
    void changeSign() {
        if (denominator < 0 && numerator != 0) {
            numerator = -numerator;
            denominator = -denominator;
        } else if (denominator < 0 && numerator == 0) {
            denominator = -denominator;
        }
    }

public:

    Rational();
    Rational(const BigInteger& num, const BigInteger& den = 1);
    Rational(int x);
    void reduce();

    Rational& operator+=(const Rational& x);
    Rational& operator-=(const Rational& x);
    Rational& operator*=(const Rational& x);
    Rational& operator/=(const Rational& x);

    Rational operator-() const;

    friend bool operator==(const Rational& x1, const Rational& x2);
    friend bool operator<(const Rational& x1, const Rational& x2);

    std::string toString() const;
    std::string asDecimal(const size_t precision = 0) const;
    explicit operator double();

    ~Rational() = default;
};

Rational operator+(const Rational& x1, const Rational& x2);
Rational operator-(const Rational& x1, const Rational& x2);
Rational operator*(const Rational& x1, const Rational& x2);
Rational operator/(const Rational& x1, const Rational& x2);
bool operator!=(const Rational& x1, const Rational& x2);
bool operator>(const Rational& x1, const Rational& x2);
bool operator<=(const Rational& x1, const Rational& x2);
bool operator>=(const Rational& x1, const Rational& x2);

BigInteger BigInteger::substrNum(size_t start, size_t count) const{
        BigInteger ans;
        ans.sign = true;
        for (size_t i = 0; i < count; ++i) {
            ans.data.push_back(data[i + start]);
        }
        return ans;
    }

int BigInteger::binSearchAns(const BigInteger& x, int min, int max) {
        int median = (min + max) / 2;
        if (min + 1 == max) {
            return min;
        }
        if (*this == (x * median)) {
            return median;
        }
        return (*this < (x * median) ? binSearchAns(x, min, median) : binSearchAns(x, median, max));
    }

BigInteger::BigInteger() {
        sign = true;
        std::vector <int> data;
    }

BigInteger::BigInteger(const BigInteger& x) {
        sign = x.sign;
        data = x.data;
    }

BigInteger::BigInteger(int y) {
        int x = y;
        sign = true;
        if (y == 0) {
            data.push_back(0);
        } else {
            if (y < 0) {
                sign = false;
                x = -x;
            }
            while (x) {
            data.push_back(x % base);
            x /= base;
            }
        }
    }

BigInteger BigInteger::operator-() const{
        BigInteger ans = *this;
        if (data.size() != 1 || data[0] != 0) ans.sign = !sign;
        return ans;
    }

BigInteger& BigInteger::operator--() {
        *this -= 1;
        return *this;
    }

BigInteger& BigInteger::operator++() {
        *this += 1;
        return *this;
    }

BigInteger BigInteger::operator--(int) {
        BigInteger ans = *this;
        *this -= 1;
        return ans;
    }

BigInteger BigInteger::operator++(int) {
        BigInteger ans = *this;
        *this += 1;
        return ans;
    }

bool operator==(const BigInteger& x1, const BigInteger& x2) {
    return (x1.sign == x2.sign && x1.data == x2.data);
}

bool operator!=(const BigInteger& x1, const BigInteger& x2) {
        return !(x1 == x2);
    }

bool operator>(const BigInteger& x1, const BigInteger& x2) {
        if (x1.sign != x2.sign || x1.data.size() > x2.data.size()) {
            return x1.sign;
        }
        if (x1.data.size() < x2.data.size()) {
            return !x1.sign;
        }
        for (int64_t i = x1.data.size() - 1; i >= 0; --i) {
            if (x1.data[i] > x2.data[i]) {
                return x1.sign;
            }
            if (x1.data[i] < x2.data[i]) {
                return !x1.sign;
            }
        }
        return false;
    }

bool operator<(const BigInteger& x1, const BigInteger& x2) {
    return (x1 == x2 ? false : !(x1 > x2));
}

bool operator>=(const BigInteger& x1, const BigInteger& x2) {
    return (x1 > x2 || x1 == x2);
}

bool operator<=(const BigInteger& x1, const BigInteger& x2) {
    return (x1 < x2 || x1 == x2);
}

void BigInteger::addNoSign(const BigInteger& x) {
    int64_t k = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        if (i >= x.data.size()) {
            if (k == 0) {
                break;
            }
            data[i] += k;
        } else {
            data[i] += x.data[i] + k;
        }
        k = data[i] / base;
        data[i] %= base;
    }
    if (k) {
        data.push_back(k);
    }
}

void BigInteger::subNoSign(const BigInteger& x) {
    int64_t k = 0;
    sign = x.sign;
    if (*this == x) {
        data.clear();
        data.push_back(0);
        sign = true;
        return;
    }
    sign = *this < x;
    if (sign == x.sign) {
        for (size_t i = 0; i < data.size(); ++i) {
            int s = k;
            k = (data[i] <= x.data[i] + k ? 0 : -1);
            data[i] = (data[i] <= x.data[i] + s ? x.data[i] + s - data[i] : x.data[i] + s - data[i] + base);
        }
        return;
    } 
    for (size_t i = 0; i < data.size(); ++i) {
        int s = k;
        if (i >= x.data.size()) {
            k = (0 <= data[i] + k ? 0 : -1);
            data[i] = (0 <= data[i] + s ? data[i] + s : data[i] + s + base);
            if (k == 0) {
                return;
            }
        } else {
            k = (x.data[i] <= data[i] + k ? 0 : -1);
            data[i] = (x.data[i] <= data[i] + s ? data[i] + s - x.data[i] : data[i] + s - x.data[i] + base);
        }
    }
}

BigInteger& BigInteger::operator+=(const BigInteger& x) {
        data.resize(std::max(data.size(), x.data.size()), 0);
        if (sign == x.sign) {
            addNoSign(x);
        } else {
            subNoSign(x);
        }
        removeZeros();
        return *this;
    }

BigInteger& BigInteger::operator-=(const BigInteger& x) {
        sign = !sign;
        *this += x;
        sign = !sign;
        if (data.size() == 1 && data[0] == 0) {
            sign = true;
        }
        return *this;
    }

BigInteger& BigInteger::operator*=(const BigInteger& x) {
        if (*this == 0 || x == 0) {
            *this = 0;
            return *this;
        }
        std::vector <int> ans(data.size() + x.data.size() + 2, 0);
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < x.data.size(); ++j) {
                ans[i + j] += (data[i] * x.data[j]);
                if (ans[i + j] >= base) {
                    ans[i + j + 1] += (ans[i + j] / base);
                    ans[i + j] %= base;
                }
            }
        }
        std::swap(data, ans);
        removeZeros();
        sign = (sign == x.sign);
        ans.clear();
        return *this;
    }

BigInteger& BigInteger::operator/=(const BigInteger& x) { 
        if ((*this < x && sign && x.sign) || (*this > x && !sign && !x.sign) || (*this < -x && sign && !x.sign) || (*this > -x && !sign && x.sign)) {
            *this = 0;
            return *this;
        }
        if (*this == x) {
            *this = 1;
            return *this;
        }
        if (*this == -x) {
            *this = -1;
            return *this;
        }
        BigInteger copy = *this;
        data.clear();
        BigInteger y = x;
        y.sign = true;
        BigInteger div = copy.substrNum(copy.data.size() - y.data.size(), y.data.size());//чиcло, которое на каждом шаге будет делится на x
        for (int64_t i = copy.data.size() - y.data.size(); i >= 0; --i) {
            if(div < y) {
                data.push_back(0);
            } else if (div == y) {
                data.push_back(1);
            } else {
                data.push_back(div.binSearchAns(y, 1, base));
            }
            if(i) {
                div -= (data[data.size() - 1] * y);
                div *= base;
                div += copy.data[i - 1];
                int j = div.data.size() - 1;
                while (!div.data[j] && j) {
                    div.data.pop_back();
                    --j;
                }
            }
        }
        reverse(data.begin(), data.end());
        removeZeros();
        if (x.sign == false) {
            sign = !copy.sign;
        } else {
            sign = copy.sign;
        }
        return *this;
    }

BigInteger& BigInteger::operator%=(const BigInteger& x) {
    BigInteger y = (*this / x) * x;
    *this -= y;
    return *this;
}

std::string BigInteger::toString() const{
        std::string str;
        str.reserve(data.size() * logBase + 1);
        for(size_t i = 0; i < data.size(); ++i) {
            int bit = data[i]; 
            for (int j = 0; j < logBase; ++j) {
                str += (bit % 10) + '0';
                bit /= 10;
            }
        }
        while (str.back() == '0' && str.size() > 1) {
            str.pop_back();
        }
        if (!sign) {
            str += '-';
        }
        reverse(str.begin(), str.end());
        return str;
    }

BigInteger operator+(const BigInteger& x1, const BigInteger& x2) {
    BigInteger ans = x1;
    ans += x2;
    return ans;
}

BigInteger operator-(const BigInteger& x1, const BigInteger& x2) {
    BigInteger ans = x1;
    ans -= x2;
    return ans;
}

BigInteger operator*(const BigInteger& x1, const BigInteger& x2) {
    BigInteger ans = x1;
    ans *= x2;
    return ans;
}

BigInteger operator/(const BigInteger& x1, const BigInteger& x2) {
    BigInteger ans = x1;
    ans /= x2;
    return ans;
}

BigInteger operator%(const BigInteger& x1, const BigInteger& x2) {
    BigInteger ans = x1;
    ans %= x2;
    return ans;
}

BigInteger gcd(const BigInteger& x1, const BigInteger& x2) {
    if (x1 == 0) {
        if (x2 > 0) {
            return x2;
        }
        return (-x2);
    }
    return gcd(x2 % x1, x1);
}

std::ostream& operator<< (std::ostream& out, const BigInteger& x) {
    out << x.toString();
    return out;
}

std::istream& operator>> (std::istream& in, BigInteger& x) {
    std::string str;
    in >> str;
    x.data.clear();
    int64_t last;
    x.sign = (str[0] == '-' ? false : true);
    last = (str[0] == '-' ? 1 : 0);
    x.data.resize(str.size() / 4 + 1, 0);
    int64_t j = 0;
    for (int i = str.size() - 1; i >= last; i -= x.logBase) {
        size_t len = 0;
        len = (i >= x.logBase ? x.logBase : i - last + 1);
        x.data[j] = stoi(str.substr(i - len + 1, len));
        ++j;
    }
    x.removeZeros();
    return in;
}



Rational::Rational(){
    numerator = 0;
    denominator = 1;
}

Rational::Rational(const BigInteger& num, const BigInteger& den) {
    if (den < 0 && num != 0) {
        numerator = -num;
        denominator = -den;
    } else if (den < 0 && num == 0) {
        numerator = num;
        denominator = -den;
    } else {
        numerator = num;
        denominator = den;
    }
    reduce();
}

Rational::Rational(int x) {
    numerator = x;
    denominator = 1;
}

void Rational::reduce() {
    BigInteger div = gcd(numerator, denominator);
    numerator /= div;
    denominator /= div;
}

Rational& Rational::operator+=(const Rational& x) {
    numerator = numerator * x.denominator + denominator * x.numerator;
	denominator *= x.denominator;
	changeSign();
    reduce();
	return *this;
}

Rational& Rational::operator-=(const Rational& x) {
    numerator = numerator * x.denominator - denominator * x.numerator;
	denominator *= x.denominator;
	changeSign();
    reduce();
	return *this;
}

Rational& Rational::operator*=(const Rational& x) {
    numerator *= x.numerator;
	denominator *= x.denominator;
	changeSign();
    reduce();
	return *this;
}

Rational& Rational::operator/=(const Rational& x) {
    numerator *= x.denominator;
	denominator *= x.numerator;
	changeSign();
    reduce();
	return *this;
}

Rational Rational::operator-() const {
    Rational copy = *this;
	copy.numerator = -copy.numerator;
	return copy;
}

bool operator==(const Rational& x1, const Rational& x2) {
    return ((x1.numerator == x2.numerator) && (x1.denominator == x2.denominator));
}
bool operator!=(const Rational& x1, const Rational& x2) {
    return !(x1 == x2);
}

bool operator<(const Rational& x1, const Rational& x2) {
    return ((x1.numerator * x2.denominator) < (x2.numerator * x1.denominator));
}

bool operator>(const Rational& x1, const Rational& x2) {
    return (x1 == x2 ? false : !(x1 < x2));
}

bool operator<=(const Rational& x1, const Rational& x2) {
    return (x1 < x2 || x1 == x2);
}

bool operator>=(const Rational& x1, const Rational& x2) {
    return (x1 > x2 || x1 == x2);
}

std::string Rational::toString() const {
    if (numerator == 0) {
        return "0";
    }
    if (denominator == 1) {
        return numerator.toString();
    }
    return numerator.toString() + "/" + denominator.toString();
}

std::string Rational::asDecimal(const size_t precision) const {
    std::string ans = "";
    BigInteger copy = numerator;
    if (copy < 0) {
        copy = -copy;
        ans += "-";
    } 
    BigInteger integ = copy / denominator;
    ans += integ.toString();
    if (precision > 0) {
        ans += ".";
        size_t j = ans.size();
        ans.reserve(precision + j);
        BigInteger fractional = copy % denominator;
        for (size_t i = 0; i < precision; ++i) {
			ans += ((fractional * 10) / denominator).toString();
            fractional *= 10;
			fractional %= denominator;
		}
    }
    return ans;
}

Rational operator+(const Rational& x1, const Rational& x2) {
    Rational ans = x1;
    ans += x2;
    return ans;
}

Rational operator-(const Rational& x1, const Rational& x2) {
    Rational ans = x1;
    ans -= x2;
    return ans;
}

Rational operator*(const Rational& x1, const Rational& x2) {
    Rational ans = x1;
    ans *= x2;
    return ans;
}

Rational operator/(const Rational& x1, const Rational& x2) {
    Rational ans = x1;
    ans /= x2;
    return ans;
}

Rational::operator double() {
    std::stringstream x;
    x << asDecimal(16);
    double ans;
    x >> ans;
    return ans;
}