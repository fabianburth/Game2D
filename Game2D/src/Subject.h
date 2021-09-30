#pragma once

#include "Observer.h"

#include <memory>

template<class T>
class Subject {
  public:
    virtual void registerObserver(Observer<T> *o) = 0;
    virtual void removeObserver(Observer<T> *o) = 0;
    virtual void notifyObservers() = 0;
};