#include <cstdint>
#include <iostream>

class UInt64Array {
 public:
  explicit UInt64Array(int initial_capacity)
      : data_(new uint64_t[initial_capacity]),
        size_(0),
        capacity_(initial_capacity) {}

  ~UInt64Array() {
    delete[] data_;
  }

  UInt64Array(const UInt64Array&) = delete;
  UInt64Array& operator=(const UInt64Array&) = delete;

  void PushBack(uint64_t value) {
    if (size_ == capacity_) {
      Reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_] = value;
    ++size_;
  }

  uint64_t& operator[](int index) {
    return data_[index];
  }

  const uint64_t& operator[](int index) const {
    return data_[index];
  }

  int Size() const {
    return size_;
  }

  void Clear() {
    size_ = 0;
  }

 private:
  void Reserve(int new_capacity) {
    uint64_t* new_data = new uint64_t[new_capacity];
    for (int i = 0; i < size_; ++i) {
      new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }

  uint64_t* data_;
  int size_;
  int capacity_;
};

void LsdSort(UInt64Array* arr) {
  const int kNumBytes = 8;
  const int kRadix = 256;
  int n = arr->Size();

  UInt64Array buffer(n);
  uint32_t count[kRadix] = {0};
  uint32_t pos[kRadix] = {0};

  for (int byte = 0; byte < kNumBytes; ++byte) {
    for (int i = 0; i < kRadix; ++i) {
      count[i] = 0;
    }

    for (int i = 0; i < n; ++i) {
      uint8_t b = static_cast<uint8_t>(((*arr)[i] >> (byte * 8)) & 0xFF);
      ++count[b];
    }

    pos[0] = 0;
    for (int i = 1; i < kRadix; ++i) {
      pos[i] = pos[i - 1] + count[i - 1];
    }

    buffer.Clear();
    for (int i = 0; i < n; ++i) {
      buffer.PushBack(0);
    }

    for (int i = 0; i < n; ++i) {
      uint8_t b = static_cast<uint8_t>(((*arr)[i] >> (byte * 8)) & 0xFF);
      buffer[pos[b]] = (*arr)[i];
      ++pos[b];
    }

    for (int i = 0; i < n; ++i) {
      (*arr)[i] = buffer[i];
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  UInt64Array arr(n);
  for (int i = 0; i < n; ++i) {
    uint64_t value = 0;
    std::cin >> value;
    arr.PushBack(value);
  }

  LsdSort(&arr);

  for (int i = 0; i < arr.Size(); ++i) {
    std::cout << arr[i] << '\n';
  }

  return 0;
}
