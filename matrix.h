#include <iostream>
#include <vector>
#include <initializer_list>
#include <sstream>
#include <algorithm>
#include <typeinfo>

class BigInteger {
private:

    static const int64_t logBase = 9;
    static const int64_t base = 1000000000; //размерость системы счисления
    bool sign;
    std::vector <int64_t> data;
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
    BigInteger(int64_t x);

    int64_t binSearchAns(const BigInteger& x, int64_t l, int64_t r);

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

    void half();

    friend std::ostream& operator<< (std::ostream& out, const BigInteger& s);
    friend std::istream& operator>> (std::istream& out, BigInteger& s);

    friend BigInteger gcd(const BigInteger& x1, const BigInteger& x2);
    ~BigInteger() = default;
};

BigInteger operator+(const BigInteger& x1, const BigInteger& x2);
BigInteger operator-(const BigInteger& x1, const BigInteger& x2);
BigInteger operator*(const BigInteger& x1, const BigInteger& x2);
BigInteger operator/(const BigInteger& x1, const BigInteger& x2);
BigInteger operator%(const BigInteger& x1, const BigInteger& x2);

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
    Rational(int64_t x);
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

    friend std::istream& operator>>(std::istream& in, Rational& x);

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

int64_t BigInteger::binSearchAns(const BigInteger& x, int64_t min, int64_t max) {
    int64_t median = (min + max) / 2;
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
    std::vector <int64_t> data;
}

BigInteger::BigInteger(const BigInteger& x) {
    sign = x.sign;
    data = x.data;
}

