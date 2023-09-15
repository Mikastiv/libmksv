#pragma once

#include "types.hpp"

namespace mksv {

template <typename T>
struct DoublyLinkedList {
    struct Node {
        T data;
        Node* next = nullptr;
        Node* prev = nullptr;
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    u64 len = 0;

    void
    append_node(Node* node) {
        if (node == nullptr) return;

        if (head == nullptr) {
            head = node;
            head->prev = nullptr;
            tail = node;
            tail->next = nullptr;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }

        ++len;
    }

    bool
    remove_node(Node* node) {
        if (node == nullptr) return false;

        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            head = node->next;
            head->prev = nullptr;
        }

        if (node->next) {
            node->next->prev = node->prev;
        }

        --len;

        return true;
    }
};

} // namespace mksv
