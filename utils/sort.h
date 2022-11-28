#ifndef SORT_H
#define SORT_H

#include <iostream>
using namespace std;
template <typename T>
bool Greater(T x,T y);
template <typename T>
void BubbleSort(T* list,size_t size,bool (*Compare)(T,T) = Greater);
template <typename T>
void SelectionSort(T* list,size_t size,bool (*Compare)(T,T) = Greater);
template <typename T>
void InsertSort(T* list,size_t size,bool (*Compare)(T,T) = Greater);
template <typename T>
void ShellSort(T* list,size_t size,bool (*Compare)(T,T) = Greater);
template <typename T>
void QuickSort(T* list,size_t size,bool (*Compare)(T,T) = Greater);
template <typename T>
void MergeSort(T* list,size_t size,bool (*Compare)(T,T) = Greater);
template <typename T>
void HeapSort(T* list,size_t size,bool (*Compare)(T,T) = Greater);

template <typename T>
bool Greater(T x,T y)
{
    try {
        if (is_same<T, int>()
            || is_same<T, float>()
            || is_same<T, double>()
            || is_same<T, unsigned int>()
            || is_same<T, signed long int>()
            || is_same<T, unsigned long int>()
            || is_same<T, unsigned long long int>()
            || is_same<T, signed long long int>()
            || is_same<T, char>()
            || is_same<T, unsigned char>()
            || is_same<T, signed char>()
            || is_same<T, unsigned short int>()
            || is_same<T, signed short int>()
            || is_same<T, long double>()) {
            return x > y;
        } else {
            throw exception();
        }
    }catch(...){
        cout << "No match default function for this type." << endl;
    }
}

/***
 *
 * 冒泡排序
 * 使用方式:
 * 传入自定义的Compare函数指针。
 *
 * 通过n^2次遍历 将极值放到对应位置
 *
 * */
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

/***
 *
 * 选择排序
 * 使用方式:
 * 传入自定义的Compare函数指针
 *
 * 每趟遍历将第n大的元素放到第n位
 * */

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

/***
 *
 * 插入排序
 * 使用方法:
 * 传入自定义Compare函数指针
 *
 * 以开头为有序序列,根据自定义规则将元素插入到合适的位置
 *
 * */
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

/*
 *
 * 希尔排序
 * 使用方式：
 * 传入自定义Compare函数指针
 *
 * 分组进行插入排序，再不断减小组的个数，达到所有元素的排序
 *
 * */
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
/***
 *
 * 快速排序
 * 使用方式:
 * 传入自定义Compare函数指针
 *
 * 通过分治的方式，将Key元素的左右两边分别放置小于或大于其的元素
 * 再细分进行同样的操作 直至组内只有一个元素
 *
 * */
template <typename T>
void QuickSort(T* list,size_t size,bool (*Compare)(T,T))
{
    QuickSort(list,(size_t)0,size-1,Compare);
}

/***
 *
 * 归并排序
 * 使用方式:
 * 传入自定义Compare函数指针
 *
 * 分治再统一
 *
 * */

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

/***
 *
 * 堆排序
 * 使用方式:
 * 传入自定义Compare函数指针
 *
 * 构建大顶堆并不断维护该大顶堆 顶部元素替换底部元素。
 *
 * */
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