BigInteger::BigInteger(int64_t y) {
    int64_t x = y;
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
            int64_t s = k;
            k = (data[i] <= x.data[i] + k ? 0 : -1);
            data[i] = (data[i] <= x.data[i] + s ? x.data[i] + s - data[i] : x.data[i] + s - data[i] + base);
        }
        return;
    }
    for (size_t i = 0; i < data.size(); ++i) {
        int64_t s = k;
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
    std::vector <int64_t> ans(data.size() + x.data.size() + 2, 0);
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
            int64_t j = div.data.size() - 1;
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
        int64_t bit = data[i];
        for (int64_t j = 0; j < logBase; ++j) {
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

void BigInteger::half() {
    int64_t rem = 0;
    int64_t current;
    for(size_t i = data.size(); i > 0; --i) {
        current = data[i - 1] + rem * base;
        data[i - 1] = current / 2;
        rem = current % 2;
    }
    removeZeros();
}

BigInteger gcd(const BigInteger& x1, const BigInteger& x2) {
    BigInteger a = (x1 >= 0 ? x1 : -x1);
    BigInteger b = (x2 >= 0 ? x2 : -x2);
    while(a && b) {
        if(a.data[0] % 2 == 0 && b.data[0] % 2 == 0) {
            a.half();
            b.half();
            return 2 * gcd(a, b);
        }
        if(a.data[0] % 2 == 0) {
            a.half();
            return gcd(a, b);
        }
        if(b.data[0] % 2 == 0) {
            b.half();
            return gcd(a, b);
        }
        if(a > b) {
            return gcd(a - b, b);
        }
        return gcd(a, b - a);
    }
    if(a == 0) {
        return b;
    }
    return a;
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
    for (int64_t i = str.size() - 1; i >= last; i -= x.logBase) {
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

Rational::Rational(int64_t x) {
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

std::istream& operator>>(std::istream& in, Rational& x) {
    BigInteger b;
    in >> b;
    x.numerator = b;
    x.denominator = 1;
    return in;
}

template <size_t N>
class Residue {
private:
    int64_t value;

public:
    Residue() = default;
    explicit Residue(int num);
    Residue(const Residue<N>& num);

    explicit operator int() const;
    explicit operator bool() const;

    template <size_t M>
    friend bool operator==(const Residue<M>& num1, const Residue<M>& num2);

    Residue<N>& operator+=(const Residue<N>& num);
    Residue<N>& operator-=(const Residue<N>& num);
    Residue<N>& operator*=(const Residue<N>& num);
    Residue<N>& operator/=(const Residue<N>& num);

};

template <size_t N>
Residue<N>::Residue(int num) {
    value = (num >= 0 ? num % N : N - (0 - num) % N);
}

template <size_t N>
Residue<N>::Residue(const Residue<N>& num) {
    value = num.value;
}

template <size_t N>
Residue<N>::operator int() const{
    return value;
}

template <size_t N>
Residue<N>::operator bool() const{
    return value != 0;
}

template <size_t N>
bool operator==(const Residue<N>& num1, const Residue<N>& num2) {
    return num1.value == num2.value;
}

template <size_t N>
bool operator!=(const Residue<N>& num1, const Residue<N>& num2) {
    return !(num1 == num2);
}

template <size_t N>
Residue<N>& Residue<N>::operator+=(const Residue<N>& num) {
    value += num.value;
    value %= N;
    return *this;
}

template <size_t N>
Residue<N>& Residue<N>::operator-=(const Residue<N>& num) {
    value -= num.value;
    value = (value >= 0 ? value % N : N - (0 - value) % N);
    return *this;
}

template <size_t N>
Residue<N>& Residue<N>::operator*=(const Residue<N>& num) {
    value *= num.value;
    value %= N;
    return *this;
}

template <size_t N>
Residue<N> operator+(const Residue<N>& num1, const Residue<N>& num2) {
    Residue<N> copy = num1;
    copy += num2;
    return copy;
}

template <size_t N>
Residue<N> operator-(const Residue<N>& num1, const Residue<N>& num2) {
    Residue<N> copy = num1;
    copy -= num2;
    return copy;
}

template <size_t N>
Residue<N> operator*(const Residue<N>& num1, const Residue<N>& num2) {
    Residue<N> copy = num1;
    copy *= num2;
    return copy;
}

template <size_t val, size_t div>
struct isPrime {
    static const bool ans = val % div != 0 && isPrime<(div * div > val) ? 0 : val, (div * div > val) ? 0 : div + 1>::ans;
};

template <>
struct isPrime <0, 0> {
    static const bool ans = true;
};

template <size_t div>
struct isPrime <1, div> {
    static const bool ans = false;
};

template <size_t N>
Residue<N> inDegree(const Residue<N>& num, size_t deg) {
    if(deg == 0) {
        return Residue<N>(1);
    }
    if(deg & 1) {
        return num * inDegree(num, deg - 1);
    }
    Residue<N> ans = inDegree(num, deg >> 1);
    return ans * ans;
}

template <size_t N>
Residue<N>& Residue<N>::operator/=(const Residue<N>& num) {
    static_assert(isPrime <N, 2>::ans, "is not Prime");
    (*this) *= inDegree(num, N - 2);
    return *this;
}

template <size_t N>
Residue<N> operator/(const Residue<N>& num1, const Residue<N>& num2) {
    Residue<N> copy = num1;
    copy /= num2;
    return copy;
}

template <size_t M, size_t N, typename Field = Rational>
class Matrix {
private:
    std::vector <std::vector <Field> > data;

public:
    Matrix();
    template <typename T>
    Matrix(const std::vector<std::vector<T> >& matrix);
    template <typename T>
    Matrix(const std::initializer_list<std::initializer_list<T>>& matrix);
    Matrix(const Matrix&) = default;

    std::vector<Field>& operator[](size_t index) {
        return data[index];
    }
    const std::vector<Field>& operator[](size_t index) const{
        return data[index];
    }

    Matrix<M, N, Field>& operator+=(const Matrix<M, N, Field>& matrix);
    Matrix<M, N, Field>& operator-=(const Matrix<M, N, Field>& matrix);
    Matrix<M, N, Field>& operator*=(const Field& num);
    Matrix<M, N, Field>& operator*=(const Matrix<M, N, Field>& matrix);

    Matrix<M, N, Field> gaussSaveDet() const;
    Matrix<M, N, Field> gaussForInvert() const;

    Field det() const;
    size_t rank() const;
    Field trace() const;
    Matrix<N, M, Field> transposed() const;
    Matrix<M, N, Field> inverted() const;
    void invert();

    std::vector<Field> getRow(size_t index) const;
    std::vector<Field> getColumn(size_t index) const;
};

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field>::Matrix() {
    static_assert(N == M, "is not square");
    data.clear();
    std::vector <std::vector <Field> > Data(M, std::vector<Field>(M, Field(0)));
    for(size_t i = 0; i < M; ++i) {
        Data[i][i] = Field(1);
    }
    data = Data;
    Data.clear();
}

template <size_t M, size_t N, typename Field>
template <typename T>
Matrix<M, N, Field>::Matrix(const std::vector<std::vector<T> >& matrix) {
    data.clear();
    std::vector <std::vector <Field> > Data(M, std::vector<Field>(N, Field(0)));
    for(size_t i = 0; i < M; ++i) {
        for(size_t j = 0; j < N; ++j) {
            Data[i][j] = Field(matrix[i][j]);
        }
    }
    data = Data;
}

template <size_t M, size_t N, typename Field>
template <typename T>
Matrix<M, N, Field>::Matrix(const std::initializer_list<std::initializer_list<T>>& matrix) {
    data.clear();
    for (auto i : matrix) {
        data.push_back({});
        for (auto j : i) {
            data.back().push_back(Field(j));
        }
    }
}

template <size_t M, size_t N, typename Field>
bool operator==(const Matrix<M, N, Field>& m1, const Matrix<M, N, Field>& m2) {
    for(size_t i = 0; i < M; ++i) {
        for(size_t j = 0; j < N; ++j) {
            if(m1[i][j] != m2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template <size_t M, size_t N, typename Field>
bool operator!=(const Matrix<M, N, Field>& m1, const Matrix<M, N, Field>& m2){
    return !(m1 == m2);
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field>& Matrix<M, N, Field>::operator+=(const Matrix<M, N, Field>& matrix){
    for(size_t i = 0; i < M; ++i) {
        for(size_t j = 0; j < N; ++j) {
            data[i][j] += matrix[i][j];
        }
    }
    return *this;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field>& Matrix<M, N, Field>::operator-=(const Matrix<M, N, Field>& matrix){
    for(size_t i = 0; i < M; ++i) {
        for(size_t j = 0; j < N; ++j) {
            data[i][j] -= matrix[i][j];
        }
    }
    return *this;
}

template <size_t M, size_t N, size_t K, typename Field>
Matrix<M, K, Field> operator*(const Matrix<M, N, Field>& m1, const Matrix<N, K, Field>& m2) {
    std::vector <std::vector <Field>> ansData(M, std::vector<Field>(K, Field(0)));
    for(size_t i = 0; i < M; ++i) {
        for(size_t j = 0; j < K; ++ j) {
            for(size_t f = 0; f < N; ++f) {
                ansData[i][j] += m1[i][f] * m2[f][j];
            }
        }
    }
    Matrix<M, K, Field> ans(ansData);
    return ans;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field>& Matrix<M, N, Field>::operator*=(const Field& num) {
    for(size_t i = 0; i < M; ++i) {
        for(size_t j = 0; j < N; ++j) {
            data[i][j] *= num;
        }
    }
    return *this;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field>& Matrix<M, N, Field>::operator*=(const Matrix<M, N, Field>& matrix) {
    static_assert(N == M, "is not square");
    *this = *this * matrix;
    return *this;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator*(const Matrix<M, N, Field>& matrix, const Field& num) {
    Matrix<M, N, Field> ans = matrix;
    ans *= num;
    return ans;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator*(const Field& num, const Matrix<M, N, Field>& matrix) {
    Matrix<M, N, Field> ans = matrix;
    ans *= num;
    return ans;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator-(const Matrix<M, N, Field>& m1, const Matrix<M, N, Field>& m2) {
    Matrix<M, N, Field> ans = m1;
    ans -= m2;
    return ans;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator+(const Matrix<M, N, Field>& m1, const Matrix<M, N, Field>& m2) {
    Matrix<M, N, Field> ans = m1;
    ans += m2;
    return ans;
}

template <size_t N, typename Field = Rational>
using SquareMatrix = Matrix<N, N, Field>;

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> Matrix<M, N, Field>::gaussSaveDet() const{
    Matrix<M, N, Field> copy(*this);
    size_t step = 0;
    for(size_t j = 0; j < N; ++j) {
        size_t i = step;
        if (step >= M) {
            break;
        }
        while(i < M && copy[i][j] == Field(0)) {
            ++i;
        }
        if (i == M) {
            continue;
        }
        if (i > step) {
            std::swap(copy[i], copy[step]);
        }
        for(size_t k = step + 1; k < M; ++k) {
            if(copy[k][j] != Field(0)){
                Field factor = copy[k][j] / copy[step][j];
                for(size_t f = j; f < N; ++f) {
                    copy[k][f] -= copy[step][f] * factor;
                }
            }
        }
        ++step;
    }
    return copy;
}

template <size_t M, size_t N, typename Field>
Field Matrix<M, N, Field>::det() const{
    static_assert(N == M, "is not square");
    Field determinant(1);
    Matrix<M, N, Field> ans = gaussSaveDet();
    for(size_t i = 0; i < M; ++i) {
        determinant *= ans[i][i];
    }
    return determinant;
}

template <size_t M, size_t N, typename Field>
size_t Matrix<M, N, Field>::rank() const{
    Matrix<M, N, Field> ans = gaussSaveDet();
    size_t step = 0;
    for(size_t i = 0; i < N; ++i) {
        if(ans[step][i] != Field(0)) {
            ++step;
        }
    }
    return step;
}

template <size_t M, size_t N, typename Field>
Field Matrix<M, N, Field>::trace() const{
    static_assert(N == M, "is not square");
    Field ans = Field(0);
    for(size_t i = 0; i < M; ++i) {
        ans += data[i][i];
    }
    return ans;
}

template <size_t M, size_t N, typename Field>
Matrix<N, M, Field> Matrix<M, N, Field>::transposed() const{
    std::vector <std::vector <Field>> ansData(N, std::vector<Field>(M, Field(0)));
    for(size_t i = 0; i < M; ++i) {
        for(size_t j = 0; j < N; ++j) {
            ansData[j][i] = data[i][j];
        }
    }
    Matrix<N, M, Field> ans(ansData);
    return ans;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> Matrix<M, N, Field>::gaussForInvert() const{
    static_assert(N == M, "is not square");
    Matrix<M, M, Field> help(*this);
    Matrix<M, M, Field> ans;
    size_t step = 0;
    for(size_t j = 0; j < N; ++j) {
        size_t i = step;
        if (step >= M) {
            break;
        }
        while(i < M && help[i][j] == Field(0)) {
            ++i;
        }
        if (i == M) {
            continue;
        }
        if (i > step) {
            std::swap(help[i], help[step]);
            std::swap(ans[i], ans[step]);
        }
        if (help[step][j] != Field(1)) {
            Field coef = help[step][j];
            for(size_t k = 0; k < M; ++k) {
                help[step][k] /= coef;
                ans[step][k] /= coef;
            }
        }
        for(size_t k = 0; k < M; ++k) {
            if(help[k][j] != Field(0) && k != step){
                Field factor = help[k][j];
                for(size_t f = 0; f < N; ++f) {
                    help[k][f] -= help[step][f] * factor;
                    ans[k][f] -= ans[step][f] * factor;
                }
            }
        }
        ++step;
    }
    return ans;
}

template <size_t M, size_t N, typename Field>
Matrix<M, N, Field> Matrix<M, N, Field>::inverted() const{
    Matrix<M, N, Field> ans = gaussForInvert();
    return ans;
}

template <size_t M, size_t N, typename Field>
void Matrix<M, N, Field>::invert() {
    *this = gaussForInvert();
}

template <size_t M, size_t N, typename Field>
std::vector<Field> Matrix<M, N, Field>::getRow(size_t index) const{
    return data[index];
}

template <size_t M, size_t N, typename Field>
std::vector<Field> Matrix<M, N, Field>::getColumn(size_t index) const{
    std::vector<Field> ans(M, Field(0));
    for(size_t i = 0; i < M; ++i) {
        ans[i] = data[i][index];
    }
    return ans;
}