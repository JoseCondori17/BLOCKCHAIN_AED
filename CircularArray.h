#ifndef BLOCKCHAIN_AED_CIRCULARARRAY_H
#define BLOCKCHAIN_AED_CIRCULARARRAY_H
#include <iostream>
#include <sstream>
#include "mergesort.h"
using namespace std;

template <class T>
class CircularArray
{
protected:
    T *array;
    int capacity;
    int back, front;
public:
    CircularArray(int _capacity = 30);
    ~CircularArray() { delete[] array; }
    T pop_front();
    T pop_back();
    void push_front(T data);
    void push_back(T data);
    void insert(T data, int pos);
    bool is_full();
    bool is_empty();
    int size();
    void clear();
    T& operator[](int index);
    void sort();
    bool is_sorted();
    void reverse();

    string to_string(string sep=" ")
    {
        ostringstream os;
        for (short i = 0; i < size(); ++i)
            os << this->operator[](i) << sep;

        string arr_str(os.str());
        return arr_str;
    }

private:
    int next(int);
    int prev(int);
    void resize();
};

template <class T>
CircularArray<T>::CircularArray(int _capacity)
{
    this->capacity = _capacity;
    this->array = new T[_capacity];
    this->front = this->back = -1;//empty
}


// Private functions

template <class T>
int CircularArray<T>::next(int idx)
{
    if (is_empty())
        return ++front;
    else
        return ( idx + 1 ) % capacity;
}

template <class T>
int CircularArray<T>::prev(int idx)
{
    if (is_empty())
    {
        back = capacity - 1;
        return capacity - 1;
    }
    else if (idx == 0 )
        return capacity - 1;

    else
        return idx - 1;
}

template <class T>
void CircularArray<T>::resize()
{
    T* new_array = new T[capacity + 3];

    for (int i = 0; i < capacity; ++i)
        new_array[i] = this->operator[](i);

    front = 0;  back = capacity - 1;
    delete[] array;
    array = new_array;

    capacity += 3;
}
// Public functions

template <class T>
void CircularArray<T>::push_front(T data)
{
    if (is_full())
        resize();
    front = prev(front);
    array[front] = data;
}

template <class T>
void CircularArray<T>::push_back(T data)
{
    if (is_full())
        resize();
    back = next(back);
    array[back] = data;
}

template <class T>
void CircularArray<T>::insert(T data, int pos)
{
    if (is_full())
        resize();

    if (pos == 0)
        push_front(data);

    else if (pos == size()-1)
        push_back(data);

    else
    {
        int Tback = back, Nback;
        back = next(back);
        T valor;
        for (short i = 0; i < (size() - pos); ++i)
        {
            valor = array[Tback];
            Nback = next(Tback--);
            array[Nback] = valor;
        }
        array[(pos + front) % capacity] = data;
    }
}

template <class T>
T CircularArray<T>::pop_front()
{
    if (is_empty())
    {
        throw out_of_range("Array is empty");
    }
    else if (size() > 1 && front == capacity - 1)
    {
        front = 0;
        return array[capacity - 1];
    }
    else if (front == back)
    {
        int aux = front;
        back = front = -1;
        return array[aux];
    }
    return array[front++];
}

template <class T>
T CircularArray<T>::pop_back()
{
    if (is_empty())
    {
        throw out_of_range("Array is empty");
    }
    else if (size() > 1 && back == 0)
    {
        back = capacity - 1;
        return array[0];
    }
    else if (back == front)
    {
        int aux = back;
        back = front = -1;
        return array[aux];
    }
    return array[back--];
}

template <class T>
bool CircularArray<T>::is_full()
{
    return (( back + 1 ) % capacity) == front;
}

template <class T>
bool CircularArray<T>::is_empty()
{
    return (back == -1 && front ==-1);
}

template <class T>
int CircularArray<T>::size()
{
    if (is_empty())
        return 0;

    return ((back - front + capacity) % capacity) + 1;
}

template <class T>
void CircularArray<T>::clear()
{
    front = back = -1;
    delete[] array;
    array = new T[capacity];
}

template <class T>
T& CircularArray<T>::operator[](int index)
{
    if (index < 0 || index > (capacity - 1))
    {
        throw out_of_range("Index out of range");
    }
    int r_index = (index + front) % capacity;
    return array[r_index];
}

template <class T>
void CircularArray<T>::sort() // afecta solo los elementos ocupados
{
    T* tempArr = new T[size()];
    for (short i= 0; i < size(); ++i)
        tempArr[i] = this->operator[](i);

    mergeSort<T>(tempArr, 0, size()-1);

    for (short i= 0; i < size(); ++i)  // actualiza en el circulararray
        this->operator[](i) = tempArr[i];

    delete[] tempArr;
}

template <class T>
bool CircularArray<T>::is_sorted()
{
    T* sortArray = new T[size()];
    for (short i= 0; i < size(); ++i)
        sortArray[i] = this->operator[](i);

    mergeSort<T>(sortArray, 0, capacity-1);

    bool valid = array == sortArray;  // verifica
    delete[] sortArray;

    return valid;
}

template <class T>
void CircularArray<T>::reverse()
{
    if (is_empty())
    {
        throw out_of_range("Array is empty");
    }
    else
    {
        T temp;
        for (short i = 0, j = size()-1; i < size()/2; ++i, --j)
        {
            temp = this->operator[](i);
            this->operator[](i) = this->operator[](j);
            this->operator[](j) = temp;
        }
    }
}
#endif //BLOCKCHAIN_AED_CIRCULARARRAY_H
