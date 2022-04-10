#include <iostream>

template<size_t N>
class StackStorage {
private:
    char data[N];
    size_t end = 0;

public:
    StackStorage() = default;
    char* alloc_mem(size_t sz, size_t align) {
        if (end % align) {
            end += align - (end % align);
        }
        end += sz;
        return data + end - sz;
    }

};

template <typename T, size_t N>
class StackAllocator {
private: 
    StackStorage<N>* storage = nullptr;

public:

    StackAllocator(const StackAllocator<T, N>& other) : storage(other.storage) {

    }

    template <typename T2>
    struct rebind {
        typedef StackAllocator<T2, N> other;
    };

    template <typename T2, size_t N2>
    friend class StackAllocator;

    template <typename T2>
    StackAllocator(const StackAllocator<T2, N>& other) : storage(other.storage) {
    }

    StackAllocator(StackStorage<N>& other_storage) : storage(&other_storage) {
    }

    StackAllocator& operator=(const StackAllocator<T, N>& other) {
        storage = other.storage;
        return *this;
    }

    T* allocate(size_t sz) {
        return reinterpret_cast<T*>(storage->alloc_mem(sz * sizeof(T), sizeof(T)));
    }

    void deallocate(T*, size_t) {

    }

    typedef T value_type;

    bool operator==(const StackAllocator<T, N>& other) {
        return storage == other.storage;
    }

    bool operator!=(const StackAllocator<T, N>& other) {
        return *this != other;
    }
    
    ~StackAllocator() {

    }

};

template <typename T, typename Alloc = std::allocator<T> >
class List {
private:
    struct Node {
        T value;
        Node* prev = nullptr;
        Node* next = nullptr;
        explicit Node(const T& elem) : value(elem) {

        }
        Node() : value() {

        }
    };
    typedef typename Alloc::template rebind<Node>::other allocator;
    typedef std::allocator_traits<allocator> allocTraits;
    typedef std::allocator_traits<Alloc> AllocTraits;

    allocator list_allocator;

    Node* fakeNode = nullptr;
    size_t list_size;

public:
    void push_back() {
        Node* help = allocateNode();
        ++list_size;
        help->next = fakeNode;
        help->prev = fakeNode->prev;
        fakeNode->prev->next = help;
        fakeNode->prev = help;
    }

    Node* allocateNode() {
        Node* help = allocTraits::allocate(list_allocator, 1);
        try {
            allocTraits::construct(list_allocator, help);
        } catch (...) {
            allocTraits::deallocate(list_allocator, help, 1);
            throw;
        }
        return help;
    }

    List() : list_allocator(Alloc()), fakeNode(allocTraits::allocate(list_allocator, 1)) {
        list_size = 0;
        fakeNode->next = fakeNode;
        fakeNode->prev = fakeNode;
    }

    List(allocator other_allocator) : list_allocator(AllocTraits::select_on_container_copy_construction(other_allocator)), fakeNode(allocTraits::allocate(list_allocator, 1)) {
        list_size = 0;
        fakeNode->next = fakeNode;
        fakeNode->prev = fakeNode;
    }

    List(size_t sz) : List(Alloc()) {
        for (size_t i = 0; i < sz; ++i) {
            push_back();
        }
    }

    List(size_t sz, const T& element) : List(Alloc()) {
        for (size_t i = 0; i < sz; ++i) {
            push_back(element);
        }
    }

    List(size_t sz, allocator other_allocator) : list_allocator(other_allocator), fakeNode(allocTraits::allocate(list_allocator, 1)) {
        list_size = 0;
        fakeNode->next = fakeNode;
        fakeNode->prev = fakeNode;
        for (size_t i = 0; i < sz; ++i) {
            push_back();
        }
    }

    List(size_t sz, const T& element, allocator other_allocator) : list_allocator(other_allocator), fakeNode(allocTraits::allocate(list_allocator, 1)) {
        list_size = 0;
        fakeNode->next = fakeNode;
        fakeNode->prev = fakeNode;
        for (size_t i = 0; i < sz; ++i) {
            push_back();
        }
    }

    allocator get_allocator() const {
        return list_allocator;
    }

