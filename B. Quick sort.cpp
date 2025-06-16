#include <cstdint>
#include <iostream>


class Int64Array {
public:
    explicit Int64Array(std::uint32_t size)
        : size_(size), data_(new std::int64_t[size]()) {}

    ~Int64Array() { delete[] data_; }

    Int64Array(const Int64Array&) = delete;
    Int64Array& operator=(const Int64Array&) = delete;

    Int64Array(Int64Array&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
    }

    Int64Array& operator=(Int64Array&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    std::int64_t& operator[](std::uint32_t index) { return data_[index]; }
    const std::int64_t& operator[](std::uint32_t index) const { return data_[index]; }

    std::uint32_t Size() const { return size_; }

private:
    std::uint32_t size_;
    std::int64_t* data_;
};

template <typename T>
inline void Swap(T& a, T& b) noexcept {
    T temp = static_cast<T&&>(a);
    a = static_cast<T&&>(b);
    b = static_cast<T&&>(temp);
}


void InsertionSort(Int64Array& array, std::uint32_t left, std::uint32_t right) {
    for (std::uint32_t i = left + 1; i <= right; ++i) {
        std::int64_t key = array[i];
        std::uint32_t j = i;
        
        if (array[j - 1] <= key) continue;
        
        do {
            array[j] = array[j - 1];
            --j;
        } while (j > left && array[j - 1] > key);
        
        array[j] = key;
    }
}

std::int64_t MedianOfThree(const Int64Array& array,
                          std::uint32_t left,
                          std::uint32_t right) noexcept {
    std::uint32_t mid = left + (right - left) / 2;
    
    const bool c1 = array[left] > array[mid];
    const bool c2 = array[left] > array[right];
    const bool c3 = array[mid] > array[right];
    
    if (c1 != c2) return array[left];
    if (c2 == c3) return array[mid];
    return array[right];
}

std::uint32_t Partition(Int64Array& array,
                       std::uint32_t left,
                       std::uint32_t right) {
    std::int64_t pivot = MedianOfThree(array, left, right);
    std::uint32_t i = left - 1;
    std::uint32_t j = right + 1;
    
    while (true) {
        do { ++i; } while (array[i] < pivot);
        do { --j; } while (array[j] > pivot);
        if (i >= j) return j;
        Swap(array[i], array[j]);
    }
}

void QuickSort(Int64Array& array, std::uint32_t left, std::uint32_t right) {
    static constexpr std::uint32_t kInsertionThreshold = 256;
    
    while (right - left > kInsertionThreshold) {
        std::uint32_t pivot = Partition(array, left, right);
        
        if (pivot - left < right - pivot) {
            QuickSort(array, left, pivot);
            left = pivot + 1;
        } else {
            QuickSort(array, pivot + 1, right);
            right = pivot;
        }
    }
    
    if (left < right) {
        InsertionSort(array, left, right);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::uint32_t n;
    std::cin >> n;
    
    Int64Array array(n);
    for (std::uint32_t i = 0; i < n; ++i) {
        std::cin >> array[i];
    }
    
    if (n > 1) {
        QuickSort(array, 0, n - 1);
    }
    
    for (std::uint32_t i = 0; i < n; ++i) {
        std::cout << array[i] << (i == n - 1 ? '\n' : ' ');
    }
    
    return 0;
}