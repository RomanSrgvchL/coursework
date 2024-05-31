#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h> 
#include <SDL_mixer.h>
#include <fstream>
#include <string>

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
bool castling[6] = { true, true, true, true, true, true };

struct move
{
	std::string table[8][8];
	SDL_Rect pieces[32];
	SDL_Texture* textures[32];
	bool game_over;
	int num_last_moved_piece;
	bool turn_move;
	bool isCheck;
	bool castling[6];
};


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

	if ((imgFlag = IMG_Init(IMG_INIT_PNG)) == 0)
	{
		std::cout << "Couldn't init SDL_Image! Error: " << SDL_GetError() << std::endl;;
		system("pause");
		DeInit(1);
	}

	if (!(imgFlag & IMG_INIT_PNG)) std::cout << "Couldn't init PNG library.\n";

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


void LoadMusic()
{
	background = Mix_LoadMUS("music/background1_1.mp3");
	Mix_PlayMusic(background, -1);
}


void Sound0(std::string name_file)
{
	sound = Mix_LoadWAV(name_file.c_str());
	Mix_PlayChannel(0, sound, 0);
}


void Sound1(std::string name_file)
{
	sound = Mix_LoadWAV(name_file.c_str());
	Mix_PlayChannel(-1, sound, 0);
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


bool AvailableMove(int* chosen_field, int *become_field,  std::string table[8][8])
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


bool BitField(bool turn_move, int* king_field, int* become_field, std::string table[8][8], SDL_Rect *pieces)
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
		if ((pieces[i].x != -100) && !((pieces[i].x == become_field[1] * size_field) && (pieces[i].y == become_field[0] * size_field)))
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
		if ((pieces[i].x != -100) && !((pieces[i].x == become_field[1] * size_field) && (pieces[i].y == become_field[0] * size_field)))
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


void CheckCastling1(bool turn_move, int* become_field, std::string table[8][8], SDL_Rect* pieces, int* chosen_field, bool* available_move, bool* isCastling)
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
			*available_move = true;
			become_field[1] = 2;
			*isCastling = true;
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
			*available_move = true;
			become_field[1] = 6;
			*isCastling = true;
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
			*available_move = true;
			become_field[1] = 2;
			*isCastling = true;
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
			*available_move = true;
			become_field[1] = 6;
			*isCastling = true;
		}
	}
	else
		*available_move = AvailableMove(chosen_field, become_field, table) &&
		!BitField(turn_move, chosen_field, become_field, table, pieces);
}


bool CheckCastling2(bool turn_move, int* become_field, std::string table[8][8], SDL_Rect* pieces, int* chosen_field)
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


void CheckEnPassant1(bool turn_move, std::string table[8][8], SDL_Rect* pieces, int* chosen_field, int* become_field, bool* available_move, bool* isEnPassant, int* dst_become_piece, int num)
{
	if (turn_move && (chosen_field[0] == 3) && (become_field[0] == 2) && (abs(chosen_field[1] - become_field[1]) == 1) &&
		(table[chosen_field[0]][become_field[1]] == "bP"))
	{
		for (int i = 8; i < 16; i++)
		{
			if ((pieces[i].x == dst_become_piece[0]) && (pieces[i].y == dst_become_piece[1] + size_field))
			{
				if (i == num)
				{
					table[chosen_field[0]][become_field[1]] = "--";
					if (!Check1(turn_move, chosen_field, become_field, table, pieces))
					{
						pieces[i].x = -100;
						pieces[i].y = -100;
						*available_move = true;
						*isEnPassant = true;
					}
					else table[chosen_field[0]][become_field[1]] = "bP";
				}
				else *available_move = AvailableMove(chosen_field, become_field, table);
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
				if (i == num)
				{
					table[chosen_field[0]][become_field[1]] = "--";
					if (!Check1(turn_move, chosen_field, become_field, table, pieces))
					{
						pieces[i].x = -100;
						pieces[i].y = -100;
						*available_move = true;
						*isEnPassant = true;
					}
					else table[chosen_field[0]][become_field[1]] = "wP";
				}
				else *available_move = AvailableMove(chosen_field, become_field, table);
				break;
			}
		}
	}
	else
		*available_move = AvailableMove(chosen_field, become_field, table) &&
		!Check1(turn_move, chosen_field, become_field, table, pieces);
}


