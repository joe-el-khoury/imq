#ifndef HEALTH_HPP
#define HEALTH_HPP

#include "Cluster.hpp"

class Health
{
private:
  Cluster* cluster = &Cluster::GetInstance();

public:
  Health ();
};

#endif // HEALTH_HPP
