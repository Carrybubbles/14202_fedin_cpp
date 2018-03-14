#ifndef XORLIST_H
#define XORLIST_H

#include <cstddef>
#include <memory>
#include <initializer_list>
#include <cstdint>

template <class T>
struct Node{
    T value;
    intptr_t ptr;
    Node(T value_){
        value = value_;
        ptr = reinterpret_cast<intptr_t>(nullptr);
    }
};
template < class T, class TAllocator = std::allocator<T> >
class LinkedList;
template <class T, class TAllocator>
class LinkedList {
public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using rebind_alloc = typename std::allocator_traits<TAllocator>::template rebind_alloc<Node<T>>;
    using allocator_type = rebind_alloc;
    using size_type = std::size_t;
public:

    void print(){
        Node<T>* current = head_;
        Node<T>* prev_node = nullptr;
        while(current){
            std::cout << current->value << std::endl;
            Node<T>* next_node = xor_func(prev_node,reinterpret_cast<Node<T>*>(current->ptr));
            prev_node = current;
            current = next_node;
        }
    }

    LinkedList(){
        tail_ = head_ = nullptr;
        size_= 0;

    }

    explicit LinkedList(const allocator_type& alloc);

    LinkedList(std::initializer_list<value_type> il, const allocator_type& alloc);

    explicit LinkedList(const std::size_t n, const allocator_type& alloc = allocator_type());

    LinkedList(const std::size_t n, const_reference val, const allocator_type& alloc = allocator_type());

    LinkedList(const LinkedList<T, TAllocator>& other);
    LinkedList(LinkedList<T, TAllocator>&& other);

    virtual ~LinkedList(){}

    LinkedList& operator=(const LinkedList<T, TAllocator>& right);
    LinkedList& operator=(LinkedList<T, TAllocator>&& right);

    void swap(LinkedList<T, TAllocator>& other);

    void push_back(const_reference data);
    void push_back(T&& data);

    void push_front(const_reference data);
    void push_front(T&& data);

    template <class K>
    void emplace_back(K&& data);

    template <class K>
    void emplace_front(K&& data);

    void pop_front();
    void pop_back();

    size_type size() const noexcept;
    bool empty() const noexcept;

    void clear();

    T& back() noexcept;
    const T& back() const noexcept;

    T& front() noexcept;
    const T& front() const noexcept;

    // Iterators and such
//    iterator begin() noexcept;
//    iterator end() noexcept;
//    const_iterator begin() const noexcept;
//    const_iterator end() const noexcept;
//    const_iterator cbegin() const noexcept;
//    const_iterator cend() const noexcept;

    void sort() noexcept;

    template <class Compare>
    void sort(Compare comp) noexcept;

//    iterator insert(const_iterator position, const_reference val);

//    template <class InputIterator>
//    iterator insert(const_iterator position, InputIterator first, InputIterator last);

//    void reverse() noexcept;

//    iterator erase(const_iterator position);
//    iterator erase(const_iterator first, const_iterator last);

    void resize(size_type n);
    void resize(size_type n, const_reference val);

    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);

    void assign(size_type n, const_reference val);
    void assign(std::initializer_list<T> il);

//    void splice(const_iterator position, LinkedList& x) noexcept;
//    void splice(const_iterator position, LinkedList& x, const_iterator i) noexcept;
//    void splice(const_iterator position, LinkedList& x, const_iterator first, const_iterator last) noexcept;

    template <class BinaryPredicate>
    void unique(BinaryPredicate binary_pred);
    void unique();

    template <class Compare>
    void merge(LinkedList& x, Compare comp) noexcept;
    void merge(LinkedList& x) noexcept;

    void insert_after(Node<T>* const pos, Node<T> * const insert_node);
    void insert_before(Node<T>* const pos, Node<T> * const insert_node);
    Node<T>* head_;
    Node<T>* tail_;
    size_type size_;
    allocator_type allocator_;
    void insert_node_into_tail(Node<T>* const insert_node);
    void insert_node_into_head(Node<T> * const insert_node);
private:

private:
    Node<T>* create_node(const_reference value);
    bool is_empty_head();
    Node<T>* xor_func(Node<T>* const prev_node, Node<T>* const current_node);

