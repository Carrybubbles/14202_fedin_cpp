#ifndef XORLIST_H
#define XORLIST_H

#include <cstddef>
#include <memory>
#include <initializer_list>
#include <cstdint>

namespace xlist {

template < class T, class TAllocator = std::allocator<T> >
class LinkedList;

template <class T>
struct Node{
    T value;
    intptr_t ptr;
    Node(T value_){
        value = value_;
        ptr = 0;
    }
    Node(){}
};

template <typename T>
class xor_list_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
public:
    using value_type = T ;
    using reference = T& ;
    using pointer = T* ;
    using pointer_node_list = Node<T>*;
    using iterator_category = std::bidirectional_iterator_tag;
public:
    xor_list_iterator(): first_(nullptr), second_(nullptr){}
    xor_list_iterator(pointer_node_list first, pointer_node_list second): first_(first), second_(second){}
    xor_list_iterator(const xor_list_iterator & other):first_(other.first_), second_(other.second_){}

    virtual ~xor_list_iterator(){
        first_ = second_ =nullptr;
    }

    xor_list_iterator& operator++()
    {
        if(second_){
            pointer_node_list next_node = xor_func(first_,reinterpret_cast<pointer_node_list>(second_->ptr));
            first_ = second_;
            second_ = next_node;
        }
        return *this;
    }
    xor_list_iterator& operator++(int) {
        xor_list_iterator tmp(*this);
        operator++();
        return tmp;
    }
    xor_list_iterator& operator--()
    {
        if(first_){
            pointer_node_list next_node = xor_func(second_,reinterpret_cast<pointer_node_list>(first_->ptr));
            second_ = first_;
            first_ = next_node;
        }
        return *this;
    }
    xor_list_iterator& operator--(int) {
        xor_list_iterator tmp(*this);
        operator--();
        return tmp;
    }
    bool operator==(const xor_list_iterator& rhs) {return first_ == rhs.first_ && second_ == rhs.second_;}
    bool operator!=(const xor_list_iterator& rhs) {return !operator==(rhs);}
    reference operator*() {return second_->value;}
    pointer operator->() {return &second_->value;}
    pointer_node_list first_;
    pointer_node_list second_;
private:

    pointer_node_list xor_func(pointer_node_list const node1, pointer_node_list const node2){
        return reinterpret_cast<pointer_node_list>(reinterpret_cast<intptr_t>(node1) ^ reinterpret_cast<intptr_t>(node2));

    }
};

template <typename T>
class xor_list_const_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
public:
    using value_type = T ;
    using const_reference = const T& ;
    using const_pointer = T* ;
    using pointer_node_list = Node<T>*;
    using iterator_category = std::bidirectional_iterator_tag;
public:
    xor_list_const_iterator(): first_(nullptr), second_(nullptr){}
    xor_list_const_iterator(pointer_node_list first, pointer_node_list second): first_(first), second_(second){}
    xor_list_const_iterator(const xor_list_const_iterator& other):first_(other.first_), second_(other.second_){}
    virtual ~xor_list_const_iterator(){
        first_ = second_ = nullptr;
    }

    xor_list_const_iterator& operator++()
    {
        if(second_){
            pointer_node_list next_node = xor_func(first_,reinterpret_cast<pointer_node_list>(second_->ptr));
            first_ = second_;
            second_ = next_node;
        }
        return *this;
    }
    xor_list_const_iterator& operator++(int) {
       xor_list_const_iterator tmp(*this);
        operator++();
        return tmp;
    }
    xor_list_const_iterator& operator--()
    {
        if(first_){
            pointer_node_list next_node = xor_func(second_,reinterpret_cast<pointer_node_list>(first_->ptr));
            second_ = first_;
            first_ = next_node;
        }
        return *this;
    }
    xor_list_const_iterator& operator--(int) {
        xor_list_const_iterator tmp(*this);
        operator--();
        return tmp;
    }
    bool operator==(const xor_list_const_iterator& rhs) {return first_ == rhs.first_ && second_ == rhs.second_;}
    bool operator!=(const xor_list_const_iterator& rhs) {return !operator ==(rhs);}
    const_reference operator*() {return second_->value;}
    const_pointer operator->() {return &second_->value;}
    pointer_node_list first_;
    pointer_node_list second_;
