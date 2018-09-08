#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>

#include "dpackapi.hpp"

void main(void)
{
	Tdpack blah;
	blah.Start(0);

	netgame_info *Gameinfo;
	int Game[NUMDGAMES];
	int i;
	int Done = 0;

	for (i = 0; i < 4; i++)
		Game[i] = 0;

	do
	{
		for (i = 0; i < 4; i++)
		{
			blah.EnterIdle();
			if (kbhit())
				if (getch() == 0x71)
					Done = 4;
			if (Game[i] == 0)
			{ //
				if (blah.GetCompletionStatus(i))
				{
					Gameinfo = blah.GetGameInfo(i);
					printf("Game: %.16s\n", Gameinfo->game_name);
					printf("Mission: %.22s\n", Gameinfo->mission_title);
					printf("Number of players: %d\n", Gameinfo->numplayers);
					printf("Game Flags: %d\n", Gameinfo->game_flags);
					printf("Players in this game:\n");
					Game[i] = 1;
					for (int z = 0; z < 8; z++)
					{
						if (Gameinfo->players[z].callsign[0] != NULL)
							printf("\t%s\n", Gameinfo->players[z].callsign);
					}
					Done++;
				}
			}
		}
	} while (Done < 4);
}
