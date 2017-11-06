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
