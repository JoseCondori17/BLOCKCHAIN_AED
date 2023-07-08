#ifndef BLOCKCHAIN_AED_HEAPMINMAX_H
#define BLOCKCHAIN_AED_HEAPMINMAX_H
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

template <typename T>
class Heap {
public:
    enum Type {
        MAX_HEAP, MIN_HEAP
    };

private:
    T *elements;
    int capacity;
    int n;
    Type type;

public:
    Heap(T *elements, int n, Type type=MAX_HEAP) : elements(elements), n(n), type(type)
    {
        this->elements = new T[n];
        for (int i = 0; i < n; i++)
        {
            this->elements[i] = elements[i];
        }
    }
    Heap(int capacity, Type type=MAX_HEAP) : capacity(capacity), type(type)
    {
        this->elements = new T[capacity];
        this->n = 0;
    }

    ~Heap(){
        delete []elements;
        elements = nullptr;
    }

    void buildFromArray(T *elements, int n){
        this->n = n;
        for (int i = 0; i < n; i++)
        {
            this->elements[i] = elements[i];
        }
        int mid = (n/2) - 1;
        for (int i = mid; i >= 0; i--){
            if (type==MAX_HEAP){
                heapify_down(i,1);
            } else {
                heapify_down(i,0);
            }
        }
    }

    int size()
    {
        return n;
    }

    bool is_empty()
    {
        return n == 0;
    }

    void push(T value)
    {
        elements[n++] = value;
        if (type==MAX_HEAP) heapify_up(n-1,true);
        else if (type==MIN_HEAP) heapify_up(n-1,false);
    }
    void display(){
        for (int i = 0;i<n;i++){
            cout<<elements[i]<<" ";
        }
        cout<<endl;
    }

    T pop()
    {
        T value = T();
        if (n == 0) return value;

        value = elements[0];
        swap(&elements[0],&elements[--n]);
        if (type == MAX_HEAP) heapify_down(0, true);
        else if (type == MIN_HEAP) heapify_down(0, false);
        return value;
    }

    T top()
    {
        if (n == 0) {
            return T();
        }
        return elements[0];
    }

    vector<T> extractTheTopK(int k){
        vector<T> topK;
        while (k-- > 0)
        {
            if (n == 0) break;
            T top = pop();
            topK.push_back(top);
        }
        return topK;
    }

    static void sortAsc(T* arr, int n){
        Heap heap(arr, n, Heap::MIN_HEAP);

        for (int i = n-1; i >= 0; i--){
            arr[i] = heap.pop();
            heap.heapify_down(0,false);
        }
    }

    static void sortDesc(T* arr, int n){
        Heap heap(arr, n, Heap::MAX_HEAP);

        for (int i = n-1; i >= 0; i--){
            arr[i] = heap.pop();
            heap.heapify_down(0, true);
        }
    }
private:
    int Parent(int i)
    {
        return (i - 1) / 2;
    }
    int Left(int i)
    {
        return (2 * i + 1);
    }
    int Right(int i)
    {
        return (2 * i + 2);
    }
    void swap(T *a,T *b){
        T tmp = *a;
        *a = *b;
        *b = tmp;
    }
    void heapify_down(int i, bool isMax)
    {
        int left = Left(i), right = Right(i);
        int post = i;
        if (isMax){
            if (left < n && elements[left] > elements[post]) post = left;
            if (right < n && elements[right] > elements[post]) post = right;
        } else {
            if (left < n && elements[left] < elements[post]) post = left;
            if (right < n && elements[right] < elements[post]) post = right;
        }
        if (post != i){
            swap(&elements[i],&elements[post]);
            heapify_down(post,isMax);
        }
    }
    void heapify_up(int i, bool isMax)
    {
        if (i == 0) return;
        int parent = Parent(i);
        if (isMax){
            if (elements[i]>elements[parent]) swap(&elements[i],&elements[parent]);
        }
        else {
            if (elements[i]<elements[parent]) swap(&elements[i],&elements[parent]);
        }
        heapify_up(parent,isMax);
    }
};
#endif //BLOCKCHAIN_AED_HEAPMINMAX_H
