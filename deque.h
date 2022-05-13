#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T>
class Deque {
private:
    static const size_t base = 32;
    size_t size_ = 0;
    std::vector <T*> external;
    size_t begin_ = 0;//начало дека
    size_t end_ = 0;//конец дека
    size_t first = 0;//начало внешнего массива
    size_t last = 0;//конец внешнего массива

    void expandCapacity() {
        if (external.size() == 0) {
            external.resize(1, nullptr);
            first = 0;
            last = 0;
        }
        std::vector <T*> newExternal(external.size() * 3, nullptr);
        size_t sizeOfPointers = last - first;
        for (size_t i = external.size(); i < external.size() + sizeOfPointers; ++i) {
            newExternal[i] = external[i - external.size()];
        }
        last += external.size();
        first += external.size();
        external = newExternal;
    }

    void addNewArrayBack() {
        if (external.size() == 0 || last == external.size()) {
            expandCapacity();
        }
        external[last] = reinterpret_cast <T*> (new uint8_t[base * sizeof(T)]);
        ++last;
    }

    void addNewArrayFront() {
        if (external.size() == 0 || first == 0) {
            expandCapacity();
        }
        external[first - 1] = reinterpret_cast <T*> (new uint8_t[base * sizeof(T)]);
        --first;
    }

    void delete_deque() {
        for(size_t i = first; i < last; ++i) {
            for(size_t j = 0; j < base; ++j) {
                (external[i] + j)->~T();
            }
            delete[] reinterpret_cast<uint8_t*>(external[i]);
        }
    }

    void clear() {
        while(size_) {
            pop_back();
        }
    }

public:
    Deque() = default;

    Deque(const Deque<T>& deq) {
        try {
            for (size_t i = 0; i < deq.size_; ++i) {
                push_back(deq[i]);
            }
        } catch (...) {
            clear();
        }
    }

    Deque(int num) {
        T emptyElem;
        try {
            for (int i = 0; i < num; ++i) {
                push_back(emptyElem);
            }
        } catch (...) {
            clear();
        }
    }

    Deque(int num, const T& d) {
        try {
            for (int i = 0; i < num; ++i) {
                push_back(d);
            }
        } catch (...) {
            clear();
        }
    }

    Deque& operator=(const Deque<T>& deq) {
        delete_deque();
        try {
            for (size_t i = 0; i < deq.size_; ++i) {
                push_back(deq[i]);
            }
        } catch (...) {
            clear();
        }
        return *this;
    }

    size_t size() const {
        return size_;
    }

    T& operator[](size_t index) {
        return *(external[(begin_ + index) / base + first] + (begin_ + index) % base);
    }

    const T& operator[](size_t index) const{
        return *(external[(begin_ + index) / base + first] + (begin_ + index) % base);
    }

