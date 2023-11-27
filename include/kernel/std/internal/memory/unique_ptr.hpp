#pragma once

#include <cassert>
#include <type_traits>
#include <utility>

#include "memory/allocation/allocator.hpp"

namespace std {

template <typename T>
class unique_ptr {
   public:
    /**
     * Create a unique pointer that doesn't own a resource.
     */
    explicit unique_ptr();

    /**
     * Take ownership of a dynamically allocated pointer.
     * @param ptr The pointer to take ownership of.
     */
    explicit unique_ptr(allocator* allocator, T* ptr);

    /**
     * Unique pointers can't be copied.
     */
    unique_ptr(const unique_ptr<T>&) = delete;

    /**
     * Move a unique pointer into self.
     * @param other The pointer to move from.
     */
    unique_ptr(unique_ptr<T>&& other);

    /**
     * Relase the owned pointer.
     */
    ~unique_ptr();

    /**
     * Move a unique pointer into self.
     * @param other The pointer to move from.
     * @return A reference to self.
     */
    unique_ptr& operator=(unique_ptr&& other);

    /**
     * Release ownership of the pointer. This means it will not be deleted when
     * the liftime of the unique pointer ends.
     * @return The no-longer-owned pointer.
     */
    T* release();

    /**
     * Replace the owned pointer with a new one. The old pointer is deleted.
     * @param t The new pointer.
     */
    void reset(allocator* allocator = nullptr, T* t = nullptr);

    /**
     * Replace the pointers owned with another unique pointer.
     * @param other The other unique pointer.
     */
    void swap(unique_ptr& other);

    /**
     * Return the owned pointer.
     * @return The owned pointer.
     */
    T* get() const;

    /**
     * Equivalent to checking if the pointer is not equal to nullptr.
     * @return true iff the pointer is not equal to nullptr.
     */
    operator bool() const;

    /**
     * Dereference the pointer. Panic if pointer is nullptr.
     * @return A reference to the underlying value.
     */
    add_lvalue_reference_t<T> operator*() const;

    /**
     * Dereference the pointer. Panic if pointer is nullptr.
     * @return A reference to the underlying value.
     */
    T* operator->() const;

    /**
     * Implicit conversion of a unique pointer to derived class to a unique
     * pointer to a base class.
     *
     * @return A unique pointer to the base class that points to the same
     * object.
     */
    template <typename Base>
    requires std::derived_from<T, Base>
    operator unique_ptr<Base>();

   private:
    T* ptr_;
    allocator* allocator_;
};

template <typename T>
unique_ptr<T>::unique_ptr() : ptr_(nullptr), allocator_(nullptr) {}

template <typename T>
unique_ptr<T>::unique_ptr(allocator* allocator, T* ptr)
    : ptr_(ptr), allocator_(allocator) {}

template <typename T>
unique_ptr<T>::unique_ptr(unique_ptr<T>&& other)
    : ptr_(move(other.ptr_)), allocator_(move(other.allocator_)) {
    other.ptr_ = nullptr;
}

template <typename T>
unique_ptr<T>::~unique_ptr() {
    reset();
}

template <typename T>
unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr<T>&& other) {
    if (&other == this) {
        return *this;
    }

    reset();
    ptr_ = other.release();

    return *this;
}

template <typename T>
T* unique_ptr<T>::release() {
    T* result = ptr_;
    ptr_ = nullptr;
    return result;
}

template <typename T>
void unique_ptr<T>::reset(allocator* allocator, T* t) {
    if (ptr_ != nullptr) {
        free(allocator_, ptr_);
    }
    ptr_ = t;
    allocator_ = allocator;
}

template <typename T>
void unique_ptr<T>::swap(unique_ptr<T>& other) {
    T* const temp = ptr_;
    ptr_ = other.ptr_;
    other.ptr_ = temp;

    allocator* const temp_allocator = allocator_;
    allocator_ = other.allocator_;
    other.allocator_ = temp_allocator;
}

template <typename T>
T* unique_ptr<T>::get() const {
    return ptr_;
}

template <typename T>
unique_ptr<T>::operator bool() const {
    return ptr_ != nullptr;
}

template <typename T>
add_lvalue_reference_t<T> unique_ptr<T>::operator*() const {
    assertm(ptr_ != nullptr, "Tried to dereference a null pointer");
    return *ptr_;
}

template <typename T>
T* unique_ptr<T>::operator->() const {
    assertm(ptr_ != nullptr, "Tried to dereference a null pointer");
    return ptr_;
}

template <typename T>
template <typename Base>
requires std::derived_from<T, Base> unique_ptr<T>::operator unique_ptr<Base>() {
    Base* const new_ptr = reinterpret_cast<Base*>(ptr_);
    ptr_ = nullptr;
    return unique_ptr<Base>(allocator_, new_ptr);
}

/**
 * Check if a pointer is equal to another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the pointers are equal.
 */
template <typename T1, typename T2>
bool operator==(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs);

/**
 * Check if a pointer is equal to another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the pointers are equal.
 */
template <typename T1, typename T2>
bool operator==(const unique_ptr<T1>& lhs, const T2* rhs);

/**
 * Check if a pointer is equal to another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the pointers are equal.
 */
template <typename T1, typename T2>
bool operator==(const T1* lhs, const unique_ptr<T2>& rhs);

/**
 * Check if a pointer is not equal to another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the pointers are not equal.
 */
template <typename T1, typename T2>
bool operator!=(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs);

