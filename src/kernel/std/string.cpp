#include <cassert>
#include <cstring>
#include <string>
#include <utility>

namespace std {

string::string() : string("") {}

string::string(const string& other) : string(other, 0) {}

string::string(string&& other)
    : data_(std::move(other.data_)),
      size_(std::move(other.size_)),
      capacity_(std::move(other.capacity_)) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

string::string(size_t count, char character)
    : data_(new char[count + 1]), size_(count), capacity_(count) {
    memset(data_, character, count);
    data_[count] = '\0';
}

string::string(const string& other, size_t position)
    : string(other, position, other.size()) {}

string::string(const string& other, size_t position, size_t count)
    : string(&other.at(position), count) {}

string::string(const char* character_array)
    : string(character_array, strlen(character_array)) {}

string::string(const char* character_array, size_t count)
    : data_(new char[count + 1]), size_(count), capacity_(count) {
    memcpy(data_, character_array, count);
    data_[count] = '\0';
}

string::~string() {
    delete[] data_;
}

string& string::operator=(const string& other) {
    if (this == &other) {
        return *this;
    }

    delete[] data_;

    const size_t byte_count_including_terminator = other.size_ + 1;
    data_ = new char[byte_count_including_terminator];
    size_ = other.size_;
    capacity_ = other.capacity_;

    memcpy(data_, other.data_, byte_count_including_terminator);

    return *this;
}

string& string::operator=(string&& other) {
    if (this == &other) {
        return *this;
    }

    delete[] data_;

    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;

    return *this;
}

char& string::operator[](size_t offset) {
    return const_cast<char&>(
        const_cast<const string*>(this)->operator[](offset));
}

const char& string::operator[](size_t offset) const {
    return at(offset);
}

const char& string::at(size_t offset) const {
    if (offset > size_) {
        assertm(false, "Access to string element out of bounds");
        placeholder_ = '\0';
        return placeholder_;
    }

    return data_[offset];
}

const char* string::c_str() const {
    return data_;
}

char* string::data() {
    return data_;
}

string::iterator string::begin() {
    return data_;
}

string::const_iterator string::cbegin() const {
    return data_;
}

string::iterator string::end() {
    return data_ + size_;
}

string::const_iterator string::cend() const {
    return data_ + size_;
}

bool string::empty() const {
    return size_ == 0;
}

size_t string::size() const {
    return size_;
}

size_t string::length() const {
    return size_;
}

size_t string::capacity() const {
    return capacity_;
}

void string::reserve(size_t size) {
    if (capacity_ >= size) {
        return;
    }

    resize(size);
}

void string::shrink_to_fit() {
    if (size_ == capacity_) {
        return;
    }

    resize(size_);
}

string& string::operator+=(const string& str) {
    return append(str.c_str(), str.size());
}

string& string::operator+=(char ch) {
    return append(&ch, 1);
}

string& string::operator+=(const char* str) {
    return append(str, strlen(str));
}

string& string::append(const char* str, size_t size) {
    reserve(size_ + size);
    memcpy(data_ + size_, str, size);
    size_ += size;
    return *this;
}

void string::resize(size_t size) {
    char* new_data = new char[size + 1];
    memcpy(new_data, data_, size_ + 1);

    delete[] data_;
    data_ = new_data;

    capacity_ = size;
}

char string::placeholder_ = '\0';

}  // namespace std
