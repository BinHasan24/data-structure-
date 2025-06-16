#include <iostream>
#include <stack>
#include <string>

struct QueueNode {
    int value;
    QueueNode* next;
    QueueNode* prev;
};

class SortingHat {
private:
    QueueNode* queue_head;
    QueueNode* queue_tail;
    std::stack<std::pair<int, int>> min_stack; // (value, count)
    int current_size;

    void updateMinOnEnqueue(int value) {
        if (min_stack.empty() || value < min_stack.top().first) {
            min_stack.push({value, 1});
        } else if (value == min_stack.top().first) {
            min_stack.top().second++;
        }
    }

    void updateMinOnDequeue(int value) {
        if (!min_stack.empty() && value == min_stack.top().first) {
            if (--min_stack.top().second == 0) {
                min_stack.pop();
            }
        }
    }

    void clearAll() {
        while (queue_head) {
            QueueNode* temp = queue_head;
            queue_head = queue_head->next;
            delete temp;
        }
        queue_tail = nullptr;
        while (!min_stack.empty()) {
            min_stack.pop();
        }
        current_size = 0;
    }

public:
    SortingHat() : queue_head(nullptr), queue_tail(nullptr), current_size(0) {}

    ~SortingHat() {
        clearAll();
    }

    void enqueue(int value) {
        QueueNode* new_node = new QueueNode{value, nullptr, nullptr};
        if (!queue_tail) {
            queue_head = queue_tail = new_node;
        } else {
            queue_tail->next = new_node;
            new_node->prev = queue_tail;
            queue_tail = new_node;
        }
        updateMinOnEnqueue(value);
        current_size++;
        std::cout << "ok\n";
    }

    void dequeue() {
        if (current_size == 0) {
            std::cout << "error\n";
            return;
        }
        int value = queue_head->value;
        QueueNode* temp = queue_head;
        if (queue_head == queue_tail) {
            queue_head = queue_tail = nullptr;
        } else {
            queue_head = queue_head->next;
            queue_head->prev = nullptr;
        }
        delete temp;
        updateMinOnDequeue(value);
        current_size--;
        std::cout << value << "\n";
    }

    void front() const {
        if (current_size == 0) {
            std::cout << "error\n";
            return;
        }
        std::cout << queue_head->value << "\n";
    }

    void size() const {
        std::cout << current_size << "\n";
    }

    void clear() {
        clearAll();
        std::cout << "ok\n";
    }

    void min() const {
        if (current_size == 0) {
            std::cout << "error\n";
            return;
        }
        std::cout << min_stack.top().first << "\n";
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    SortingHat hat;
    int M;
    std::cin >> M;

    for (int i = 0; i < M; ++i) {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "enqueue") {
            int value;
            std::cin >> value;
            hat.enqueue(value);
        } else if (cmd == "dequeue") {
            hat.dequeue();
        } else if (cmd == "front") {
            hat.front();
        } else if (cmd == "size") {
            hat.size();
        } else if (cmd == "clear") {
            hat.clear();
        } else if (cmd == "min") {
            hat.min();
        }
    }

    return 0;
}