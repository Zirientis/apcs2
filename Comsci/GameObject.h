#pragma once
#include "ObjectCode.h"
class GameObject
{
private:
    ObjectCode code;
public:
    GameObject();
    GameObject(ObjectCode);
    GameObject(GameObject&);
    ObjectCode getCode();

    bool onBeforeWalk();
    bool onWalk();
    bool onAfterWalk();
};