    T& at(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("out of range");
        }
        return *(external[(begin_ + pos) / base + first] + (begin_ + pos) % base);
    }

    const T& at(size_t pos) const {
        if (pos >= size_) {
            throw std::out_of_range("out of range");
        }
        return *(external[(begin_ + pos) / base + first] + (begin_ + pos) % base);
    }

    void push_back(const T& val) {
        if (size_ && end_ < base) {
            ++end_;
        } else {
            addNewArrayBack();
            end_ = 1;
        }
        try {
            ++size_;
            new(external[last - 1] + end_ - 1) T(val);
        } catch(...) {
            pop_back();
            throw;
        }
    }

    void pop_back() {
        --size_;
        if (size_ == 0) {
            external[first] = nullptr;
            external.clear();
        } else {
            if (end_ == 1) {
                end_ = base;
                --last;
                delete[] external[last];
                external[last] = nullptr;
            } else {
                --end_;
            }
        }
    }

    void push_front(const T& val) {
        if (size_ && begin_ > 0) {
            --begin_;
        } else {
            addNewArrayFront();
            begin_ = base - 1;
            if (end_ == 0) {
                end_ = 32;
            }
        }
        try {
            ++size_;
            new(external[first] + begin_) T(val);
        } catch(...) {
            pop_front();
            throw;
        }
    }

    void pop_front() {
        --size_;
        if (size_ == 0) {
            external[first] = nullptr;
            external.clear();
        } else {
            if (begin_ == base - 1) {
                begin_ = 0;
                ++first;
                delete[] external[first - 1];
                external[first - 1] = nullptr;
            } else {
                ++begin_;
            }
        }
    }

    template <bool isconst>
    class Iterator {

    public:

        size_t pos;
        T** pointer_ = nullptr;

        typedef std::conditional_t<isconst, const T, T> value_type;
        typedef std::conditional_t<isconst, const T*, T*> pointer;
        typedef std::conditional_t<isconst, const T&, T&> reference;

        Iterator() = default;

        Iterator(T** other, size_t num) {
            pointer_ = other;
            pos = num;
        }

        Iterator(const Iterator<false>& other) {
            pointer_ = other.pointer_;
            pos = other.pos;
        }

        ~Iterator() = default;

        Iterator& operator=(const Iterator<false>& other) {
            pointer_ = other.pointer_;
            pos = other.pos;
        }

        Iterator& operator++(){
            if (pos < base - 1) {
                ++pos;
            } else {
                ++pointer_;
                pos = 0;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator copy = *this;
            ++*this;
            return copy;
        }

        Iterator& operator--(){
            if (pos > 0) {
                --pos;
            } else {
                --pointer_;
                pos = base - 1;
            }
            return *this;
        }

        Iterator operator--(int) {
            Iterator copy = *this;
            --*this;
            return copy;
        }

        Iterator operator+(int num) const {
            Iterator copy(*this);
            if (num > 0) {
                copy.pos = (pos + num) % base;
                copy.pointer_ += (pos + num) / base;
            } else if (num < 0) {
                copy.pos = ((int(pos) + num) % base + base) % base;
                copy.pointer_ += ((int(pos) + num) - int(base) + 1) / int(base);
            }
            return copy;
        }

        Iterator operator-(int num) const {
            Iterator copy(*this);
            return copy + (-num);
        }

        bool operator==(const Iterator& other) const {
            return (pointer_ == other.pointer_) && (pos == other.pos);
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        bool operator<(const Iterator& other) const {
            return (pointer_ == other.pointer_ ? pos < other.pos : pointer_ < other.pointer_);
        }

        bool operator<=(const Iterator& other) const {
            return (*this < other) || (*this == other);
        }

        bool operator>(const Iterator& other) const {
            return !(*this <= other);
        }

        bool operator>=(const Iterator& other) const {
            return !(*this < other);
        }

        long long operator-(const Iterator& other) const {
            Iterator copy(*this);
            long long num = 0;
            while (copy > other) {
                ++num;
                --copy;
            }
            while (copy < other) {
                --num;
                ++copy;
            }
            return num;
        }

        reference operator*() const {
            return *(*pointer_ + pos);
        }

        pointer operator->() const{
            return *pointer_ + pos;
        }

    };

    typedef Iterator<false> iterator;
    typedef Iterator<true> const_iterator;

    iterator begin(){
        return iterator(&external[first], begin_);
    }

    iterator end() {
        return iterator(&external[last - 1], end_);
    }

    const_iterator begin() const {
        return const_iterator(const_cast<T**>(&external[first]), begin_);
    }

    const_iterator end() const {
        return const_iterator(const_cast<T**>(&external[last - 1]), end_);
    }

    const_iterator cbegin() const {
        return const_iterator(const_cast<T**>(&external[first]), begin_);
    }

    const_iterator cend() const {
        return const_iterator(const_cast<T**>(&external[last - 1]), end_);
    }

    template <bool isconst>
    class revIterator {

    public:

        size_t revPos;
        T** revPointer = nullptr;

        typedef std::conditional_t<isconst, const T, T> value_type;
        typedef std::conditional_t<isconst, const T*, T*> pointer;
        typedef std::conditional_t<isconst, const T&, T&> reference;

        revIterator() = default;

        revIterator(T** other, size_t num) {
            revPointer = other;
            revPos = num;
        }

        revIterator(const revIterator<false>& other) {
            revPointer = other.revPointer;
            revPos = other.revPos;
        }

        ~revIterator() = default;

        revIterator& operator=(const revIterator<false>& other) {
            revPointer = other.revPointer;
            revPos = other.revPos;
        }

        revIterator& operator++(){
            if (revPos > 0) {
                --revPos;
            } else {
                --revPointer;
                revPos = base - 1;
            }
        }

        revIterator operator++(int) {
            revIterator copy = *this;
            ++*this;
            return copy;
        }

        revIterator& operator--(){
            if (revPos < base - 1) {
                ++revPos;
            } else {
                ++revPointer;
                revPos = 0;
            }
            return *this;
        }

        revIterator operator--(int) {
            revIterator copy = *this;
            --*this;
            return copy;
        }

        revIterator operator+(int num) const {
            return (*this) - num;
        }

        revIterator operator-(int num) const {
            Iterator copy(*this);
            if (num > 0) {
                copy.pos = (revPos + num) % base;
                copy.pointer_ += (revPos + num) / base;
            } else if (num < 0) {
                copy.pos = ((int(revPos) + num) % base + base) % base;
                copy.pointer_ += ((int(revPos) + num) - int(base) + 1) / int(base);
            }
            return copy;
        }

        bool operator==(const revIterator<isconst>& other) const {
            return (revPointer == other.revPointer) && (revPos == other.revPos);
        }

        bool operator!=(const revIterator<isconst>& other) const {
            return !(*this == other);
        }

        bool operator<(const revIterator<isconst>& other) const {
            return (revPointer > other.revPointer ? revPointer > other.revPointer : revPos > other.revPos);
        }

        bool operator<=(const revIterator<isconst>& other) const {
            return (*this < other) || (*this == other);
        }

        bool operator>(const revIterator<isconst>& other) const {
            return !(*this <= other);
        }

        bool operator>=(const revIterator<isconst>& other) const {
            return !(*this < other);
        }

        long long operator-(const revIterator& other) const {
            revIterator copy(*this);
            long long num = 0;
            while (copy > other) {
                ++num;
                --copy;
            }
            while (copy < other) {
                --num;
                ++copy;
            }
            return num;
        }

        reference operator*() const {
            return *(*revPointer + revPos);
        }

        pointer operator->() const{
            return *revPointer + revPos;
        }

    };

    typedef revIterator<false> reverse_iterator;
    typedef revIterator<true> const_reverse_iterator;

    reverse_iterator rbegin(){
        return reverse_iterator(&external[last - 1], (base + end_ - 1) % base);
    }

    reverse_iterator rend(){
        return reverse_iterator(&external[first], (base + begin_ - 1) % base);
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(const_cast<T**>(&external[last - 1], (base + end_ - 1) % base));
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(const_cast<T**>(&external[first], (base + begin_ - 1) % base));
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(const_cast<T**>(&external[last - 1], (base + end_ - 1) % base));
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(const_cast<T**>(&external[first], (base + begin_ - 1) % base));
    }

    void insert(iterator iter, const T& val) {
        push_back(val);
        iterator pos = end() - 1;
        while (pos > iter) {
            *pos = *(pos - 1);
            --pos;
        }
        *iter = val;
    }

    void erase(iterator iter) {
        iterator pos = iter;
        while (pos < end() - 1) {
            *pos = *(pos + 1);
            ++pos;
        }
        pop_back();
    }

    ~Deque() {
        delete_deque();
    }

};