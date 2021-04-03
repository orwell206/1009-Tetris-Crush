#pragma once
#include <string>
using namespace std;

class Player
{

public:
	Player();
	struct PlayerInfo
	{
		int playerScore;
		string playerName;
	};
	PlayerInfo playerInfo;
	int awardPoints(int, int);
	Player operator+(Player);
};
Player::Player() {}
int Player::awardPoints(int currentPlayerScore, int lines)
{
	int tempPlayerPoints;
	tempPlayerPoints = currentPlayerScore + (20 * lines);
	return tempPlayerPoints;
}
