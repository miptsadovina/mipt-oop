#include <iostream>
#include <cstring>

class String {
private:
    size_t capacity;
	size_t size;
	char* data;

	void changeCapacity(size_t newSizeCapacity) {
		capacity = newSizeCapacity;
		char* newData = new char[newSizeCapacity];
		memcpy (newData, data, size);
		delete[] data;
        data = newData;
	}

	void expandCapacity(size_t newSize) {
		if (newSize > capacity) {
            changeCapacity (newSize * 2);
		}
	}

    void swap(String& other) {
        std::swap(capacity, other.capacity);
        std::swap(size, other.size);
        std::swap(data, other.data);
    }

public:
    String(const char* c) {
        capacity = strlen(c) * 2;
        size = strlen(c);
        data = new char[capacity];
        memcpy(data, c, size);
    }

    String(size_t n, char c = '\0') {
        capacity = n * 2;
        size = n;
        data = new char[capacity];
        memset(data, c, size);
    }

    String() {
        capacity = 2;
        size = 0;
        data = new char[2];
    }

    String(const String& s) {
        capacity = s.capacity;
        size = s.size;
        data = new char[capacity];
        memcpy(data, s.data, size);
    }

    friend bool operator==(const String& s1, const String& s2);

    String& operator=(const String& other) {
        String copy = other;
        swap(copy);
        return *this;
    }

    char& operator[](size_t index) {
        return data[index];
    }

    const char& operator[](size_t index) const {
        return data[index];
    }

    size_t length() const{
        return size;
    }

    void push_back(char c) {
        expandCapacity(size + 1);
        data[size] = c;
        ++size;
    }

    void pop_back() {
        if (size > 0) --size;
    }

    const char& front() const {
        return data[0];
    }

    char& front() {
        return data[0];
    }

    const char& back() const {
        return data[size - 1];
    }

    char& back() {
        return data[size - 1];
    }

    String& operator+=(const String& other) {
        expandCapacity(size + other.size);
        memcpy(data + size, other.data, other.size);
        size += other.size;
        return *this;
    }

    String& operator+=(const char c) {
        push_back(c);
        return *this;
    }

    String substr(size_t start, size_t count) const{
        String ans = String(count, '0');
        memcpy(ans.data, data + start, count);
        return ans;
    }

    size_t find(const String& substring) const{
        if (substring.size > size) return size;
        for (size_t i = 0; i <= size - substring.size; ++i) {
            int rc = std::memcmp(data + i, substring.data, substring.size);
            if (rc == 0) return i;
        }
        return size;
    }

    size_t rfind(const String& substring) const{
        if (substring.size > size) return size;
        for (size_t i = size - substring.size; i >= 1; --i) {
            int rc = std::memcmp(data + i, substring.data, substring.size);
            if (rc == 0) return i;
        }
        if (data[0] == substring.data[0]) {
            int rc = std::memcmp(data, substring.data, substring.size);
            if (rc == 0) return 0;
        }
        return size;
    }

    bool empty() const{
        if (size == 0) return true;
        else return false;
    }

    void clear() {
        delete[] data;
        size = 0;
        capacity = 2;
        data = new char[2];
    }

    friend std::ostream& operator<< (std::ostream& out, const String& s);

    friend std::istream& operator>> (std::istream& out, String& s);

    ~String() {
        delete[] data;
    }

};

bool operator==(const String& s1, const String& s2) {
        bool ans;
        int difference = std::memcmp(s1.data, s2.data, std::min(s1.length(), s2.length()));
        ans = ((s1.length() == s2.length() && difference == 0) ? true : false);
        return ans;
    }

String operator+(const String& s,const char c) {
        String ans = s;
        ans.push_back(c);
        return ans;
    }

String operator+(const String& s1, const String& s2) {
    String ans;
    ans = s1;
    ans += s2;
    return ans;
}

String operator+(const char c, const String& s) {
    String ans(1, c);
    ans += s;
    return ans;
}

std::ostream& operator<< (std::ostream& out, const String& s) {
    for (size_t i = 0; i < s.size; i++) {
        out << s.data[i];
    }
    return out;
}

std::istream& operator>> (std::istream& in, String& s) {
    s.size = 0;
    char c;
    in.get(c);
    while(isspace(c)) {
        if (!in.get(c)) return in;
    }
    s.push_back(c);
    while (in.get(c) && !isspace(c)) {
        s.push_back(c);
    }
    return in;
}