    List(const List& other) : List(AllocTraits::select_on_container_copy_construction(other.list_allocator)) {
        for (auto &element : other) {
            push_back(element);
        }
    }

    List& operator=(const List& other) {
        if (AllocTraits::propagate_on_container_copy_assignment::value) {
            list_allocator = other.list_allocator;
        } else {
            list_allocator = AllocTraits::select_on_container_copy_construction(other.list_allocator);
        }
        try {
            for (auto &element : other) {
                push_back(element);
            }
        } catch(...) {
            
        }
        while (list_size > other.list_size) {
            pop_front();
        }
        return *this;
    }

    size_t size() const{
        return list_size;
    }

    void push_back(const T& element) {
        Node* help = allocateNode(element);
        help->next = fakeNode;
        help->prev = fakeNode->prev;
        fakeNode->prev->next = help;
        fakeNode->prev = help;
        ++list_size;
    }

    Node* allocateNode(const T& element) {
        Node* help = allocTraits::allocate(list_allocator, 1);
        try {
            allocTraits::construct(list_allocator, help, element);
        } catch (...) {
            allocTraits::deallocate(list_allocator, help, 1);
            throw;
        }
        return help;
    }

    template <bool isconst>
    class Iterator {

    public:

        Node* pointerr;
        typedef std::conditional_t<isconst, const T, T> value_type;
        typedef std::conditional_t<isconst, const T*, T*> pointer;
        typedef std::conditional_t<isconst, const T&, T&> reference;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef int difference_type;
        
        Iterator() = default;

        Iterator(Node* ptr) {
            pointerr = ptr;
        }

        Iterator(const Iterator<false>& other) {
            pointerr = other.pointerr;
        }

        ~Iterator() = default;

        Iterator& operator=(const Iterator<false>& other) {
            pointerr = other.pointerr;
            return *this;
        }

        Iterator& operator++(){
            pointerr = pointerr->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator copy = *this;
            ++*this;
            return copy;
        }

        Iterator& operator--(){
            pointerr = pointerr->prev;
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

        Iterator operator-(long long num) const {
            Iterator copy(*this);
            return copy + (-num);
        }

        bool operator==(const Iterator& other) const {
            return (pointerr == other.pointerr);
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        reference operator*() const {
            return pointerr->value;
        }

        pointer operator->() const{
            return &(pointerr->value);
        }
    };

    typedef Iterator<false> iterator;
    typedef Iterator<true> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    iterator begin() const {
        return iterator(fakeNode->next);
    }

    iterator end() const {
        return iterator(fakeNode);
    }

    const_iterator cbegin() const {
        return const_iterator(fakeNode->next);
    }

    const_iterator cend() const {
        return const_iterator(fakeNode);
    }

    reverse_iterator rbegin() const {
        return reverse_iterator(fakeNode);
    }

    reverse_iterator rend() const {
        return reverse_iterator(fakeNode->prev);
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(fakeNode);
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(fakeNode->prev);
    }

    void insert(const_iterator iter, const T& elem) {
        Node* help = allocateNode(elem);
        Node* oldNode = iter.pointerr;
        help->value = elem;
        help->next = oldNode;
        help->prev = oldNode->prev;
        oldNode->prev->next = help;
        oldNode->prev = help;
        ++list_size;
    }

    void erase(const_iterator iter) {
        Node* oldNode = iter.pointerr;
        oldNode->prev->next = oldNode->next;
        oldNode->next->prev = oldNode->prev;
        deallocateNode(oldNode);
        --list_size;
    }

    void deallocateNode(Node* oldNode) {
        allocTraits::destroy(list_allocator, oldNode);
        allocTraits::deallocate(list_allocator, oldNode, 1);
    }

    void push_front(const T& elem) {
        insert(cbegin(), elem);
    }

    void pop_back() {
        erase(--cend());
    }

    void pop_front() {
        erase(cbegin());
    }

    ~List() {
        if (list_size == 0) {
            return;
        }
        while (list_size) {
            pop_back();
        }
        //allocTraits::deallocate(list_allocator, fakeNode, 1);
    }
};
