#pragma once
#include <iostream>
#include <vector>
#include <exception>

template <typename T>
class Deque {
private: 
    static const size_t base = 32;//размер блока
    size_t sizee;//размер deque
    std::vector <T*> external;//внешний массив указателей
    size_t beginn;//начало дека
    size_t endd;//конец дека
    size_t first;//начало внешнего массива
    size_t last;//конец внешнего массива

    void expandCapacity() {
        if (external.size() == 0) {
            external.resize(1, nullptr);
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
        if (last == external.size()) {
            expandCapacity();
        }
        external[last] = reinterpret_cast <T*> (new uint8_t[base * sizeof(T)]);
        ++last;
        return;
    }

    void addNewArrayFront() {
        if (first == 0) {
            expandCapacity();
        }
        --first;
        external[first] = reinterpret_cast <T*> (new uint8_t[base * sizeof(T)]);
        return;
    }

public:
    Deque() {
        external.resize(3, nullptr);
        sizee = 0;
        beginn = 0;
        endd = 0;
        first = 1;
        last = 1;
    }

    Deque(const Deque<T>& deq) {
        sizee = 0;
        beginn = 0;
        endd = 0;
        first = 0;
        last = 0;
        for (size_t i = 0; i < deq.sizee; ++i) {
            push_back(deq[i]);
        }
    }

    Deque(int num) {
        sizee = 0;
        beginn = 0;
        endd = 0;
        first = 0;
        last = 0;
        T emptyElem;
        for (int i = 0; i < num; ++i) {
            push_back(emptyElem);
        }
    }

    Deque(int num, const T& d) {
        sizee = 0;
        beginn = 0;
        endd = 0;
        first = 0;
        last = 0;
        for (int i = 0; i < num; ++i) {
            push_back(d);
        }
    }

    Deque& operator=(const Deque<T>& deq) {
        for (size_t i = 0; i < external.size(); ++i) {
            external[i] = nullptr;
        }
        sizee = 0;
        beginn = 0;
        endd = 0;
        first = 0;
        last = 0;
        for (size_t i = 0; i < deq.sizee; ++i) {
            push_back(deq[i]);
        }
        return *this;
    }

    size_t size() const {
        return sizee;
    }

    T& operator[](size_t index) {
        return *(external[(beginn + index) / base + first] + (beginn + index) % base);
    }

    const T& operator[](size_t index) const{
        return *(external[(beginn + index) / base + first] + (beginn + index) % base);
    }

    T& at(size_t pos) {
        if (pos >= sizee) {
            throw std::out_of_range("out of range");
        }
        return *(external[(beginn + pos) / base + first] + (beginn + pos) % base);
    }

    const T& at(size_t pos) const {
        if (pos >= sizee) {
            throw std::out_of_range("out of range");
        }
        return *(external[(beginn + pos) / base + first] + (beginn + pos) % base);
    }

    void push_back(const T& val) {
        if (sizee == 0) {
            if (external.size() == 0) {
                external.resize(3, nullptr);
            }
            external[2 * (external.size() / 3)] = reinterpret_cast <T*> (new uint8_t[base * sizeof(T)]);
            beginn = 0;
            endd = 1;
            first = 2 * (external.size() / 3);
            last = first + 1;
            new(external[last - 1] + endd - 1) T(val);
            ++sizee;
        } else if (endd < base) {
            new(external[last - 1] + endd) T(val);
            ++endd;
            ++sizee;
        } else {
            addNewArrayBack();
            endd = 1;
            new(external[last - 1] + endd - 1) T(val);
            ++sizee;
        }
        return;
    }

    void pop_back() {
        --sizee;
        if (sizee == 0) {
            delete[] external[first];
            external[first] = nullptr;
            sizee = 0;
            beginn = 0;
            endd = 0;
            first = 0;
            last = 0;
        } else {
            if (endd == 1) {
                endd = base;
                --last;
                delete[] external[last];
                external[last] = nullptr;
            } else {
                --endd;
            }
        }
    }

    void push_front(const T& val) {
        if (sizee == 0) {
            if (external.size() == 0) {
                external.resize(3, nullptr);
            }
            external[2 * (external.size() / 3)] = reinterpret_cast <T*> (new uint8_t[base * sizeof(T)]);
            beginn = 0;
            endd = 1;
            first = 2 * (external.size() / 3);
            last = first + 1;
            new(external[last - 1] + endd - 1) T(val);
            ++sizee;
            std::cerr << first << "\n";
            std::cerr << external.size() <<"\n";
        } else if (beginn > 0) {
            --beginn;
            new(external[first] + beginn) T(val);
            ++sizee;
        } else {
            addNewArrayFront();
            beginn = base - 1;
            new(external[first] + beginn) T(val);
            ++sizee;
        }
        return;
    }

    void pop_front() {
        --sizee;
        if (sizee == 0) {
            delete[] external[first];
            external[first] = nullptr;
            sizee = 0;
            beginn = 0;
            endd = 0;
            first = 0;
            last = 0;
        } else {
            if (beginn == base - 1) {
                beginn = 0;
                ++first;
                delete[] external[first - 1];
                external[first - 1] = nullptr;
            } else {
                ++beginn;
            }
        }
    }

    template <bool isconst>
    class Iterator {

    public:

        size_t pos;
        T** pointerr = nullptr;

        typedef std::conditional_t<isconst, const T, T> value_type;
        typedef std::conditional_t<isconst, const T*, T*> pointer;
        typedef std::conditional_t<isconst, const T&, T&> reference;
        typedef int difference_type;
        typedef std::random_access_iterator_tag iterator_category;
        
        Iterator() = default;

        Iterator(T** other, size_t num) {
            pointerr = other;
            pos = num;
        }

        Iterator(const Iterator<false>& other) {
            pointerr = other.pointerr;
            pos = other.pos;
        }

        ~Iterator() = default;

        Iterator& operator=(const Iterator<false>& other) {
            pointerr = other.pointerr;
            pos = other.pos;
            return *this;
        }

        Iterator& operator++(){
            if (pos < base - 1) {
                ++pos;
            } else {
                ++pointerr;
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
                --pointerr;
                pos = base - 1;
            }
            return *this;
        }

        Iterator operator--(int) {
            Iterator copy = *this;
            --*this;
            return copy;
        }

        Iterator operator+(long long num) const {
            Iterator copy(*this);
            if (num > 0) {
                copy.pointerr += (this->pos + pointerr) / base;
                copy.pos += (this->pos + pointerr) % base;
            } else {
                copy.pointerr += ((this->pos + pointerr) - base + 1) / base;
                copy.pos += ((this->pos + pointerr) % base + base) % base;
            }
            return copy;
        }

        Iterator operator-(long long num) const {
            Iterator copy(*this);
            return copy + (-num);
        }

        bool operator==(const Iterator& other) const {
            return (pointerr == other.pointerr) && (pos == other.pos);
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        bool operator<(const Iterator& other) const {
            return (pointerr == other.pointerr ? pos < other.pos : pointerr < other.pointerr);
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
            return *(*pointerr + pos);
        }

        pointer operator->() const{
            return *pointerr + pos;
        }

    };

    typedef Iterator<false> iterator;
    typedef Iterator<true> const_iterator;

    iterator begin(){
        return iterator(&external[first], beginn);
    }

    iterator end() {
        return iterator(&external[last - 1], endd);
    }

    const_iterator begin() const {
        return const_iterator(const_cast<T**>(&external[first]), beginn);
    }

    const_iterator end() const {
        return const_iterator(const_cast<T**>(&external[last - 1]), endd);
    }

    const_iterator cbegin() const {
        return const_iterator(const_cast<T**>(&external[first]), beginn);
    }

    const_iterator cend() const {
        return const_iterator(const_cast<T**>(&external[last - 1]), endd);
    }

    template <bool isconst>
    class revIterator {

    public:

        size_t revPos;
        T** revPointer = nullptr;

        typedef std::conditional_t<isconst, const T, T> value_type;
        typedef std::conditional_t<isconst, const T*, T*> pointer;
        typedef std::conditional_t<isconst, const T&, T&> reference;
        typedef int difference_type;
        typedef std::random_access_iterator_tag iterator_category;

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

        revIterator operator+(long long num) const {
            revIterator copy(*this);
            if (num > 0) {
                for (long long i = 0; i < num; ++i) {
                    ++copy;
                }
            } else {
                for (long long i = 0; i < (-num); ++i) {
                    --copy;
                }
            }
            return copy;
        }

        revIterator operator-(long long num) const {
            revIterator copy(*this);
            return copy + (-num);
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
            return *(*revPointer + revPos);
        }

        pointer operator->() const{
            return *revPointer + revPos;
        }

    };

    typedef revIterator<false> reverse_iterator;
    typedef revIterator<true> const_reverse_iterator;

    reverse_iterator rbegin(){
        return reverse_iterator(&external[last - 1], (base + endd - 1) % base);
    }
    
    reverse_iterator rend(){
        return reverse_iterator(&external[first], (base + beginn - 1) % base);
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(const_cast<T**>(&external[last - 1], (base + endd - 1) % base));
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(const_cast<T**>(&external[first], (base + beginn - 1) % base));
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(const_cast<T**>(&external[last - 1], (base + endd - 1) % base));
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(const_cast<T**>(&external[first], (base + beginn - 1) % base));
    }

    void insert(iterator iter, const T& val) {
        std::cerr << 8 << "\n";
        push_back(val);
        iterator pos = end() - 1;
        while (pos > iter) {
            *pos = *(pos - 1);
            --pos;
        }
        *iter = val;
    }

    void erase(iterator iter) {
        std::cerr << 9 << "\n";
        iterator pos = iter;
        while (pos < end() - 1) {
            *pos = *(pos + 1);
            ++pos;
        }
        pop_back();
    }

    ~Deque() {
        std::cerr << 10 << "\n";
        for(size_t i = first; i < last; ++i) {
            for(size_t j = 0; j < base; ++j) {
                (external[i] + j)->~T();
            }
            delete[] reinterpret_cast<uint8_t*>(external[i]);
        }
        sizee = 0;
        beginn = 0;
        endd = 0;
        first = 0;
        last = 0;
        external.clear();
    }

};