#pragma once
#include "./Library/GameObject.h"
#include "./globals.h"


class Enemy :
    public GameObject
{
    Point pos_;
    Point dir_;
    bool isAlive_;
    float speed_;
    int moveVal_;
    DIR forward_;
public:
    Enemy();
    ~Enemy();

    void Update() override;
    void Draw() override;
    bool CheckHit(const Rect& me, const Rect& other);
private:
    void RightHandMove();
    void RandomMove();
};

