#pragma once
#include "ObjectCode.h"
#include "ActionCode.h"
class GameObject
{
private:
    ObjectCode code;
    int health;
public:
    GameObject(); // NONE
    GameObject(const ObjectCode, int);
    GameObject(const GameObject&);
    ObjectCode getCode() const;
    void setCode(ObjectCode);
    int getHealth();
    int setHealth(int);
    int changeHealth(int);
    bool isAttackable();
    int attack(int);

    bool onBeforeWalk(GameObject*);
    ActionCode onWalk(GameObject*);
    bool onAfterWalk(GameObject*);
};
