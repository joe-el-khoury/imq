#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <queue>

template<typename T>
class Queue
{
private:
  std::queue<T> queue_;

public:
  unsigned Size ();
  
  void Push (const T&);
  const T& Pop ();
};

#endif // QUEUE_HPP
