#pragma once


class Faction
{
	
	int factionTeam;


public:
	
	Faction() {}
	Faction(int startTeam) : factionTeam(startTeam) {}

	int getTeam()
	{
		return factionTeam;
	}

};