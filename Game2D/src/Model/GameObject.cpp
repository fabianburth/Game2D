#include "GameObject.h"


GameObject::GameObject() : position({0.0f, 0.0f}) {}

GameObject::GameObject(std::array<float, 2> pos, std::array<float, 4> size) : position{pos}, size{size} {}

void GameObject::registerObserver(Observer<GameObject> *observer) {
    observers.push_back(observer);
}

void GameObject::removeObserver(Observer<GameObject> *observer) {
    observers.erase(std::remove_if(observers.begin(), observers.end(),
                                   [&](Observer<GameObject> *comparison) { return comparison == observer; }),
                    observers.end());
}

void GameObject::notifyObservers() {
    for (Observer<GameObject> *observer : observers) {
        observer->update(this);
    }
}