#include "Grid.h"

#include <assert.h> /* assert */

#include "World.h"

extern World world;

Grid::Grid()
{
	m_hoverGrid = nullptr;
	m_onTurn = 1;
}

Grid::~Grid()
{
}

void Grid::load()
{
	int2 coordinates; // from where do we start

	string temp , player1OnTurn, player2OnTurn, tutorailImg;
	fstream stream;

	int squareSize;
	int2 cardDim;

	m_player1Cards[0] = new Card;
	m_player1Cards[1] = new Card;
	m_player2Cards[0] = new Card;
	m_player2Cards[1] = new Card;
	m_player1nextMove = new Card;
	m_player2nextMove = new Card;

	stream.open(CONFIG_FOLDER + GAME_FOLDER + "grid.txt");

	stream >> temp >> squareSize;

	stream >> temp >> m_player1OnTurn.rect.x >> m_player1OnTurn.rect.y >> m_player1OnTurn.rect.w >> m_player1OnTurn.rect.h;
	stream >> temp >> player1OnTurn >> player2OnTurn;
	stream >> temp >> m_tutorial.rect.x >> m_tutorial.rect.y >> m_tutorial.rect.w >> m_tutorial.rect.h;
	stream >> temp >> tutorailImg;
	
	stream >> temp >> temp;

	m_drawSelectedPawn.texture = loadTexture(GAME_FOLDER + temp);

	stream >> temp >> temp;

	m_drawSelectedCard.texture = loadTexture(GAME_FOLDER + temp);

	stream >> temp >> cardDim.x >> cardDim.y;

	stream >> temp >> temp;
	stream >> temp >> m_player1Cards[0]->rect.x >> m_player1Cards[0]->rect.y;
	m_player1Cards[0]->rect.w = cardDim.x;
	m_player1Cards[0]->rect.h = cardDim.y;

	stream >> temp >> temp;
	stream >> temp >> m_player1Cards[1]->rect.x >> m_player1Cards[1]->rect.y;
	m_player1Cards[1]->rect.w = cardDim.x;
	m_player1Cards[1]->rect.h = cardDim.y;

	stream >> temp >> temp;
	stream >> temp >> m_player2Cards[0]->rect.x >> m_player2Cards[0]->rect.y;
	m_player2Cards[0]->rect.w = cardDim.x;
	m_player2Cards[0]->rect.h = cardDim.y;
	
	stream >> temp >> temp;
	stream >> temp >> m_player2Cards[1]->rect.x >> m_player2Cards[1]->rect.y;
	m_player2Cards[1]->rect.w = cardDim.x;
	m_player2Cards[1]->rect.h = cardDim.y;

	stream >> temp >> temp;
	stream >> temp >> m_player1nextMove->rect.x >> m_player1nextMove->rect.y;
	m_player1nextMove->rect.w = cardDim.x;
	m_player1nextMove->rect.h = cardDim.y;

	stream >> temp >> temp;
	stream >> temp >> m_player2nextMove->rect.x >> m_player2nextMove->rect.y;
	m_player2nextMove->rect.w = cardDim.x;
	m_player2nextMove->rect.h = cardDim.y;

	stream.close();

	m_player1Cards[0]->data = world.m_config.m_allCards[0];
	m_player1Cards[0]->texture = m_player1Cards[0]->data.texture;

	m_player1Cards[1]->data = world.m_config.m_allCards[1];
	m_player1Cards[1]->texture = m_player1Cards[1]->data.texture;

	m_player2Cards[0]->data = world.m_config.m_allCards[2];
	m_player2Cards[0]->texture = m_player2Cards[0]->data.reversedTexture;

	m_player2Cards[1]->data = world.m_config.m_allCards[3];
	m_player2Cards[1]->texture = m_player2Cards[1]->data.reversedTexture;

	m_player1nextMove->data = world.m_config.m_allCards[4];
	m_player1nextMove->texture = m_player1nextMove->data.texture;

	m_player2nextMove->data = world.m_config.m_allCards[5];
	m_player2nextMove->texture = m_player2nextMove->data.reversedTexture;

	m_player2OnTurn.rect = m_player1OnTurn.rect;

	m_player1OnTurn.texture = loadTexture(GAME_FOLDER + player1OnTurn);
	m_player2OnTurn.texture = loadTexture(GAME_FOLDER + player2OnTurn);
	m_tutorial.texture = loadTexture(GAME_FOLDER + tutorailImg);
	
	m_drawTutorial = false;
	
	m_availableMove.texture = loadTexture(GAME_FOLDER + "gridPossMove.bmp");
	m_hover.texture = loadTexture(GAME_FOLDER + "hover.bmp");
	
	SDL_Texture* modelSquareTexture;
	modelSquareTexture = loadTexture(GAME_FOLDER + "tile.bmp");
	
	m_background = loadTexture(GAME_FOLDER + "background.bmp");

	coordinates.x = (Presenter::m_SCREEN_WIDTH - squareSize * BOARD_SIZE) / 2;
	coordinates.y = (Presenter::m_SCREEN_HEIGHT - squareSize * BOARD_SIZE) / 2;	
		
	for (int r = 0; r < BOARD_SIZE; r++)
	{
		for (int c = 0; c < BOARD_SIZE; c++)
		{
			m_gridSquares[r][c].rect = { c * squareSize + coordinates.x,
				r * squareSize + coordinates.y, 
				squareSize, 
				squareSize};
			
			m_gridSquares[r][c].texture = modelSquareTexture;
		}
	}

	loadPawns();
}

