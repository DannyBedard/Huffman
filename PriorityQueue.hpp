#ifndef PriorityQueue_h
#define PriorityQueue_h

#include <cstdlib>
#include "PQNode.hpp"

template<typename T>
class PriorityQueue{
private:
    size_t count;
    PQNode<T>* first;
public:
    PriorityQueue(){
        count = 0;
        first = nullptr;
    }
    ~PriorityQueue() {
        while (first)
            pop();
    }
    inline T front() {
        return(first) ? first->data : NULL;
    }
    inline size_t frontPriority(){
        return first->priority;
    }
    void pop() {
        if (first) {
            PQNode<T>* toDelete = first;
            first = first->next;
            delete toDelete;
            count--;
        }
    }
    void push(T data, size_t priority){
        if(first){
            PQNode<T>* runner = first;
            PQNode<T>* noeud = new PQNode<T>(data, priority);
            
            if(noeud->priority < first->priority){
                noeud->next = first;
                first = noeud;
                count ++;
            }
            else{
                while(runner->next && priority >= runner->next->priority)
                    runner = runner->next;
                noeud->next = runner->next;
                runner->next = noeud;
                count ++;
            }
        }
        else{
            first =  new PQNode<T>(data, priority);
            count ++;
        }
    }
    inline size_t size(){
        return count;
    }
};

#endif
