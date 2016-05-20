#include "GameObject.h"

GameObject::GameObject()
{
    code = ObjectCode::NONE;
}

GameObject::GameObject(const ObjectCode c)
{
    code = c;
}

GameObject::GameObject(const GameObject& g)
{
    code = g.code;
}

ObjectCode GameObject::getCode() const
{
    return code;
}

void GameObject::setCode(ObjectCode c)
{
    code = c;
}

bool GameObject::onBeforeWalk(GameObject& trigger)
{
    //return code <= ObjectCode::MAX_WALL;
    return true;
}

void GameObject::onWalk(GameObject& trigger)
{
    return;
}

bool GameObject::onAfterWalk(GameObject& trigger)
{
    return true;
}