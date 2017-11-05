#include <queue>

template<typename T>
class Queue
{
private:
  std::queue<T> queue_;

public:
  enum ElementPosition
  {
    Back, Front
  };

  unsigned Size ();
  
  void Push (const T&, ElementPosition);
  const T& Remove (ElementPosition);
};
