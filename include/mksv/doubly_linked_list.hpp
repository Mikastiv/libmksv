#pragma once

#include "mem.hpp"
#include "types.hpp"

namespace mksv {

template <typename T>
struct DoublyLinkedList {
    struct Node {
        T data;
        Node* next;
        Node* prev;
    };

    mem::Allocator allocator;
    Node* head;
    Node* tail;
    u64 len;

    static DoublyLinkedList
    init(const mem::Allocator allocator) {
        return {
            .allocator = allocator,
            .head = nullptr,
            .tail = nullptr,
            .len = 0,
        };
    }

    Node*
    create_node(const T& elem) {
        Node* node = allocator.alloc<Node>(1);
        if (node == nullptr) return nullptr;

        mem::zero({ .ptr = node, .len = 1 });
        node->data = elem;

        return node;
    }

    void
    delete_node(Node* node) {
        allocator.free<Node>({ .ptr = node, .len = 1 });
    }

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

    void
    remove_node(Node* node) {
        if (node == nullptr) return;

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
    }

    void
    deinit() {
        Node* ptr = head;
        while (ptr != nullptr) {
            head = ptr->next;
            delete_node(ptr);
            ptr = head;
        }
        head = nullptr;
        len = 0;
    }
};

} // namespace mksv
