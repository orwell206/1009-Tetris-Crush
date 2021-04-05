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
	int awardPoints(Player *, int);
	Player operator+(int);
};
Player::Player()
{
	this->playerInfo.playerScore = 0;
}
int Player::awardPoints(Player *player, int lines)
{
	/*
		Desc: Award player points when lines are cleared on the game.

		Params: Player (*player), int (lines)
	*/
	//Calculate score for player when cleared line
	player->playerInfo.playerScore = player->playerInfo.playerScore + (20 * lines);
	//Using the overloaded operator, sum both player class and return playscore.
	return player->playerInfo.playerScore;
}
Player Player::operator+(int inputScore)
{
	/*
		Desc: Overloading operator + for adding player score.

		Params: Player (*userPlayer)
	*/
	//Temp Player
	Player temp;
	//Sum player score
	temp.playerInfo.playerScore = this->playerInfo.playerScore + inputScore;
	//Return Temp
	return temp;
}