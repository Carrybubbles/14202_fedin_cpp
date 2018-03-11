#ifndef XORLIST_H
#define XORLIST_H

#include <cstddef>
#include <memory>
#include <initializer_list>

template <class T, class TAllocator>
class LinkedList {
public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using rebind_alloc = std::allocator_traits<TAllocator>::template rebind_alloc<Node<T>>;
    using allocator_type = rebind_alloc;
    using size_type = std::size_t;
public:

    LinkedList();

    explicit LinkedList(const allocator_type& alloc);

    LinkedList(std::initializer_list<value_type> il, const allocator_type& alloc);

    explicit LinkedList(const std::size_t n, const allocator_type& alloc = allocator_type());

    LinkedList(const std::size_t n, const_reference val, const allocator_type& alloc = allocator_type());

    LinkedList(const LinkedList<T, TAllocator>& other);
    LinkedList(LinkedList<T, TAllocator>&& other);

    virtual ~LinkedList();

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
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    void sort() noexcept;

    template <class Compare>
    void sort(Compare comp) noexcept;

    iterator insert(const_iterator position, const_reference val);

    template <class InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last);

    void reverse() noexcept;

    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);

    void resize(size_type n);
    void resize(size_type n, const_reference val);

    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);

    void assign(size_type n, const_reference val);
    void assign(std::initializer_list<T> il);

    void splice(const_iterator position, LinkedList& x) noexcept;
    void splice(const_iterator position, LinkedList& x, const_iterator i) noexcept;
    void splice(const_iterator position, LinkedList& x, const_iterator first, const_iterator last) noexcept;

    template <class BinaryPredicate>
    void unique(BinaryPredicate binary_pred);
    void unique();

    template <class Compare>
    void merge(LinkedList& x, Compare comp) noexcept;
    void merge(LinkedList& x) noexcept;
private:

    struct Node{
        T value;
        Node* ptr;
    };
    Node<T>* head_;
    Node<T>* tail_;
    size_type size_;
    allocator_type allocator_;
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




#endif // XORLIST_H
