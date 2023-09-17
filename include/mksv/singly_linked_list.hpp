#pragma once

#include "types.hpp"

namespace mksv {

template <typename T>
struct SinglyLinkedList {
    struct Node {
        T data;
        Node* next = nullptr;
    };

    Node* head = nullptr;
    u64 len = 0;

    void
    append_node(Node* node) {
        if (head == nullptr) {
            head = node;
        } else {
            Node* ptr = head;
            while (ptr->next) {
                ptr = ptr->next;
            }
            ptr->next = node;
        }
        ++len;
    }

    bool
    remove_node(Node* node) {
        if (node == nullptr) return false;

        if (node == head) {
            head = node->next;
        } else {
            Node* ptr = head;
            while (ptr->next != node) {
                ptr = ptr->next;
            }
            ptr->next = node->next;
        }
        node->next = nullptr;

        --len;

        return true;
    }

    bool
    pop_front(Node* out) {
        if (head == nullptr) return false;

        const Node* ptr = head;
        if (!remove_node(head)) return false;

        *out = { .data = ptr->data, .next = nullptr};
        return true;
    }
};

} // namespace mksv
