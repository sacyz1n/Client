#pragma once
#include "IComponent.h"
class Model :
    public IComponent
{
public:
    void Render(const Vector2& _position);
};

