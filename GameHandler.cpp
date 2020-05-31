#include "GameHandler.h"
#include <iostream>
void GameHandler::displayBoard()
{
	std::cout << std::endl<<"  ";
	for (int i = 0; i < 10; i++) std::cout << "  " << i << " ";
	std::cout << std::endl;

	std::cout << "  " << char(201) << char(205);
	for (int i = 0; i < 2 * size - 1; i++)
	{
		if (i % 2 == 0) std::cout << char(205);
		else std::cout << char(205) << char(203) << char(205);

	}
	std::cout << char(205) << char(187) << std::endl;


	for (int j = 0; j < 10; j++)
	{
		std::cout << j << " ";
		for (int i = 0; i < 2 * size + 1; i++)
		{
			if (i % 2 == 0) std::cout << char(186);
			else std::cout << " " << positionData[i / 2][j] << " ";
		}


		if (j == 9)break;
		std::cout << std::endl << "  " << char(204) << char(205);
		for (int i = 0; i < 2 * size - 1; i++)
		{
			if (i % 2 == 0) std::cout << char(205);
			else std::cout << char(205) << char(206) << char(205);

		}
		std::cout << char(205) << char(185) << std::endl;

	}


	std::cout << std::endl << "  " << char(200) << char(205);
	for (int i = 0; i < 2 * size - 1; i++)
	{
		if (i % 2 == 0) std::cout << char(205);
		else std::cout << char(205) << char(202) << char(205);

	}
	std::cout << char(205) << char(188) << std::endl;
}

GameHandler::GameHandler()
{
	playerid = 0;
	pf = new ProfessionsFactory();
	clearBoard();
	linesInDisplayBoard = 24;
}

void GameHandler::createPlayer()
{
	players.push_back(new Player(playerid++));
}



void GameHandler::updatePositionData() //nazwa update board data czy cos
{
	clearBoard();
	for (auto player : players)
	{
		for (auto player_unit : player->getUnits())
		{
			if (player_unit->isAlive())
			{
				positionData[player_unit->getPosition().first][player_unit->getPosition().second] = player_unit->getDisplayChar();
				professionBoardData[player_unit->getPosition().first][player_unit->getPosition().second] = player_unit;
			}
		}
	}
}

void GameHandler::clearBoard()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			positionData[i][j] = ' ';
			professionBoardData[i][j] = nullptr;

		}
	}
}
void GameHandler::test()
{
	createPlayer();
	createPlayer();

	askPlayerToBuyUnits(players[0]);
	//players[0]->buyUnit(pf->createUnit(0));
	//players[0]->moveUnit(players[0]->getUnits()[0], std::make_pair(7, 3));
	//players[1]->buyUnit(sellUnit(0,players[1]));
	//players[1]->moveUnit(players[1]->getUnits()[0], std::make_pair(4, 3));
	updatePositionData();
	displayBoard();
	std::cout<<"char: "<<players[0]->getUnits()[0]->getPosition().first;
	

//	players[1]->getUnits()[0]->attack( professionBoardData[2][3]);

	updatePositionData();

	displayBoard();
	
}

void GameHandler::askPlayerToBuyUnits(Player* _p)
{
	int id;
	while (true)
	{
		system("cls");
		std::cout << "Hi player " << _p->getID() +1<< std::endl;//ewentualnie nickname
		std::cout << "You have " << _p->getGold() << " gold.\n";
		pf->getProfLibrary()->displayUnitsAndCosts();
		std::cout << "Write profession id to buy unit or -1 to exit shop\n";
		id=getValidInput<int>(); 
		if (id == -1) break;
		try
		{
			if (pf->getProfLibrary()->getProfessionCost(id) > _p->getGold()) // playera nie stac
				std::cout << "You have not enough gold to buy this unit!\n";
			else
				_p->buyUnit(pf->createUnit(id), pf->getCost(id));
		}
		catch (...)
		{
			std::cout << "Something went wrong,check if unit ID is valid\n";
				system("pause");
		}
	}
	
}
void GameHandler::putAllPlayerUnitsOnBoard(Player* _p)
{
	system("cls");
	for (auto unit : _p->getUnits())
	{
		bool ok = false;
		std::cout << "P" << _p->getID() + 1 << ":\n";
		std::cout << "Write x and y cords to put your " << unit->getName() << " on board.\n";
		std::pair<int, int> coords;
		while (ok != true)
		{
			coords = askPlayersForCordsToPutUnit();
			if (_p->getID() == 0 && coords.second > 3)
			{
				std::cout << "At the beginning you must put your unit on one of first 3 rows\nTry again\n";
				continue;
			}
			if (_p->getID() == 1 && coords.second < 6)
			{
				std::cout << "At the beginning you must put your unit on one of last 3 rows\nTry again\n";
				continue;
			}
			
			ok = true;
		}
		unit->setPosition(coords);
		updatePositionData();
		system("cls");

	}
}
std::pair<int, int> GameHandler::askPlayersForCordsToPutUnit()
{
	int x=-1, y=-1;
	while (x < 0 || x>9 || y < 0 || y>9)
	{
		std::cout << "Write x cord: \n"; //zabezpieczyc to, only int
		x=getValidInput<int>();
		std::cout << "Write y cord: \n"; //zabezpieczyc to
		y=getValidInput<int>();
		if (x < 0 || x>9 || y < 0 || y>9||professionBoardData[x][y] != nullptr)
		{
			std::cout << "There is already unit on this coords/Out of board range, try again\n";
			x = -1;
			y = -1;
		}
	}
	return std::make_pair(x, y);

}

