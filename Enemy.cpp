#include "Enemy.h"
#include "./Stage.h"
#include "globals.h"
#include "Player.h"

bool operator == (Point p1, Point p2) {
    if (p1.x == p2.x && p1.y == p2.y)
        return true;
    return false;
}

Enemy::Enemy()
    :pos_({ 0,0 }), isAlive_(true),timer_(60.0f*5.0f),routeCount_(1)
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
    //RandomMove();
    RightHandMove();
    /*timer_ -= 1;
    if ((timer_ <=  0.0f && moveVal_ == 0) || (routeCount_ >= route_.size() && moveVal_ >= CHA_WIDTH)) {
        DS();
        timer_ = 60 * 5;
    }
    RouteMove();*/
}

void Enemy::Draw()
{
    static int r = 0;
    r++;
    int c = r / 20;
    int count = 0;
    for (auto itr : route_) {
        //if (count >= c)
       //     break;
        DrawBox(itr.x * CHA_WIDTH, itr.y * CHA_HEIGHT, itr.x * CHA_WIDTH + CHA_WIDTH, itr.y * CHA_HEIGHT + CHA_HEIGHT,
            GetColor(0, 205, 100), TRUE);
        count++;
    }
    DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(80, 89, 10), TRUE);
    DrawFormatString(0, 0, GetColor(255, 255, 255), "%d", route_.size());
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
    Point c = { pos_.x + move.x + CHA_WIDTH / 2, pos_.y + move.y + CHA_HEIGHT / 2 };
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
        //forward_ = (DIR)(GetRand(3));
        CloseMove();
    }
}

void Enemy::XCloseMove()
{
    Player* player = (Player*)FindGameObject<Player>();
    if (player == nullptr)
        return;

    if (rand() % 4 == 0) {
        if(rand()%2==0)
            forward_ = DIR::DOWN;
        else
            forward_ = DIR::UP;
        return;
    }

    if (player->GetPos().x > pos_.x) {
        forward_ = DIR::RIGHT;
    }
    else {
        forward_ = DIR::LEFT;
    }
}

void Enemy::YCloseMove()
{
    Player* player = (Player*)FindGameObject<Player>();
    if (player == nullptr)
        return;

    if (rand() % 4 == 0) {
        if (rand() % 2 == 0)
            forward_ = DIR::LEFT;
        else
            forward_ = DIR::RIGHT;
        return;
    }

    if (player->GetPos().y > pos_.y) {
        forward_ = DIR::DOWN;
    }
    else {
        forward_ = DIR::UP;
    }
}

void Enemy::CloseMove()
{
    Player* player = (Player*)FindGameObject<Player>();
    if (player == nullptr)
        return;

    int ranNum = rand() % 3;

    if (ranNum == 0) {
        int dir = rand() % DIR::MAXDIR;
        switch (dir) {
        case DIR::UP:forward_ = DIR::UP; break;
        case DIR::DOWN:forward_ = DIR::DOWN; break;
        case DIR::LEFT:forward_ = DIR::LEFT; break;
        case DIR::RIGHT:forward_ = DIR::RIGHT; break;
        }
        return;
    }
    else if (ranNum == 1) {
        float xdis = player->GetPos().x - pos_.x;
        float ydis = player->GetPos().y - pos_.y;
        if (abs(xdis) > abs(ydis)) {
            if (player->GetPos().x > pos_.x) {
                forward_ = DIR::RIGHT;
            }
            else {
                forward_ = DIR::LEFT;
            }
        }
        else {
            if (player->GetPos().y > pos_.y) {
                forward_ = DIR::DOWN;
            }
            else {
                forward_ = DIR::UP;
            }
        }
    }
}

