#include <iostream>


bool BinarySearch(const int* begin, const int* end, int target) {
    while (begin < end) {
        const int* mid = begin + (end - begin) / 2;
        
        if (*mid == target) {
            return true;
        } else if (*mid < target) {
            begin = mid + 1;
        } else {
            end = mid;
        }
    }
    return false;
}

int main() {
   
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);


    int array_size;
    std::cin >> array_size;

    int* data = new int[array_size];
    for (int i = 0; i < array_size; ++i) {
        std::cin >> data[i];
    }

   
    int query_count;
    std::cin >> query_count;

    while (query_count--) {
        int left, right, target;
        std::cin >> left >> right >> target;

        bool found = BinarySearch(data + left, data + right, target);
        std::cout << (found ? "YES\n" : "NO\n");
    }

    delete[] data;
    return 0;
}