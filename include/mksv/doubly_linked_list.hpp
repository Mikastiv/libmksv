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
    u64 len;

    static DoublyLinkedList
    init(const mem::Allocator allocator) {
        return {
            .allocator = allocator,
            .head = nullptr,
            .len = 0,
        };
    }

    Node*
    create_node(const T& elem) {
        Node* node = allocator.alloc<Node>(1);
        if (node == nullptr) return nullptr;

        node->data = elem;
        node->next = nullptr;
        node->prev = nullptr;

        return node;
    }

    void
    delete_node(Node* node) {
        allocator.free<Node>({ .ptr = node, .len = 1 });
    }

    void
    append_node(Node* node) {
        if (head == nullptr) {
            head = node;
            head->prev = nullptr;
        } else {
            Node* ptr = head;
            while (ptr->next) {
                ptr = ptr->next;
            }
            ptr->next = node;
            node->prev = ptr;
        }
        ++len;
    }

    void
    remove_node(Node* node) {
        if (node == head) {
            head = node->next;
            head->prev = nullptr;
            return;
        }

        Node* ptr = head;
        while (ptr->next) {
            if (ptr->next == node) break;
            ptr = ptr->next;
        }

        ptr->next = node->next;
        ptr->next->prev = ptr;
        --len;
    }

    void
    deinit() {
        Node* ptr = head;
        while (ptr != nullptr) {
            head = ptr->next;
            allocator.free<Node>({ .ptr = ptr, .len = 1 });
            ptr = head;
        }
        head = nullptr;
        len = 0;
    }
};

} // namespace mksv
