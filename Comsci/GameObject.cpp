#include "GameObject.h"

GameObject::GameObject()
{
    code = ObjectCode::ZERO;
}

GameObject::GameObject(ObjectCode c)
{
    code = c;
}

GameObject::GameObject(GameObject& g)
{
    code = g.code;
}

ObjectCode GameObject::getCode()
{
    return code;
}

bool GameObject::onBeforeWalk()
{
    return code <= ObjectCode::MAX_WALL;
}

bool GameObject::onWalk()
{
    return true;
}

bool GameObject::onAfterWalk()
{
    return true;
}