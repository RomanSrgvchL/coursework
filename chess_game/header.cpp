#include "header.h"

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
Mix_Music* background = NULL;
Mix_Chunk* sound = NULL;

int size_field = 80;
int size_piece = size_field;
int miss = size_field / 40;
int fps = 150;
int board_width = size_field * 8, board_height = size_field * 8, menu_bar = 100;
int win_width = board_width + menu_bar, win_height = board_height;

void DeInit(int error)
{
	if (ren != NULL) SDL_DestroyRenderer(ren);
	if (win != NULL) SDL_DestroyWindow(win);
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(error);
}
void Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		std::cout << "Couldn't init SDL! Error: " << SDL_GetError() << std::endl;;
		system("pause");
		DeInit(1);
	}

	int imgFlag;

	if ((imgFlag = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) == 0)
	{
		std::cout << "Couldn't init SDL_Image! Error: " << SDL_GetError() << std::endl;;
		system("pause");
		DeInit(1);
	}

	if (!(imgFlag & (IMG_INIT_PNG | IMG_INIT_JPG))) std::cout << "Couldn't init PNG or JPG library.\n";

	if (TTF_Init() == -1)
	{
		std::cout << "Couldn't init SDL_ttf! Error: " << SDL_GetError() << std::endl;;
		system("pause");
		DeInit(1);
	}

	win = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, SDL_WINDOW_SHOWN);

	if (win == NULL)
	{
		std::cout << "Couldn't create window! Error: " << SDL_GetError() << std::endl;
		system("pause");
		DeInit(1);
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);

	if (ren == NULL)
	{
		std::cout << "Couldn't create render! Error: " << SDL_GetError() << std::endl;
		system("pause");
		DeInit(1);
	}

	Mix_Init(0);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
}
void LoadMusic(bool isMusic)
{
	if (isMusic)
	{
		background = Mix_LoadMUS("music/background1_1.mp3");
		Mix_PlayMusic(background, -1);
	}
}
void PrintTable(std::string table[8][8])
{
	std::cout << "--------------------------" << std::endl;
	for (int i = 0; i < 8; i++)
	{
		std::cout << "|";
		for (int j = 0; j < 8; j++)
			std::cout << table[i][j] << " ";
		std::cout << "|" << std::endl;
	}
	std::cout << "--------------------------";
	std::cout << "\n\n";
}
void Sound0(std::string name_file, bool isChunk)
{
	if (isChunk)
	{
		sound = Mix_LoadWAV(name_file.c_str());
		Mix_PlayChannel(0, sound, 0);
	}
}
void Sound1(std::string name_file, bool isChunk)
{
	if (isChunk)
	{
		sound = Mix_LoadWAV(name_file.c_str());
		Mix_PlayChannel(-1, sound, 0);
	}
}
SDL_Texture* LoadTextureFromFile(const char* filename)
{
	SDL_Surface* surface = IMG_Load(filename);
	if (surface == NULL)
	{
		std::cout << "Couldn't load image " << filename << "!" << " Error: " << SDL_GetError() << std::endl;
		system("pause");
		DeInit(1);
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	return texture;
}
bool AvailableMove(int* chosen_field, int* become_field, std::string table[8][8])
{
	bool available_move = false;
	switch (table[chosen_field[0]][chosen_field[1]][1])
	{
	case 'K':
		if ((abs(chosen_field[0] - become_field[0]) <= 1) && (abs(chosen_field[1] - become_field[1]) <= 1))
			available_move = true;
		break;
	case 'P':
		if (table[chosen_field[0]][chosen_field[1]][0] == 'w')
		{
			if (((chosen_field[0] - become_field[0] == 1) || ((chosen_field[0] == 6 && become_field[0] == 4) && (table[5][chosen_field[1]] == "--"))) &&
				(chosen_field[1] == become_field[1]) && (table[become_field[0]][become_field[1]] == "--") || ((table[become_field[0]][become_field[1]] != "--") &&
					(chosen_field[0] - become_field[0] == 1) && (abs(chosen_field[1] - become_field[1]) == 1)))
				available_move = true;
		}
		else
		{
			if (((become_field[0] - chosen_field[0] == 1) || ((chosen_field[0] == 1 && become_field[0] == 3) && (table[2][chosen_field[1]] == "--"))) &&
				(chosen_field[1] == become_field[1]) && (table[become_field[0]][become_field[1]] == "--") || ((table[become_field[0]][become_field[1]] != "--") &&
					(become_field[0] - chosen_field[0] == 1) && (abs(chosen_field[1] - become_field[1]) == 1)))
				available_move = true;
		}
		break;
	case 'B':
		if (abs(chosen_field[0] - become_field[0]) == abs(chosen_field[1] - become_field[1]))
		{
			available_move = true;
			int minX = std::min(chosen_field[0], become_field[0]);
			int maxX = std::max(chosen_field[0], become_field[0]);
			int minY = std::min(chosen_field[1], become_field[1]);
			int maxY = std::max(chosen_field[1], become_field[1]);
			if ((chosen_field[0] > become_field[0]) && (chosen_field[1] > become_field[1]) ||
				(chosen_field[0] < become_field[0]) && (chosen_field[1] < become_field[1]))
			{
				for (int i = minX + 1, j = minY + 1; (i < maxX) && (j < maxY); i++, j++)
				{
					if (table[i][j] != "--")
					{
						available_move = false;
						break;
					}
				}
			}
			else
			{
				for (int i = maxX - 1, j = minY + 1; (i > minX) && (j < maxY); i--, j++)
				{
					if (table[i][j] != "--")
					{
						available_move = false;
						break;
					}
				}
			}
		}
		break;
	case 'R':
		if ((chosen_field[0] != become_field[0]) && (chosen_field[1] == become_field[1]))
		{
			available_move = true;
			int min = std::min(chosen_field[0], become_field[0]);
			int max = std::max(chosen_field[0], become_field[0]);
			for (int i = min + 1; i < max; i++)
			{
				if (table[i][chosen_field[1]] != "--")
				{
					available_move = false;
					break;
				}
			}
		}
		else if ((chosen_field[0] == become_field[0]) && (chosen_field[1] != become_field[1]))
		{
			available_move = true;
			int min = std::min(chosen_field[1], become_field[1]);
			int max = std::max(chosen_field[1], become_field[1]);
			for (int i = min + 1; i < max; i++)
			{
				if (table[chosen_field[0]][i] != "--")
				{
					available_move = false;
					break;
				}
			}
		}
		break;
	case 'Q':
		if (abs(chosen_field[0] - become_field[0]) == abs(chosen_field[1] - become_field[1]))
		{
			available_move = true;
			int minX = std::min(chosen_field[0], become_field[0]);
			int maxX = std::max(chosen_field[0], become_field[0]);
			int minY = std::min(chosen_field[1], become_field[1]);
			int maxY = std::max(chosen_field[1], become_field[1]);
			if ((chosen_field[0] > become_field[0]) && (chosen_field[1] > become_field[1]) ||
				(chosen_field[0] < become_field[0]) && (chosen_field[1] < become_field[1]))
			{
				for (int i = minX + 1, j = minY + 1; (i < maxX) && (j < maxY); i++, j++)
				{
					if (table[i][j] != "--")
					{
						available_move = false;
						break;
					}
				}
			}
			else
			{
				for (int i = maxX - 1, j = minY + 1; (i > minX) && (j < maxY); i--, j++)
				{
					if (table[i][j] != "--")
					{
						available_move = false;
						break;
					}
				}
			}
		}
		else if ((chosen_field[0] != become_field[0]) && (chosen_field[1] == become_field[1]))
		{
			available_move = true;
			int min = std::min(chosen_field[0], become_field[0]);
			int max = std::max(chosen_field[0], become_field[0]);
			for (int i = min + 1; i < max; i++)
			{
				if (table[i][chosen_field[1]] != "--")
				{
					available_move = false;
					break;
				}
			}
		}
		else if ((chosen_field[0] == become_field[0]) && (chosen_field[1] != become_field[1]))
		{
			available_move = true;
			int min = std::min(chosen_field[1], become_field[1]);
			int max = std::max(chosen_field[1], become_field[1]);
			for (int i = min + 1; i < max; i++)
			{
				if (table[chosen_field[0]][i] != "--")
				{
					available_move = false;
					break;
				}
			}
		}
		break;
	case 'N':
		if ((abs(chosen_field[0] - become_field[0]) == 1) && (abs(chosen_field[1] - become_field[1]) == 2) ||
			(abs(chosen_field[0] - become_field[0]) == 2) && (abs(chosen_field[1] - become_field[1]) == 1))
			available_move = true;
		break;
	}

	return available_move;
}
bool BitField(bool turn_move, int* king_field, int* become_field, std::string table[8][8], SDL_Rect* pieces)
{
	int i = 16;
	int max = 32;
	if (turn_move)
	{
		i = 0;
		max = 16;

	}
	bool field_is_bit = false;
	int chosen_field[2];
	for (i; i < max; i++)
	{
		if ((pieces[i].x != -100) && !((pieces[i].x == become_field[1] * size_field) && 
			(pieces[i].y == become_field[0] * size_field)))
		{
			chosen_field[0] = pieces[i].y / size_field;
			chosen_field[1] = pieces[i].x / size_field;
			std::string space = table[become_field[0]][become_field[1]];
			if (turn_move) table[become_field[0]][become_field[1]] = "wK";
			else table[become_field[0]][become_field[1]] = "bK";
			table[king_field[0]][king_field[1]] = "--";
			if (AvailableMove(chosen_field, become_field, table))
			{
				field_is_bit = true;
				i = max;
			}
			if (turn_move) table[king_field[0]][king_field[1]] = "wK";
			else table[king_field[0]][king_field[1]] = "bK";
			table[become_field[0]][become_field[1]] = space;
		}
	}
	return field_is_bit;
}
bool Check1(bool turn_move, int* piece_chosen_field, int* become_field, std::string table[8][8], SDL_Rect* pieces)
{
	int king_field[2];
	std::string find_field = "bK";
	int i = 16, max = 32;
	if (turn_move)
	{
		find_field = "wK";
		i = 0;
		max = 16;
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
			if (table[i][j] == find_field)
			{
				king_field[0] = i;
				king_field[1] = j;
				i = 8;
				break;
			}
	}
	bool check = false;
	int chosen_field[2];
	for (i; i < max; i++)
	{
		if ((pieces[i].x != -100) && !((pieces[i].x == become_field[1] * size_field) && 
			(pieces[i].y == become_field[0] * size_field)))
		{
			chosen_field[0] = pieces[i].y / size_field;
			chosen_field[1] = pieces[i].x / size_field;
			std::string space = table[become_field[0]][become_field[1]];
			table[become_field[0]][become_field[1]] = table[piece_chosen_field[0]][piece_chosen_field[1]];
			table[piece_chosen_field[0]][piece_chosen_field[1]] = "--";
			if (AvailableMove(chosen_field, king_field, table))
			{
				check = true;
				i = max;
			}
			table[piece_chosen_field[0]][piece_chosen_field[1]] = table[become_field[0]][become_field[1]];
			table[become_field[0]][become_field[1]] = space;
		}
	}
	return check;
}
bool Check2(bool turn_move, std::string table[8][8], SDL_Rect* pieces)
{
	int king_field[2];
	std::string find_field = "bK";
	int i = 16, max = 32;
	if (turn_move)
	{
		find_field = "wK";
		i = 0;
		max = 16;
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
			if (table[i][j] == find_field)
			{
				king_field[0] = i;
				king_field[1] = j;
				i = 8;
				break;
			}
	}
	bool check = false;
	int chosen_field[2];
	for (i; i < max; i++)
	{
		if (pieces[i].x != -100)
		{
			chosen_field[0] = pieces[i].y / size_field;
			chosen_field[1] = pieces[i].x / size_field;
			if (AvailableMove(chosen_field, king_field, table))
			{
				check = true;
				break;
			}
		}
	}
	return check;
}
bool CheckCastling1(bool turn_move, int* become_field, std::string table[8][8], SDL_Rect* pieces, int* chosen_field, bool* castling)
{
	if ((become_field[0] == 0) && (become_field[1] == 2) && castling[0] && castling[1] && (table[0][1] + table[0][2] +
		table[0][3] == "------") && !Check2(turn_move, table, pieces) && !BitField(turn_move, chosen_field, become_field, table, pieces))
	{
		become_field[1] = 3;
		if (!BitField(turn_move, chosen_field, become_field, table, pieces))
		{
			table[0][3] = "bR";
			table[0][0] = "--";
			pieces[0].x = 3 * size_field;
			pieces[0].y = 0;
			castling[0] = false;
			castling[1] = false;
			become_field[1] = 2;
			return true;
		}
	}
	else if ((become_field[0] == 0) && (become_field[1] == 6) && castling[0] && castling[2] && (table[0][5] + table[0][6] == "----") &&
		!Check2(turn_move, table, pieces) && !BitField(turn_move, chosen_field, become_field, table, pieces))
	{
		become_field[1] = 5;
		if (!BitField(turn_move, chosen_field, become_field, table, pieces))
		{
			table[0][5] = "bR";
			table[0][7] = "--";
			pieces[7].x = 5 * size_field;
			pieces[7].y = 0;
			castling[0] = false;
			castling[2] = false;
			become_field[1] = 6;
			return true;
		}
	}
	else if ((become_field[0] == 7) && (become_field[1] == 2) && castling[3] && castling[4] && (table[7][1] + table[7][2] +
		table[7][3] == "------") && !Check2(turn_move, table, pieces) && !BitField(turn_move, chosen_field, become_field, table, pieces))
	{
		become_field[1] = 3;
		if (!BitField(turn_move, chosen_field, become_field, table, pieces))
		{
			table[7][3] = "wR";
			table[7][0] = "--";
			pieces[16].x = 3 * size_field;
			pieces[16].y = 7 * size_field;
			castling[3] = false;
			castling[4] = false;
			become_field[1] = 2;
			return true;
		}
	}
	else if ((become_field[0] == 7) && (become_field[1] == 6) && castling[3] && castling[5] && (table[7][5] + table[7][6] == "----") &&
		!Check2(turn_move, table, pieces) && !BitField(turn_move, chosen_field, become_field, table, pieces))
	{
		become_field[1] = 5;
		if (!BitField(turn_move, chosen_field, become_field, table, pieces))
		{
			table[7][5] = "wR";
			table[7][7] = "--";
			pieces[23].x = 5 * size_field;
			pieces[23].y = 7 * size_field;
			castling[3] = false;
			castling[5] = false;
			become_field[1] = 6;
			return true;
		}
	}
	return false;
}
bool CheckCastling2(bool turn_move, int* become_field, std::string table[8][8], SDL_Rect* pieces, int* chosen_field, bool* castling)
{
	bool CheckCastling = false;
	if ((become_field[0] == 0) && (become_field[1] == 2) && castling[0] && castling[1] && (table[0][1] + table[0][2] +
		table[0][3] == "------") && !Check2(turn_move, table, pieces) && !BitField(turn_move, chosen_field, become_field, table, pieces))
	{
		become_field[1] = 3;
		if (!BitField(turn_move, chosen_field, become_field, table, pieces))
		{
			CheckCastling = true;
		}
	}
	else if ((become_field[0] == 0) && (become_field[1] == 6) && castling[0] && castling[2] && (table[0][5] + table[0][6] == "----") &&
		!Check2(turn_move, table, pieces) && !BitField(turn_move, chosen_field, become_field, table, pieces))
	{
		become_field[1] = 5;
		if (!BitField(turn_move, chosen_field, become_field, table, pieces))
		{
			CheckCastling = true;
		}
	}
	else if ((become_field[0] == 7) && (become_field[1] == 2) && castling[3] && castling[4] && (table[7][1] + table[7][2] +
		table[7][3] == "------") && !Check2(turn_move, table, pieces) && !BitField(turn_move, chosen_field, become_field, table, pieces))
	{
		become_field[1] = 3;
		if (!BitField(turn_move, chosen_field, become_field, table, pieces))
		{
			CheckCastling = true;
		}
	}
	else if ((become_field[0] == 7) && (become_field[1] == 6) && castling[3] && castling[5] && (table[7][5] + table[7][6] == "----") &&
		!Check2(turn_move, table, pieces) && !BitField(turn_move, chosen_field, become_field, table, pieces))
	{
		become_field[1] = 5;
		if (!BitField(turn_move, chosen_field, become_field, table, pieces))
		{
			CheckCastling = true;
		}
	}
	else
		CheckCastling = AvailableMove(chosen_field, become_field, table) &&
		!BitField(turn_move, chosen_field, become_field, table, pieces);
	return CheckCastling;
}
bool CheckEnPassant1(bool turn_move, std::string table[8][8], SDL_Rect* pieces, int* chosen_field, int* become_field, int* dst_become_piece, int num, move* moves_history, int moves_num)
{
	if (turn_move && (chosen_field[0] == 3) && (become_field[0] == 2) && (abs(chosen_field[1] - become_field[1]) == 1) &&
		(table[chosen_field[0]][become_field[1]] == "bP"))
	{
		for (int i = 8; i < 16; i++)
		{
			if ((pieces[i].x == dst_become_piece[0]) && (pieces[i].y == dst_become_piece[1] + size_field))
			{
				if (i == num && (pieces[num].y == moves_history[moves_num - 1].pieces[num].y + size_field * 2))
				{
					table[chosen_field[0]][become_field[1]] = "--";
					if (!Check1(turn_move, chosen_field, become_field, table, pieces))
					{
						pieces[i].x = -100;
						pieces[i].y = -100;
						return true;
					}
					else table[chosen_field[0]][become_field[1]] = "bP";
				}
				return false;
			}
		}
	}
	else if (!turn_move && (chosen_field[0] == 4) && (become_field[0] == 5) && (abs(chosen_field[1] - become_field[1]) == 1) &&
		(table[chosen_field[0]][become_field[1]] == "wP"))
	{
		for (int i = 24; i < 32; i++)
		{
			if ((pieces[i].x == dst_become_piece[0]) && (pieces[i].y == dst_become_piece[1] - size_field))
			{
				if (i == num && (pieces[num].y == moves_history[moves_num - 1].pieces[num].y - size_field * 2))
				{
					table[chosen_field[0]][become_field[1]] = "--";
					if (!Check1(turn_move, chosen_field, become_field, table, pieces))
					{
						pieces[i].x = -100;
						pieces[i].y = -100;
						return true;
					}
					else table[chosen_field[0]][become_field[1]] = "wP";
				}
				return false;
			}
		}
	}
	return false;
}
bool CheckEnPassant2(bool turn_move, std::string table[8][8], SDL_Rect* pieces, int* chosen_field, int* become_field, int* dst_become_piece, int num, move* moves_history, int moves_num)
{
	bool available_move = false;

	if (turn_move && (chosen_field[0] == 3) && (become_field[0] == 2) && (abs(chosen_field[1] - become_field[1]) == 1) &&
		(table[chosen_field[0]][become_field[1]] == "bP"))
	{
		for (int i = 8; i < 16; i++)
		{
			if ((pieces[i].x == dst_become_piece[0]) && (pieces[i].y == dst_become_piece[1] + size_field))
			{
				if (i == num && (pieces[num].y == moves_history[moves_num - 1].pieces[num].y + size_field * 2))
				{
					table[chosen_field[0]][become_field[1]] = "--";
					if (!Check1(turn_move, chosen_field, become_field, table, pieces)) available_move = true;
					table[chosen_field[0]][become_field[1]] = "bP";
				}
				else available_move = AvailableMove(chosen_field, become_field, table);
				break;
			}
		}
	}
	else if (!turn_move && (chosen_field[0] == 4) && (become_field[0] == 5) && (abs(chosen_field[1] - become_field[1]) == 1) &&
		(table[chosen_field[0]][become_field[1]] == "wP"))
	{
		for (int i = 24; i < 32; i++)
		{
			if ((pieces[i].x == dst_become_piece[0]) && (pieces[i].y == dst_become_piece[1] - size_field))
			{
				if (i == num && (pieces[num].y == moves_history[moves_num - 1].pieces[num].y - size_field * 2))
				{
					table[chosen_field[0]][become_field[1]] = "--";
					if (!Check1(turn_move, chosen_field, become_field, table, pieces)) available_move = true;
					table[chosen_field[0]][become_field[1]] = "wP";
				}
				else available_move = AvailableMove(chosen_field, become_field, table);
				break;
			}
		}
	}
	else
		available_move = AvailableMove(chosen_field, become_field, table) &&
		!Check1(turn_move, chosen_field, become_field, table, pieces);

	return available_move;
}
bool Checkmate(bool turn_move, std::string table[8][8], SDL_Rect* pieces, int num, move* moves_history, int moves_num)
{
	int i = 0, max = 16;
	if (turn_move)
	{
		i = 16;
		max = 32;
	}
	bool checkmate = true;
	int chosen_field[2];
	int become_field[2];
	int dst_become_piece[2];
	for (i; i < max; i++)
	{
		if (pieces[i].x != -100)
		{
			chosen_field[0] = pieces[i].y / size_field;
			chosen_field[1] = pieces[i].x / size_field;
			for (int n = 0; n < 8; n++)
				for (int m = 0; m < 8; m++)
				{
					become_field[0] = n;
					become_field[1] = m;
					dst_become_piece[0] = become_field[1] * size_field;
					dst_become_piece[1] = become_field[0] * size_field;
					if (table[chosen_field[0]][chosen_field[1]][1] == 'K')
					{
						if ((table[chosen_field[0]][chosen_field[1]][0] != table[become_field[0]][become_field[1]][0]) &&
							AvailableMove(chosen_field, become_field, table) && !BitField(turn_move, chosen_field, become_field, table, pieces))
						{
							checkmate = false;
							n = 8;
							i = max;
							break;
						}
					}
					else if (table[chosen_field[0]][chosen_field[1]][1] == 'P')
					{
						if (CheckEnPassant2(turn_move, table, pieces, chosen_field, become_field, dst_become_piece, num, moves_history, moves_num))
						{
							checkmate = false;
							n = 8;
							i = max;
							break;
						}
					}
					else
					{
						if ((table[chosen_field[0]][chosen_field[1]][0] != table[become_field[0]][become_field[1]][0]) &&
							AvailableMove(chosen_field, become_field, table) && !Check1(turn_move, chosen_field, become_field, table, pieces))
						{
							checkmate = false;
							n = 8;
							i = max;
							break;
						}
					}
				}
		}
	}
	return checkmate;
}
bool Stalemate(bool turn_move, std::string table[8][8], SDL_Rect* pieces, int num, move* moves_history, int moves_num)
{
	int i = 0, max = 16;
	if (turn_move)
	{
		i = 16;
		max = 32;
	}
	bool stalemate = true;
	int chosen_field[2];
	int become_field[2];
	int dst_become_piece[2];
	for (i; i < max; i++)
	{
		if (pieces[i].x != -100)
		{
			chosen_field[0] = pieces[i].y / size_field;
			chosen_field[1] = pieces[i].x / size_field;
			for (int n = 0; n < 8; n++)
				for (int m = 0; m < 8; m++)
				{
					become_field[0] = n;
					become_field[1] = m;
					dst_become_piece[0] = become_field[1] * size_field;
					dst_become_piece[1] = become_field[0] * size_field;
					if (table[chosen_field[0]][chosen_field[1]][1] == 'K')
					{
						if ((table[chosen_field[0]][chosen_field[1]][0] != table[become_field[0]][become_field[1]][0]) &&
							AvailableMove(chosen_field, become_field, table) && !BitField(turn_move, chosen_field, become_field, table, pieces))
						{
							stalemate = false;
							n = 8;
							i = max;
							break;
						}
					}
					else if (table[chosen_field[0]][chosen_field[1]][1] == 'P')
					{
						if (CheckEnPassant2(turn_move, table, pieces, chosen_field, become_field, dst_become_piece, num, moves_history, moves_num))
						{
							stalemate = false;
							n = 8;
							i = max;
							break;
						}
					}
					else
					{
						if ((table[chosen_field[0]][chosen_field[1]][0] != table[become_field[0]][become_field[1]][0]) &&
							AvailableMove(chosen_field, become_field, table) && !Check1(turn_move, chosen_field, become_field, table, pieces))
						{
							stalemate = false;
							n = 8;
							i = max;
							break;
						}
					}
				}
		}
	}
	return stalemate;
}
bool Draw(std::string table[8][8])
{
	std::vector <std::string> white_pieces = { "K" }, black_pieces = { "K" };

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (table[i][j] == "wB") white_pieces.push_back("B");
			else if (table[i][j] == "bB") black_pieces.push_back("B");
			else if (table[i][j] == "wN") white_pieces.push_back("N");
			else if (table[i][j] == "bN") black_pieces.push_back("N");
			else if ((table[i][j][1] != 'K') && (table[i][j] != "--")) return false;
		}

	return (white_pieces.size() <= 2) && (black_pieces.size() <= 2);
}
void RenAvailMove(bool turn_move, SDL_Renderer* ren, SDL_Texture* tex_dot, SDL_Texture* tex_frame, SDL_Texture* tex_star, SDL_Rect* pieces, 
	std::string table[8][8], int* chosen_field, int num, bool* castling, move* moves_history, int moves_num)
{
	int dst_become_piece[2];
	int become_field[2];
	SDL_Rect dot, frame;
	SDL_Rect star = { chosen_field[1] * size_field + 28, chosen_field[0] * size_field + 28, 24, 24 };
	SDL_RenderCopy(ren, tex_star, NULL, &star);
	for (int n = 0; n < 8; n++)
		for (int m = 0; m < 8; m++)
		{
			become_field[0] = n;
			become_field[1] = m;
			dst_become_piece[0] = become_field[1] * size_field;
			dst_become_piece[1] = become_field[0] * size_field;
			dot = { dst_become_piece[0] + 28, dst_become_piece[1] + 28, 24, 24 };
			frame = { dst_become_piece[0], dst_become_piece[1], size_field, size_field };
			if (table[chosen_field[0]][chosen_field[1]][0] != table[become_field[0]][become_field[1]][0])
			{
				if (table[chosen_field[0]][chosen_field[1]][1] == 'K')
				{
					if (CheckCastling2(turn_move, become_field, table, pieces, chosen_field, castling))
					{
						if (table[become_field[0]][become_field[1]] == "--") SDL_RenderCopy(ren, tex_dot, NULL, &dot);
						else SDL_RenderCopy(ren, tex_frame, NULL, &frame);
					}
				}
				else if (table[chosen_field[0]][chosen_field[1]][1] == 'P')
				{
					if (CheckEnPassant2(turn_move, table, pieces, chosen_field, become_field, dst_become_piece, num, moves_history, moves_num))
					{
						if (table[become_field[0]][become_field[1]] == "--") SDL_RenderCopy(ren, tex_dot, NULL, &dot);
						else SDL_RenderCopy(ren, tex_frame, NULL, &frame);
					}
				}
				else
				{
					if (AvailableMove(chosen_field, become_field, table) && !Check1(turn_move, chosen_field, become_field, table, pieces))
					{
						if (table[become_field[0]][become_field[1]] == "--") SDL_RenderCopy(ren, tex_dot, NULL, &dot);
						else SDL_RenderCopy(ren, tex_frame, NULL, &frame);
					}
				}
			}
		}
}
void RenCheck(bool turn_move, std::string table[8][8], SDL_Texture* tex_check)
{
	std::string find_field = "bK";

	if (turn_move) find_field = "wK";

	int king_field[2] = { -100, -100 };

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
			if (table[i][j] == find_field)
			{
				king_field[0] = i;
				king_field[1] = j;
				i = 8;
				break;
			}
	}

	SDL_Rect check = { king_field[1] * size_field, king_field[0] * size_field, size_field, size_field };

	SDL_RenderCopy(ren, tex_check, NULL, &check);
}
SDL_Texture* get_text_texture(SDL_Renderer*& ren, char* text, TTF_Font* font)
{
	SDL_Color fore_color = { 184, 231, 252 };
	SDL_Color back_color = { 105, 63, 181 };
	SDL_Surface* textSurface = TTF_RenderText_Shaded(font, text, fore_color, back_color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(ren, textSurface);
	SDL_FreeSurface(textSurface);
	return tex_text;
}
void ReturnTextures(int moves_num, SDL_Texture* textures[32], SDL_Texture* textures1[32], SDL_Texture* textures2[32], SDL_Texture* textures3[32], move* moves_history)
{
	for (int i = 0; i < 32; i++)
	{
		textures[i] = moves_history[moves_num].textures[i];
		textures1[i] = moves_history[moves_num].textures1[i];
		textures2[i] = moves_history[moves_num].textures2[i];
		textures3[i] = moves_history[moves_num].textures3[i];
	}
}
void ReturnMove(int moves_num, bool* turn_move, std::string table[8][8], SDL_Rect* pieces, bool* game_over, int* num, bool* isCheck, move* moves_history, bool* castling)
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			table[i][j] = moves_history[moves_num].table[i][j];

	for (int i = 0; i < 32; i++)
	{
		pieces[i].x = moves_history[moves_num].pieces[i].x;
		pieces[i].y = moves_history[moves_num].pieces[i].y;
		pieces[i].w = moves_history[moves_num].pieces[i].w;
		pieces[i].h = moves_history[moves_num].pieces[i].h;
	}

	*game_over = moves_history[moves_num].game_over;
	*num = moves_history[moves_num].num_last_moved_piece;
	*turn_move = moves_history[moves_num].turn_move;
	*isCheck = moves_history[moves_num].isCheck;

	for (int i = 0; i < 6; i++)
		castling[i] = moves_history[moves_num].castling[i];

	PrintTable(table);
}
void ChangeTextures(SDL_Texture* textures[32], SDL_Texture* textures1[32], SDL_Texture* textures2[32], SDL_Texture* textures3[32], int active_pieces)
{
	switch (active_pieces)
	{
	case 1:
		for (int i = 0; i < 32; i++) textures[i] = textures1[i];
		break;
	case 2:
		for (int i = 0; i < 32; i++) textures[i] = textures2[i];
		break;
	case 3:
		for (int i = 0; i < 32; i++) textures[i] = textures3[i];
		break;
	}
}
void SaveSettings(bool isMusic, bool isChunk, int active_board, int active_pieces)
{
	std::ofstream file_settings("settings.txt");
	file_settings << isMusic << std::endl;
	file_settings << isChunk << std::endl;
	file_settings << active_board << std::endl;
	file_settings << active_pieces;
	file_settings.close();
}
void SaveTextures(int moves_num, SDL_Texture* textures[32], SDL_Texture* textures1[32], SDL_Texture* textures2[32], SDL_Texture* textures3[32], move* moves_history)
{
	for (int i = 0; i < 32; i++)
	{
		moves_history[moves_num].textures[i] = textures[i];
		moves_history[moves_num].textures1[i] = textures1[i];
		moves_history[moves_num].textures2[i] = textures2[i];
		moves_history[moves_num].textures3[i] = textures3[i];
	}
}
void MoveRecording(int moves_num, move* moves_history, std::string table[8][8], SDL_Rect* pieces, bool game_over, int num_last_moved_piece, bool turn_move, bool isCheck, bool* castling)
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			moves_history[moves_num].table[i][j] = table[i][j];

	for (int i = 0; i < 32; i++)
	{
		moves_history[moves_num].pieces[i].x = pieces[i].x;
		moves_history[moves_num].pieces[i].y = pieces[i].y;
		moves_history[moves_num].pieces[i].w = pieces[i].w;
		moves_history[moves_num].pieces[i].h = pieces[i].h;
	}

	moves_history[moves_num].game_over = game_over;
	moves_history[moves_num].num_last_moved_piece = num_last_moved_piece;
	moves_history[moves_num].turn_move = turn_move;
	moves_history[moves_num].isCheck = isCheck;

	for (int i = 0; i < 6; i++)
		moves_history[moves_num].castling[i] = castling[i];
}
void ComputerMove(int* chosen_field2, int* become_field2, int* dst_chosen_piece2, int* dst_become_piece2, std::string table2[8][8], int* num_chosen_piece2, SDL_Rect* pieces2,
	bool* isCastling2, bool* isEnPassant2, bool* turn_move2, SDL_Texture* textures2[32], SDL_Texture* textures1_2[32], SDL_Texture* textures2_2[32], SDL_Texture* textures3_2[32],
	bool* available_move2, int* num_last_moved_piece2, bool* isCheck2, bool* isChunk, bool* game_over2, int* moves_num2, move* moves_history2, bool* isChosen2, bool* castling2)
{
	srand(time(NULL));

	std::this_thread::sleep_for(std::chrono::seconds(1));

	bool black_pieces[16] = {};
	bool fields[8][8] = {};

	int n = rand() % 16;
	while (pieces2[n].x == -100) n = rand() % 16;
	black_pieces[n] = true;

	dst_chosen_piece2[0] = pieces2[n].x;
	dst_chosen_piece2[1] = pieces2[n].y;
	chosen_field2[0] = pieces2[n].y / size_field;
	chosen_field2[1] = pieces2[n].x / size_field;
	*num_chosen_piece2 = n;

	*isCastling2 = false;
	*isEnPassant2 = false;

	bool correct = false;

	int k = 0;
	while (!correct)
	{
		if (k == 64)
		{
			while ((pieces2[n].x == -100) || black_pieces[n]) n = rand() % 16;
			black_pieces[n] = true;
			dst_chosen_piece2[0] = pieces2[n].x;
			dst_chosen_piece2[1] = pieces2[n].y;
			chosen_field2[0] = pieces2[n].y / size_field;
			chosen_field2[1] = pieces2[n].x / size_field;
			*num_chosen_piece2 = n;
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
					fields[i][j] = false;
			k = 0;
		}

		do
		{
			become_field2[0] = rand() % 8;
			become_field2[1] = rand() % 8;
		} while (fields[become_field2[0]][become_field2[1]]);
		fields[become_field2[0]][become_field2[1]] = true;
		k++;

		dst_become_piece2[0] = become_field2[1] * size_field;
		dst_become_piece2[1] = become_field2[0] * size_field;

		if (!((become_field2[0] == chosen_field2[0]) && (become_field2[1] == chosen_field2[1])) &&
			(table2[chosen_field2[0]][chosen_field2[1]][0] != table2[become_field2[0]][become_field2[1]][0]))
		{
			if (table2[chosen_field2[0]][chosen_field2[1]][1] == 'K')
			{
				if (CheckCastling1(turn_move2, become_field2, table2, pieces2, chosen_field2, castling2))
				{
					*available_move2 = true;
					*isEnPassant2 = true;
				}
				else
				{
					*available_move2 = AvailableMove(chosen_field2, become_field2, table2) &&
						!BitField(turn_move2, chosen_field2, become_field2, table2, pieces2);
				}
			}
			else if ((table2[chosen_field2[0]][chosen_field2[1]][1] == 'P') && (table2[become_field2[0]][become_field2[1]] == "--"))
			{
				if (CheckEnPassant1(turn_move2, table2, pieces2, chosen_field2, become_field2, dst_become_piece2, *num_last_moved_piece2, moves_history2, *moves_num2))
				{
					*available_move2 = true;
					*isEnPassant2 = true;
				}
				else
				{
					*available_move2 = AvailableMove(chosen_field2, become_field2, table2) &&
						!Check1(turn_move2, chosen_field2, become_field2, table2, pieces2);
				}
			}
			else
			{
				*available_move2 = AvailableMove(chosen_field2, become_field2, table2) &&
					!Check1(turn_move2, chosen_field2, become_field2, table2, pieces2);
			}
		}
		if (*available_move2) correct = true;
	}

	if ((table2[become_field2[0]][become_field2[1]] == "--") && !*isEnPassant2) Sound0("music/move.wav", *isChunk);
	else Sound0("music/take.wav", *isChunk);

	if (table2[become_field2[0]][become_field2[1]] != "--")
	{
		for (int i = 0; i < 32; i++)
		{
			if ((pieces2[i].x == dst_become_piece2[0]) && (pieces2[i].y == dst_become_piece2[1]))
			{
				pieces2[i].x = -100;
				pieces2[i].y = -100;
				break;
			}
		}
	}

	pieces2[*num_chosen_piece2].x = dst_become_piece2[0];
	pieces2[*num_chosen_piece2].y = dst_become_piece2[1];
	table2[become_field2[0]][become_field2[1]] = table2[chosen_field2[0]][chosen_field2[1]];
	table2[chosen_field2[0]][chosen_field2[1]] = "--";
	*turn_move2 = !*turn_move2;
	*available_move2 = false;
	*isCheck2 = false;
	*num_last_moved_piece2 = *num_chosen_piece2;

	if (*turn_move2 && (*num_last_moved_piece2 >= 8) && (*num_last_moved_piece2 <= 16))
	{
		if (become_field2[0] == 7)
		{
			textures2[*num_last_moved_piece2] = textures2[3];
			textures1_2[*num_last_moved_piece2] = textures1_2[3];
			textures2_2[*num_last_moved_piece2] = textures2_2[3];
			textures3_2[*num_last_moved_piece2] = textures3_2[3];
			table2[become_field2[0]][become_field2[1]] = "bQ";
		}
	}
	else if (!*turn_move2 && (*num_last_moved_piece2 >= 24) && (*num_last_moved_piece2 <= 32))
	{
		if (become_field2[0] == 0)
		{
			textures2[*num_last_moved_piece2] = textures2[19];
			textures1_2[*num_last_moved_piece2] = textures1_2[19];
			textures2_2[*num_last_moved_piece2] = textures2_2[19];
			textures3_2[*num_last_moved_piece2] = textures3_2[19];
			table2[become_field2[0]][become_field2[1]] = "wQ";
		}
	}

	std::cout << "AI has made its move" << "\n\n";
	PrintTable(table2);

	if (!((pieces2[4].x == 4 * size_field) && (pieces2[4].y == 0)))
		castling2[0] = false;;
	if (!((pieces2[0].x == 0) && (pieces2[0].y == 0)))
		castling2[1] = false;
	if (!((pieces2[7].x == 7 * size_field) && (pieces2[7].y == 0)))
		castling2[2] = false;
	if (!((pieces2[20].x == 4 * size_field) && (pieces2[20].y == 7 * size_field)))
		castling2[3] = false;
	if (!((pieces2[16].x == 0) && (pieces2[16].y == 7 * size_field)))
		castling2[4] = false;
	if (!((pieces2[23].x == 7 * size_field) && (pieces2[23].y == 7 * size_field)))
		castling2[5] = false;

	if (Draw(table2))
	{
		std::cout << "draw" << "\n\n";
		Sound1("music/victory_draw.wav", *isChunk);
		*game_over2 = true;
	}

	else if (Check2(*turn_move2, table2, pieces2))
	{
		*isCheck2 = true;
		if (Checkmate(*turn_move2, table2, pieces2, *num_last_moved_piece2, moves_history2, *moves_num2))
		{
			std::string winner = "white";
			if (*turn_move2) winner = "black";
			std::cout << "checkmate, " << winner << " won" << "\n\n";
			Sound1("music/victory_draw.wav", *isChunk);
			*game_over2 = true;
		}
		else std::cout << "check" << "\n\n";
	}

	else if (Stalemate(*turn_move2, table2, pieces2, *num_last_moved_piece2, moves_history2, *moves_num2))
	{
		std::cout << "stalemate" << "\n\n";
		Sound1("music/victory_draw.wav", *isChunk);
		*game_over2 = true;
	}

	if (*moves_num2 < 90)
	{
		(*moves_num2)++;
		MoveRecording(*moves_num2, moves_history2, table2, pieces2, *game_over2, *num_last_moved_piece2, *turn_move2, *isCheck2, castling2);
		SaveTextures(*moves_num2, textures2, textures1_2, textures2_2, textures3_2, moves_history2);
	}

	*isChosen2 = false;
}