#include "GameObject.h"

GameObject::GameObject()
{
    code = ObjectCode::NONE;
    health = -1;
}

GameObject::GameObject(const ObjectCode c, int hp)
{
    code = c;
    health = hp;
}

GameObject::GameObject(const GameObject& g)
{
    code = g.code;
    health = g.health;
}

ObjectCode GameObject::getCode() const
{
    return code;
}

void GameObject::setCode(ObjectCode c)
{
    code = c;
}

int GameObject::getHealth()
{
    return health;
}

int GameObject::setHealth(int newHp)
{
    int oldHp = health;
    health = newHp;
    return oldHp;
}

int GameObject::changeHealth(int changeAmt)
{
    health += changeAmt;
    return health;
}

bool GameObject::isAttackable()
{
    return code >= MIN_MONST && code <= MAX_MONST;
}

int GameObject::attack(int dmg)
{
    return isAttackable() ? changeHealth(-dmg) : health;
}

bool GameObject::onBeforeWalk(GameObject* trigger)
{
    return code > ObjectCode::MAX_WALL;
    //return true;
}

// should return an ActionCode
void GameObject::onWalk(GameObject* trigger)
{
    if (code == STAIRS)
    {

    }
    return;
}

bool GameObject::onAfterWalk(GameObject* trigger)
{ 
    return true;
}
