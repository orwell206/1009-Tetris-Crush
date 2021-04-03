#pragma once
#include <string>
using namespace std;


class Player
{
public:
	struct PlayerInfo
	{
		int playerScore;
		string playerName;
	};
	PlayerInfo playerInfo;
};