void GameHandler::manageGame()
{
	createPlayer();
	createPlayer();
	std::cout << "Take a look how does the board look like: \n";
	displayBoard();
	std::cout << "First all players have to buy units and place them on board.\n";
	system("pause");
	for (auto player:players)
	{
		
		askPlayerToBuyUnits(player);
		putAllPlayerUnitsOnBoard(player);
	}
	updatePositionData();
	system("cls");
	
	//displayAllplayersUnitsAndBasicStats();
	//start rozgrywki
	bool gameON = true;
	while (gameON)
	{
		for (auto player : players)
		{
			for (auto unit : player->getUnits())
			{
			
				if (unit->isAlive())
				{
					if (unit->getStunned()) //jesli unit jest zestunowany i jest jego kolej to zdejmuje mu stuna i pomijam jego ruch, stun ma zabierac jeden ruch
					{
						unit->setStunned(false);
						continue;
					}
					//std::cout << "HP: " << unit->getCurrentHP() << "\n";
					displayBoard();
					displayAllplayersUnitsAndBasicStats();
					displayLog();
					//system("pause");
					player->doSomethingWithUnit(unit,positionData,professionBoardData,player->getUnits(),players[abs(player->getID()-1)]->getUnits(),gameLOG); //abs(player->getID()-1) daje przeciwnego playera, jesli player ma id 0 to przeciwny ma abs(0-1),jesli 1 to abs(1-1)
					updatePositionData();
					system("cls");
					
					
				}
				
			}

		}
		updateAllUnitsStatsAfterRound();
		roundsCounter++;
		
		
	}
}
void GameHandler::checkIfPlayersUnitsAreAlive()
{
	for (auto p : players)
	{
		int i = 0;
		for (auto u : p->getUnits())
		{
			
			if (!u->isAlive())
			{
				p->getUnits().erase(p->getUnits().begin() + i);
			}
			i++;
		}
	}
	updatePositionData();
}
void GameHandler::displayAllplayersUnitsAndBasicStats()
{
	int newNewLines = 0;
	for (int i = 0; i < linesInDisplayBoard-5; i++)
	{
		std::cout << "\033[F"; //cofam linie w coucie
	}

	for (auto p : players)
	{
		std::cout << "\t\t\t\t\t\t\t" <<"Player "<< p->getID()<<":\n";
		newNewLines++;
		for (auto u : p->getUnits())
		{
			if (u->isAlive())
			{
				std::cout << "\t\t\t\t\t\t\t" << "[" << u->getPosition().first << "," << u->getPosition().second << "] " << u->getName() << " HP: " << u->getCurrentHP() << "/" << u->getStat(HP_MAX);
				std::cout << " MANA: " << u->getCurrentMana() << "/" << u->getStat(MANA_MAX) <<  " AR: "<<u->getStat(AR)<<"\n";
			}
			else
			std::cout << "\t\t\t\t\t\t\t" << "DEATH " << u->getName() << "\n";
			newNewLines++;
		}
	}
	for (int i = 0; i < linesInDisplayBoard - 5-newNewLines; i++)
	{
		std::cout << "\n"; //odcofuje linie w coucie
	}
}
void GameHandler::displayLog()
{
	int linesToBack = 0;
	if (!gameLOG.empty())
	{
		linesToBack = 1;
	std::cout << "\t\t\t\t\t\t" << "Game LOG: \n";
	}
	for (int i = (int)gameLOG.size()-1; i > (int)gameLOG.size()-6; i--)
	{
	
		if (i < 0) break;
		if (gameLOG.empty()) break;
		std::cout << "\t\t\t\t\t\t" << gameLOG[i];
		linesToBack++;
	}
	for(int i=0;i<linesToBack;i++) std::cout << "\033[F"; //cofam linie w coucie
}

void GameHandler::updateAllUnitsStatsAfterRound()
{
	for (auto p : players)
	{
		for (auto u : p->getUnits())
		{
			u->updateStatsAfterRound();
		}
	}
}