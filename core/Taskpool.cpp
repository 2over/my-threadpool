//
// Created by xiehao on 2024/10/11.
//

#include "Taskpool.h"
#include "Task.h"

void Taskpool::AddTask(Task *node) {
    assert(node != NULL, "should not dequeue NULL node");
    assert(node->_prev == NULL, "node already in list");
    assert(node->_next == NULL, "node already in list");

    if (_tasks == NULL) {
        _tasks = node;
        node->_prev = node;
        node->_next = node;
    } else {
        Task *head = _tasks;
        Task *tail = head->_prev;

        assert(tail->_next == head, "invariant check");
        tail->_next = node;
        head->_prev = node;

        node->_next = head;
        node->_prev = tail;
    }
}

Task *Taskpool::DequeueTask() {
    Task *node = _tasks;
    if (node) {
        DequeueSpecificTask(node);
    }
    return node;
}

void Taskpool::DequeueSpecificTask(Task *node) {
    assert(node != NULL, "shound not dequeue NULL node");
    assert(node->_prev != NULL, "node already removed from list");
    assert(node->_next != NULL, "node already remmoved from list");


    Task *next = node->_next;
    if (next == node) {
        assert(node->_prev == node, "invariant check");
        _tasks = NULL;
    } else {
        Task *prev = node->_prev;
        assert(prev->_next == node, "invariant check");
        assert(next->_prev == node, "invariant check");

        next->_prev = prev;
        prev->_next = next;
        if (_tasks == node) {
            _tasks = next;
        }
    }

    node->_next = NULL;
    node->_prev = NULL;
}