void Grid::draw()
{
	drawObject(m_background);

	drawGridSquares();

	drawAvailableMoves();

	drawPawns();

	drawCards();

	drawHover();
	
	if (m_drawTutorial)
	{
		drawObject(m_tutorial);
	}
	
	if (m_onTurn == 1)
	{
		drawObject(m_player1OnTurn);
	}
	else if(m_onTurn == 2)
	{
		drawObject(m_player2OnTurn);
	}
}

void Grid::destroy()
{
/*
	for (int i = 0; i < m_entities.size(); i++)
	{
		delete m_entities[i];
	}
	m_entities.clear();
*/
	SDL_DestroyTexture(m_gridSquares[0][0].texture);
}

vector<int2> Grid::availableMoves(Pawn* pawn, Card* card)
{
	return vector<int2>();
}

void Grid::loadPawns()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		Pawn buff;

		buff.m_coor.x = i;
		buff.m_coor.y = 0;
		buff.rect = m_gridSquares[i][0].rect;
		buff.texture = ConfigManager::m_pawn1;
		buff.m_owner = 1;
		m_player1Pawns.push_back(buff);

		buff.m_coor.x = i;
		buff.m_coor.y = BOARD_SIZE - 1;
		buff.rect = m_gridSquares[i][BOARD_SIZE - 1].rect;
		buff.texture = ConfigManager::m_pawn2;
		buff.m_owner = 2;
		m_player2Pawns.push_back(buff);
	}
}

void Grid::select()
{
	if (mouseIsPressed())
	{
		m_selected = false;

		selectPawns();
		selectCards();

		if (!m_selected)
		{
			m_selectedCard = nullptr;
			m_selectedPawn = nullptr;
		}
	}
}

void Grid::selectPawns()
{
	bool selected = false;

	if (m_onTurn == 1)
	{
		for (int i = 0; i < m_player1Pawns.size(); i++)
		{
			if (isMouseInRect(m_player1Pawns[i].rect))
			{
				m_selectedPawn = &m_player1Pawns[i];
				selected = true;
				m_selected = true;
			}
		}
	}
	else if (m_onTurn == 2)
	{
		for (int i = 0; i < m_player2Pawns.size(); i++)
		{
			if (isMouseInRect(m_player2Pawns[i].rect))
			{
				m_selectedPawn = &m_player2Pawns[i];
				selected = true;
				m_selected = true;
			}
		}
	}

	if (!selected)
	{
		if (m_selectedPawn != nullptr)
		{
			for (int r = 0; r < BOARD_SIZE; r++)
			{
				for (int c = 0; c < BOARD_SIZE; c++)
				{
					if (isMouseInRect(m_gridSquares[r][c].rect))
					{
						if (possMove({ r, c }))
						{
							killPawn({ r, c });
							m_selectedPawn->m_coor.x = r;
							m_selectedPawn->m_coor.y = c;

							m_selectedPawn->rect = m_gridSquares[r][c].rect;

							cardSwitch();

							m_onTurn = (m_onTurn == 1) + 1;

							m_selectedPawn = nullptr;
						}
					}
				}
			}
		}
	}
	else
	{
		m_drawSelectedPawn.rect = m_selectedPawn->rect;
	}
}

void Grid::selectCards()
{
	if (m_onTurn == 1)
	{
		for (int i = 0; i < 2; i++)
		{
			if (isMouseInRect(m_player1Cards[i]->rect))
			{
				m_selectedCard = m_player1Cards[i];
				m_drawSelectedCard.rect = m_selectedCard->rect;
				m_selected = true;
			}
		}
	}
	else if (m_onTurn == 2)
	{
		for (int i = 0; i < 2; i++)
		{
			if (isMouseInRect(m_player2Cards[i]->rect))
			{
				m_selectedCard = m_player2Cards[i];
				m_drawSelectedCard.rect = m_selectedCard->rect;
				m_selected = true;
			}
		}
	}
}

void Grid::killPawn(int2 coor)
{
	for (int i = 0; i < m_player1Pawns.size(); i++)
	{
		if (m_player1Pawns[i].m_coor == coor)
		{
			m_player1Pawns.erase(m_player1Pawns.begin() + i);
		}
	}

	for (int i = 0; i < m_player2Pawns.size(); i++)
	{
		if (m_player2Pawns[i].m_coor == coor)
		{
			m_player2Pawns.erase(m_player2Pawns.begin() + i);
		}
	}
}

