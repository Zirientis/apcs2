#include "GameObject.h"
#include "ObjectCode.h"
#include "ActionCode.h"

GameObject::GameObject()
{
    code = ObjectCode::NONE;
    health = -1;
    actionPerformed = false;
}

GameObject::GameObject(const ObjectCode c)
{
    code = c;
    health = GetDefaultHealth(c);
    actionPerformed = true;
}

GameObject::GameObject(const ObjectCode c, double factorDefaultHealth)
{
    code = c;
    health = (int)(factorDefaultHealth * GetDefaultHealth(c));
    actionPerformed = true;
}

GameObject::GameObject(const ObjectCode c, int hp) // expert use only!
{
    code = c;
    health = hp;
    actionPerformed = true;
}

GameObject::GameObject(const GameObject& g)
{
    code = g.code;
    health = g.health;
    actionPerformed = g.actionPerformed;
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
    return IsCodeMonst(code) || IsCodePlayer(code);
}

int GameObject::attack(int dmg)
{
    return isAttackable() ? changeHealth(-dmg) : health;
}

bool GameObject::isActionPerformed()
{
    return actionPerformed;
}

void GameObject::setActionPerformed(bool state)
{
    actionPerformed = state;
}

bool GameObject::onBeforeWalk(GameObject* trigger)
{
    return !IsCodeWall(code);
    //return true;
}

// should return an ActionCode
ActionCode GameObject::onWalk(GameObject* trigger)
{
    if (code == STAIRS)
    {
        return ActionCode::AC_STAIR_TRIGGERED;
    }
    return ActionCode::AC_NONE;
}

bool GameObject::onAfterWalk(GameObject* trigger)
{ 
    return true;
}
