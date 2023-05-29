#ifndef BLOCKCHAIN_AED_FORWARDLIST_H
#define BLOCKCHAIN_AED_FORWARDLIST_H
#include <iostream>

template<typename T>
class ForwardList{
private:
    struct Node{
        T data;
        Node* next;
        explicit Node(T value) : data(value), next(nullptr){}
        void killSelf(){}
    };
private:
    class iteratorList{
    private:
        Node* current;
    public:
        iteratorList() : current(nullptr){}
        explicit iteratorList(Node* node) : current(node){}
        ~iteratorList(){ delete current; }
        bool operator!=(const iteratorList &other) const {
            return current != other.current;
        }
        iteratorList& operator++(){
            current = current->next;
            return *this;
        }
        T& operator*(){
            return current->data;
        }
    };
private:
    int nodes;
    Node* head;
public:
    ForwardList() : head(nullptr), nodes(0){}
    ~ForwardList(){ clear(); delete head; }

    typedef iteratorList iterator;

    T front() {
        if (head)
            return head->data;
        else
            throw std::out_of_range("List is empty");
    }

    T back() {
        T value = T();
        if (head) {
            Node *temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            value = temp->data;
        }
        return value;
    }

    void push_front(T value) {
        Node* new_node = new Node(value);
        new_node->next =head;
        head = new_node;
        ++nodes;
    }

    void push_back(T value) {
        Node* new_node= new Node(value);
        if (head) {
            Node *temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = new_node;
            new_node->next = nullptr; ++nodes;
        }
        else {
            head = new_node; ++nodes;
        }
    }

    T pop_front() {
        T value = T();
        if (head) {
            Node* temp = head;
            head = head->next;
            value = temp->data;
            delete temp; --nodes;
        }
        return value;
    }

    T pop_back() {
        T value = T();
        if (head == nullptr){
            return value;
        }
        else if (head->next == nullptr){
            value = head->data;
            delete head;
            head = nullptr;
        } else {
            Node* temp = head;
            while (temp->next->next != nullptr)
                temp = temp->next;

            value = temp->next->data;
            delete temp->next;
            temp->next = nullptr;
            --nodes;
        }
        return  value;
    }

    void insert(T value, int position) {
        if (position < 0 || position >= nodes) {
            throw std::invalid_argument("Index out of range"); // Error
        } else if(position == 0) { // Complexity O(1)
            push_front(value);
        } else { // Complexity O(n)
            Node* new_node = new Node(value);
            Node* temp = head;

            while (--position > 0 )
                temp = temp->next;

            new_node->next = temp->next;
            temp->next = new_node;
        }
    }

    void remove(int position) {
        if (position < 0 || position >= nodes){
            throw std::invalid_argument("Index out of range"); // Error
        } else if (position == 0) {
            pop_front(); // Complexity O(1)
        } else { // Complexity O(n)
            Node* nodo = head;
            while (--position > 0)
                nodo = nodo->next;

            Node* temp = nodo->next;
            nodo->next = nodo->next->next;
            delete temp;
        }
    }

    T &operator[](int position) {
        if (position < 0 || position >= nodes) {
            throw std::invalid_argument("Index out of range");
        } else {
            Node* temp = head;
            while (position-- > 0)
            {
                temp = temp->next;
            }
            return temp->data;
        }
    }

    bool empty() {
        return head == nullptr;
    }

    int size() {
        return nodes;
    }

    void clear() {
        while (head != nullptr)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void reverse(){
        Node* temp = head;
        head = nullptr;
        while (temp != nullptr){
            Node* new_node = new Node(temp->data);
            new_node->next = head;
            head = new_node;
            temp = temp->next;
        }
    }

    void display(){
        Node* temp=head;
        while (temp != nullptr) {
            std::cout<<temp->data<<" ";
            temp = temp->next;
        }
        std::cout<<std::endl;
    }

    iterator begin(){ return iteratorList(head); }
    iterator end(){ return iteratorList(); }
};
#endif //BLOCKCHAIN_AED_FORWARDLIST_H