bool CheckEnPassant2(bool turn_move, std::string table[8][8], SDL_Rect* pieces, int* chosen_field, int* become_field, int* dst_become_piece, int num)
{
	bool available_move = false;

	if (turn_move && (chosen_field[0] == 3) && (become_field[0] == 2) && (abs(chosen_field[1] - become_field[1]) == 1) &&
		(table[chosen_field[0]][become_field[1]] == "bP"))
	{
		for (int i = 8; i < 16; i++)
		{
			if ((pieces[i].x == dst_become_piece[0]) && (pieces[i].y == dst_become_piece[1] + size_field))
			{
				if (i == num)
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
				if (i == num)
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


bool Checkmate(bool turn_move, std::string table[8][8], SDL_Rect* pieces, int num)
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
						if (CheckEnPassant2(turn_move, table, pieces, chosen_field, become_field, dst_become_piece, num))
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


bool Stalemate(bool turn_move, std::string table[8][8], SDL_Rect* pieces, int num)
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
						if (CheckEnPassant2(turn_move, table, pieces, chosen_field, become_field, dst_become_piece, num))
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


bool Draw(bool turn_move, std::string table[8][8], SDL_Rect* pieces)
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

	if ((white_pieces.size() <= 2) && (black_pieces.size() <= 2)) return true;
}


void RenAvailMove(bool turn_move, SDL_Renderer* ren, SDL_Texture* tex_dot, SDL_Texture* tex_frame, SDL_Texture* tex_star, SDL_Rect* pieces, std::string table[8][8], int* chosen_field, int num)
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
					if (CheckCastling2(turn_move, become_field, table, pieces, chosen_field))
					{
						if (table[become_field[0]][become_field[1]] == "--") SDL_RenderCopy(ren, tex_dot, NULL, &dot);
						else SDL_RenderCopy(ren, tex_frame, NULL, &frame);
					}
				}
				else if (table[chosen_field[0]][chosen_field[1]][1] == 'P')	
				{
					if (CheckEnPassant2(turn_move, table, pieces, chosen_field, become_field, dst_become_piece, num))
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


int SDL_main(int argc, char* argv[])
{
	Init();

	#pragma region LoadTexture
	SDL_Texture* tex_dot = LoadTextureFromFile("pictures/dot.png");
	SDL_Texture* tex_frame = LoadTextureFromFile("pictures/frame.png");
	SDL_Texture* tex_check = LoadTextureFromFile("pictures/check.png");
	SDL_Texture* tex_star = LoadTextureFromFile("pictures/star.png");
	SDL_Texture* tex_logout = LoadTextureFromFile("pictures/logout.png");
	SDL_Texture* tex_return = LoadTextureFromFile("pictures/return.png");
	SDL_Texture* tex_return_gray = LoadTextureFromFile("pictures/return_gray.png");
	SDL_Texture* tex_flag = LoadTextureFromFile("pictures/flag.png");
	SDL_Texture* tex_menu_bar = LoadTextureFromFile("pictures/menu_bar.png");
	SDL_Texture* tex_wb = LoadTextureFromFile("pictures/wb.png");
	SDL_Texture* tex_wk = LoadTextureFromFile("pictures/wk.png");
	SDL_Texture* tex_wn = LoadTextureFromFile("pictures/wn.png");
	SDL_Texture* tex_wp = LoadTextureFromFile("pictures/wp.png");
	SDL_Texture* tex_wq = LoadTextureFromFile("pictures/wq.png");
	SDL_Texture* tex_wr = LoadTextureFromFile("pictures/wr.png");
	SDL_Texture* tex_bb = LoadTextureFromFile("pictures/bb.png");
	SDL_Texture* tex_bk = LoadTextureFromFile("pictures/bk.png");
	SDL_Texture* tex_bn = LoadTextureFromFile("pictures/bn.png");
	SDL_Texture* tex_bp = LoadTextureFromFile("pictures/bp.png");
	SDL_Texture* tex_bq = LoadTextureFromFile("pictures/bq.png");
	SDL_Texture* tex_br = LoadTextureFromFile("pictures/br.png");
	SDL_Texture* tex_board = LoadTextureFromFile("pictures/brown.png");
	SDL_Texture* textures[32] = {
			tex_br, tex_bn, tex_bb, tex_bq, tex_bk, tex_bb, tex_bn, tex_br,
			tex_bp, tex_bp, tex_bp, tex_bp, tex_bp, tex_bp, tex_bp, tex_bp,
			tex_wr, tex_wn, tex_wb, tex_wq, tex_wk, tex_wb, tex_wn, tex_wr,
			tex_wp, tex_wp, tex_wp, tex_wp, tex_wp, tex_wp, tex_wp, tex_wp
	};
	#pragma endregion

	move moves_history[200];

	bool isRunning = true;
	bool isCastling = false;
	bool isEnPassant = false;

	SDL_Event ev;
	SDL_Rect rect_board = { 0, 0, board_width, board_height };
	SDL_Rect rect_menu_bar = { board_width, 0, menu_bar, board_height };
	SDL_Rect rect_logout = { board_width + 10, 100, 80, 80 };
	SDL_Rect rect_return = { board_width + 10, 100 * 2 + 80, 80, 80 };
	SDL_Rect rect_flag = { board_width + 10, 100 * 3 + 80 * 2, 80, 80 };
	SDL_Rect rects[10] = {
			{ board_width, 0 , 100, 95 },
			{ board_width, 100 + 80 + 5, 100, 90 },
			{ board_width, 100 * 2 + 80 * 2 + 5, 100, 90 },
			{ board_width, 100 * 3 + 80 * 3 + 5, 100, 95 },
	};

	std::string table[8][8] = {
		{ "bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"},
		{ "bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP"},
		{ "--", "--", "--", "--", "--", "--", "--", "--"},
		{ "--", "--", "--", "--", "--", "--", "--", "--"},
		{ "--", "--", "--", "--", "--", "--", "--", "--"},
		{ "--", "--", "--", "--", "--", "--", "--", "--"},
		{ "wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP"},
		{ "wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"},
	};

	// 0 - 15 black, 16 - 31 white
	// 8 - 15 pawns, 24 - 31 pawns

	#pragma region Destinations
	SDL_Rect pieces[32];
	pieces[0] = { 0 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[1] = { 1 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[2] = { 2 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[3] = { 3 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[4] = { 4 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[5] = { 5 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[6] = { 6 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[7] = { 7 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[8] = { 0 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[9] = { 1 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[10] = { 2 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[11] = { 3 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[12] = { 4 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[13] = { 5 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[14] = { 6 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[15] = { 7 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[16] = { 0 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[17] = { 1 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[18] = { 2 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[19] = { 3 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[20] = { 4 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[21] = { 5 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[22] = { 6 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[23] = { 7 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[24] = { 0 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[25] = { 1 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[26] = { 2 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[27] = { 3 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[28] = { 4 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[29] = { 5 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[30] = { 6 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[31] = { 7 * size_field, 6 * size_field, size_piece, size_piece };
	#pragma endregion

	int chosen_field[2], become_field[2];
	int dst_chosen_piece[2], dst_become_piece[2];
	int num_chosen_piece = 0;
	int num_last_moved_piece = -1;
	int moves_num = 0;
	
	bool game_over = false;
	bool isChosen = false;
	bool isCheck = false;
	bool turn_move = true;
	bool available_move = false;

	//LoadMusic();

	#pragma region FirstMoveRecording;
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

	for (int i = 0; i < 32; i++)
		moves_history[moves_num].textures[i] = textures[i];

	moves_history[moves_num].game_over = game_over;
	moves_history[moves_num].num_last_moved_piece = num_last_moved_piece;
	moves_history[moves_num].turn_move = turn_move;
	moves_history[moves_num].isCheck = isCheck;

	for (int i = 0; i < 6; i++)
		moves_history[moves_num].castling[i] = castling[i];
	#pragma endregion


	while (isRunning)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					chosen_field[0] = ev.button.y / size_field;
					chosen_field[1] = ev.button.x / size_field;
					if ((ev.button.x >= 0) && (ev.button.y >= 0) && (ev.button.x < board_width) && (ev.button.y < board_height))
					{
						if ((table[chosen_field[0]][chosen_field[1]] != "--") && !game_over)
						{
							dst_chosen_piece[0] = chosen_field[1] * size_field;
							dst_chosen_piece[1] = chosen_field[0] * size_field;
							if ((turn_move == false) && (table[chosen_field[0]][chosen_field[1]][0] == 'w'))
								std::cout << "It's black's turn now" << std::endl;
							else if ((turn_move == true) && (table[chosen_field[0]][chosen_field[1]][0] == 'b'))
								std::cout << "It's white's turn now" << std::endl;
							else
								if (((ev.button.x > dst_chosen_piece[0] + miss) && (ev.button.x + miss < dst_chosen_piece[0] + size_field)) &&
									((ev.button.y > dst_chosen_piece[1] + miss) && (ev.button.y + miss < dst_chosen_piece[1] + size_field)))
								{
									std::cout << "you chose the " << table[chosen_field[0]][chosen_field[1]] << std::endl;
									for (int i = 0; i < 32; i++)
									{
										if ((pieces[i].x == dst_chosen_piece[0]) && (pieces[i].y == dst_chosen_piece[1]))
										{
											isChosen = true;
											num_chosen_piece = i;
											break;
										}
									}
								}
								else std::cout << "miss click protection" << std::endl;
						}
						else if ((table[chosen_field[0]][chosen_field[1]] == "--") && !game_over) std::cout << "you clicked on an empty field" << std::endl;
						else std::cout << "game over" << "\n";
					}
					else if ((ev.button.x > board_width + 5) && (ev.button.y > rect_logout.y - 5) && 
						(ev.button.x < board_width + menu_bar - 5) && (ev.button.y < rect_logout.y + rect_logout.h + 5))
					{
						std::cout << "logout" << "\n\n";
						DeInit(0);
					}
					else if ((ev.button.x > board_width + 5) && (ev.button.y > rect_return.y - 5) && 
						(ev.button.x < board_width + menu_bar - 5) && (ev.button.y < rect_return.y + rect_return.h + 5))
					{
						#pragma region MoveReturn
						if (moves_num > 0)
						{
							moves_num--;
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

							for (int i = 0; i < 32; i++)
								textures[i] = moves_history[moves_num].textures[i];
							
							game_over = moves_history[moves_num].game_over;
							num_last_moved_piece = moves_history[moves_num].num_last_moved_piece;
							turn_move = moves_history[moves_num].turn_move;
							isCheck = moves_history[moves_num].isCheck;

							for (int i = 0; i < 6; i++)
								castling[i] = moves_history[moves_num].castling[i];
							std::cout << "you returned the move" << "\n\n";
							PrintTable(table);
						}
						else
							std::cout << "you haven't made any moves yet" << "\n\n";
						#pragma endregion
					}
					else if ((ev.button.x > board_width + 5) && (ev.button.y > rect_flag.y - 5) && 
						(ev.button.x < board_width + menu_bar - 5) && (ev.button.y < rect_flag.y + rect_flag.h + 5) && !game_over)
					{
						if (turn_move) std::cout << "black won" << "\n\n";
						else std::cout << "white won" << "\n\n";
						Sound1("music/victory_draw.wav");
						game_over = true;
					}
					else std::cout << "empty" << std::endl;
				}
				break;
				case SDL_MOUSEMOTION:
				if (isChosen)
				{
					pieces[num_chosen_piece].x = ev.motion.x - 40;
					pieces[num_chosen_piece].y = ev.motion.y - 40;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					isCastling = false;
					isEnPassant = false;
					become_field[0] = ev.button.y / size_field;
					become_field[1] = ev.button.x / size_field;
					dst_become_piece[0] = become_field[1] * size_field;
					dst_become_piece[1] = become_field[0] * size_field;

					if ((isChosen == 1) && (ev.button.x >= 0) && (ev.button.y >= 0) && (ev.button.x < board_width) && (ev.button.y < board_height) && 
						!((become_field[0] == chosen_field[0]) && (become_field[1] == chosen_field[1])) && 
						(table[chosen_field[0]][chosen_field[1]][0] != table[become_field[0]][become_field[1]][0]))
					{
						if (table[chosen_field[0]][chosen_field[1]][1] == 'K')
							CheckCastling1(turn_move, become_field, table, pieces, chosen_field, &available_move, &isCastling);
						else if ((table[chosen_field[0]][chosen_field[1]][1] == 'P') && (table[become_field[0]][become_field[1]] == "--"))
							CheckEnPassant1(turn_move, table, pieces, chosen_field, become_field, &available_move, &isEnPassant, dst_become_piece, num_last_moved_piece);
						else
							available_move = AvailableMove(chosen_field, become_field, table) &&
							!Check1(turn_move, chosen_field, become_field, table, pieces);
						if (available_move)
						{ 
							if (isCastling) std::cout << "you castled" << "\n\n";
							else if (isEnPassant) std::cout << "you captured en passant" << "\n\n";
							else std::cout << "you moved the " << table[chosen_field[0]][chosen_field[1]] << "\n\n";

							if ((table[become_field[0]][become_field[1]] == "--") && !isEnPassant) Sound0("music/move.wav");
							else Sound0("music/take.wav");
							

							if (table[become_field[0]][become_field[1]] != "--")
							{
								for (int i = 0; i < 32; i++)
								{
									if ((pieces[i].x == dst_become_piece[0]) && (pieces[i].y == dst_become_piece[1]))
									{
										pieces[i].x = -100;
										pieces[i].y = -100;
										break;
									}
								}
							}

							pieces[num_chosen_piece].x = dst_become_piece[0];
							pieces[num_chosen_piece].y = dst_become_piece[1];
							table[become_field[0]][become_field[1]] = table[chosen_field[0]][chosen_field[1]];
							table[chosen_field[0]][chosen_field[1]] = "--";
							turn_move = !turn_move;
							available_move = false;
							isCheck = false;
							num_last_moved_piece = num_chosen_piece;
							
							if (turn_move && (num_last_moved_piece >= 8) && (num_last_moved_piece <= 16))
							{
								if (become_field[0] == 7)
								{
									textures[num_last_moved_piece] = tex_bq;
									table[become_field[0]][become_field[1]] = "bQ";
								}
							}		
							else if (!turn_move && (num_last_moved_piece >= 24) && (num_last_moved_piece <= 32))
							{
								if (become_field[0] == 0)
								{
									textures[num_last_moved_piece] = tex_wq;
									table[become_field[0]][become_field[1]] = "wQ";
								}
							}					

							PrintTable(table);

							if (!((pieces[4].x == 4 * size_field) && (pieces[4].y == 0)))
								castling[0] = false;
							if (!((pieces[0].x == 0) && (pieces[0].y == 0)))
								castling[1] = false;
							if (!((pieces[7].x == 7 * size_field) && (pieces[7].y == 0)))
								castling[2] = false;
							if (!((pieces[20].x == 4 * size_field) && (pieces[20].y == 7 * size_field)))
								castling[3] = false;
							if (!((pieces[16].x == 0) && (pieces[16].y == 7 * size_field)))
								castling[4] = false;
							if (!((pieces[23].x == 7 * size_field) && (pieces[23].y == 7 * size_field)))
								castling[5] = false;		

							if (Draw(turn_move, table, pieces))
							{
								std::cout << "draw" << "\n\n";
								Sound1("music/victory_draw.wav");
								game_over = true;
							}
							
							else if (Check2(turn_move, table, pieces))
							{
								isCheck = true;
								if (Checkmate(turn_move, table, pieces, num_last_moved_piece))
								{
									std::string winner = "white";
									if (turn_move) winner = "black";
									std::cout << "checkmate, " << winner << " won" << "\n\n";
									Sound1("music/victory_draw.wav");
									game_over = true;
								}
								else std::cout << "check" << "\n\n";
							}

							else if (Stalemate(turn_move, table, pieces, num_last_moved_piece))
							{
								std::cout << "stalemate" << "\n\n";
								Sound1("music/victory_draw.wav");
								game_over = true;
							}

							#pragma region MoveRecording
							moves_num++;
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

							for (int i = 0; i < 32; i++)
								moves_history[moves_num].textures[i] = textures[i];

							moves_history[moves_num].game_over = game_over;
							moves_history[moves_num].num_last_moved_piece = num_last_moved_piece;
							moves_history[moves_num].turn_move = turn_move;
							moves_history[moves_num].isCheck = isCheck;

							for (int i = 0; i < 6; i++)
								moves_history[moves_num].castling[i] = castling[i];
							#pragma endregion
						}
						else
						{
							std::cout << "this move is impossible" << std::endl;
							pieces[num_chosen_piece].x = dst_chosen_piece[0];
							pieces[num_chosen_piece].y = dst_chosen_piece[1];;
						}
					}
					else if (isChosen && !((dst_become_piece[0] == dst_chosen_piece[0]) &&
						(dst_become_piece[1] == dst_chosen_piece[1])))
					{
						std::cout << "this move is impossible" << std::endl;
						pieces[num_chosen_piece].x = dst_chosen_piece[0];
						pieces[num_chosen_piece].y = dst_chosen_piece[1];
					}
					else if (isChosen && (dst_become_piece[0] == dst_chosen_piece[0]) &&
						(dst_become_piece[1] == dst_chosen_piece[1]))
					{
						std::cout << "you put the " << table[become_field[0]][become_field[1]] << " back" << std::endl;
						pieces[num_chosen_piece].x = dst_chosen_piece[0];
						pieces[num_chosen_piece].y = dst_chosen_piece[1];
					}
					isChosen = false;
				}
				break;
			case SDL_KEYDOWN:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					isRunning = false;
					break;
				}
				break;
			}
		}

		SDL_RenderCopy(ren, tex_board, NULL, &rect_board);
		SDL_RenderCopy(ren, tex_menu_bar, NULL, &rect_menu_bar);
		SDL_RenderCopy(ren, tex_logout, NULL, &rect_logout);
		if (moves_num == 0) SDL_RenderCopy(ren, tex_return_gray, NULL, &rect_return);
		else SDL_RenderCopy(ren, tex_return, NULL, &rect_return);
		SDL_RenderCopy(ren, tex_flag, NULL, &rect_flag);
		SDL_RenderFillRects(ren, rects, 4);
		for (int i = 0; i < 32; i++) SDL_RenderCopy(ren, textures[i], NULL, &pieces[i]);
		if (isChosen) RenAvailMove(turn_move, ren, tex_dot, tex_frame, tex_star, pieces, table, chosen_field, num_last_moved_piece);
		if (isCheck) RenCheck(turn_move, table, tex_check);
		for (int i = 0; i < 32; i++) if (i == num_chosen_piece) SDL_RenderCopy(ren, textures[i], NULL, &pieces[i]);
		SDL_RenderPresent(ren);
		SDL_Delay(1000/fps);
	}

	#pragma region Destroy&Free&Close
	SDL_DestroyTexture(tex_dot);
	SDL_DestroyTexture(tex_frame);
	SDL_DestroyTexture(tex_check);
	SDL_DestroyTexture(tex_star);
	SDL_DestroyTexture(tex_menu_bar);
	SDL_DestroyTexture(tex_logout);
	SDL_DestroyTexture(tex_return);
	SDL_DestroyTexture(tex_return_gray);
	SDL_DestroyTexture(tex_flag);
	SDL_DestroyTexture(tex_bb);
	SDL_DestroyTexture(tex_bk);
	SDL_DestroyTexture(tex_bn);
	SDL_DestroyTexture(tex_bp);
	SDL_DestroyTexture(tex_bq);
	SDL_DestroyTexture(tex_br);
	SDL_DestroyTexture(tex_wb);
	SDL_DestroyTexture(tex_wk);
	SDL_DestroyTexture(tex_wn);
	SDL_DestroyTexture(tex_wp);
	SDL_DestroyTexture(tex_wq);
	SDL_DestroyTexture(tex_wr);
	SDL_DestroyTexture(tex_board);
	Mix_FreeMusic(background);
	Mix_FreeChunk(sound);
	Mix_CloseAudio();
	#pragma endregion

	DeInit(0);
	return 0;
}