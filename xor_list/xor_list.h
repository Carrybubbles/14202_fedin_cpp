#ifndef XORLIST_H
#define XORLIST_H

#include <cstddef>
#include <memory>
#include <initializer_list>
#include <cstdint>
#include <algorithm>

namespace xlist {

template < class T, class TAllocator = std::allocator<T> >
class LinkedList;

template <class T>
struct Node{
    T value;
    intptr_t ptr;
    Node(T&& value_):value(std::move(value_)), ptr(0){}
    Node(const T& value_) : value(value_), ptr(0){}
    Node(){}

};

template <typename T>
class xor_list_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
private:
    friend class LinkedList<T>;
public:
    using value_type = T ;
    using reference = T& ;
    using pointer = T* ;
    using pointer_node_list = Node<T>*;
    using const_pointer_node_list = const Node<T>*;

    using iterator_category = std::bidirectional_iterator_tag;
public:
    xor_list_iterator(): first_(nullptr), second_(nullptr){}
    xor_list_iterator(const xor_list_iterator* other): first_(other.first_), second_(other.second_){}
    xor_list_iterator(pointer_node_list first, pointer_node_list second): first_(first), second_(second){}
    xor_list_iterator(const xor_list_iterator & other):first_(other.first_), second_(other.second_){}
    xor_list_iterator& operator=(const xor_list_iterator& right){
        if(&right != this){
            first_ = right.first_;
            second_ = right.second_;
        }
        return *this;
    }

    ~xor_list_iterator(){}

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
        ++(*this);
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
        --(*this);
        return tmp;
    }
    bool operator==(const xor_list_iterator& rhs) {return first_ == rhs.first_ && second_ == rhs.second_;}
    bool operator!=(const xor_list_iterator& rhs) {return first_ != rhs.first_ || second_ != rhs.second_;}
    reference operator*() {return second_->value;}
    pointer operator->() {return &second_->value;}

private:

    pointer_node_list first_;
    pointer_node_list second_;

    pointer_node_list xor_func(pointer_node_list const node1, pointer_node_list const node2){
        return reinterpret_cast<pointer_node_list>(reinterpret_cast<intptr_t>(node1) ^ reinterpret_cast<intptr_t>(node2));
    }
};

template <typename T>
class xor_list_const_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
private:
    friend class LinkedList<T>;
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
    xor_list_const_iterator& operator=(const xor_list_const_iterator& right){
        if(&right != this){
            first_ = right.first_;
            second_ = right.second_;
        }
        return *this;
    }
    ~xor_list_const_iterator(){}
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
        ++(*this);
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
        --(*this);
        return tmp;
    }
    bool operator==(const xor_list_const_iterator& rhs) {return first_ == rhs.first_ && second_ == rhs.second_;}
    bool operator!=(const xor_list_const_iterator& rhs) {return first_ != rhs.first_ || second_ != rhs.second_;}

    const_reference operator*() {return second_->value;}
    const_pointer operator->() {return &second_->value;}

private:

    pointer_node_list first_;
    pointer_node_list second_;

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
    using const_pointer_node_list = Node<T>* const;