private:

    pointer_node_list xor_func(pointer_node_list const node1, pointer_node_list const node2){
        return reinterpret_cast<pointer_node_list>(reinterpret_cast<intptr_t>(node1) ^ reinterpret_cast<intptr_t>(node2));

    }
};

template <class T, class TAllocator>
class LinkedList {
public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using rebind_alloc = typename std::allocator_traits<TAllocator>::template rebind_alloc<Node<T>>;
    using allocator_type = rebind_alloc;
    using size_type = std::size_t;
    using iterator = xor_list_iterator<T>;
    using const_iterator = xor_list_const_iterator<T>;
    using pointer_node_list = Node<T>*;
public:

    void print(){
        pointer_node_list current = head_;
        pointer_node_list prev_node = nullptr;
        while(current){
            std::cout << current->value << std::endl;
            pointer_node_list next_node = xor_func(prev_node,reinterpret_cast<pointer_node_list>(current->ptr));
            prev_node = current;
            current = next_node;
        }
    }

    LinkedList(): head_(nullptr), tail_(nullptr), size_(0),allocator_(allocator_type()) {}

    explicit LinkedList(const allocator_type& alloc): head_(nullptr),tail_(nullptr), size_(0),allocator_(alloc) {}

    LinkedList(std::initializer_list<value_type> il, const allocator_type& alloc): allocator_(alloc){
        typename std::initializer_list<value_type>::iterator it;
        for (it=il.begin(); it!=il.end(); ++it){
            push_back(*it);
        }
    }

    explicit LinkedList(const std::size_t n, const allocator_type& alloc = allocator_type()) : allocator_(alloc){
        for(size_type i= 0; i < n; ++i){
            push_back(0);
        }
    }

    LinkedList(const std::size_t n, const_reference val, const allocator_type& alloc = allocator_type()) : allocator_(alloc){
        for(size_type i= 0; i < n; ++i){
            push_back(val);
        }
    }

    LinkedList(const LinkedList<T, TAllocator>& other):
        head_(other.head_),
        tail_(other.tail_),
        size_(other.size_),
        allocator_(other.allocator_)
    {}

    LinkedList(LinkedList<T, TAllocator>&& other):
        head_(other.head_),
        tail_(other.tail_),
        size_(other.size_),
        allocator_(std::move(other.allocator_))
    {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = nullptr;
    }

    virtual ~LinkedList(){clear();}

    LinkedList& operator=(const LinkedList<T, TAllocator>& right){
        if(&right != this){
            clear();
            size_ = right.size_;
            allocator_ = right.allocator_;
            head_ = right.head_;
            tail_ = right.tail_;
            right.head_ = nullptr;
            right.tail_ = nullptr;
        }
        return *this;
    }
    LinkedList& operator=(LinkedList<T, TAllocator>&& right){
        if(&right != this){
            clear();
            size_ =      std::move(right.size_);
            allocator_ = std::move(right.allocator_);
            head_ =      std::move(right.head_);
            tail_ =      std::move(right.tail_);
        }
        return *this;
    }

    void swap(LinkedList<T, TAllocator>& other){
        std::swap(size_,other.size_);
        std::swap(allocator_, other.allocator_);
        std::swap(head_, other.head_);
        std::swap(tail_,other.tail_);
    }

    void push_back(const_reference data){
        auto node = create_node(data);
        insert_into_tail(node);
    }
    void push_back(T&& data){
        auto node = create_node(data);
        insert_into_tail(node);
    }

    void push_front(const_reference data){
        auto node = create_node(data);
        insert_into_head(node);
    }
    void push_front(T&& data){
        auto node = create_node(data);
        insert_into_head(node);
    }

