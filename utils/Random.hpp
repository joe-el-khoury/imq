#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

class Random
{
private:
  Random () {}

public:
  static Random& GetInstance ()
  {
    static Random instance;
    return instance;
  }

  int GenerateRandomNumber ()
  {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dis;
    
    return dis(rng);
  }

  int GenerateRandomNumber (int lo, int hi)
  {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dis(lo, hi);
    
    return dis(rng);
  }
};

#endif // RANDOM_HPP
