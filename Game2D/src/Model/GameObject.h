#pragma once

#ifndef GAMEOBJECT_H
    #define GAMEOBJECT_H

    #include "../Constants.h"
    //#include <GL/glew.h>
    #include <array>
    #include <memory>
    #include <string>
    #include <vector>

    //#include "../View/Texture.h"
    #include "../Subject.h"


/**
 * Container object for holding all state relevant for a single
 * game object entity. Each object in the game likely needs the
 * minimal of state as described within GameObject.
 */
class GameObject : public Subject<GameObject> {
  public:
    // object state
    std::array<float, 2> position;
    std::array<float, 4> size{};

    GameObject();

    explicit GameObject(std::array<float, 2> pos, std::array<float, 4> size = {1.0f, 1.0f, 0.0f, 0.0f});

    void registerObserver(Observer<GameObject> *o) override;
    void removeObserver(Observer<GameObject> *o) override;
    void notifyObservers() override;

  private:
    std::vector<Observer<GameObject> *> observers;
};

#endif