    template <class K>
    void emplace_back(K&& data){
        auto node = create_node(std::forward(data));
        insert_into_tail(std::forward(node));
    }

    template <class K>
    void emplace_front(K&&  data){
        auto node = create_node(std::forward(data));
//        node->value = std::forward(data);
        insert_into_head(node);
    }

    void pop_front(){
        delete_node(pop(head_));
    }
    void pop_back(){
        delete_node(pop(tail_));
    }

    size_type size() const noexcept{
        return size_;
    }
    bool empty() const noexcept{
        return size_ == 0;
    }

    void clear(){
        clear_list();
    }

    T& back() noexcept{
        return tail_->value;
    }
    const T& back() const noexcept{
        return tail_->value;
    }

    T& front() noexcept{
        return head_->value;
    }
    const T& front() const noexcept{
        return head_->value;
    }


    // Iterators and such
    iterator begin() noexcept{
        return iterator(nullptr,head_);
    }
    iterator end() noexcept{
        return iterator(tail_,nullptr);
    }
    const_iterator begin() const noexcept{
        return const_iterator(nullptr,head_);
    }
    const_iterator end() const noexcept{
        return const_iterator(tail_,nullptr);
    }
    const_iterator cbegin() const noexcept{
        return const_iterator(nullptr,head_);
    }
    const_iterator cend() const noexcept{
        return const_iterator(tail_,nullptr);
    }

    void sort() noexcept;

    template <class Compare>
    void sort(Compare comp) noexcept{

    }

    iterator insert(const_iterator position, const_reference val){
        auto node = create_node(val);
        insert_before(position.second_,node);
        return iterator(position.first_, position.second_);
    }

    template <class InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last){
        for(auto it = first; it != last; it++){
            auto node = create_node(*it);
            insert_before(position.second_, node);
        }
        return iterator(position.first_, position.second_);
    }

    void reverse() noexcept{
        std::swap(head_,tail_);
    }

    iterator erase(const_iterator position){

    }
    iterator erase(const_iterator first, const_iterator last){

    }

    void resize(size_type n){
        resize(n,0);
    }

    void resize(size_type n, const_reference val){
        if(n > size_){
            push_back(val);
        }else{
            size_type pop_size = size_ - n;
            for(size_type i = 0; i < pop_size; i++){
                pop_back();
            }
        }
    }

    template <class InputIterator>
    void assign(InputIterator first, InputIterator last){
        clear();
        for(InputIterator it = first; it != last; ++it){
            push_back(*it);
        }
    }

    void assign(size_type n, const_reference val){
        clear();
        for(size_type i = 0; i < n; i++){
            push_back(val);
        }
    }

    void assign(std::initializer_list<value_type> il){
        clear();
        typename std::initializer_list<value_type>::iterator it;
        for(it = il.begin(); it != il.end(); ++it){
            push_back(*it);
        }
    }

//    void splice(const_iterator position, LinkedList& x) noexcept;
//    void splice(const_iterator position, LinkedList& x, const_iterator i) noexcept;
//    void splice(const_iterator position, LinkedList& x, const_iterator first, const_iterator last) noexcept;

    template <class BinaryPredicate>
    iterator unique(BinaryPredicate binary_pred){
        auto first = begin();
        auto last = end();
        if (first == last){
            return last;
        }

        auto result = first;
        while (++first != last) {
            if (!binary_pred(*result, *first)) {
                *(++result) = *first;
            }
        }
        return ++result;
    }
    void unique(){
        unique([](const_reference a, const_reference b){
            return a == b;
        });
    }

    template <class Compare>
    void merge(LinkedList& x, Compare comp) noexcept;
    void merge(LinkedList& x) noexcept;


private:
    pointer_node_list head_;
    pointer_node_list tail_;
    size_type size_;
    allocator_type allocator_;
    pointer_node_list unlink(pointer_node_list const node);
