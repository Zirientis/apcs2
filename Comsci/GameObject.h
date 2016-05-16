#pragma once
#include "ObjectCode.h"
class GameObject
{
private:
    ObjectCode code;
public:
    GameObject(ObjectCode);
    ObjectCode getCode();

    bool onBeforeWalk();
    bool onWalk();
    bool onAfterWalk();
};

GameObject::GameObject(ObjectCode c)
{
    code = c;
}

ObjectCode GameObject::getCode()
{
    return code;
}

bool GameObject::onBeforeWalk()
{
    return true;
}

bool GameObject::onWalk()
{
    return true;
}

bool GameObject::onAfterWalk()
{
    return true;
}