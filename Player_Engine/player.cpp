#pragma once
#include <string>
#include <iostream>
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
	Player operator+(Player *);
};
Player::Player()
{
	this->playerInfo.playerScore = 0;
}
int Player::awardPoints(Player *player, int lines)
{
	Player tempPlayer;
	player->playerInfo.playerScore = player->playerInfo.playerScore + (20 * lines);
	//tempPlayer = ;
	return (tempPlayer + player).playerInfo.playerScore;
}
Player Player::operator+(Player *userPlayer)
{
	Player temp;
	temp.playerInfo.playerScore = this->playerInfo.playerScore + userPlayer->playerInfo.playerScore;
	return temp;
}