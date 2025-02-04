#pragma once
#include "./Library/GameObject.h"
#include <vector>
#include <map>
#include <string>
#include <vector>
#include "./Source/Screen.h"
#include "./globals.h"
#include <stack>

using std::vector;


constexpr int STAGE_WIDTH = (Screen::WIDTH / CHA_WIDTH) % 2 ? (Screen::WIDTH / CHA_WIDTH) : (Screen::WIDTH / CHA_WIDTH) - 1;
constexpr int STAGE_HEIGHT = (Screen::HEIGHT / CHA_HEIGHT) % 2 ? (Screen::HEIGHT / CHA_HEIGHT) : (Screen::HEIGHT / CHA_HEIGHT) - 1;


class Stage :
    public GameObject
{
	vector<vector<STAGE_OBJ>> stageData;
	vector<Rect> stageRects;
	//ŒŠŒ@‚è
	Point dpos_;
	std::stack<Point> pastPos_;
	enum Direction {
		Up = 0, Down, Left, Right, MAX
	};
	void Dig();
public:
	Stage();
	~Stage();
	void Update() override;
	void Draw() override;
	STAGE_OBJ GetStageData(int x, int y) {return stageData[y][x];}
	vector<Rect> GetStageRects() { return stageRects; }
	void setStageRects();
};