private:
    void insert_after(pointer_node_list const pos, Node<T> * const insert_node){
        if(is_empty_head() || nullptr == pos){
            insert_into_head(insert_node);
        }else if(tail_ == pos){
            insert_into_tail(insert_node);
        }else{
            pointer_node_list prev = find_previous(pos);
            pointer_node_list next_node = xor_func(prev,reinterpret_cast<pointer_node_list>(pos->ptr));
            if(nullptr == next_node){
                insert_into_tail(insert_node);
            }else{
                next_node->ptr ^= reinterpret_cast<intptr_t>(pos) ^ reinterpret_cast<intptr_t>(insert_node);
                insert_node->ptr = reinterpret_cast<intptr_t>(pos) ^  reinterpret_cast<intptr_t>(next_node);
                pos->ptr ^= reinterpret_cast<intptr_t>(next_node) ^ reinterpret_cast<intptr_t>(insert_node);
                size_++;
            }
        }
    }

    void insert_before(pointer_node_list const pos, Node<T> * const insert_node){
        std::swap(head_,tail_);
        insert_after(pos,insert_node);
        std::swap(head_,tail_);
    }
    pointer_node_list create_node(const_reference value){
        Node<T> *node = allocator_.allocate(1);
        allocator_.construct(node,value);
        return node;
    }
    pointer_node_list create_node(T&& value){
        Node<T> *node = allocator_.allocate(1);
        allocator_.construct(node,std::move(value));
        return node;
    }
    bool is_empty_head(){
        return head_ == nullptr;
    }
    pointer_node_list xor_func(pointer_node_list const node1, pointer_node_list const node2){
        return reinterpret_cast<pointer_node_list>(reinterpret_cast<intptr_t>(node1) ^ reinterpret_cast<intptr_t>(node2));

    }

    void insert_into_tail(pointer_node_list const insert_node){
        if(is_empty_head()){
            insert_node->ptr = reinterpret_cast<intptr_t>(xor_func(nullptr,head_));
            head_ = insert_node;
            tail_ = insert_node;
        }else{
             insert_node->ptr = reinterpret_cast<intptr_t>(xor_func(nullptr, tail_));
             tail_->ptr = reinterpret_cast<intptr_t>(xor_func(reinterpret_cast<pointer_node_list>(tail_->ptr), insert_node));
             tail_ = insert_node;
        }
        size_++;
    }
    void insert_into_head(Node<T> * const insert_node){
        std::swap(tail_,head_);
        insert_into_tail(insert_node);
        std::swap(tail_, head_);
    }
    void clear_list(){
        pointer_node_list current = head_;
        pointer_node_list prev_node = nullptr;
        while(current){
            pointer_node_list next_node = xor_func(prev_node,reinterpret_cast<pointer_node_list>(current->ptr));
            prev_node = current;
            current = next_node;
            delete_node(prev_node);
        }
        size_ = 0;
        head_ = tail_ = nullptr;
    }
    pointer_node_list find_previous(Node<T> * const node){
        pointer_node_list current = head_;
        pointer_node_list prev_node = nullptr;
        while(current != node){
            auto temp = current;
            current = xor_func(prev_node,reinterpret_cast<pointer_node_list>(current->ptr));
            prev_node = temp;
        }
        return prev_node;
    }
    pointer_node_list pop(pointer_node_list const node){
        auto prev_node = find_previous(node);
        pointer_node_list next_node = xor_func(prev_node,reinterpret_cast<pointer_node_list>(node->ptr));
        //tail
        if(nullptr == prev_node){
            head_ = next_node;
            next_node->ptr ^=  reinterpret_cast<intptr_t>(node);
        }
        //head
        else if(nullptr == next_node){
            tail_ = prev_node;
            tail_->ptr ^= reinterpret_cast<intptr_t>(node);
        }
        return node;
    }

    void delete_node(pointer_node_list const node){
        allocator_.destroy(node);
        allocator_.deallocate(node,1);
        size_--;
    }
};

}//xlist













#endif // XORLIST_H