/**
 * Check if a pointer is not equal to another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the pointers are not equal.
 */
template <typename T1, typename T2>
bool operator!=(const unique_ptr<T1>& lhs, const T2* rhs);

/**
 * Check if a pointer is not equal to another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the pointers are not equal.
 */
template <typename T1, typename T2>
bool operator!=(const T1* lhs, const unique_ptr<T2>& rhs);

/**
 * Check if a pointer is smaller than another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the first pointer is smaller than the second pointer.
 */
template <typename T1, typename T2>
bool operator<(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs);

/**
 * Check if a pointer is smaller than another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the first pointer is smaller than the second pointer.
 */
template <typename T1, typename T2>
bool operator<(const unique_ptr<T1>& lhs, const T2* rhs);

/**
 * Check if a pointer is smaller than another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the first pointer is smaller than the second pointer.
 */
template <typename T1, typename T2>
bool operator<(const T1* lhs, const unique_ptr<T2>& rhs);

/**
 * Check if a pointer is smaller or equal to another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the first pointer is smaller or equal to the second pointer.
 */
template <typename T1, typename T2>
bool operator<=(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs);

/**
 * Check if a pointer is smaller or equal to another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the first pointer is smaller or equal to the second pointer.
 */
template <typename T1, typename T2>
bool operator<=(const unique_ptr<T1>& lhs, const T2* rhs);

/**
 * Check if a pointer is smaller or equal to another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the first pointer is smaller or equal to the second pointer.
 */
template <typename T1, typename T2>
bool operator<=(const T1* lhs, const unique_ptr<T2>& rhs);

/**
 * Check if a pointer is larger than another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the first pointer is larger than the second pointer.
 */
template <typename T1, typename T2>
bool operator>(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs);

/**
 * Check if a pointer is larger than another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the first pointer is larger than the second pointer.
 */
template <typename T1, typename T2>
bool operator>(const unique_ptr<T1>& lhs, const T2* rhs);

/**
 * Check if a pointer is larger than another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the first pointer is larger than the second pointer.
 */
template <typename T1, typename T2>
bool operator>(const T1* lhs, const unique_ptr<T2>& rhs);

/**
 * Check if a pointer is larger or equal to another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the first pointer is larger or equal to the second pointer.
 */
template <typename T1, typename T2>
bool operator>=(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs);

/**
 * Check if a pointer is larger or equal to another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the first pointer is larger or equal to the second pointer.
 */
template <typename T1, typename T2>
bool operator>=(const unique_ptr<T1>& lhs, const T2* rhs);

/**
 * Check if a pointer is larger or equal to another pointer.
 * @param lhs The first pointer.
 * @param rhs The second pointer.
 * @return True iff the first pointer is larger or equal to the second pointer.
 */
template <typename T1, typename T2>
bool operator>=(const T1* lhs, const unique_ptr<T2>& rhs);

template <typename T1, typename T2>
bool operator==(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs) {
    return lhs.get() == rhs.get();
}

template <typename T1, typename T2>
bool operator==(const unique_ptr<T1>& lhs, const T2* rhs) {
    return lhs.get() == rhs;
}

template <typename T1, typename T2>
bool operator==(const T1* lhs, const unique_ptr<T2>& rhs) {
    return lhs == rhs.get();
}

template <typename T1, typename T2>
bool operator!=(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs) {
    return lhs.get() != rhs.get();
}

template <typename T1, typename T2>
bool operator!=(const unique_ptr<T1>& lhs, const T2* rhs) {
    return lhs.get() != rhs;
}

template <typename T1, typename T2>
bool operator!=(const T1* lhs, const unique_ptr<T2>& rhs) {
    return lhs != rhs.get();
}

template <typename T1, typename T2>
bool operator<(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs) {
    return lhs.get() < rhs.get();
}

template <typename T1, typename T2>
bool operator<(const unique_ptr<T1>& lhs, const T2* rhs) {
    return lhs.get() < rhs;
}

template <typename T1, typename T2>
bool operator<(const T1* lhs, const unique_ptr<T2>& rhs) {
    return lhs < rhs.get();
}

template <typename T1, typename T2>
bool operator<=(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs) {
    return lhs.get() <= rhs.get();
}

template <typename T1, typename T2>
bool operator<=(const unique_ptr<T1>& lhs, const T2* rhs) {
    return lhs.get() <= rhs;
}

template <typename T1, typename T2>
bool operator<=(const T1* lhs, const unique_ptr<T2>& rhs) {
    return lhs <= rhs.get();
}

template <typename T1, typename T2>
bool operator>(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs) {
    return lhs.get() > rhs.get();
}

template <typename T1, typename T2>
bool operator>(const unique_ptr<T1>& lhs, const T2* rhs) {
    return lhs.get() > rhs;
}

template <typename T1, typename T2>
bool operator>(const T1* lhs, const unique_ptr<T2>& rhs) {
    return lhs > rhs.get();
}

template <typename T1, typename T2>
bool operator>=(const unique_ptr<T1>& lhs, const unique_ptr<T2>& rhs) {
    return lhs.get() >= rhs.get();
}

template <typename T1, typename T2>
bool operator>=(const unique_ptr<T1>& lhs, const T2* rhs) {
    return lhs.get() >= rhs;
}

template <typename T1, typename T2>
bool operator>=(const T1* lhs, const unique_ptr<T2>& rhs) {
    return lhs >= rhs.get();
}

}  // namespace std
