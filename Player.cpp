#include "Player.h"
#include "./Source/Input.h"
#include "./Stage.h"
#include "globals.h"

namespace {
	//enum DIR
	//{
	//	UP,
	//	DOWN,
	//	LEFT,
	//	RIGHT,
	//	NONE,
	//	MAXDIR
	//};
	DIR inputDir = NONE;
}


Player::Player()
	: pos_({ CHA_WIDTH * 5, CHA_HEIGHT * 3 }), playerImage_(-1)
{
}

Player::~Player()
{
}

void Player::Update()
{
	int ox = pos_.x, oy = pos_.y;

	if (Input::IsKeepKeyDown(KEY_INPUT_UP))
	{
		pos_.y--;
		inputDir = UP;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_DOWN))
	{
		pos_.y++;
		inputDir = DOWN;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_LEFT))
	{
		pos_.x--;
		inputDir = LEFT;
	}
	else if (Input::IsKeepKeyDown(KEY_INPUT_RIGHT))
	{
		pos_.x++;
		inputDir = RIGHT;
	}

	Stage* stage = (Stage*)FindGameObject<Stage>();
	Rect playerRect = { pos_.x, pos_.y, CHA_WIDTH, CHA_HEIGHT };

	for (auto& obj : stage->GetStageRects())
	{
		if (CheckHit(playerRect, obj))
		{
			Rect tmpRectX = { ox, pos_.y, CHA_WIDTH, CHA_HEIGHT };
			Rect tmpRecty = { pos_.x, oy, CHA_WIDTH, CHA_HEIGHT };
			//x軸方向で引っ掛かった
			if (!CheckHit(tmpRectX, obj))
			{
				pos_.x = ox;//x軸方向にめり込み修正
				//壁ズリ
				Point centerMe = Rect{ pos_.x, pos_.y, CHA_WIDTH, CHA_HEIGHT }.GetCenter();
				Point centerObj = obj.GetCenter();
				if (centerMe.y > centerObj.y)
				{
					pos_.y++;
				}
				if (centerMe.y < centerObj.y)
				{
					pos_.y--;
				}
			}
			else if (!CheckHit(tmpRecty, obj))
			{
				pos_.y = oy;//y方向に引っかかったらめり込み修正
				//壁ズリ
				Point centerMe = Rect{ pos_.x, pos_.y, CHA_WIDTH, CHA_HEIGHT }.GetCenter();
				Point centerObj = obj.GetCenter();
				if (centerMe.x > centerObj.x)
				{
					pos_.x++;
				}
				if (centerMe.x < centerObj.x)
				{
					pos_.x--;
				}
			}
			else
			{
				//それ以外で引っ掛かった時（いつやん）
				pos_.x = ox;
				pos_.y = oy;
			}
		}
	}
}

void Player::Draw()
{
	DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(255, 10, 10), TRUE);
	DrawFormatString(0, 20, GetColor(0, 0, 0), "%d.%d", pos_.x/CHA_WIDTH,pos_.y/CHA_HEIGHT);
}

bool Player::CheckHit(const Rect& me, const Rect& other)
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
