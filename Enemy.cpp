#include "Enemy.h"
#include "./Stage.h"
#include "globals.h"

Enemy::Enemy()
    :pos_({ 0,0 }), isAlive_(true)
{
    int rx = GetRand(STAGE_WIDTH * CHA_WIDTH);
    int ry = GetRand(STAGE_HEIGHT * CHA_HEIGHT);
    rx = (STAGE_WIDTH - 2) * CHA_HEIGHT;
    ry = (STAGE_HEIGHT - 2) * CHA_HEIGHT;
    pos_ = { rx, ry };
    dir_ = { -1,0 };
    speed_ = 1;
    moveVal_ = 0;

    forward_ = RIGHT;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
    RandomMove();
}

void Enemy::Draw()
{
    DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(80, 89, 10), TRUE);
}

bool Enemy::CheckHit(const Rect& me, const Rect& other)
{
    if (me.x < other.x + other.w &&
        me.x + me.w > other.x &&
        me.y < other.y + other.h &&
        me.y + me.h > other.y)
    {
        return true;
    }
    return false;
}

void Enemy::RightHandMove()
{
    Point move = { speed_ * dir_.x, speed_ * dir_.y };

    Stage* stage = (Stage*)FindGameObject<Stage>();
    //移動先でブロックと衝突しているか
    //右回転させたdir
    Point rDir = dir_;
    rDir.x = dir_.y * -1;
    rDir.y = dir_.x;
    //左回転
    Point lDir = dir_;
    lDir.x = dir_.y;
    lDir.y = dir_.x * -1;
    //中心位置
    Point c = { pos_.x + move.x + CHA_WIDTH / 2,
                pos_.y + move.y + CHA_HEIGHT / 2 };
    Point rc = c;
    //当たり判定をする点を求める
    c.x += (CHA_WIDTH / 2) * dir_.x;
    c.y += (CHA_HEIGHT / 2) * dir_.y;
    //STAGE上の位置
    Point p = { c.x / CHA_WIDTH,c.y / CHA_HEIGHT };
    Point rp = { rc.x / CHA_WIDTH,rc.y / CHA_HEIGHT };

    if (abs(rDir.x) > abs(rDir.y)) {
        rp.x += rDir.x;
        rp.y -= rDir.y;
    }
    else {
        rp.x -= rDir.x;
        rp.y += rDir.y;
    }

    //衝突していたら右にdir_を回す。moveの移動量調節
    if (stage->GetStageData(p.x, p.y) == STAGE_OBJ::WALL) {
        moveVal_ = 0;
        if (stage->GetStageData(rp.x, rp.y) == STAGE_OBJ::EMPTY) {
            dir_ = rDir;
        }
        else {
            dir_ = lDir;
        }
    }
    else if (moveVal_ >= CHA_WIDTH) {
        moveVal_ = 0;
        //衝突していないなら右のブロックを確認

        if (stage->GetStageData(rp.x, rp.y) == STAGE_OBJ::EMPTY) {
            //ブロックがないならdir_を右に回す
            dir_ = rDir;
        }
    }
    else {
        //移動
        pos_.x += move.x;
        pos_.y += move.y;
        moveVal_ += abs(move.x) + abs(move.y);
    }
}

void Enemy::RandomMove()
{
    static bool stop = false;
    Point nDir[4] = { {0,-1},{0,1},{-1,0},{1,0} };

    if (!stop) {
        Point op = pos_;
        Point move = { nDir[forward_].x, nDir[forward_].y };
        Rect eRect = { pos_.x, pos_.y,CHA_WIDTH, CHA_HEIGHT };
        Stage* stage = (Stage*)FindGameObject<Stage>();
        pos_ = { pos_.x + move.x, pos_.y + move.y };
        for (auto& obj : stage->GetStageRects())
        {
            if (CheckHit(eRect, obj)) {
                Rect tmpRectX = { op.x, pos_.y, CHA_WIDTH, CHA_HEIGHT };
                Rect tmpRecty = { pos_.x, op.y, CHA_WIDTH, CHA_HEIGHT };
                if (!CheckHit(tmpRectX, obj))
                {
                    pos_.x = op.x;
                }
                else if (!CheckHit(tmpRecty, obj))
                {
                    pos_.y = op.y;
                }
                else
                {
                    pos_ = op;
                }
                forward_ = (DIR)(GetRand(3));
                break;
            }
        }
    }
    int prgssx = pos_.x % (CHA_WIDTH);
    int prgssy = pos_.y % (CHA_HEIGHT);
    int cx = (pos_.x / (CHA_WIDTH)) % 2;
    int cy = (pos_.y / (CHA_HEIGHT)) % 2;
    if (prgssx == 0 && prgssy == 0 && cx && cy)
    {
        forward_ = (DIR)(GetRand(3));
    }
}
