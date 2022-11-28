#ifndef SAMPLESORT_H
#define SAMPLESORT_H

#include <iostream>
#include <thread>
#include <limits>



template <typename T>
struct thread_unit{
    int id;                 /* 线程id */
    size_t upper;           /* 上界 */
    size_t lower;           /* 下届 */
    size_t count;           /* 大小 */
    T upperValue;           /* 最大值 */
    T lowerValue;           /* 最小值 */
    T* unsorted;            /* 未排序数组 */
    size_t unsorted_length; /* 未排序数组大小 */
    T* sorted;              /* 已排序数组 */
};

template <typename T>
void SampleSort(T* array,size_t size,int thread_num);
template <typename T>
void spliter(T* array,T* candidate,T* final,size_t size,int thread_num);
template <typename T>
void count(T* array,T* final,T* number,size_t size,int thread_num);
template <typename T>
void threadSampleSort(thread_unit<T>* _unit);
template <typename T>
void findBounds(T* bucketsSize,T* sortedBoundOfBuckets,int thread_num);

int (*compare)(const void*,const void*);

/***
 *
 * 样本排序
 *
 * 使用方式：
 * 传入自定义Compare函数指针
 *
 * 一般用于大量数据的多线程排序
 *
 * */
template <typename T>
void SampleSort(T* array,size_t size,int thread_num,int (*Compare)(const void*,const void*))
{
    compare = Compare;
    T* candidate = new T[thread_num*(thread_num-1)];
    T* final = new T[thread_num-1];
    spliter(array,candidate,final,size,thread_num);
    T* number = new T[thread_num];
    count(array,final,number,size,thread_num);
    T* sorted = new T[size];
    T* sortedBuckets = new T[thread_num+1];
    findBounds(number,sortedBuckets,thread_num);
    auto* units = new thread_unit<T>[thread_num];
    for(int i=0;i<thread_num;i++){
        thread_unit<T> unit;
        unit.id = i;
        unit.upper = sortedBuckets[i+1];
        unit.lower = sortedBuckets[i];
        unit.count = number[i];
        unit.unsorted = array;
        unit.unsorted_length = size;
        unit.sorted = sorted;
        if(i==0){
            unit.upperValue = final[i];
            unit.lowerValue = std::numeric_limits<T>::min();
        }else if(i==thread_num-1){
            unit.upperValue = std::numeric_limits<T>::max();
            unit.lowerValue = final[i-1];
        }else{
            unit.upperValue = final[i];
            unit.lowerValue = final[i-1];
        }
        if(i==0&&i==thread_num-1){
            unit.upperValue = std::numeric_limits<T>::max();
            unit.lowerValue = std::numeric_limits<T>::min();
        }
        units[i] = unit;
    }
    auto threads = new std::thread[thread_num];
    for(int i=0;i<thread_num;i++){
        threads[i] = std::thread(threadSampleSort<T>,(units+i));
    }
    for(int i=0;i<thread_num;i++){
        threads[i].join();
    }
    for(size_t i=0;i<size;i++){
        array[i] = sorted[i];
    }
    delete []candidate;
    delete []sortedBuckets;
    delete []final;
    delete []number;
    delete []units;
    delete []threads;
}
template <typename T>
void spliter(T* array,T* candidate,T* final,size_t size,int thread_num)
{
    size_t gap = size/(thread_num*thread_num);
    size_t bucketlen;
    for(int i=0;i<thread_num*thread_num;i++){
        if(i%thread_num == 0){
            bucketlen = (i+gap*thread_num > size) ? size-i : gap*thread_num;
            qsort(array+i,bucketlen,sizeof(T), compare);
        }
    }
    int index = 0;
    for(int i=0;i<thread_num*thread_num;i++){
        if(i%thread_num != 0){
            candidate[index] = array[i*gap];
            index++;
        }
    }
    qsort(candidate,(thread_num-1)*thread_num,sizeof(T),compare);
    for(int i=1;i<thread_num;i++){
        final[i-1] = candidate[i*thread_num-1];
    }
}
template <typename T>
void count(T* array,T* final,T* number,size_t size,int thread_num)
{
    for(int i=0;i<thread_num+1;i++){
        number[i] = 0;
    }
    int flag = 0;
    for(size_t i=0;i<size;i++){
        flag = 0;
        for(int j=0;j<thread_num-1;j++){
            if(final[j] >= array[i]){
                flag = 1;
                number[j]++;
                break;
            }
        }
        if(flag == 0){
            number[thread_num-1]++;
        }
    }
}
template <typename T>
void findBounds(T* bucketsSize,T* sortedBoundOfBuckets,int thread_num)
{
    sortedBoundOfBuckets[0] = 0;
    for(int i=1;i<thread_num+1;i++){
        sortedBoundOfBuckets[i] = sortedBoundOfBuckets[i-1] + bucketsSize[i-1];
    }
}
template <typename T>
void threadSampleSort(thread_unit<T>* _unit)
{
    thread_unit<T> unit = *_unit;
    T* local = new T[unit.count];
    size_t index = 0;
    for(size_t i=0;i<unit.unsorted_length;i++){
        if(unit.unsorted[i] <= unit.upperValue && unit.unsorted[i] > unit.lowerValue){
            local[index] = unit.unsorted[i];
            index++;
        }
    }
    qsort(local,unit.count,sizeof(T), compare);
    index = 0;
    for(size_t i=unit.lower;i<unit.upper;i++){
        unit.sorted[i] = local[index];
        index++;
    }
    delete []local;
}
#endif