public:

    LinkedList(): head_(nullptr), tail_(nullptr), size_(0),allocator_(allocator_type()) {}

    explicit LinkedList(const allocator_type& alloc): head_(nullptr),tail_(nullptr), size_(0),allocator_(alloc) {}

    LinkedList(std::initializer_list<value_type> il, const allocator_type& alloc = allocator_type()): allocator_(alloc){
        std::for_each(il.begin(), il.end(), [this](const_reference n){ push_back(n); });
    }

    explicit LinkedList(const size_type n, const allocator_type& alloc = allocator_type()) : allocator_(alloc){
        std::fill_n(std::back_inserter(*this),n,0);
    }

    LinkedList(const size_type n, const_reference val, const allocator_type& alloc = allocator_type()) : allocator_(alloc){
        std::fill_n(std::back_inserter(*this),n,val);
    }

    LinkedList(const LinkedList<T, TAllocator>& other):allocator_(other.allocator_)
    {
        assign(other.cbegin(), other.cend());
    }

    LinkedList(LinkedList<T, TAllocator>&& other):
        head_(other.head_),
        tail_(other.tail_),
        size_(other.size_),
        allocator_(std::move(other.allocator_))
    {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    virtual ~LinkedList(){clear();}

    LinkedList& operator=(LinkedList<T, TAllocator>& right){
        if(&right != this){
            clear();
            size_ = right.size_;
            allocator_ = right.allocator_;
            head_ = right.head_;
            tail_ = right.tail_;
            right.tail_ = nullptr;
            right.head_ = nullptr;
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
            right.tail_ = nullptr;
            right.head_ = nullptr;
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
        emplace_back(data);
    }

    void push_back(T&& data){
        emplace_back(std::move(data));
    }

    void push_front(const_reference data){
        emplace_front(data);
    }
    void push_front(T&& data){
        emplace_front(std::move(data));
    }

    template <class K>
    void emplace_back(K&& data){
        auto node = create_node(std::forward<K>(data));
        insert_into_tail(node);
    }

    template <class K>
    void emplace_front(K&&  data){
        auto node = create_node(std::forward<K>(data));
        insert_into_head(node);
    }

    void pop_front(){
        if(!head_ && !tail_){
            return;
        }
        if(head_ == tail_){
            delete_node(head_);
            head_ = nullptr;
            tail_ = nullptr;
        }else{
            delete_node(pop(head_));
        }
    }
    void pop_back(){
        if(!head_ && !tail_){
            return;
        }
        if(head_ == tail_){
            delete_node(tail_);
            head_ = nullptr;
            tail_ = nullptr;
        }else{
            delete_node(pop(tail_));
        }
    }

    size_type size() const noexcept{
        return size_;
    }
    bool empty() const noexcept{
        return size_ == 0;
    }

    void clear(){
        if(!head_  && !tail_){
            return;
        }
        clear_list();
    }

    reference back() noexcept{
        return tail_->value;
    }
    const_reference back() const noexcept{
        return tail_->value;
    }

    reference front() noexcept{
        return head_->value;
    }
    const_reference front() const noexcept{
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

    iterator insert(const_iterator position, const_reference val){
        auto node = create_node(val);
        insert_before(position.second_,node);
        return iterator(node, position.second_);
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


    void resize(size_type n){
        resize(n,0);
    }

    void resize(size_type n, const_reference val){
        if(n > size_){
            size_type final_size = n - size_;            
            for(size_type i = 0; i < final_size; i++){
                push_back(val);
            }
        }else{
            size_type pop_size = size_ - n;
            for(size_type i = 0; i < pop_size; i++){
                pop_back();
            }
        }
    }


    void assign(size_type n, const_reference val){
        clear();
        for(size_type i = 0; i < n; i++){
            push_back(val);
        }
    }

    template <class InputIterator>
    void assign(InputIterator first,
                typename std::enable_if<std::is_class<InputIterator>::value, InputIterator>::type last){
        clear();

        for(InputIterator it = first; it != last; ++it){
            push_back(*it);
        }
    }

    void assign(std::initializer_list<value_type> il){
        clear();
        std::for_each(il.begin(), il.end(), [this](const_reference n){ push_back(n); });
    }

    template <class BinaryPredicate>
    void unique(BinaryPredicate binary_pred){
        auto first = begin();
        auto last = end();
        if (first == last){
            return;
        }
        auto result = first;
        while (++first != last) {
            if (!binary_pred(*result, *first)) {
                *(++result) = *first;
            }
        }
        ++result;
        erase(const_iterator(result.first_,result.second_),cend());
    }
    
    void unique(){
        unique([](const_reference a, const_reference b){
            return a == b;
        });
    }


    iterator erase(const_iterator position){
        auto first_position = position.first_;
        auto second_position = position.second_;
        auto next_node = ++position;
        if(!second_position){
            return end();
        }
        delete_node(remove_node_from_list(first_position,second_position,next_node.second_));
        --size_;
        return iterator(first_position,next_node.second_);
    }

    iterator erase(const_iterator first, const_iterator last){
        iterator temp = iterator(first.first_, first.second_);
        while(temp.second_ != last.second_){
            temp = erase(const_iterator(temp.first_ ,temp.second_));
        }
        return iterator();
    }


    void sort() noexcept{
        sort(std::greater<T>());
    }

    template <class Compare>
    void sort(Compare comp) noexcept{
       quick_sort(begin(),--end(),comp);
    }


    void splice(const_iterator position, LinkedList& x) noexcept{
        while(!x.empty()){
            auto node = create_node(std::move(x.front()));
            x.pop_front();
            insert_before(position.second_,node);
        }
    }
    
    void splice(const_iterator position, LinkedList& x, const_iterator i) noexcept{
        auto node = create_node(*i);
        x.erase(i);
        insert_before(position.second_,node);
    }
    
    void splice(const_iterator position, LinkedList& x, const_iterator first, const_iterator last) noexcept{
        for(auto it = first; it != last; ++it){
            auto node = create_node(*it);
            insert_before(position.second_,node);
        }
        x.erase(first,last);
    }

    template <class Compare>
    void merge(LinkedList& x, Compare comp) noexcept{
        if(x.empty() || this == &x){
           return;
        }
        auto it = cbegin();
        while(!x.empty()){
            if(it == cend()){
                splice(it,x);
                break;
            }
            if(comp(*it, x.front())){
                auto node = create_node(std::move(x.front()));
                insert_before(it.second_,node);
                x.pop_front();
                it = const_iterator(node,it.second_);
            }else{
                it++;

            }
        }
    }
    void merge(LinkedList& x) noexcept{
        merge(x, std::greater<T>());
    }


private:
    pointer_node_list head_ = nullptr;
    pointer_node_list tail_ = nullptr;
    size_type size_ = 0;
    allocator_type allocator_;
private:

    template<class Compare>
    void quick_sort(iterator begin, iterator end, Compare comp)
    {
        if(begin.second_ == nullptr || end.second_ == nullptr)
            return;
        if(begin == end)
            return;
        auto it = begin;
        auto temp = begin;
        auto it_next  = ++temp;
        while(it_next.second_ != nullptr)
        {
            if(comp(*begin,*it_next))
            {
                it++;
                std::swap(*it, *it_next);
            }
            it_next++;
        }
        std::swap(*it , *begin);
        quick_sort(begin, it,comp);
        quick_sort(++it, end,comp);
    }
    pointer_node_list remove_node_from_list(const_pointer_node_list prev, const_pointer_node_list cur,const_pointer_node_list next ){
        if(prev){
            prev->ptr ^= reinterpret_cast<intptr_t>(xor_func(next,cur));
        }
        if(next){
            next->ptr ^= reinterpret_cast<intptr_t>(xor_func(cur,prev));
        }

        if(cur == head_){
            head_ = next;
        }

        if(cur == tail_){
            tail_ = prev;
        }

        return cur;
    }

    void insert_after(pointer_node_list const pos, pointer_node_list const insert_node){
        if(is_empty_head() || nullptr == pos){
            insert_into_head(insert_node);
        }else if(tail_ == pos){
            insert_into_tail(insert_node);
        }else{
            pointer_node_list prev = find_previous(pos);
            pointer_node_list next_node = xor_func(prev,reinterpret_cast<pointer_node_list>(pos->ptr));
            next_node->ptr ^= reinterpret_cast<intptr_t>(pos) ^ reinterpret_cast<intptr_t>(insert_node);
            insert_node->ptr = reinterpret_cast<intptr_t>(pos) ^  reinterpret_cast<intptr_t>(next_node);
            pos->ptr ^= reinterpret_cast<intptr_t>(next_node) ^ reinterpret_cast<intptr_t>(insert_node);
            size_++;

        }
    }

    void insert_before(pointer_node_list const pos, pointer_node_list const insert_node){
        std::swap(head_,tail_);
        insert_after(pos,insert_node);
        std::swap(head_,tail_);
    }
    pointer_node_list create_node(const_reference value){

        pointer_node_list node = allocator_.allocate(1);
        allocator_.construct(node,value);

        return node;
    }

    pointer_node_list create_node(T&& value){
        pointer_node_list node = allocator_.allocate(1);
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
        erase(cbegin(), cend());
    }
    
    pointer_node_list find_previous(pointer_node_list const node){
        pointer_node_list current = head_;
        pointer_node_list prev_node = nullptr;
        while(current != node){
            pointer_node_list temp = current;
            current = xor_func(prev_node,reinterpret_cast<pointer_node_list>(current->ptr));
            prev_node = temp;
        }
        return prev_node;
    }
    
    pointer_node_list pop(pointer_node_list const node){
        auto prev_node = find_previous(node);
        pointer_node_list next_node = xor_func(prev_node,reinterpret_cast<pointer_node_list>(node->ptr));
        //tail
        if(!prev_node && next_node){
            head_ = next_node;
            next_node->ptr ^=  reinterpret_cast<intptr_t>(node);
        }
        //head
        else if(!next_node && prev_node){
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