//    void insert_node_into_tail(Node<T>* const insert_node);
//    void insert_node_into_head(Node<T> * const insert_node);
    void unlink(Node<T>* const node);
    Node<T>* find_previous(Node<T> * const node);
};

template <class T, class TAllocator>
LinkedList<T,TAllocator>::LinkedList(LinkedList<T, TAllocator> &&other):
    head_(other.head_),
    tail_(other.tail_),
    size_(other.size_),
    allocator_(std::move(other.allocator_))
{
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = nullptr;
}

template <class T, class TAllocator>
LinkedList<T,TAllocator>::LinkedList(const LinkedList<T, TAllocator> &other):
    head_(other.head_),
    tail_(other.tail_),
    size_(other.size_),
    allocator_(other.allocator_)
{}

template<class T, class TAllocator>
Node<T>* LinkedList<T,TAllocator>::create_node(const_reference value){
    Node<T> *node = allocator_.allocate(1);
    allocator_.construct(node);
    node->value = value;
    return node;
}

template<class T, class TAllocator>
bool LinkedList<T,TAllocator>::is_empty_head(){
    return head_ == nullptr;
}



template<class T, class TAllocator>
Node<T>* LinkedList<T,TAllocator>::xor_func(Node<T>* const node1, Node<T>* const node2){
    return reinterpret_cast<Node<T>*>(reinterpret_cast<intptr_t>(node1) ^ reinterpret_cast<intptr_t>(node2));

} 

template<class T, class TAllocator>
void LinkedList<T,TAllocator>::insert_after(Node<T> * const pos, Node<T> * const insert_node){
    if(nullptr == pos){
        insert_node_into_head(insert_node);
    }else if(tail_ == pos){
        insert_node_into_tail(insert_node);
    }else{
        Node<T>* prev = find_previous(pos);
        Node<T>* next_node = xor_func(prev,pos);
        if(nullptr == next_node){
            insert_node_into_tail(insert_node);
        }else{
            next_node->ptr ^= reinterpret_cast<intptr_t>(pos) ^ reinterpret_cast<intptr_t>(insert_node);
            insert_node->ptr = reinterpret_cast<intptr_t>(prev) ^  reinterpret_cast<intptr_t>(next_node);
            pos->ptr = reinterpret_cast<intptr_t>(prev) ^ reinterpret_cast<intptr_t>(insert_node);
        }
    }
}

template<class T, class TAllocator>
void LinkedList<T,TAllocator>::insert_before(Node<T> * const pos, Node<T> * const insert_node){
    if(pos != nullptr){
        Node<T>* prev_node = find_previous(pos);
        if(nullptr == prev_node){
            insert_after(prev_node,insert_node);
        }else{
            insert_node_into_tail(insert_node);
        }
    }else if(head_ == nullptr){
        insert_node_into_head(insert_node);
    }

}


template<class T, class TAllocator>
void LinkedList<T,TAllocator>::insert_node_into_tail(Node<T> * const insert_node){
    if(head_ == nullptr){
        insert_node->ptr = reinterpret_cast<intptr_t>(xor_func(nullptr,head_));
        head_ = insert_node;
        tail_ = insert_node;
    }else{
         insert_node->ptr = reinterpret_cast<intptr_t>(xor_func(nullptr, tail_));
         tail_->ptr = reinterpret_cast<intptr_t>(xor_func(reinterpret_cast<Node<T>*>(tail_->ptr), insert_node));
         tail_ = insert_node;
    }
}

template<class T, class TAllocator>
void LinkedList<T,TAllocator>::insert_node_into_head(Node<T> * const insert_node){
    std::swap(tail_,head_);
    insert_node_into_tail(insert_node);
    std::swap(tail_, head_);
}


template<class T, class TAllocator>
Node<T>* LinkedList<T,TAllocator>::find_previous(Node<T>* const node){
    Node<T>* current = head_;
    Node<T>* prev_node = nullptr;
    while(current != node){
        auto temp = current;
        Node<T>* current = xor_func(prev_node,current);
        prev_node = temp;
    }
    return prev_node;
}












#endif // XORLIST_H
