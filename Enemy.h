#pragma once
#include "./Library/GameObject.h"
#include "./globals.h"
#include<queue>
#include"Stage.h"

class Enemy :
    public GameObject
{
    Point pos_;
    Point dir_;
    bool isAlive_;
    float speed_;
    int moveVal_;
    DIR forward_;

    std::queue<Point> queue;
public:
    Enemy();
    ~Enemy();

    void Update() override;
    void Draw() override;
    bool CheckHit(const Rect& me, const Rect& other);
private:
    void RightHandMove();
    void RandomMove();
    void XCloseMove();
    void YCloseMove(); 
    void CloseMove();
    void BFSMove();
};

