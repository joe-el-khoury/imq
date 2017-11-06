#include "Queue.hpp"

#include <stdexcept>

template<typename T>
unsigned Queue<T>::Size()
{
  return queue_.size();
}

template<typename T>
void Queue<T>::Push (const T& element)
{
  queue_.push(element);
}

template<typename T>
const T& Queue<T>::Pop ()
{
  if (Size() == 0) {
    throw std::out_of_range("");
  }

  const T& popped = queue_.front();
  queue_.pop();
  return popped;
}
