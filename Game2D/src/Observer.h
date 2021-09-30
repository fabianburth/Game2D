#pragma once

template<class T>
class Observer {
  public:
    virtual void update(T *s) = 0;
};