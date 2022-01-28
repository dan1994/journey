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
    : string(&other.at(position), count) {
    assertm(position < other.size_, "Position is out of string range");
}

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

int string::compare(const string& str) const {
    return compare(str.c_str());
}

int string::compare(const char* str) const {
    const size_t length = strlen(str);
    for (size_t i = 0; i < length && i < size_; i++) {
        if (data_[i] < str[i]) {
            return -1;
        } else if (data_[i] > str[i]) {
            return 1;
        }
    }

    const int length_diff = size_ - length;
    return length_diff < 0 ? -1 : (length_diff > 0 ? 1 : 0);
}

string string::substr(size_t pos, size_t count) const {
    if (pos >= size_) {
        return "";
    }

    if (count >= size_ - pos) {
        return string(data_ + pos);
    }

    return string(data_ + pos, count);
}

void string::resize(size_t size) {
    char* new_data = new char[size + 1];
    memcpy(new_data, data_, size_ + 1);

    delete[] data_;
    data_ = new_data;

    capacity_ = size;
}

char string::placeholder_ = '\0';

bool operator==(const string& lhs, const string& rhs) {
    return lhs.compare(rhs) == 0;
}

bool operator==(const string& lhs, const char* rhs) {
    return lhs.compare(rhs) == 0;
}

bool operator==(const char* lhs, const string& rhs) {
    return rhs.compare(lhs) == 0;
}

bool operator!=(const string& lhs, const string& rhs) {
    return lhs.compare(rhs) != 0;
}

bool operator!=(const string& lhs, const char* rhs) {
    return lhs.compare(rhs) != 0;
}

bool operator!=(const char* lhs, const string& rhs) {
    return rhs.compare(lhs) != 0;
}

bool operator<(const string& lhs, const string& rhs) {
    return lhs.compare(rhs) < 0;
}

bool operator<(const string& lhs, const char* rhs) {
    return lhs.compare(rhs) < 0;
}

bool operator<(const char* lhs, const string& rhs) {
    return rhs.compare(lhs) > 0;
}

bool operator<=(const string& lhs, const string& rhs) {
    return lhs.compare(rhs) <= 0;
}

bool operator<=(const string& lhs, const char* rhs) {
    return lhs.compare(rhs) <= 0;
}

bool operator<=(const char* lhs, const string& rhs) {
    return rhs.compare(lhs) >= 0;
}

bool operator>(const string& lhs, const string& rhs) {
    return lhs.compare(rhs) > 0;
}

bool operator>(const string& lhs, const char* rhs) {
    return lhs.compare(rhs) > 0;
}

bool operator>(const char* lhs, const string& rhs) {
    return rhs.compare(lhs) < 0;
}

bool operator>=(const string& lhs, const string& rhs) {
    return lhs.compare(rhs) >= 0;
}

bool operator>=(const string& lhs, const char* rhs) {
    return lhs.compare(rhs) >= 0;
}

bool operator>=(const char* lhs, const string& rhs) {
    return rhs.compare(lhs) <= 0;
}

string to_string(int number) {
    return to_string(static_cast<long>(number));
}

string to_string(long number) {
    if (number >= 0) {
        return to_string(static_cast<unsigned long>(number));
    }

    const size_t digit_number = internal::get_digit_number(-number);
    string result(digit_number + 1, 0);

    result[0] = '-';
    internal::number_to_characters(result.data() + 1, -number, digit_number);

    return result;
}

string to_string(unsigned int number) {
    return to_string(static_cast<unsigned long>(number));
}

string to_string(unsigned long number) {
    const size_t digit_number = internal::get_digit_number(number);
    string result(digit_number, 0);

    internal::number_to_characters(result.data(), number, digit_number);

    return result;
}

string to_string(const void* ptr) {
    return to_string(reinterpret_cast<unsigned int>(ptr));
}

namespace internal {

size_t get_digit_number(unsigned long number) {
    if (number == 0) {
        return 1;
    }

    size_t digit_number = 0;

    for (; number > 0; number /= 10) {
        digit_number++;
    }

    return digit_number;
}

void number_to_characters(char* buffer, unsigned long number,
                          size_t digit_num) {
    buffer[digit_num] = 0;
    for (; digit_num > 0; number /= 10, digit_num--) {
        buffer[digit_num - 1] = number % 10 + '0';
    }
}

}  // namespace internal

string operator+(const string& lhs, const string& rhs) {
    string s;
    s.reserve(lhs.size() + rhs.size());
    return (s += lhs) += rhs;
}

string operator+(const char* lhs, const string& rhs) {
    string s;
    const size_t lhs_size = strlen(lhs);
    s.reserve(lhs_size + rhs.size());
    return s.append(lhs, lhs_size) += rhs;
}

string operator+(const string& lhs, const char* rhs) {
    string s;
    const size_t rhs_size = strlen(rhs);
    s.reserve(lhs.size() + rhs_size);
    return (s += lhs).append(rhs, rhs_size);
}

}  // namespace std