void Enemy::BFS()
{
    Player* player = (Player*)FindGameObject<Player>();
    if (player == nullptr)
        return;
    Stage* stage = (Stage*)FindGameObject<Stage>();
    if (stage == nullptr)
        return;


    std::vector<std::vector<int>> smap;
    std::vector<int> m;
    for (int i = 0; i < STAGE_WIDTH; i++) {
        m.push_back(-1);
    }
    for (int i = 0; i < STAGE_HEIGHT; i++) {
        smap.push_back(m);
    }
    m.clear();
    std::vector<Point> route;
    route_.clear();
    Point goal = { player->GetPos().x / CHA_WIDTH,player->GetPos().y / CHA_HEIGHT };
    Point start = { pos_.x / CHA_WIDTH, pos_.y / CHA_HEIGHT };

    std::queue<Point> queue;
    while(!queue.empty())
        queue.pop();
    queue.push(start);
    int sNum = 0;
    int qSize = queue.size();
    int qCount = 0;
    routeCount_ = 1;
    moveVal_ = 100;
    while (!queue.empty()) {
        Point pos = queue.front();
        smap[pos.y][pos.x] = sNum;
        //goal
        if (goal == pos) {
            break;
        }
        if (qCount % qSize == 0) {
            sNum++;
            qCount = 0;
        }
        queue.pop();
        if (pos.y - 1 > 0) {
            Point p = pos;
            p.y -= 1;
            if (stage->GetStageData(p.x,p.y) != STAGE_OBJ::WALL && (smap[p.y][p.x] > sNum || smap[p.y][p.x] < 0)) {
                queue.push(p);
            }
        }
        if (pos.y + 1 < STAGE_HEIGHT) {
            Point p = pos;
            p.y += 1;
            if (stage->GetStageData(p.x, p.y) != STAGE_OBJ::WALL && (smap[p.y][p.x] > sNum || smap[p.y][p.x] < 0)) {
                queue.push(p);
            }
        }
        if (pos.x - 1 > 0) {
            Point p = pos;
            p.x -= 1;
            if (stage->GetStageData(p.x, p.y) != STAGE_OBJ::WALL && (smap[p.y][p.x] > sNum || smap[p.y][p.x] < 0)) {
                queue.push(p);
            }
        }
        if (pos.x + 1 < STAGE_WIDTH) {
            Point p = pos;
            p.x += 1;
            if (stage->GetStageData(p.x, p.y) != STAGE_OBJ::WALL && (smap[p.y][p.x] > sNum || smap[p.y][p.x] < 0)) {
                queue.push(p);
            }
        }

        if (qCount % qSize == 0)
            qSize = queue.size();

        qCount++;
    }
    //route
    Point nPos = goal;
    if (smap[nPos.y][nPos.x] < 0)return;
    int count = 0;
    struct ROUTE_DIR {
        Point pos;
        int c;
    };
    while (true) {
        //routeにnposを入れる
        route.push_back(nPos);
        if (nPos == start) {
            break;
        }
        //上下左右で自分より1小さい数を探す
        std::vector<ROUTE_DIR> r;
        ROUTE_DIR tmp;
        if (nPos.x - 1 > 0) {
            tmp.c = smap[nPos.y][nPos.x - 1];
            tmp.pos = { nPos.x - 1,nPos.y };
            r.push_back(tmp);
        }
        if (nPos.x + 1 < STAGE_WIDTH) {
            tmp.c = smap[nPos.y][nPos.x + 1];
            tmp.pos = { nPos.x + 1,nPos.y };
            r.push_back(tmp);
        }
        if (nPos.y - 1 > 0) {
            tmp.c = smap[nPos.y - 1][nPos.x];
            tmp.pos = { nPos.x,nPos.y - 1 };
            r.push_back(tmp);
        }
        if (nPos.y + 1 < STAGE_HEIGHT) {
            tmp.c = smap[nPos.y + 1][nPos.x];
            tmp.pos = { nPos.x,nPos.y + 1 };
            r.push_back(tmp);
        }
        for (auto itr:r) {
            if (itr.c == smap[nPos.y][nPos.x] - 1) {
                //そこへ移動 nposにその座標を入れる
                nPos = itr.pos;
                break;
            }
        }
        count++;
    }
    //routeの配列反転

    for (int i = route.size() - 1; i >= 0;i--) {
        route_.push_back(route[i]);
    }
    route.clear();
    smap.clear();
}

