#ifndef QUEUE_H
#define QUEUE_H

#include "Node.h"
#include <iostream>

template <typename T>
class Queue {
private:
    Node<T>* front;
    Node<T>* back;
    
public:
    
    Queue() : front(nullptr), back(nullptr) {}
    
    ~Queue() {
        while(front != nullptr) {
            deQueue();
        }
    }
    
    void enQueue(T data) {
        Node<T>* node = new Node<T>(data);
        
        if(back == nullptr) {
            front = back = node;
            return;
        }
        
        back->next = node;
        back = node;
    }
    
    void deQueue() {
        if(front == nullptr) return;
        
        Node<T>* tmp = front;
        front = front->next;
        
        if(front == nullptr) back = nullptr;
        
        delete tmp;
    }
    
    void printQueue() const {
        Node<T>* node = front;
        
        while(node != nullptr) {
            std::cout << node->data << " -> ";
            node = node->next;
        }
        
        std::cout << " NULL" << std::endl;
    }
    
    T getFront() const {
        if(front == nullptr) return 0;
        
        return front->data;
    }
    
    T getBack() const {
        if(back == nullptr) return 0;
        
        return back->data;
    }
    
    bool empty() {
        return front == nullptr;
    }
    
};

#endif