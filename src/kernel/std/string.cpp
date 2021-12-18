#include <cstring>
#include <string>

namespace std {

string::string() : string("") {}

string::string(const string& other) : string(other, 0) {}

string::string(string&& other)
    : data_(other.data()), size_(other.size()), capacity_(other.capacity()) {
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

    const size_t byte_count_including_terminator = other.capacity() + 1;
    data_ = new char[byte_count_including_terminator];
    size_ = other.size();
    capacity_ = other.capacity();

    memcpy(data_, other.data(), byte_count_including_terminator);

    return *this;
}

string& string::operator=(string&& other) {
    if (this == &other) {
        return *this;
    }

    data_ = other.data();
    size_ = other.size();
    capacity_ = other.capacity();

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;

    return *this;
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

const char* string::c_str() const {
    return data_;
}

char* string::data() const {
    return data_;
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
        placeholder_ = '\0';
        return placeholder_;
    }

    return data_[offset];
}

char string::placeholder_ = '\0';

}  // namespace std
