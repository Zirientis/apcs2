#include "GameObject.h"

GameObject::GameObject()
{
    code = ObjectCode::PLAYER;
}

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
    return code < ObjectCode::MAX_WALL;
}

bool GameObject::onWalk()
{
    return true;
}

bool GameObject::onAfterWalk()
{
    return true;
}