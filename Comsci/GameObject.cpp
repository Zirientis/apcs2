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