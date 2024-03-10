#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <math.h> 

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;

int size_field = 80;
int size_piece = size_field;
int miss = size_field / 16;
int height_font = 38;
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
// turn_move = true - проверка на шах белым | turn_move = false - проеверка на шах чёрным

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
						if (AvailableMove(chosen_field, become_field, table) &&
							!Check1(turn_move, chosen_field, become_field, table, pieces))
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
	#pragma endregion

	bool isRunning = true;
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
	
	bool turn_move = true;
	bool available_move = false;

	/*TTF_Font* font = TTF_OpenFont("fonts/sylfaen.ttf", height_font);
	char message[100] = "Checkmate! Do you want to play again?";
	SDL_Surface* surf_message = TTF_RenderText_Blended(font, message, { 180, 0, 0, 255 });
	SDL_Rect rect_message = { 0, 301, surf_message->w, surf_message->h };
	SDL_Texture* tex_message = SDL_CreateTextureFromSurface(ren, surf_message);
	SDL_FreeSurface(surf_message);*/

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
					become_field[0] = ev.button.y / size_field;
					become_field[1] = ev.button.x / size_field;
					dst_become_piece[0] = become_field[1] * size_field;
					dst_become_piece[1] = become_field[0] * size_field;
					if ((chosed == 1) && (ev.button.x >= 0) && (ev.button.y >= 0) && (ev.button.x < win_width) && (ev.button.y < win_height) && 
						!((become_field[0] == chosen_field[0]) && (become_field[1] == chosen_field[1])) && 
						(table[chosen_field[0]][chosen_field[1]][0] != table[become_field[0]][become_field[1]][0]))
					{
						if (table[chosen_field[0]][chosen_field[1]].find('K') != std::string::npos)
							if ((become_field[0] == 0) && (become_field[1] == 2) && castling[0] && castling[1] && (table[0][1] + table[0][2] +
								table[0][3] == "------") && !Check2(turn_move, table, pieces) && !BitField(turn_move, chosen_field, become_field, table, pieces))
							{
								become_field[1] = 3;
								if (!BitField(turn_move, chosen_field, become_field, table, pieces) )
								{
									table[0][3] = "bR";
									table[0][0] = "--";
									pieces[0].x = 240;
									pieces[0].y = 0;
									castling[0] = false;
									castling[1] = false;
									available_move = true;
									become_field[1] = 2;
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
									pieces[7].x = 400;
									pieces[7].y = 0;
									castling[0] = false;
									castling[2] = false;
									available_move = true;
									become_field[1] = 6;
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
									pieces[16].x = 240;
									pieces[16].y = 560;
									castling[3] = false;
									castling[4] = false;
									available_move = true;
									become_field[1] = 2;
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
									pieces[23].x = 400;
									pieces[23].y = 560;
									castling[3] = false;
									castling[5] = false;
									available_move = true;
									become_field[1] = 6;
								}
							}
							else
								available_move = AvailableMove(chosen_field, become_field, table) &&
								!BitField(turn_move, chosen_field, become_field, table, pieces);
						else
							available_move = AvailableMove(chosen_field, become_field, table) &&
							!Check1(turn_move, chosen_field, become_field, table, pieces);
						if (available_move)
						{ 
							std::cout << "you moved the " << table[chosen_field[0]][chosen_field[1]] << "\n\n";
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

							for (int i = 0; i < 8; i++)
							{
								for (int j = 0; j < 8; j++)
									std::cout << table[i][j] << " ";
								std::cout << std::endl;
							}
							std::cout << std::endl;

							if (!((pieces[4].x == 320) && (pieces[4].y == 0)))
								castling[0] = false;
							if (!((pieces[0].x == 0) && (pieces[0].y == 0)))
								castling[1] = false;
							if (!((pieces[7].x == 560) && (pieces[7].y == 0)))
								castling[2] = false;
							if (!((pieces[20].x == 320) && (pieces[20].y == 560)))
								castling[3] = false;
							if (!((pieces[16].x == 0) && (pieces[16].y == 560)))
								castling[4] = false;
							if (!((pieces[23].x == 560) && (pieces[23].y == 560)))
								castling[5] = false;

							if (Check2(turn_move, table, pieces))
							{
								if (Checkmate(turn_move, table, pieces))
								{
									std::string winner = "white";
									if (turn_move) winner = "black";
									std::cout << "checkmate, " << winner << " won" << "\n\n";
									for (int i = 0; i < 8; i++)
										for (int j = 0; j < 8; j++) {
											table[i][j] = "00";
										}
								}
								else std::cout << "check" << "\n";
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
		SDL_RenderCopy(ren, tex_br, NULL, &pieces[0]);
		SDL_RenderCopy(ren, tex_bn, NULL, &pieces[1]);
		SDL_RenderCopy(ren, tex_bb, NULL, &pieces[2]);
		SDL_RenderCopy(ren, tex_bq, NULL, &pieces[3]);
		SDL_RenderCopy(ren, tex_bk, NULL, &pieces[4]);
		SDL_RenderCopy(ren, tex_bb, NULL, &pieces[5]);
		SDL_RenderCopy(ren, tex_bn, NULL, &pieces[6]);
		SDL_RenderCopy(ren, tex_br, NULL, &pieces[7]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[8]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[9]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[10]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[11]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[12]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[13]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[14]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[15]);
		SDL_RenderCopy(ren, tex_wr, NULL, &pieces[16]);
		SDL_RenderCopy(ren, tex_wn, NULL, &pieces[17]);
		SDL_RenderCopy(ren, tex_wb, NULL, &pieces[18]);
		SDL_RenderCopy(ren, tex_wq, NULL, &pieces[19]);
		SDL_RenderCopy(ren, tex_wk, NULL, &pieces[20]);
		SDL_RenderCopy(ren, tex_wb, NULL, &pieces[21]);
		SDL_RenderCopy(ren, tex_wn, NULL, &pieces[22]);
		SDL_RenderCopy(ren, tex_wr, NULL, &pieces[23]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[24]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[25]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[26]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[27]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[28]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[29]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[30]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[31]);
		//SDL_RenderCopy(ren, tex_message, NULL, &rect_message);
		#pragma endregion
		// можно изменить размеры/координаты фигуры, чтобы от неё избавиться, но ещё можно просто перестать её рендерить
		// но в таком случае нужно будет постоянно проверять, находится ли фигура на доске, что сильно затратно

		SDL_RenderPresent(ren);

		SDL_Delay(1000/fps);
	}
	#pragma region DestroyTexture
	SDL_DestroyTexture(tex_wb);
	SDL_DestroyTexture(tex_wk);
	SDL_DestroyTexture(tex_wn);
	SDL_DestroyTexture(tex_wp);
	SDL_DestroyTexture(tex_wq);
	SDL_DestroyTexture(tex_wr);
	SDL_DestroyTexture(tex_bb);
	SDL_DestroyTexture(tex_bk);
	SDL_DestroyTexture(tex_bn);
	SDL_DestroyTexture(tex_bp);
	SDL_DestroyTexture(tex_bq);
	SDL_DestroyTexture(tex_br);
	SDL_DestroyTexture(tex_board);
	//SDL_DestroyTexture(tex_message);
	#pragma endregion

	//TTF_CloseFont(font);
	DeInit(0);
	return 0;
}