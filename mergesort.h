#ifndef BLOCKCHAIN_AED_MERGESORT_H
#define BLOCKCHAIN_AED_MERGESORT_H
#include <iostream>
using namespace std;

template <class T>
void merge(T*& array, int left, int mid, int right)
{
    int l_size = mid - left + 1;
    int r_size = right - mid;

    T *left_arr = new T[l_size];
    T *right_arr = new T[r_size];

    for (short i = 0; i < l_size; ++i)
        left_arr[i] = array[left + i];

    for (short j = 0; j < r_size; ++j)
        right_arr[j] = array[mid + 1 + j];

    int i_left = 0, i_right = 0;
    int k = left;

    while (i_left < l_size && i_right < r_size)
    {
        if (left_arr[i_left] <= right_arr[i_right])
            array[k++] = left_arr[i_left++];
        else
            array[k++] = right_arr[i_right++];
    }

    while (i_left < l_size)
    {
        array[k++] = left_arr[i_left++];
    }

    while (i_right < r_size)
    {
        array[k++] = right_arr[i_right++];
    }

    delete[] left_arr;
    delete[] right_arr;
}

template <class T>
void mergeSort(T*& array, int begin, int end)
{
    if (begin >= end)   return;

    int mid = begin + (end - begin) / 2;
    mergeSort<T>(array, begin, mid);
    mergeSort<T>(array, mid + 1, end);
    merge<T>(array, begin, mid, end);
}
#endif //BLOCKCHAIN_AED_MERGESORT_H