void Grid::checkForClick()
{
	if (mouseIsPressed())
	{
		for (int r = 0; r < BOARD_SIZE; r++)
		{
			for (int c = 0; c < BOARD_SIZE; c++)
			{
				if (isMouseInRect(m_gridSquares[r][c].rect))
				{
					int2 coor;
					coor.x = r;
					coor.y = c;
				}
			}
		}
	}
}

void Grid::drawGridSquares()
{
	for (int r = 0; r < BOARD_SIZE; r++)
	{
		for (int c = 0; c < BOARD_SIZE; c++)
		{
			drawObject(m_gridSquares[r][c]);
		}
	}
}

void Grid::drawPawns()
{
	if (m_selectedPawn != nullptr)
	{
		drawObject(m_drawSelectedPawn);
	}
	
	for (auto& pawn : m_player1Pawns)
	{
		drawObject(pawn);
	}

	for (auto& pawn : m_player2Pawns)
	{
		drawObject(pawn);
	}
}

void Grid::drawCards()
{
	for (int i = 0; i < 2; i++)
	{
		drawObject(*m_player1Cards[i]);
		drawObject(*m_player2Cards[i]);
	}

	if (m_onTurn == 1)
	{
		drawObject(*m_player1nextMove);
	}
	else if(m_onTurn == 2)
	{
		drawObject(*m_player2nextMove);
	}

	if (m_selectedCard != nullptr)
	{
		drawObject(m_drawSelectedCard);
	}

}

void Grid::onHover()
{
	m_hoverGrid = nullptr;
	for (int r = 0; r < BOARD_SIZE; r++)
	{
		for (int c = 0; c < BOARD_SIZE; c++)
		{
			if (isMouseInRect(m_gridSquares[r][c].rect))
			{
				m_hoverGrid = &m_gridSquares[r][c];
			}
		}
	}
}

void Grid::drawHover()
{
	if (m_hoverGrid != nullptr)
	{
		m_hover.rect = m_hoverGrid->rect;
		drawObject(m_hover);
	}
}

void Grid::drawAvailableMoves()
{
	for(Drawable* dr : m_availableMoves)
	{
		m_availableMove.rect = dr->rect;
		drawObject(m_availableMove);
	}
}

/*
* Fill the vector of available moves
*/
void Grid::calcAvailableMoves()
{
	m_availableMoves.clear();

	// check if should use this function
	if (m_selectedCard != nullptr && m_selectedPawn != nullptr)
	{
		// get the coor of all possible moves
		for (auto& move : m_selectedCard->data.m_availableMoves)
		{
			int2 tile = move + m_selectedPawn->m_coor;

			// check if we are in the board
			if (tile.x >= 0 && tile.y >= 0 
				&& tile.x < BOARD_SIZE && tile.y < BOARD_SIZE)
			{
				m_availableMoves.push_back(&m_gridSquares[tile.x][tile.y]);
			}
		}
	}
}

void Grid::cardSwitch()
{
	// this function requires a card to be selected
	assert(m_selectedCard != nullptr);

	if (m_onTurn == 1)
	{
		m_player2nextMove->data = m_selectedCard->data;
		m_player2nextMove->texture = m_player2nextMove->data.reversedTexture;
		m_selectedCard->data = m_player1nextMove->data;
		m_selectedCard->texture = m_selectedCard->data.texture;
	}
	else if(m_onTurn == 2)
	{
		m_player1nextMove->data = m_selectedCard->data;
		m_player1nextMove->texture = m_player1nextMove->data.texture;
		m_selectedCard->data = m_player2nextMove->data;
		m_selectedCard->texture = m_selectedCard->data.reversedTexture;
	}

	// flip the moves of the card, that is being passed
	for (auto& move : m_selectedCard->data.m_availableMoves)
	{
		move.y *= -1;
	}
}

/*
* check for valid move when we add entity
* @param coor - the coordinates of the square we are checking
* @return true if the square is free, false if it is not
*/
bool Grid::possMove(int2 coor)
{
	if (m_selectedCard == nullptr || m_selectedPawn == nullptr)
	{
		return false;
	}

	for (auto& move : m_selectedCard->data.m_availableMoves)
	{
		if (m_selectedPawn->m_coor + move == coor)
		{
			return true;
		}
	}

	return false;
}

int Grid::checkForWinner()
{
	// temple win condition
	const int2 _player1Temple = { 2, 0 };
	const int2 _player2Temple = { 2, 4 };

	for (int i = 0; i < m_player1Pawns.size(); i++)
	{
		if (m_player1Pawns[i].m_coor == _player2Temple)
		{
			return 1;
		}
	}

	for (int i = 0; i < m_player2Pawns.size(); i++)
	{
		if (m_player2Pawns[i].m_coor == _player1Temple)
		{
			return 2;
		}
	}

	// no pawns win condition
	if (m_player1Pawns.size() == 0)
	{
		return 2;
	}
	else if (m_player2Pawns.size() == 0)
	{
		return 1;
	}

	return 0;
}

void Grid::update()
{
	onHover();

	checkForClick();
	select();
	calcAvailableMoves();

	if (checkForWinner())
	{
		// go to the next scene
		world.m_stateManager.changeGameState(GAME_STATE::WIN_SCREEN);
	}
}
