//
// Created by Zhenyu on 2021/8/8.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_ARRAYQUEUE_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_ARRAYQUEUE_HPP_

#include <iostream>

namespace SESAME{
template <class T> class ArrayQueue;
typedef std::shared_ptr<ArrayQueue<class T>> ArrayQueuePtr;
template<class T> class ArrayQueue{
  public:
  explicit ArrayQueue(unsigned int size);
  ~ArrayQueue();
  void add(T t);
  T front();
  T back();
  T pop();
  T Element(unsigned int i);
  int size();
  int isEmpty();
  bool isExist(T element);

 private:
  T *arr=NULL;//TODO :think how to replace *arr
  int count=0;
  unsigned int length=20;
};

// create queue with default size is 20
template<class T>
[[maybe_unused]] ArrayQueue<T>::ArrayQueue( unsigned int size)
{
  length=size;
  arr = new T[length];
  if (!arr)
  {
    std::cout<<"arr malloc error!"<<std::endl;
  }
}

// destroy shapshot queue
template<class T>
ArrayQueue <T>::~ArrayQueue()
{
  if (arr)
  {
    delete[] arr;
    arr = NULL;
  }
}

// add val to the end of queue
template<class T>
void ArrayQueue<T>::add(T t)
{
  arr[count++] = t;
}


// return the front element of queue
template<class T>
T ArrayQueue<T>::front()
{
  return arr[0];
}

// return the front element of queue
template<class T>
T ArrayQueue<T>::back()
{
  return arr[count-1];
}
template<class T>
T ArrayQueue<T>::Element(unsigned int i)
{
  // T element=0;
  // if(i>=count)
  //     std::cout<<"Illegal index! Index must below queue size!";
  //   else

  return  arr[i];
}

//return and delete the back element of queue
template<class T>
T ArrayQueue<T>::pop()
{
  int i = 0;
  T ret = arr[0];

  count--;
  while (i++<count)
    arr[i-1] = arr[i];

  return ret;
}

// return the size of queue
template<class T>
int ArrayQueue<T>::size()
{
  return count;
}

// return the judge of whether this queue is empty
template<class T>
int ArrayQueue<T>::isEmpty()
{
  return count==0;
}


// return the judge of whether this queue is empty
template<class T>
bool ArrayQueue<T>::isExist(T element)
{
  bool existence=false;
  for(int i=0;i<count;i++)
  {
    if(arr[i]==element)
      existence=true;
  }
  return existence;
}

}


#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_ARRAYQUEUE_HPP_
