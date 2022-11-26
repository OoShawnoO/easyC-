#ifndef TEST_SORT_H
#define TEST_SORT_H

#include <iostream>

template <typename T>
void BubbleSort(T* list,size_t size,bool (*Compare)(T,T));
template <typename T>
void SelectionSort(T* list,size_t size,bool (*Compare)(T,T));
template <typename T>
void InsertSort(T* list,size_t size,bool (*Compare)(T,T));
template <typename T>
void ShellSort(T* list,size_t size,bool (*Compare)(T,T));
template <typename T>
void QuickSort(T* list,size_t size,bool (*Compare)(T,T));
template <typename T>
void MergeSort(T* list,size_t size,bool (*Compare)(T,T));
template <typename T>
void HeapSort(T* list,size_t size,bool (*Compare)(T,T));


template <typename T>
void BubbleSort(T* list,size_t size,bool (*Compare)(T,T))
{
    for(size_t i=0;i<size;i++)
    {
        for(size_t j=i;j<size;j++)
        {
            if(Compare(list[i],list[j]))
            {
                T temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
}
template <typename T>
void SelectionSort(T* list,size_t size,bool (*Compare)(T,T))
{
    T temp;
    size_t index;
    for(size_t i=0;i<size;i++){
        index = i;
        for(size_t j=i;j<size;j++){
            if(Compare(list[index],list[j])){
                index = j;
            }
        }
        if(index != i){
            temp = list[index];
            list[index] = list[i];
            list[i] = temp;
        }
    }
}
template <typename T>
void InsertSort(T* list,size_t size,bool (*Compare)(T,T))
{
    for(size_t i=1;i<size;i++){
        size_t j = 0;
        if(Compare(list[i-1],list[i])){
            T temp = list[i];
            for(j=i-1;Compare(list[j],temp);j--){
                list[j+1] = list[j];
            }
            list[j+1] = temp;
        }
    }
}
template <typename T>
void ShellSort(T* list,size_t size,bool (*Compare)(T,T))
{
    size_t group = size / 2;
    size_t i,j,k;
    while(group > 1){
        group = group / 2;
        for(i = 0; i < group ; i++){
            for( j = i + group; j < size; j += group){
                if(Compare(list[j-group],list[j])){
                    T temp = list[j];
                    for(k = j-group; Compare(list[k],temp); k-=group){
                        list[k+group] = list[k];
                    }
                    list[k+group] = temp;
                }
            }
        }
    }
}
template <typename T>
void QuickSort(T* list,size_t left,size_t right,bool (*Compare)(T,T))
{
    if(left >= right) return;
    size_t start = left;
    size_t end = right;
    T key = list[left];
    while(left < right){
        while(left < right && !Compare(key,list[right])){
            right--;
        }
        if(left < right){
            list[left] = list[right];
            left++;
        }
        while(left < right && !Compare(list[left],key)){
            left++;
        }
        if(left < right){
            list[right] = list[left];
            right--;
        }
    }
    list[left] = key;
    QuickSort(list,start,left-1,Compare);
    QuickSort(list,left+1,end,Compare);
}
template <typename T>
void QuickSort(T* list,size_t size,bool (*Compare)(T,T))
{
    QuickSort(list,(size_t)0,size-1,Compare);
}
template <typename T>
void MergeSort(T* list,size_t left,size_t right,bool (*Compare)(T,T))
{
    if(left >= right) return;
    size_t mid = (left + right) / 2;
    MergeSort(list,left,mid,Compare);
    MergeSort(list,mid+1,right,Compare);
    T temp[right-left+1];
    size_t len = 0;
    size_t tleft = left;
    size_t tmid = mid+1;
    while(tleft <= mid && tmid <= right){
        temp[len++] = Compare(list[tleft],list[tmid]) ? list[tmid++] : list[tleft++];
    }
    if(tleft <= mid){
        while(tleft <= mid){
            temp[len++] = list[tleft++];
        }
    }
    if(tmid <= right){
        while(tmid <= right){
            temp[len++] = list[tmid++];
        }
    }
    int j=0;
    for(size_t i = left;i <= right;i++,j++){
        list[i] = temp[j];
    }
}
template <typename T>
void MergeSort(T* list,size_t size,bool (*Compare)(T,T))
{
    MergeSort(list,0,size-1,Compare);
}

template <typename T>
void Heapify(T* list,size_t size,bool (*Compare)(T,T),size_t index)
{
    size_t largest = index;
    size_t lson = index * 2 + 1;
    size_t rson = index * 2 + 2;
    if(lson < size && Compare(list[lson],list[largest])){
        largest = lson;
    }
    if(rson < size && Compare(list[rson],list[largest])){
        largest = rson;
    }
    if(largest != index){
        T temp = list[index];
        list[index] = list[largest];
        list[largest] = temp;
        Heapify(list,size,Compare,largest);
    }
}

template <typename T>
void HeapSort(T* list,size_t size,bool (*Compare)(T,T))
{
    long long int index;
    for(index = size / 2 -1;index >= 0;index--){
        Heapify(list,size,Compare,index);
    }
    for(index = size - 1;index > 0;index-- ){
        T temp = list[index];
        list[index] = list[0];
        list[0] = temp;
        Heapify(list,index,Compare,0);
    }
}

#endif
