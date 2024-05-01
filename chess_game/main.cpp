#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h> 

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;

int size_field = 80;
int size_piece = size_field;
int miss = size_field / 16;
//int height_font = 38;
int fps = 150;
int win_width = size_field * 8, win_height = size_field * 8;
bool castling[6] = { true, true, true, true, true, true };
// bk , b 0-0-0,  b 0-0, wk, w 0-0-0, w 0-0 

void DeInit(int error)
{
	if (ren != NULL) SDL_DestroyRenderer(ren);
	if (win != NULL) SDL_DestroyWindow(win);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(error);
}


void Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
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
		if (table[chosen_field[0]][chosen_field[1]].find('w') != std::string::npos)
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
			{
				available_move = true;
			}
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
				if (turn_move) table[king_field[0]][king_field[1]] = "wK";
				else table[king_field[0]][king_field[1]] = "bK";
				table[become_field[0]][become_field[1]] = space;
				break;
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
				table[piece_chosen_field[0]][piece_chosen_field[1]] = table[become_field[0]][become_field[1]];
				table[become_field[0]][become_field[1]] = space;
				check = true;
				break;
			}
			table[piece_chosen_field[0]][piece_chosen_field[1]] = table[become_field[0]][become_field[1]];
			table[become_field[0]][become_field[1]] = space;
		}
	}
	return check;
}
// будет ли шах при таком ходе?

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
// есть ли шах? (ход уже сделан)
// turn_move == true - проверка на шах белым | turn_move == false - проеверка на шах чёрным

