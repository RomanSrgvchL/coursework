#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <math.h> 

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;

int miss = 5;
int size_field = 80;
int size_piece = 80;
int fps = 200;
int win_width = size_field * 8, win_height = size_field * 8;


void DeInit(int error)
{
	if (ren != NULL) SDL_DestroyRenderer(ren);
	if (win != NULL) SDL_DestroyWindow(win);
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

	if (imgFlag & IMG_INIT_PNG) std::cout << "Initialized PNG library.\n";
	else std::cout << "Couldn't init PNG library.\n";

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
	pieces[0] = { 2 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[1] = { 5 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[2] = { 4 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[3] = { 1 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[4] = { 6 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[5] = { 0 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[6] = { 1 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[7] = { 2 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[8] = { 3 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[9] = { 4 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[10] = { 5 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[11] = { 6 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[12] = { 7 * size_field, 6 * size_field, size_piece, size_piece };
	pieces[13] = { 3 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[14] = { 0 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[15] = { 7 * size_field, 7 * size_field, size_piece, size_piece };
	pieces[16] = { 2 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[17] = { 5 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[18] = { 4 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[19] = { 1 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[20] = { 6 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[21] = { 0 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[22] = { 1 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[23] = { 2 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[24] = { 3 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[25] = { 4 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[26] = { 5 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[27] = { 6 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[28] = { 7 * size_field, 1 * size_field, size_piece, size_piece };
	pieces[29] = { 3 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[30] = { 0 * size_field, 0 * size_field, size_piece, size_piece };
	pieces[31] = { 7 * size_field, 0 * size_field, size_piece, size_piece };
	#pragma endregion

	int chosed = 0;
	int chosed_field[2], became_field[2];
	int dst_chosed_piece[2], dst_became_piece[2];
	int number_chosed_piece = 0;
	std::string space = "";
	bool turn_move = true;
	bool available_move = false;
	bool castling = true;

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
					chosed_field[0] = ev.button.y / size_field;
					chosed_field[1] = ev.button.x / size_field;
					if (table[chosed_field[0]][chosed_field[1]] != "--")
					{
						dst_chosed_piece[0] = chosed_field[1] * size_field;
						dst_chosed_piece[1] = chosed_field[0] * size_field;
						if ((turn_move == false) && (table[chosed_field[0]][chosed_field[1]].find('w') != std::string::npos))
						{
							std::cout << "It's black's turn now" << std::endl;
						}
						else if ((turn_move == true) && (table[chosed_field[0]][chosed_field[1]].find('b') != std::string::npos))
							std::cout << "It's white's turn now" << std::endl;
						else
							if (((ev.button.x > dst_chosed_piece[0] + miss) && (ev.button.x + miss < dst_chosed_piece[0] + size_field)) &&
								((ev.button.y > dst_chosed_piece[1] + miss) && (ev.button.y + miss < dst_chosed_piece[1] + size_field)))
							{
								std::cout << "you chose " << table[chosed_field[0]][chosed_field[1]] << std::endl;
								chosed = 1;
								for (int i = 0; i < 32; i++)
								{
									if ((pieces[i].x == dst_chosed_piece[0]) && (pieces[i].y == dst_chosed_piece[1]))
									{
										number_chosed_piece = i;
										break;
									}
								}
							}
							else std::cout << "miss click protection" << std::endl;
					}
					else std::cout << "you clicked on an empty field" << std::endl;
				}
				break;
				case SDL_MOUSEMOTION:
				if (chosed == 1)
				{
					pieces[number_chosed_piece].x = ev.motion.x - 40;
					pieces[number_chosed_piece].y = ev.motion.y - 40;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					became_field[0] = ev.button.y / size_field;
					became_field[1] = ev.button.x / size_field;
					dst_became_piece[0] = became_field[1] * size_field;
					dst_became_piece[1] = became_field[0] * size_field;
					if ((chosed == 1) && (ev.button.x >= 0) && (ev.button.y >= 0) && (ev.button.x < win_width) && (ev.button.y < win_height) && 
						!((became_field[0] == chosed_field[0]) && (became_field[1] == chosed_field[1])) && !((turn_move == true) && 
						(table[became_field[0]][became_field[1]].find('w') != std::string::npos)) && 
						!((turn_move == false) && (table[became_field[0]][became_field[1]].find('b') != std::string::npos)))
					{
						// добавить рокировку
						// слон и ферзь не должны ходить сквозь фигуры
						switch (table[chosed_field[0]][chosed_field[1]][1])
						{
						case 'K': 
							if ((abs(chosed_field[0] - became_field[0]) <= 1) && (abs(chosed_field[1] - became_field[1]) <= 1))
							{
								castling = false;
								available_move = true;
							}
							break;
						case 'P':
							if (table[chosed_field[0]][chosed_field[1]].find('w') != std::string::npos)
							{
								if (((chosed_field[0] - became_field[0] == 1) || ((chosed_field[0] == 6 && became_field[0] == 4) && (table[5][chosed_field[1]] == "--"))) && 
									(chosed_field[1] == became_field[1]) && (table[became_field[0]][became_field[1]] == "--") ||  ((table[became_field[0]][became_field[1]] != "--") && 
									(chosed_field[0] - became_field[0] == 1) && (abs(chosed_field[1] - became_field[1]) == 1)))
								available_move = true;
							}
							else
							{
								if (((became_field[0] - chosed_field[0] == 1) || ((chosed_field[0] == 1 && became_field[0] == 3) && (table[2][chosed_field[1]] == "--"))) && 
									(chosed_field[1] == became_field[1]) && (table[became_field[0]][became_field[1]] == "--") || ((table[became_field[0]][became_field[1]] != "--") && 
									(became_field[0] - chosed_field[0] == 1) && (abs(chosed_field[1] - became_field[1]) == 1)))
								{
									available_move = true;
								}
							}
							break;
						case 'B':
							if (abs(chosed_field[0] - became_field[0]) == abs(chosed_field[1] - became_field[1]))
								available_move = true;
							break;
						case 'R':
							if ((chosed_field[0] != became_field[0]) && (chosed_field[1] == became_field[1]))
							{
								available_move = true;
								int min = std::min(chosed_field[0], became_field[0]);
								int max = std::max(chosed_field[0], became_field[0]);
								for (int i = min + 1; i < max; i++)
								{
									if (table[i][chosed_field[1]] != "--")
									{
										available_move = false;
										break;
									}
								}
							}
							else if ((chosed_field[0] == became_field[0]) && (chosed_field[1] != became_field[1]))
							{
								available_move = true;
								int min = std::min(chosed_field[1], became_field[1]);
								int max = std::max(chosed_field[1], became_field[1]);
								for (int i = min + 1; i < max; i++)
								{
									if (table[chosed_field[0]][i] != "--")
									{
										available_move = false;
										break;
									}
								}
							}
							break;
						case 'Q':
							if (((chosed_field[0] != became_field[0]) && (chosed_field[1] == became_field[1]) || (chosed_field[0] == became_field[0]) && 
								(chosed_field[1] != became_field[1])) || (abs(chosed_field[0] - became_field[0]) == abs(chosed_field[1] - became_field[1])) || 
								((abs(chosed_field[0] - became_field[0]) <= 1) && (abs(chosed_field[1] - became_field[1]) <= 1)))
								available_move = true;
							break;
						case 'N':
							if ((abs(chosed_field[0] - became_field[0]) == 1) && (abs(chosed_field[1] - became_field[1]) == 2) ||
								(abs(chosed_field[0] - became_field[0]) == 2) && (abs(chosed_field[1] - became_field[1]) == 1))
								available_move = true;
							break;
						}
							if (available_move)
							{ 
								std::cout << "you moved " << table[chosed_field[0]][chosed_field[1]] << "\n\n";
								space = table[chosed_field[0]][chosed_field[1]];
								table[chosed_field[0]][chosed_field[1]] = "--";
								if (table[became_field[0]][became_field[1]] != "--")
								{
									for (int i = 0; i < 32; i++)
									{
										if ((pieces[i].x == dst_became_piece[0]) && (pieces[i].y == dst_became_piece[1]))
										{
											pieces[i].x = -100;
											pieces[i].y = -100;
											break;
										}
									}
								}
								pieces[number_chosed_piece].x = dst_became_piece[0];
								pieces[number_chosed_piece].y = dst_became_piece[1];
								table[became_field[0]][became_field[1]] = space;
								for (int i = 0; i < 8; i++)
								{
									for (int j = 0; j < 8; j++)
										std::cout << table[i][j] << " ";
									std::cout << std::endl;
								}
								std::cout << std::endl;
								turn_move = !turn_move;
								available_move = false;
							}
							else
							{
								std::cout << "this move is impossible" << "\n\n";
								pieces[number_chosed_piece].x = dst_chosed_piece[0];
								pieces[number_chosed_piece].y = dst_chosed_piece[1];;
							}

					}
					else if ((chosed == 1) && !((dst_became_piece[0] == dst_chosed_piece[0]) &&
						(dst_became_piece[1] == dst_chosed_piece[1])))
					{
						std::cout << "this move is impossible" << "\n\n";
						pieces[number_chosed_piece].x = dst_chosed_piece[0];
						pieces[number_chosed_piece].y = dst_chosed_piece[1];
					}
					else if ((chosed == 1) && (dst_became_piece[0] == dst_chosed_piece[0]) &&
						(dst_became_piece[1] == dst_chosed_piece[1]))
					{
						std::cout << "you put " << table[became_field[0]][became_field[1]] << " back" << "\n\n";
						pieces[number_chosed_piece].x = dst_chosed_piece[0];
						pieces[number_chosed_piece].y = dst_chosed_piece[1];
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
		SDL_RenderCopy(ren, tex_board, NULL, NULL);
		SDL_RenderCopy(ren, tex_wb, NULL, &pieces[0]);
		SDL_RenderCopy(ren, tex_wb, NULL, &pieces[1]);
		SDL_RenderCopy(ren, tex_wk, NULL, &pieces[2]);
		SDL_RenderCopy(ren, tex_wn, NULL, &pieces[3]);
		SDL_RenderCopy(ren, tex_wn, NULL, &pieces[4]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[5]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[6]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[7]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[8]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[9]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[10]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[11]);
		SDL_RenderCopy(ren, tex_wp, NULL, &pieces[12]);
		SDL_RenderCopy(ren, tex_wq, NULL, &pieces[13]);
		SDL_RenderCopy(ren, tex_wr, NULL, &pieces[14]);
		SDL_RenderCopy(ren, tex_wr, NULL, &pieces[15]);
		SDL_RenderCopy(ren, tex_bb, NULL, &pieces[16]);
		SDL_RenderCopy(ren, tex_bb, NULL, &pieces[17]);
		SDL_RenderCopy(ren, tex_bk, NULL, &pieces[18]);
		SDL_RenderCopy(ren, tex_bn, NULL, &pieces[19]);
		SDL_RenderCopy(ren, tex_bn, NULL, &pieces[20]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[21]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[22]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[23]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[24]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[25]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[26]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[27]);
		SDL_RenderCopy(ren, tex_bp, NULL, &pieces[28]);
		SDL_RenderCopy(ren, tex_bq, NULL, &pieces[29]);
		SDL_RenderCopy(ren, tex_br, NULL, &pieces[30]);
		SDL_RenderCopy(ren, tex_br, NULL, &pieces[31]);
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
	#pragma endregion

	DeInit(0);
	return 0;
}