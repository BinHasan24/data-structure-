#include <cstdint>
#include <iostream>

constexpr int32_t kModulus = 10000000 + 4321;
constexpr int32_t kGroupSize = 5;

inline void SwapValues(int32_t& a, int32_t& b) {
    int32_t temp = a;
    a = b;
    b = temp;
}

void GenerateSequence(int32_t* seq, int32_t n, int32_t a0, int32_t a1) {
    if (n >= 1) seq[0] = a0;
    if (n >= 2) seq[1] = a1;
    
    for (int32_t i = 2; i < n; ++i) {
        int64_t next_val = (static_cast<int64_t>(seq[i-1]) * 123 + 
                          static_cast<int64_t>(seq[i-2]) * 45);
        seq[i] = static_cast<int32_t>(next_val % kModulus);
    }
}

void SortFiveElements(int32_t* arr, int32_t left, int32_t right) {
    for (int32_t i = left + 1; i <= right; ++i) {
        int32_t key = arr[i];
        int32_t j = i;
        while (j > left && arr[j-1] > key) {
            arr[j] = arr[j-1];
            --j;
        }
        arr[j] = key;
    }
}

int32_t Partition(int32_t* arr, int32_t left, int32_t right, int32_t pivot_idx) {
    SwapValues(arr[pivot_idx], arr[right]);
    int32_t pivot = arr[right];
    int32_t i = left;
    
    for (int32_t j = left; j < right; ++j) {
        if (arr[j] <= pivot) {
            if (i != j) SwapValues(arr[i], arr[j]);
            ++i;
        }
    }
    SwapValues(arr[i], arr[right]);
    return i;
}

int32_t MedianOfMedians(int32_t* arr, int32_t left, int32_t right) {
    if (right - left < kGroupSize) {
        SortFiveElements(arr, left, right);
        return left + (right - left)/2;
    }

    for (int32_t i = left; i <= right; i += kGroupSize) {
        int32_t end = (i + kGroupSize - 1) > right ? right : i + kGroupSize - 1;
        SortFiveElements(arr, i, end);
        int32_t median_pos = i + (end - i)/2;
        SwapValues(arr[median_pos], arr[left + (i - left)/kGroupSize]);
    }

    return MedianOfMedians(arr, left, left + (right - left)/kGroupSize);
}

int32_t QuickSelect(int32_t* arr, int32_t left, int32_t right, int32_t k) {
    while (left < right) {
        int32_t pivot_idx = MedianOfMedians(arr, left, right);
        pivot_idx = Partition(arr, left, right, pivot_idx);
        
        if (k == pivot_idx) return arr[k];
        k < pivot_idx ? right = pivot_idx - 1 : left = pivot_idx + 1;
    }
    return arr[left];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int32_t n, k, a0, a1;
    std::cin >> n >> k >> a0 >> a1;
    
    if (n == 1) {
        std::cout << a0 << '\n';
        return 0;
    }
    
    if (n < 1 || k < 1 || k > n) {
        std::cerr << "Invalid input\n";
        return 1;
    }
    
    int32_t* sequence = new int32_t[n];
    if (!sequence) {
        std::cerr << "Memory allocation failed\n";
        return 1;
    }
    
    GenerateSequence(sequence, n, a0, a1);
    int32_t result = QuickSelect(sequence, 0, n - 1, k - 1);
    std::cout << result << '\n';
    
    delete[] sequence;
    return 0;
}