bool Checkmate(bool turn_move, std::string table[8][8], SDL_Rect* pieces)
{
	int king_field[2];
	std::string find_field = "bK";
	int i = 0, max = 16;
	if (turn_move)
	{
		find_field = "wK";
		i = 16;
		max = 32;
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
	bool checkmate = true;
	int chosen_field[2];
	int become_field[2];
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
					if (table[chosen_field[0]][chosen_field[1]].find('K') != std::string::npos)
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


bool Stalemate(bool turn_move, std::string table[8][8], SDL_Rect* pieces)
{
	/*bool stalemate = true;
	int chosen_field[2];
	int become_field[2];
	int i = 0, max = 16;
	if (turn_move)
	{
		i = 16;
		max = 32;
	}
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
					if (table[chosen_field[0]][chosen_field[1]].find('K') != std::string::npos)
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
					else
					{
						if (AvailableMove(chosen_field, become_field, table) &&
							!Check1(turn_move, chosen_field, become_field, table, pieces))
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
	return stalemate;*/
	return false;
}


int SDL_main(int argc, char* argv[])
{
	Init();

	#pragma region LoadTexture
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
	SDL_Texture* tex_board = LoadTextureFromFile("pictures/green.png");
	SDL_Texture* textures[32] = {
			tex_br, tex_bn, tex_bb, tex_bq, tex_bk, tex_bb, tex_bn, tex_br,
			tex_bp, tex_bp, tex_bp, tex_bp, tex_bp, tex_bp, tex_bp, tex_bp,
			tex_wr, tex_wn, tex_wb, tex_wq, tex_wk, tex_wb, tex_wn, tex_wr,
			tex_wp, tex_wp, tex_wp, tex_wp, tex_wp, tex_wp, tex_wp, tex_wp
	};
	#pragma endregion
	bool isRunning = true;
	bool isCastling = false;
	bool isEnPassant = false;
	SDL_Event ev;
	SDL_Rect rect_board = { 0, 0, win_width, win_height };
	
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

	// 0 - 15 чёрные, 16 - 31 - белые
	// 8 - 15 пешки, 24 - 31 пешки
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

	int chosed = 0;
	int chosen_field[2], become_field[2];
	int dst_chosen_piece[2], dst_become_piece[2];
	int number_chosen_piece;
	int number_last_moved_piece;
	int chosen_index = 0;
	
	bool turn_move = true;
	bool available_move = false;

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
					if ((table[chosen_field[0]][chosen_field[1]] != "--") && (table[chosen_field[0]][chosen_field[1]] != "00"))
					{
						dst_chosen_piece[0] = chosen_field[1] * size_field;
						dst_chosen_piece[1] = chosen_field[0] * size_field;
						if ((turn_move == false) && (table[chosen_field[0]][chosen_field[1]].find('w') != std::string::npos))
						{
							std::cout << "It's black's turn now" << std::endl;
						}
						else if ((turn_move == true) && (table[chosen_field[0]][chosen_field[1]].find('b') != std::string::npos))
							std::cout << "It's white's turn now" << std::endl;
						else
							if (((ev.button.x > dst_chosen_piece[0] + miss) && (ev.button.x + miss < dst_chosen_piece[0] + size_field)) &&
								((ev.button.y > dst_chosen_piece[1] + miss) && (ev.button.y + miss < dst_chosen_piece[1] + size_field)))
							{
								std::cout << "you chose the " << table[chosen_field[0]][chosen_field[1]] << std::endl;
								chosed = 1;
								for (int i = 0; i < 32; i++)
								{
									if ((pieces[i].x == dst_chosen_piece[0]) && (pieces[i].y == dst_chosen_piece[1]))
									{
										chosen_index = i;
										number_chosen_piece = i;
										break;
									}
								}
							}
							else std::cout << "miss click protection" << std::endl;
					}
					else if (table[chosen_field[0]][chosen_field[1]] == "--") std::cout << "you clicked on an empty field" << std::endl;
					else std::cout << "game over" << "\n";
				}
				break;
				case SDL_MOUSEMOTION:
				if (chosed == 1)
				{
					pieces[number_chosen_piece].x = ev.motion.x - 40;
					pieces[number_chosen_piece].y = ev.motion.y - 40;
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
					if ((chosed == 1) && (ev.button.x >= 0) && (ev.button.y >= 0) && (ev.button.x < win_width) && (ev.button.y < win_height) && 
						!((become_field[0] == chosen_field[0]) && (become_field[1] == chosen_field[1])) && 
						(table[chosen_field[0]][chosen_field[1]][0] != table[become_field[0]][become_field[1]][0]))
					{
						// рокировка
						if (table[chosen_field[0]][chosen_field[1]].find('K') != std::string::npos)
							if ((become_field[0] == 0) && (become_field[1] == 2) && castling[0] && castling[1] && (table[0][1] + table[0][2] +
								table[0][3] == "------") && !Check2(turn_move, table, pieces) && !BitField(turn_move, chosen_field, become_field, table, pieces))
							{
								become_field[1] = 3;
								if (!BitField(turn_move, chosen_field, become_field, table, pieces) )
								{
									table[0][3] = "bR";
									table[0][0] = "--";
									pieces[0].x = 3 * size_field;
									pieces[0].y = 0;
									castling[0] = false;
									castling[1] = false;
									available_move = true;
									become_field[1] = 2;
									isCastling = true;
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
									available_move = true;
									become_field[1] = 6;
									isCastling = true;
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
									available_move = true;
									become_field[1] = 2;
									isCastling = true;
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
									available_move = true;
									become_field[1] = 6;
									isCastling = true;
								}
							}
							else
								available_move = AvailableMove(chosen_field, become_field, table) &&
								!BitField(turn_move, chosen_field, become_field, table, pieces);
						// взятие на проходе
						else if ((table[chosen_field[0]][chosen_field[1]].find('P') != std::string::npos) && !Check1(turn_move, chosen_field, become_field, table, pieces) &&
							(table[become_field[0]][become_field[1]] == "--"))
						{
							if (turn_move && (chosen_field[0] == 3) && (become_field[0] == 2) && (abs(chosen_field[1] - become_field[1]) == 1) &&
								(table[chosen_field[0]][become_field[1]] == "bP"))
							{
								for (int i = 8; i < 16; i++)
								{
									if ((pieces[i].x == dst_become_piece[0]) && (pieces[i].y == dst_become_piece[1] + size_field))
									{
										if (i == number_last_moved_piece)
										{
											pieces[i].x = -100;
											pieces[i].y = -100;
											available_move = true;
											table[chosen_field[0]][become_field[1]] = "--";
											isEnPassant = true;
										}
										else
											available_move = AvailableMove(chosen_field, become_field, table);
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
										if (i == number_last_moved_piece)
										{
											pieces[i].x = -100;
											pieces[i].y = -100;
											available_move = true;
											table[chosen_field[0]][become_field[1]] = "--";
											isEnPassant = true;
										}
										else
											available_move = AvailableMove(chosen_field, become_field, table);
										break;
									}
								}
							}
							else
								available_move = AvailableMove(chosen_field, become_field, table);
						}
						else
							available_move = AvailableMove(chosen_field, become_field, table) &&
							!Check1(turn_move, chosen_field, become_field, table, pieces);
						if (available_move)
						{ 
							if (isCastling) std::cout << "you castled" << "\n\n";
							else if (isEnPassant) std::cout << "you captured en passant" << "\n\n";
							else std::cout << "you moved the " << table[chosen_field[0]][chosen_field[1]] << "\n\n";
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
							pieces[number_chosen_piece].x = dst_become_piece[0];
							pieces[number_chosen_piece].y = dst_become_piece[1];
							table[become_field[0]][become_field[1]] = table[chosen_field[0]][chosen_field[1]];
							table[chosen_field[0]][chosen_field[1]] = "--";
							turn_move = !turn_move;
							available_move = false;
							number_last_moved_piece = number_chosen_piece;

							for (int i = 0; i < 8; i++)
							{
								for (int j = 0; j < 8; j++)
									std::cout << table[i][j] << " ";
								std::cout << std::endl;
							}
							std::cout << std::endl;

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

							if (Check2(turn_move, table, pieces))
							{
								if (Checkmate(turn_move, table, pieces))
								{
									std::string winner = "white";
									if (turn_move) winner = "black";
									std::cout << "checkmate, " << winner << " won" << "\n\n";
									for (int i = 0; i < 8; i++)
										for (int j = 0; j < 8; j++)
											table[i][j] = "00";
								}
								else std::cout << "check" << "\n";
							}

							if (Stalemate(turn_move, table, pieces))
							{
								std::cout << "stalemate" << "\n\n";
								for (int i = 0; i < 8; i++)
									for (int j = 0; j < 8; j++)
										table[i][j] = "00";
							}
						}
						else
						{
							std::cout << "this move is impossible" << "\n\n";
							pieces[number_chosen_piece].x = dst_chosen_piece[0];
							pieces[number_chosen_piece].y = dst_chosen_piece[1];;
						}
					}
					else if ((chosed == 1) && !((dst_become_piece[0] == dst_chosen_piece[0]) &&
						(dst_become_piece[1] == dst_chosen_piece[1])))
					{
						std::cout << "this move is impossible" << "\n\n";
						pieces[number_chosen_piece].x = dst_chosen_piece[0];
						pieces[number_chosen_piece].y = dst_chosen_piece[1];
					}
					else if ((chosed == 1) && (dst_become_piece[0] == dst_chosen_piece[0]) &&
						(dst_become_piece[1] == dst_chosen_piece[1]))
					{
						std::cout << "you put the " << table[become_field[0]][become_field[1]] << " back" << "\n\n";
						pieces[number_chosen_piece].x = dst_chosen_piece[0];
						pieces[number_chosen_piece].y = dst_chosen_piece[1];
					}
					chosed = 0;
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

		#pragma region RenderCopy
		SDL_RenderCopy(ren, tex_board, NULL, &rect_board);
		for (int i = 0; i < 32; i++) SDL_RenderCopy(ren, textures[i], NULL, &pieces[i]);
		for (int i = 0; i < 32; i++)
			if (i == chosen_index) SDL_RenderCopy(ren, textures[i], NULL, &pieces[i]);
		#pragma endregion

		SDL_RenderPresent(ren);
		SDL_Delay(1000/fps);
	}

	#pragma region DestroyTexture
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
	#pragma endregion

	DeInit(0);
	return 0;
}