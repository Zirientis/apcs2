#pragma once
#include "ObjectCode.h"
class GameObject
{
private:
    ObjectCode code;
public:
    GameObject();
    GameObject(const ObjectCode);
    GameObject(const GameObject&);
    ObjectCode getCode() const;
    void setCode(ObjectCode);

    bool onBeforeWalk(GameObject&);
    void onWalk(GameObject&);
    bool onAfterWalk(GameObject&);
};
