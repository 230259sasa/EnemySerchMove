#include "Stage.h"
#include "./globals.h"

void Stage::Dig()
{
	dpos_ = { 1,1 };
	stageData[dpos_.y][dpos_.x] = STAGE_OBJ::EMPTY;
	pastPos_.push(dpos_);
	while (!pastPos_.empty()) {
		while (true) {
			//現在の位置から掘れる場所を確認
			std::vector<Direction> direction;
			//上
			if (dpos_.y - 2 > 0)
				if (stageData[dpos_.y - 1][dpos_.x] == STAGE_OBJ::WALL && stageData[dpos_.y - 2][dpos_.x] == STAGE_OBJ::WALL) {
					direction.push_back(Up);
				}
			//下
			if (dpos_.y + 2 < STAGE_HEIGHT)
				if (stageData[dpos_.y + 1][dpos_.x] == STAGE_OBJ::WALL && stageData[dpos_.y + 2][dpos_.x] == STAGE_OBJ::WALL) {
					direction.push_back(Down);
				}
			//左
			if (dpos_.x - 2 > 0)
				if (stageData[dpos_.y][dpos_.x - 1] == STAGE_OBJ::WALL && stageData[dpos_.y][dpos_.x - 2] == STAGE_OBJ::WALL) {
					direction.push_back(Left);
				}
			// 右
			if (dpos_.x + 2 < STAGE_WIDTH)
				if (stageData[dpos_.y][dpos_.x + 1] == STAGE_OBJ::WALL && stageData[dpos_.y][dpos_.x + 2] == STAGE_OBJ::WALL) {
					direction.push_back(Right);
				}


			//掘れなくなったら
			if (direction.size() <= 0) {
				dpos_ = pastPos_.top();
				pastPos_.pop();
				break;
			}

			//ランダムに掘
			int r = rand() % direction.size();
			switch (direction[r]) {
			case Up:
				stageData[dpos_.y - 1][dpos_.x] = STAGE_OBJ::EMPTY;
				stageData[dpos_.y - 2][dpos_.x] = STAGE_OBJ::EMPTY;
				dpos_.y -= 2;
				break;
			case Down:
				stageData[dpos_.y + 1][dpos_.x] = STAGE_OBJ::EMPTY;
				stageData[dpos_.y + 2][dpos_.x] = STAGE_OBJ::EMPTY;
				dpos_.y += 2;
				break;
			case Right:
				stageData[dpos_.y][dpos_.x + 1] = STAGE_OBJ::EMPTY;
				stageData[dpos_.y][dpos_.x + 2] = STAGE_OBJ::EMPTY;
				dpos_.x += 2;
				break;
			case Left:
				stageData[dpos_.y][dpos_.x - 1] = STAGE_OBJ::EMPTY;
				stageData[dpos_.y][dpos_.x - 2] = STAGE_OBJ::EMPTY;
				dpos_.x -= 2;
				break;
			}
			if (direction.size() > 1)
				pastPos_.push(dpos_);
			direction.clear();
		}
	}
}

Stage::Stage()
{
	stageData = vector(STAGE_HEIGHT, vector<STAGE_OBJ>(STAGE_WIDTH, STAGE_OBJ::EMPTY));

	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (y == 0 || y == STAGE_HEIGHT - 1 || x == 0 || x == STAGE_WIDTH - 1)
			{
				stageData[y][x] = STAGE_OBJ::WALL;
			}
			else
			{
				if (x % 2 == 0 && y % 2 == 0)
					stageData[y][x] = STAGE_OBJ::WALL;
				else
					stageData[y][x] = STAGE_OBJ::EMPTY;
			}
			stageData[y][x] = STAGE_OBJ::WALL;
		}
	}

	//穴掘り
	Dig();

	//ループ作成
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (x + 1 < STAGE_WIDTH && x - 1 > 0 && y -1 > 0 && y + 1 < STAGE_HEIGHT) {
				if ((stageData[y][x - 1] == STAGE_OBJ::EMPTY && stageData[y][x + 1] == STAGE_OBJ::EMPTY &&
					stageData[y - 1][x] == STAGE_OBJ::WALL && stageData[y + 1][x] == STAGE_OBJ::WALL) ||
					(stageData[y][x - 1] == STAGE_OBJ::WALL && stageData[y][x + 1] == STAGE_OBJ::WALL &&
						stageData[y - 1][x] == STAGE_OBJ::EMPTY && stageData[y + 1][x] == STAGE_OBJ::EMPTY)) {
					int r = rand() % 5;
					if (r == 0) {
						stageData[y][x] = STAGE_OBJ::EMPTY;
					}
				}
			}

		}
	}


	setStageRects();
}

Stage::~Stage()
{
}

void Stage::Update()
{
}

void Stage::Draw()
{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			switch (stageData[y][x])
			{
			case STAGE_OBJ::EMPTY:
				DrawBox(x * CHA_WIDTH, y * CHA_HEIGHT, x * CHA_WIDTH + CHA_WIDTH, y * CHA_HEIGHT + CHA_HEIGHT, GetColor(102, 205, 170), TRUE);
				break;
			case STAGE_OBJ::WALL:
				DrawBox(x * CHA_WIDTH, y * CHA_HEIGHT, x * CHA_WIDTH + CHA_WIDTH, y * CHA_HEIGHT + CHA_HEIGHT, GetColor(119, 136, 153), TRUE);
				break;
			case STAGE_OBJ::GOAL:
			
				break;
			default:
				break;
			}
		}
	}
}

void Stage::setStageRects()
{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (stageData[y][x] == STAGE_OBJ::WALL)
			{
				stageRects.push_back(Rect(x * CHA_WIDTH, y * CHA_HEIGHT,  CHA_WIDTH, CHA_HEIGHT));
			}
		}
	}

}