void Enemy::DS()
{
    Player* player = (Player*)FindGameObject<Player>();
    if (player == nullptr)
        return;
    Stage* stage = (Stage*)FindGameObject<Stage>();
    if (stage == nullptr)
        return;

    using Pair = std::pair<int, std::pair<int, int>>;
    std::priority_queue<Pair, std::vector<Pair>
        , std::greater<Pair>> queue;
    std::vector<Point> route;
    route_.clear();
    routeCount_ = 0;
    moveVal_ = 100;
    Point goal = { player->GetPos().x / CHA_WIDTH,player->GetPos().y / CHA_HEIGHT };
    Point start = { pos_.x / CHA_WIDTH, pos_.y / CHA_HEIGHT };
    Pair pair;
    pair.first = 0;
    pair.second.first = start.x;
    pair.second.second = start.y;
    queue.push(pair);
    struct Edge {
        Point to;
        int cost;
    };
    enum CMapData {
        C_WALL=-1
    };
    std::vector<std::vector<Edge>> dMap;
    std::vector<std::vector<int>> cMap;
    for (int y = 0; y < STAGE_HEIGHT; y++) {
        std::vector<int> m;
        std::vector<Edge> ed;
        for (int x = 0; x < STAGE_WIDTH; x++) {
            if (stage->GetStageData(x, y) == STAGE_OBJ::WALL) {
                m.push_back((int)C_WALL);
            }
            else {
                m.push_back(1);
            }
            Edge e;
            e.to = { 0, 0 };
            e.cost = 1000000;
            ed.push_back(e);
        }
        cMap.push_back(m);
        dMap.push_back(ed);
    }
    cMap[start.y][start.x] = 0;

    while (!queue.empty()) {
        int cost = queue.top().first;
        Point pos = { queue.top().second.first,
            queue.top().second.second };
        queue.pop();
        route.push_back(pos);

        Edge edge[4];
        for (int i = 0; i < 4; i++) {
            edge[i].cost = -1;
            edge[i].to = Point{ -1, -1 };
        }

        if (pos.x - 1 >= 0) {
            Point p{ pos.x - 1, pos.y };
            if (cMap[p.y][p.x] >= 0) {
                edge[0].cost = cost + cMap[p.y][p.x];
                edge[0].to = p;
            }
        }
        if (pos.x + 1 < cMap[0].size()) {
            Point p{ pos.x + 1, pos.y };
            if (cMap[p.y][p.x] >= 0) {
                edge[1].cost = cost + cMap[p.y][p.x];
                edge[1].to = p;
            }
        }
        if (pos.y - 1 >= 0) {
            Point p{ pos.x, pos.y - 1 };
            if (cMap[p.y][p.x] >= 0) {
                edge[2].cost = cost + cMap[p.y][p.x];
                edge[2].to = p;
            }
        }
        if (pos.y + 1 < cMap.size()) {
            Point p{pos.x, pos.y + 1};
            if (cMap[p.y][p.x] >= 0) {
                edge[3].cost = cost + cMap[p.y][p.x];
                edge[3].to = p;
            }
        }

        for (int i = 0; i < 4; i++) {
            if (edge[i].cost < 0)
                continue;
            if (dMap[edge[i].to.y][edge[i].to.x].cost < edge[i].cost)
                continue;
            Pair p;
            p.first = edge[i].cost;
            p.second.first = edge[i].to.x;
            p.second.second = edge[i].to.y;
            dMap[edge[i].to.y][edge[i].to.x].cost = edge[i].cost;
            dMap[edge[i].to.y][edge[i].to.x].to = pos;
            queue.push(p);
        }
    }

    Point gpos = dMap[goal.y][goal.x].to;
    Point pastPos{ -1, -1 };
    std::vector<Point> r;
    r.push_back(goal);
    while (true) {
        if (start == gpos || pastPos == gpos)
            break;
        r.push_back(gpos);
        pastPos = gpos;
        gpos = dMap[gpos.y][gpos.x].to;
    }
    for (int i = r.size() - 1; i >= 0; i--) {
        route_.push_back(r[i]);
    }
}

void Enemy::RouteMove()
{
    Point nDir[4] = { {0,-1},{0,1},{-1,0},{1,0} };
    Point p;
    p.x = pos_.x / CHA_WIDTH;
    p.y = pos_.y / CHA_HEIGHT;
    
    if (routeCount_ < route_.size() && moveVal_ >= CHA_WIDTH) {
        Point route = route_[routeCount_];
        moveVal_ = 0;
        if (p.x < route.x) {
            forward_ = DIR::RIGHT;
        }
        else if (p.x > route.x) {
            forward_ = DIR::LEFT;
        }
        else if (p.y > route.y) {
            forward_ = DIR::UP;
        }
        else if (p.y < route.y) {
            forward_ = DIR::DOWN;
        }
        routeCount_++;
    }
    else if (routeCount_ > route_.size() || moveVal_ >= CHA_WIDTH) {
        return;
    }
    else {
        pos_.x += nDir[forward_].x;
        pos_.y += nDir[forward_].y;
        moveVal_ += 1;
    }

}
