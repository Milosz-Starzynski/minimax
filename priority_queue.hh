#pragma once
#include "heap.hh"

template<typename T>
class PriorityQueue{
  Heap<T> H;

  public:
  int size() const {return H.size();}
  bool empty() const {return size() == 0;}
  T& min() {return H.root()->element;}
  int min_key() {return H.root()->key;}
  void insert(T& new_element, int new_key){
    H.addLast(new_element, new_key);
    H.upHeapBubling();
  }
  
  void removeMin(){
    if(size() == 1)
      H.removeLast();
    else{
      typename Heap<T>::Node *u = H.root();
      H.swap(*u, *(H.last()));
      H.removeLast();
      H.downHeapBubling();
    }
  }
  
};
