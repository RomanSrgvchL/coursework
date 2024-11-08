#include "header.h"

extern SDL_Window* win;
extern SDL_Renderer* ren;
extern Mix_Music* background;
extern Mix_Chunk* sound;

extern int size_field;
extern int size_piece;
extern int miss;
extern int fps;
extern int board_width;
extern int board_height;
extern int menu_bar;
extern int win_width;
extern int win_height;

int SDL_main(int argc, char* argv[])
{
	Init();

	#pragma region Menu&Settings&MenuBar
	int height_font = 500;
	TTF_Font* font = TTF_OpenFont("fonts/beer_money.ttf", height_font);
	char text_settings[9] = "Settings";
	char text_mode1[19] = "Play with a friend";
	char text_mode2[13] = "Play with AI";
	char text_quit[5] = "Quit";
	char text_back[5] = "Back";
	char text_music[6] = "Music";
	char text_sound_effects[14] = "Sound effects";
	char text_board_design[13] = "Board design";
	char text_pieces_design[14] = "Pieces design";
	SDL_Texture* tex_settings = get_text_texture(ren, text_settings, font);
	SDL_Texture* tex_mode1 = get_text_texture(ren, text_mode1, font);
	SDL_Texture* tex_mode2 = get_text_texture(ren, text_mode2, font);
	SDL_Texture* tex_quit = get_text_texture(ren, text_quit, font);
	SDL_Texture* tex_back = get_text_texture(ren, text_back, font);
	SDL_Texture* tex_music = get_text_texture(ren, text_music, font);
	SDL_Texture* tex_sound_effects = get_text_texture(ren, text_sound_effects, font);
	SDL_Texture* tex_board_design = get_text_texture(ren, text_board_design, font);
	SDL_Texture* tex_pieces_design = get_text_texture(ren, text_pieces_design, font);
	SDL_Rect rect_settings = { 220, 48, 300, 100 };
	SDL_Rect rect_mode1 = { 110, 48 * 2 + 100, 520, 100 };
	SDL_Rect rect_mode2 = { 170, 48 * 3 + 100 * 2, 400, 100 };
	SDL_Rect rect_quit = { 270, 48 * 4 + 100 * 3, 200, 100 };
	SDL_Rect rect_back = { 600, 10, 130, 70 };
	SDL_Rect rect_music = { 10, 15, 150, 70 };
	SDL_Rect rect_sound_effects = { 10, 100, 300, 70 };
	SDL_Rect rect_board_design = { 220, 185, 300, 70 };
	SDL_Rect rect_pieces_design = { 220, 413, 300, 70 };

	int l = 101, r = 112, h1 = 278, h2 = 505;

	SDL_Rect rect_board_brown = { l, h1, r, r };
	SDL_Rect rect_board_green = { l * 2 + r, h1, r, r };
	SDL_Rect rect_board_blue = { l * 3 + r * 2, h1, r, r };

	SDL_Rect rect_pieces1 = { l, h2, r, r };
	SDL_Rect rect_pieces2 = { l * 2 + r, h2, r, r };
	SDL_Rect rect_pieces3 = { l * 3 + r * 2, h2, r, r };

	SDL_Rect rects_s[6];
	for (int i = 0; i < 3; i++)
	{
		rects_s[i] = { 165 + i, 15 + i, 70 - 2 * i, 70 - 2 * i };
		rects_s[i + 3] = { 315 + i, 100 + i, 70 - 2 * i, 70 - 2 * i };
	}

	SDL_Rect rects_board_brown[7];
	SDL_Rect rects_board_green[7];
	SDL_Rect rects_board_blue[7];
	SDL_Rect rects_pieces1[7];
	SDL_Rect rects_pieces2[7];
	SDL_Rect rects_pieces3[7];

	for (int i = 7; i != 0; i--)
	{
		rects_board_brown[i - 1] = { l - i, h1 - i, r + 2 * i, r + 2 * i };
		rects_pieces1[i - 1] = { l - i, h2 - i, r + 2 * i, r + 2 * i };
	}
	for (int i = 7; i != 0; i--) 
	{
		rects_board_green[i - 1] = { l * 2 + r - i, h1 - i, r + 2 * i, r + 2 * i };
		rects_pieces2[i - 1] = { l * 2 + r - i, h2 - i, r + 2 * i, r + 2 * i };
	}
	for (int i = 7; i != 0; i--) 
	{
		rects_board_blue[i - 1] = { l * 3 + r * 2 - i, h1 - i, r + 2 * i, r + 2 * i };
		rects_pieces3[i - 1] = { l * 3 + r * 2 - i, h2 - i, r + 2 * i, r + 2 * i };
	}

	SDL_Rect rect_board = { 0, 0, board_width, board_height };
	int frame = 3;
	SDL_Rect rect_menu_bar = { board_width + frame, frame, menu_bar - 2 * frame, board_height - 2 * frame};
	SDL_Rect rect_logout = { board_width + 10, 65, 80, 80 };
	SDL_Rect rect_return = { board_width + 10, 65 * 2 + 80, 80, 80 };
	SDL_Rect rect_flag = { board_width + 10, 65 * 3 + 80 * 2, 80, 80 };
	SDL_Rect rect_restart = { board_width + 10, 65 * 4 + 80 * 3, 80, 80 };
	#pragma endregion

	#pragma region LoadTexture
	SDL_Texture* tex_dot = LoadTextureFromFile("pictures/dot.png");
	SDL_Texture* tex_frame = LoadTextureFromFile("pictures/frame.png");
	SDL_Texture* tex_check = LoadTextureFromFile("pictures/check.png");
	SDL_Texture* tex_star = LoadTextureFromFile("pictures/star.png");
	SDL_Texture* tex_logout = LoadTextureFromFile("pictures/logout.png");
	SDL_Texture* tex_return = LoadTextureFromFile("pictures/return.png");
	SDL_Texture* tex_return_gray = LoadTextureFromFile("pictures/return_gray.png");
	SDL_Texture* tex_flag = LoadTextureFromFile("pictures/flag.png");
	SDL_Texture* tex_flag_gray = LoadTextureFromFile("pictures/flag_gray.png");
	SDL_Texture* tex_restart = LoadTextureFromFile("pictures/restart.png");
	SDL_Texture* tex_restart_gray = LoadTextureFromFile("pictures/restart_gray.png");
	SDL_Texture* tex_tick = LoadTextureFromFile("pictures/tick.png");
	SDL_Texture* tex_cross = LoadTextureFromFile("pictures/cross.png");
	SDL_Texture* tex_wb1 = LoadTextureFromFile("pictures/pieces1/wb.png");
	SDL_Texture* tex_wk1 = LoadTextureFromFile("pictures/pieces1/wk.png");
	SDL_Texture* tex_wn1 = LoadTextureFromFile("pictures/pieces1/wn.png");
	SDL_Texture* tex_wp1 = LoadTextureFromFile("pictures/pieces1/wp.png");
	SDL_Texture* tex_wq1 = LoadTextureFromFile("pictures/pieces1/wq.png");
	SDL_Texture* tex_wr1 = LoadTextureFromFile("pictures/pieces1/wr.png");
	SDL_Texture* tex_bb1 = LoadTextureFromFile("pictures/pieces1/bb.png");
	SDL_Texture* tex_bk1 = LoadTextureFromFile("pictures/pieces1/bk.png");
	SDL_Texture* tex_bn1 = LoadTextureFromFile("pictures/pieces1/bn.png");
	SDL_Texture* tex_bp1 = LoadTextureFromFile("pictures/pieces1/bp.png");
	SDL_Texture* tex_bq1 = LoadTextureFromFile("pictures/pieces1/bq.png");
	SDL_Texture* tex_br1 = LoadTextureFromFile("pictures/pieces1/br.png");
	SDL_Texture* tex_wb2 = LoadTextureFromFile("pictures/pieces2/wb.png");
	SDL_Texture* tex_wk2 = LoadTextureFromFile("pictures/pieces2/wk.png");
	SDL_Texture* tex_wn2 = LoadTextureFromFile("pictures/pieces2/wn.png");
	SDL_Texture* tex_wp2 = LoadTextureFromFile("pictures/pieces2/wp.png");
	SDL_Texture* tex_wq2 = LoadTextureFromFile("pictures/pieces2/wq.png");
	SDL_Texture* tex_wr2 = LoadTextureFromFile("pictures/pieces2/wr.png");
	SDL_Texture* tex_bb2 = LoadTextureFromFile("pictures/pieces2/bb.png");
	SDL_Texture* tex_bk2 = LoadTextureFromFile("pictures/pieces2/bk.png");
	SDL_Texture* tex_bn2 = LoadTextureFromFile("pictures/pieces2/bn.png");
	SDL_Texture* tex_bp2 = LoadTextureFromFile("pictures/pieces2/bp.png");
	SDL_Texture* tex_bq2 = LoadTextureFromFile("pictures/pieces2/bq.png");
	SDL_Texture* tex_br2 = LoadTextureFromFile("pictures/pieces2/br.png");
	SDL_Texture* tex_wb3 = LoadTextureFromFile("pictures/pieces3/wb.png");
	SDL_Texture* tex_wk3 = LoadTextureFromFile("pictures/pieces3/wk.png");
	SDL_Texture* tex_wn3 = LoadTextureFromFile("pictures/pieces3/wn.png");
	SDL_Texture* tex_wp3 = LoadTextureFromFile("pictures/pieces3/wp.png");
	SDL_Texture* tex_wq3 = LoadTextureFromFile("pictures/pieces3/wq.png");
	SDL_Texture* tex_wr3 = LoadTextureFromFile("pictures/pieces3/wr.png");
	SDL_Texture* tex_bb3 = LoadTextureFromFile("pictures/pieces3/bb.png");
	SDL_Texture* tex_bk3 = LoadTextureFromFile("pictures/pieces3/bk.png");
	SDL_Texture* tex_bn3 = LoadTextureFromFile("pictures/pieces3/bn.png");
	SDL_Texture* tex_bp3 = LoadTextureFromFile("pictures/pieces3/bp.png");
	SDL_Texture* tex_bq3 = LoadTextureFromFile("pictures/pieces3/bq.png");
	SDL_Texture* tex_br3 = LoadTextureFromFile("pictures/pieces3/br.png");
	SDL_Texture* textures1_1[32] = {
			tex_br1, tex_bn1, tex_bb1, tex_bq1, tex_bk1, tex_bb1, tex_bn1, tex_br1,
			tex_bp1, tex_bp1, tex_bp1, tex_bp1, tex_bp1, tex_bp1, tex_bp1, tex_bp1,
			tex_wr1, tex_wn1, tex_wb1, tex_wq1, tex_wk1, tex_wb1, tex_wn1, tex_wr1,
			tex_wp1, tex_wp1, tex_wp1, tex_wp1, tex_wp1, tex_wp1, tex_wp1, tex_wp1
	};
	SDL_Texture* textures2_1[32] = {
			tex_br2, tex_bn2, tex_bb2, tex_bq2, tex_bk2, tex_bb2, tex_bn2, tex_br2,
			tex_bp2, tex_bp2, tex_bp2, tex_bp2, tex_bp2, tex_bp2, tex_bp2, tex_bp2,
			tex_wr2, tex_wn2, tex_wb2, tex_wq2, tex_wk2, tex_wb2, tex_wn2, tex_wr2,
			tex_wp2, tex_wp2, tex_wp2, tex_wp2, tex_wp2, tex_wp2, tex_wp2, tex_wp2
	};
	SDL_Texture* textures3_1[32] = {
			tex_br3, tex_bn3, tex_bb3, tex_bq3, tex_bk3, tex_bb3, tex_bn3, tex_br3,
			tex_bp3, tex_bp3, tex_bp3, tex_bp3, tex_bp3, tex_bp3, tex_bp3, tex_bp3,
			tex_wr3, tex_wn3, tex_wb3, tex_wq3, tex_wk3, tex_wb3, tex_wn3, tex_wr3,
			tex_wp3, tex_wp3, tex_wp3, tex_wp3, tex_wp3, tex_wp3, tex_wp3, tex_wp3
	};
	SDL_Texture* textures1_2[32] = {
			tex_br1, tex_bn1, tex_bb1, tex_bq1, tex_bk1, tex_bb1, tex_bn1, tex_br1,
			tex_bp1, tex_bp1, tex_bp1, tex_bp1, tex_bp1, tex_bp1, tex_bp1, tex_bp1,
			tex_wr1, tex_wn1, tex_wb1, tex_wq1, tex_wk1, tex_wb1, tex_wn1, tex_wr1,
			tex_wp1, tex_wp1, tex_wp1, tex_wp1, tex_wp1, tex_wp1, tex_wp1, tex_wp1
	};
	SDL_Texture* textures2_2[32] = {
			tex_br2, tex_bn2, tex_bb2, tex_bq2, tex_bk2, tex_bb2, tex_bn2, tex_br2,
			tex_bp2, tex_bp2, tex_bp2, tex_bp2, tex_bp2, tex_bp2, tex_bp2, tex_bp2,
			tex_wr2, tex_wn2, tex_wb2, tex_wq2, tex_wk2, tex_wb2, tex_wn2, tex_wr2,
			tex_wp2, tex_wp2, tex_wp2, tex_wp2, tex_wp2, tex_wp2, tex_wp2, tex_wp2
	};
	SDL_Texture* textures3_2[32] = {
			tex_br3, tex_bn3, tex_bb3, tex_bq3, tex_bk3, tex_bb3, tex_bn3, tex_br3,
			tex_bp3, tex_bp3, tex_bp3, tex_bp3, tex_bp3, tex_bp3, tex_bp3, tex_bp3,
			tex_wr3, tex_wn3, tex_wb3, tex_wq3, tex_wk3, tex_wb3, tex_wn3, tex_wr3,
			tex_wp3, tex_wp3, tex_wp3, tex_wp3, tex_wp3, tex_wp3, tex_wp3, tex_wp3
	};
	SDL_Texture* tex_board_brown = LoadTextureFromFile("pictures/brown.png");
	SDL_Texture* tex_board_green = LoadTextureFromFile("pictures/green.png");
	SDL_Texture* tex_board_blue = LoadTextureFromFile("pictures/blue.png");
	#pragma endregion

	#pragma region Destinations
	SDL_Rect pieces1[32], pieces2[32];
	pieces2[0] = pieces1[0] = { 0 * size_field, 0 * size_field, size_piece, size_piece };
	pieces2[1] = pieces1[1] = { 1 * size_field, 0 * size_field, size_piece, size_piece };
	pieces2[2] = pieces1[2] = { 2 * size_field, 0 * size_field, size_piece, size_piece };
	pieces2[3] = pieces1[3] = { 3 * size_field, 0 * size_field, size_piece, size_piece };
	pieces2[4] = pieces1[4] = { 4 * size_field, 0 * size_field, size_piece, size_piece };
	pieces2[5] = pieces1[5] = { 5 * size_field, 0 * size_field, size_piece, size_piece };
	pieces2[6] = pieces1[6] = { 6 * size_field, 0 * size_field, size_piece, size_piece };
	pieces2[7] = pieces1[7] = { 7 * size_field, 0 * size_field, size_piece, size_piece };
	pieces2[8] = pieces1[8] = { 0 * size_field, 1 * size_field, size_piece, size_piece };
	pieces2[9] = pieces1[9] = { 1 * size_field, 1 * size_field, size_piece, size_piece };
	pieces2[10] = pieces1[10] = { 2 * size_field, 1 * size_field, size_piece, size_piece };
	pieces2[11] = pieces1[11] = { 3 * size_field, 1 * size_field, size_piece, size_piece };
	pieces2[12] = pieces1[12] = { 4 * size_field, 1 * size_field, size_piece, size_piece };
	pieces2[13] = pieces1[13] = { 5 * size_field, 1 * size_field, size_piece, size_piece };
	pieces2[14] = pieces1[14] = { 6 * size_field, 1 * size_field, size_piece, size_piece };
	pieces2[15] = pieces1[15] = { 7 * size_field, 1 * size_field, size_piece, size_piece };
	pieces2[16] = pieces1[16] = { 0 * size_field, 7 * size_field, size_piece, size_piece };
	pieces2[17] = pieces1[17] = { 1 * size_field, 7 * size_field, size_piece, size_piece };
	pieces2[18] = pieces1[18] = { 2 * size_field, 7 * size_field, size_piece, size_piece };
	pieces2[19] = pieces1[19] = { 3 * size_field, 7 * size_field, size_piece, size_piece };
	pieces2[20] = pieces1[20] = { 4 * size_field, 7 * size_field, size_piece, size_piece };
	pieces2[21] = pieces1[21] = { 5 * size_field, 7 * size_field, size_piece, size_piece };
	pieces2[22] = pieces1[22] = { 6 * size_field, 7 * size_field, size_piece, size_piece };
	pieces2[23] = pieces1[23] = { 7 * size_field, 7 * size_field, size_piece, size_piece };
	pieces2[24] = pieces1[24] = { 0 * size_field, 6 * size_field, size_piece, size_piece };
	pieces2[25] = pieces1[25] = { 1 * size_field, 6 * size_field, size_piece, size_piece };
	pieces2[26] = pieces1[26] = { 2 * size_field, 6 * size_field, size_piece, size_piece };
	pieces2[27] = pieces1[27] = { 3 * size_field, 6 * size_field, size_piece, size_piece };
	pieces2[28] = pieces1[28] = { 4 * size_field, 6 * size_field, size_piece, size_piece };
	pieces2[29] = pieces1[29] = { 5 * size_field, 6 * size_field, size_piece, size_piece };
	pieces2[30] = pieces1[30] = { 6 * size_field, 6 * size_field, size_piece, size_piece };
	pieces2[31] = pieces1[31] = { 7 * size_field, 6 * size_field, size_piece, size_piece };
	// 0 - 15 black, 8 - 15 black pawns
	// 16 - 31 white, 24 - 31 white pawns
	#pragma endregion

	#pragma region Variables
	move moves_history1[100];
	move moves_history2[100];
	SDL_Event ev;
	std::string table1[8][8] = {
		{ "bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"},
		{ "bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP"},
		{ "--", "--", "--", "--", "--", "--", "--", "--"},
		{ "--", "--", "--", "--", "--", "--", "--", "--"},
		{ "--", "--", "--", "--", "--", "--", "--", "--"},
		{ "--", "--", "--", "--", "--", "--", "--", "--"},
		{ "wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP"},
		{ "wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"},
	};
	std::string table2[8][8] = {
		{ "bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"},
		{ "bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP"},
		{ "--", "--", "--", "--", "--", "--", "--", "--"},
		{ "--", "--", "--", "--", "--", "--", "--", "--"},
		{ "--", "--", "--", "--", "--", "--", "--", "--"},
		{ "--", "--", "--", "--", "--", "--", "--", "--"},
		{ "wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP"},
		{ "wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"},
	};

	int chosen_field1[2], become_field1[2], chosen_field2[2], become_field2[2];
	int dst_chosen_piece1[2], dst_become_piece1[2], dst_chosen_piece2[2], dst_become_piece2[2];
	int num_chosen_piece1 = 0, num_chosen_piece2 = 0;
	int num_last_moved_piece1 = -1, num_last_moved_piece2 = -1;
	int moves_num1 = 0, moves_num2 = 0;

	int mode = 0;
	int active_board;
	int active_pieces;
	bool isRunning = true;

	bool game_over1 = false, game_over2 = false;
	bool isChosen1 = false, isChosen2 = false;
	bool isCheck1 = false, isCheck2 = false;
	bool turn_move1 = true, turn_move2 = true;
	bool available_move1 = false, available_move2 = false;
	bool isCastling1 = false, isCastling2 = false;
	bool isEnPassant1 = false, isEnPassant2 = false;
	bool castling1[6] = { true, true, true, true, true, true };
	bool castling2[6] = { true, true, true, true, true, true };
	#pragma endregion

	#pragma region BasicSettins
	std::ifstream file_settings("settings.txt");
	std::string line1, line2, line3, line4;
	getline(file_settings, line1);
	getline(file_settings, line2);
	getline(file_settings, line3);
	getline(file_settings, line4);
	
	bool isMusic, isChunk;

	if (line1 == "1") isMusic = true;
	else isMusic = false;

	LoadMusic(true);
	if (!isMusic) Mix_PauseMusic();

	if (line2 == "1") isChunk = true;
	else isChunk = false;
	
	active_board = stoi(line3);
	active_pieces = stoi(line4);

	SDL_Texture* textures1[32];
	SDL_Texture* textures2[32];

	ChangeTextures(textures1, textures1_1, textures2_1, textures3_1, active_pieces);
	ChangeTextures(textures2, textures1_2, textures2_2, textures3_2, active_pieces);

	file_settings.close();
	#pragma endregion

	#pragma region RecordingFirstMove;
	MoveRecording(0, moves_history1, table1, pieces1, game_over1, num_last_moved_piece1, turn_move1, isCheck1, castling1);
	SaveTextures(0, textures1, textures1_1, textures2_1, textures3_1, moves_history1);
	MoveRecording(0, moves_history2, table2, pieces2, game_over2, num_last_moved_piece2, turn_move2, isCheck2, castling2);
	SaveTextures(0, textures2, textures1_2, textures2_2, textures3_2, moves_history2);
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
						if (mode == 0)
						{
							if ((ev.button.x > rect_settings.x) && (ev.button.y > rect_settings.y) &&
								(ev.button.x < rect_settings.x + rect_settings.w) && (ev.button.y < rect_settings.y + rect_settings.h))
							{
								std::cout << "settings" << "\n\n";
								mode = 3;
							}
							else if ((ev.button.x > rect_mode1.x) && (ev.button.y > rect_mode1.y) &&
								(ev.button.x < rect_mode1.x + rect_mode1.w) && (ev.button.y < rect_mode1.y + rect_mode1.h))
							{
								std::cout << "mode1" << "\n\n";
								mode = 1;
							}
							else if ((ev.button.x > rect_mode2.x) && (ev.button.y > rect_mode2.y) &&
								(ev.button.x < rect_mode2.x + rect_mode2.w) && (ev.button.y < rect_mode2.y + rect_mode2.h))
							{
								std::cout << "mode2" << "\n\n";
								mode = 2;
							}
							else if ((ev.button.x > rect_quit.x) && (ev.button.y > rect_quit.y) &&
								(ev.button.x < rect_quit.x + rect_quit.w) && (ev.button.y < rect_quit.y + rect_quit.h))
							{
								std::cout << "quit" << "\n\n";
								DeInit(0);
							}
						}
						else if (mode == 1)
						{
							#pragma region Game
							if ((ev.button.x >= 0) && (ev.button.y >= 0) && (ev.button.x < board_width) && (ev.button.y < board_height))
							{
								chosen_field1[0] = ev.button.y / size_field;
								chosen_field1[1] = ev.button.x / size_field;
								if ((table1[chosen_field1[0]][chosen_field1[1]] != "--") && !game_over1)
								{
									dst_chosen_piece1[0] = chosen_field1[1] * size_field;
									dst_chosen_piece1[1] = chosen_field1[0] * size_field;
									if ((turn_move1 == false) && (table1[chosen_field1[0]][chosen_field1[1]][0] == 'w'))
										std::cout << "It's black's turn now" << std::endl;
									else if ((turn_move1 == true) && (table1[chosen_field1[0]][chosen_field1[1]][0] == 'b'))
										std::cout << "It's white's turn now" << std::endl;
									else
										if (((ev.button.x > dst_chosen_piece1[0] + miss) && (ev.button.x + miss < dst_chosen_piece1[0] + size_field)) &&
											((ev.button.y > dst_chosen_piece1[1] + miss) && (ev.button.y + miss < dst_chosen_piece1[1] + size_field)))
										{
											std::cout << "you chose the " << table1[chosen_field1[0]][chosen_field1[1]] << std::endl;
											for (int i = 0; i < 32; i++)
											{
												if ((pieces1[i].x == dst_chosen_piece1[0]) && (pieces1[i].y == dst_chosen_piece1[1]))
												{
													isChosen1 = true;
													num_chosen_piece1 = i;
													break;
												}
											}
										}
										else std::cout << "miss click protection" << std::endl;
								}
								else if ((table1[chosen_field1[0]][chosen_field1[1]] == "--") && !game_over1) std::cout << "you clicked on an empty field" << std::endl;
								else std::cout << "game over" << "\n";
							}
							#pragma endregion

							#pragma region MenuBar
							else if ((ev.button.x > board_width + 5) && (ev.button.y > rect_logout.y - 5) && 
								(ev.button.x < board_width + menu_bar - 5) && (ev.button.y < rect_logout.y + rect_logout.h + 5))
							{
								std::cout << "you have entered the menu" << "\n\n";
								mode = 0;
							}
							else if ((ev.button.x > board_width + 5) && (ev.button.y > rect_return.y - 5) && 
								(ev.button.x < board_width + menu_bar - 5) && (ev.button.y < rect_return.y + rect_return.h + 5))
							{
								if ((moves_num1 != 0) && (moves_num1 < 90))
								{
									moves_num1--;
									std::cout << "you returned the move" << "\n\n";
									ReturnMove(moves_num1, &turn_move1, table1, pieces1, &game_over1, &num_last_moved_piece1, &isCheck1, moves_history1, castling1);
									ReturnTextures(moves_num1, textures1, textures1_1, textures2_1, textures3_1, moves_history1);
									ChangeTextures(textures1, textures1_1, textures2_1, textures3_1, active_pieces);
								}
								else if (moves_num1 == 0) std::cout << "the game hasn't started yet" << "\n\n";
								else std::cout << "you have exceeded the limit of moves" << "\n\n";
							}
							else if ((ev.button.x > board_width + 5) && (ev.button.y > rect_flag.y - 5) && 
								(ev.button.x < board_width + menu_bar - 5) && (ev.button.y < rect_flag.y + rect_flag.h + 5))
							{
								if ((moves_num1 != 0) && !game_over1)
								{
									if (turn_move1) std::cout << "black won" << "\n\n";
									else std::cout << "white won" << "\n\n";
									Sound1("music/victory_draw.wav", isChunk);
									game_over1 = true;
								}
								else if (game_over1) std::cout << "the game is over" << "\n\n";
								else std::cout << "the game hasn't started yet" << "\n\n";
							}
							else if ((ev.button.x > board_width + 5) && (ev.button.y > rect_restart.y - 5) &&
								(ev.button.x < board_width + menu_bar - 5) && (ev.button.y < rect_restart.y + rect_restart.h + 5))
							{
								if (moves_num1 != 0)
								{
									moves_num1 = 0;
									std::cout << "you restarted the game" << "\n\n";
									ReturnMove(moves_num1, &turn_move1, table1, pieces1, &game_over1, &num_last_moved_piece1, &isCheck1, moves_history1, castling1);
									ReturnTextures(moves_num1, textures1, textures1_1, textures2_1, textures3_1, moves_history1);
									ChangeTextures(textures1, textures1_1, textures2_1, textures3_1, active_pieces);
								}
								else std::cout << "the game hasn't started yet" << "\n\n";
							}
							#pragma endregion

							else std::cout << "empty" << std::endl;
						}
						else if (mode == 2)
						{
							if (turn_move2 || game_over2)
							{
								#pragma region Game	
								if ((ev.button.x >= 0) && (ev.button.y >= 0) && (ev.button.x < board_width) && (ev.button.y < board_height))
								{
									chosen_field2[0] = ev.button.y / size_field;
									chosen_field2[1] = ev.button.x / size_field;
									if ((table2[chosen_field2[0]][chosen_field2[1]] != "--") && !game_over2)
									{
										dst_chosen_piece2[0] = chosen_field2[1] * size_field;
										dst_chosen_piece2[1] = chosen_field2[0] * size_field;
										if (table2[chosen_field2[0]][chosen_field2[1]][0] == 'b') std::cout << "It's white's turn now" << std::endl;
										else
											if (((ev.button.x > dst_chosen_piece2[0] + miss) && (ev.button.x + miss < dst_chosen_piece2[0] + size_field)) &&
												((ev.button.y > dst_chosen_piece2[1] + miss) && (ev.button.y + miss < dst_chosen_piece2[1] + size_field)))
											{
												std::cout << "you chose the " << table2[chosen_field2[0]][chosen_field2[1]] << std::endl;
												for (int i = 0; i < 32; i++)
												{
													if ((pieces2[i].x == dst_chosen_piece2[0]) && (pieces2[i].y == dst_chosen_piece2[1]))
													{
														isChosen2 = true;
														num_chosen_piece2 = i;
														break;
													}
												}
											}
											else std::cout << "miss click protection" << std::endl;
									}
									else if ((table2[chosen_field2[0]][chosen_field2[1]] == "--") && !game_over2) std::cout << "you clicked on an empty field" << std::endl;
									else std::cout << "game over" << "\n";
								}
								#pragma endregion

								#pragma region MenuBar
								else if ((ev.button.x > board_width + 5) && (ev.button.y > rect_logout.y - 5) &&
									(ev.button.x < board_width + menu_bar - 5) && (ev.button.y < rect_logout.y + rect_logout.h + 5))
								{
									std::cout << "you have entered the menu" << "\n\n";
									mode = 0;
								}
								else if ((ev.button.x > board_width + 5) && (ev.button.y > rect_return.y - 5) &&
									(ev.button.x < board_width + menu_bar - 5) && (ev.button.y < rect_return.y + rect_return.h + 5))
								{
									if ((moves_num2 > 1) && (moves_num2 < 90))
									{
										if (!game_over2 || (turn_move2 && game_over2)) moves_num2 -= 2;
										else moves_num2--;
										std::cout << "you returned the move" << "\n\n";
										ReturnMove(moves_num2, &turn_move2, table2, pieces2, &game_over2, &num_last_moved_piece2, &isCheck2, moves_history2, castling2);
										ReturnTextures(moves_num2, textures2, textures1_2, textures2_2, textures3_2, moves_history2);
										ChangeTextures(textures2, textures1_2, textures2_2, textures3_2, active_pieces);
									}
									else if (moves_num2 < 90) std::cout << "the game hasn't started yet" << "\n\n";
									else std::cout << "you have exceeded the limit of moves" << "\n\n";
								}
								else if ((ev.button.x > board_width + 5) && (ev.button.y > rect_flag.y - 5) &&
									(ev.button.x < board_width + menu_bar - 5) && (ev.button.y < rect_flag.y + rect_flag.h + 5))
								{
									if ((moves_num2 != 0) && !game_over2)
									{
										std::cout << "black won" << "\n\n";
										Sound1("music/victory_draw.wav", isChunk);
										game_over2 = true;
									}
									else if (game_over2) std::cout << "the game is over" << "\n\n";
									else std::cout << "the game hasn't started yet" << "\n\n";
								}
								else if ((ev.button.x > board_width + 5) && (ev.button.y > rect_restart.y - 5) &&
									(ev.button.x < board_width + menu_bar - 5) && (ev.button.y < rect_restart.y + rect_restart.h + 5))
								{
									if (moves_num2 != 0)
									{
										moves_num2 = 0;
										std::cout << "you restarted the game" << "\n\n";
										ReturnMove(moves_num2, &turn_move2, table2, pieces2, &game_over2, &num_last_moved_piece2, &isCheck2, moves_history2, castling2);
										ReturnTextures(moves_num2, textures2, textures1_2, textures2_2, textures3_2, moves_history2);
										ChangeTextures(textures2, textures1_2, textures2_2, textures3_2, active_pieces);;
									}
									else std::cout << "the game hasn't started yet" << "\n\n";
								}
								#pragma endregion

								else std::cout << "empty" << std::endl;			
							}
							else std::cout << "It's AI's turn now" << std::endl;
						}
						else if (mode == 3)
						{
							if ((ev.button.x > rect_back.x) && (ev.button.y > rect_back.y) &&
								(ev.button.x < rect_back.x + rect_settings.w) && (ev.button.y < rect_back.y + rect_back.h))
							{
								std::cout << "back" << "\n\n";
								mode = 0;
							}
							else if ((ev.button.x > rects_s[2].x) && (ev.button.y > rects_s[2].y) &&
								(ev.button.x < rects_s[2].x + rects_s[2].w) && (ev.button.y < rects_s[2].y + rects_s[2].h))
							{
								isMusic = !isMusic;
								if (!isMusic) Mix_PauseMusic();
								else Mix_ResumeMusic();
								SaveSettings(isMusic, isChunk, active_board, active_pieces);
							}
							else if ((ev.button.x > rects_s[5].x) && (ev.button.y > rects_s[5].y) &&
								(ev.button.x < rects_s[5].x + rects_s[5].w) && (ev.button.y < rects_s[5].y + rects_s[5].h))
							{
								isChunk = !isChunk;
								SaveSettings(isMusic, isChunk, active_board, active_pieces);
							}
							else if ((ev.button.x > rect_board_brown.x) && (ev.button.y > rect_board_brown.y) &&
								(ev.button.x < rect_board_brown.x + rect_board_brown.w) && (ev.button.y < rect_board_brown.y + rect_board_brown.h))
							{
								active_board = 1;
								SaveSettings(isMusic, isChunk, active_board, active_pieces);
							}
							else if ((ev.button.x > rect_board_green.x) && (ev.button.y > rect_board_green.y) &&
								(ev.button.x < rect_board_green.x + rect_board_green.w) && (ev.button.y < rect_board_green.y + rect_board_green.h))
							{
								active_board = 2;
								SaveSettings(isMusic, isChunk, active_board, active_pieces);
							}
							else if ((ev.button.x > rect_board_blue.x) && (ev.button.y > rect_board_blue.y) &&
								(ev.button.x < rect_board_blue.x + rect_board_blue.w) && (ev.button.y < rect_board_blue.y + rect_board_blue.h))
							{
								active_board = 3;
								SaveSettings(isMusic, isChunk, active_board, active_pieces);
							}
							else if ((ev.button.x > rect_pieces1.x) && (ev.button.y > rect_pieces1.y) &&
								(ev.button.x < rect_pieces1.x + rect_pieces1.w) && (ev.button.y < rect_pieces1.y + rect_pieces1.h))
							{
								active_pieces = 1;
								for (int i = 0; i < 32; i++)
								{
									textures1[i] = textures1_1[i];
									textures2[i] = textures1_2[i];
								}
								SaveSettings(isMusic, isChunk, active_board, active_pieces);
							}
							else if ((ev.button.x > rect_pieces2.x) && (ev.button.y > rect_pieces2.y) &&
								(ev.button.x < rect_pieces2.x + rect_pieces2.w) && (ev.button.y < rect_pieces2.y + rect_pieces2.h))
							{
								active_pieces = 2;
								for (int i = 0; i < 32; i++)
								{
									textures1[i] = textures2_1[i];
									textures2[i] = textures2_2[i];
								}
								SaveSettings(isMusic, isChunk, active_board, active_pieces);
							}
							else if ((ev.button.x > rect_pieces3.x) && (ev.button.y > rect_pieces3.y) &&
								(ev.button.x < rect_pieces3.x + rect_pieces3.w) && (ev.button.y < rect_pieces3.y + rect_pieces3.h))
							{
								active_pieces = 3;
								for (int i = 0; i < 32; i++)
								{
									textures1[i] = textures3_1[i];
									textures2[i] = textures3_2[i];
								}
								SaveSettings(isMusic, isChunk, active_board, active_pieces);
							}
						}
					}
					break;
				case SDL_MOUSEMOTION:
					if (mode == 1)
					{
						if (isChosen1)
						{
							pieces1[num_chosen_piece1].x = ev.motion.x - size_field / 2;
							pieces1[num_chosen_piece1].y = ev.motion.y - size_field / 2;
						}	
					}
					else if (mode == 2)
					{
						if (turn_move2 && isChosen2)
						{
							pieces2[num_chosen_piece2].x = ev.motion.x - size_field / 2;
							pieces2[num_chosen_piece2].y = ev.motion.y - size_field / 2;
						}
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (ev.button.button == SDL_BUTTON_LEFT)
					{
						if (mode == 1)
						{
							isCastling1 = false;
							isEnPassant1 = false;
							become_field1[0] = ev.button.y / size_field;
							become_field1[1] = ev.button.x / size_field;
							dst_become_piece1[0] = become_field1[1] * size_field;
							dst_become_piece1[1] = become_field1[0] * size_field;

							if (isChosen1 && (ev.button.x >= 0) && (ev.button.y >= 0) && (ev.button.x < board_width) && (ev.button.y < board_height) && 
								!((become_field1[0] == chosen_field1[0]) && (become_field1[1] == chosen_field1[1])) && 
								(table1[chosen_field1[0]][chosen_field1[1]][0] != table1[become_field1[0]][become_field1[1]][0]))
							{
								if (table1[chosen_field1[0]][chosen_field1[1]][1] == 'K')
								{
									if (CheckCastling1(turn_move1, become_field1, table1, pieces1, chosen_field1, castling1))
									{
										available_move1 = true;
										isEnPassant1 = true;
									}
									else
									{
										available_move1 = AvailableMove(chosen_field1, become_field1, table1) &&
											!BitField(turn_move1, chosen_field1, become_field1, table1, pieces1);
									}
								}
									
								else if ((table1[chosen_field1[0]][chosen_field1[1]][1] == 'P') && (table1[become_field1[0]][become_field1[1]] == "--"))
								{
									if (CheckEnPassant1(turn_move1, table1, pieces1, chosen_field1, become_field1, dst_become_piece1, num_last_moved_piece1, moves_history1, moves_num1))
									{
										available_move1 = true;
										isEnPassant1 = true;
									}
									else
									{
										available_move1 = AvailableMove(chosen_field1, become_field1, table1) &&
											!Check1(turn_move1, chosen_field1, become_field1, table1, pieces1);
									}
								}
								else
								{
									available_move1 = AvailableMove(chosen_field1, become_field1, table1) &&
										!Check1(turn_move1, chosen_field1, become_field1, table1, pieces1);
								}
								if (available_move1)
								{ 
									if (isCastling1) std::cout << "you castled" << "\n\n";
									else if (isEnPassant1) std::cout << "you captured en passant" << "\n\n";
									else std::cout << "you moved the " << table1[chosen_field1[0]][chosen_field1[1]] << "\n\n";

									if ((table1[become_field1[0]][become_field1[1]] == "--") && !isEnPassant1) Sound0("music/move.wav", isChunk);
									else Sound0("music/take.wav", isChunk);

									if (table1[become_field1[0]][become_field1[1]] != "--")
									{
										for (int i = 0; i < 32; i++)
										{
											if ((pieces1[i].x == dst_become_piece1[0]) && (pieces1[i].y == dst_become_piece1[1]))
											{
												pieces1[i].x = -100;
												pieces1[i].y = -100;
												break;
											}
										}
									}

									pieces1[num_chosen_piece1].x = dst_become_piece1[0];
									pieces1[num_chosen_piece1].y = dst_become_piece1[1];
									table1[become_field1[0]][become_field1[1]] = table1[chosen_field1[0]][chosen_field1[1]];
									table1[chosen_field1[0]][chosen_field1[1]] = "--";
									turn_move1 = !turn_move1;
									available_move1 = false;
									isCheck1 = false;
									num_last_moved_piece1 = num_chosen_piece1;
							
									if (turn_move1 && (num_last_moved_piece1 >= 8) && (num_last_moved_piece1 <= 16))
									{
										if (become_field1[0] == 7)
										{
											textures1[num_last_moved_piece1] = textures1[3];
											textures1_1[num_last_moved_piece1] = textures1_1[3];
											textures2_1[num_last_moved_piece1] = textures2_1[3];
											textures3_1[num_last_moved_piece1] = textures3_1[3];
											table1[become_field1[0]][become_field1[1]] = "bQ";
										}
									}		
									else if (!turn_move1 && (num_last_moved_piece1 >= 24) && (num_last_moved_piece1 <= 32))
									{
										if (become_field1[0] == 0)
										{
											textures1[num_last_moved_piece1] = textures1[19];
											textures1_1[num_last_moved_piece1] = textures1_1[19];
											textures2_1[num_last_moved_piece1] = textures2_1[19];
											textures3_1[num_last_moved_piece1] = textures3_1[19];
											table1[become_field1[0]][become_field1[1]] = "wQ";
										}
									}					

									PrintTable(table1);

									if (!((pieces1[4].x == 4 * size_field) && (pieces1[4].y == 0)))
										castling1[0] = false;;
									if (!((pieces1[0].x == 0) && (pieces1[0].y == 0)))
										castling1[1] = false;
									if (!((pieces1[7].x == 7 * size_field) && (pieces1[7].y == 0)))
										castling1[2] = false;
									if (!((pieces1[20].x == 4 * size_field) && (pieces1[20].y == 7 * size_field)))
										castling1[3] = false;
									if (!((pieces1[16].x == 0) && (pieces1[16].y == 7 * size_field)))
										castling1[4] = false;
									if (!((pieces1[23].x == 7 * size_field) && (pieces1[23].y == 7 * size_field)))
										castling1[5] = false;		

									if (Draw(table1))
									{
										std::cout << "draw" << "\n\n";
										Sound1("music/victory_draw.wav", isChunk);
										game_over1 = true;
									}
							
									else if (Check2(turn_move1, table1, pieces1))
									{
										isCheck1 = true;
										if (Checkmate(turn_move1, table1, pieces1, num_last_moved_piece1, moves_history1, moves_num1))
										{
											std::string winner = "white";
											if (turn_move1) winner = "black";
											std::cout << "checkmate, " << winner << " won" << "\n\n";
											Sound1("music/victory_draw.wav", isChunk);
											game_over1 = true;
										}
										else std::cout << "check" << "\n\n";
									}

									else if (Stalemate(turn_move1, table1, pieces1, num_last_moved_piece1, moves_history1, moves_num1))
									{
										std::cout << "stalemate" << "\n\n";
										Sound1("music/victory_draw.wav", isChunk);
										game_over1 = true;
									}

									if (moves_num1 < 90)
									{
										moves_num1++;
										MoveRecording(moves_num1, moves_history1, table1, pieces1, game_over1, num_last_moved_piece1, turn_move1, isCheck1, castling1);
										SaveTextures(moves_num1, textures1, textures1_1, textures2_1, textures3_1, moves_history1);
									}					
								}
								else
								{
									std::cout << "this move is impossible" << std::endl;
									pieces1[num_chosen_piece1].x = dst_chosen_piece1[0];
									pieces1[num_chosen_piece1].y = dst_chosen_piece1[1];;
								}
							}
							else if (isChosen1)
							{
								if ((dst_become_piece1[0] == dst_chosen_piece1[0]) && (dst_become_piece1[1] == dst_chosen_piece1[1]))
								{
									std::cout << "you put the " << table1[become_field1[0]][become_field1[1]] << " back" << std::endl;
								}
								else
								{
									std::cout << "this move is impossible" << std::endl;
								}
								pieces1[num_chosen_piece1].x = dst_chosen_piece1[0];
								pieces1[num_chosen_piece1].y = dst_chosen_piece1[1];
							}
							isChosen1 = false;
						}	
						else if (mode == 2)
						{
							if (turn_move2)
							{
								#pragma region Players_Move
								isCastling2 = false;
								isEnPassant2 = false;
								become_field2[0] = ev.button.y / size_field;
								become_field2[1] = ev.button.x / size_field;
								dst_become_piece2[0] = become_field2[1] * size_field;
								dst_become_piece2[1] = become_field2[0] * size_field;

								if (isChosen2 && (ev.button.x >= 0) && (ev.button.y >= 0) && (ev.button.x < board_width) && (ev.button.y < board_height) &&
									!((become_field2[0] == chosen_field2[0]) && (become_field2[1] == chosen_field2[1])) &&
									(table2[chosen_field2[0]][chosen_field2[1]][0] != table2[become_field2[0]][become_field2[1]][0]))
								{
									if (table2[chosen_field2[0]][chosen_field2[1]][1] == 'K')
									{
										if (CheckCastling1(turn_move2, become_field2, table2, pieces2, chosen_field2, castling2))
										{
											available_move2 = true;
											isEnPassant2 = true;
										}
										else
										{
											available_move2 = AvailableMove(chosen_field2, become_field2, table2) &&
												!BitField(turn_move2, chosen_field2, become_field2, table2, pieces2);
										}
									}
									else if ((table2[chosen_field2[0]][chosen_field2[1]][1] == 'P') && (table2[become_field2[0]][become_field2[1]] == "--"))
									{
										if (CheckEnPassant1(turn_move2, table2, pieces2, chosen_field2, become_field2, dst_become_piece2, num_last_moved_piece2, moves_history2, moves_num2))
										{
											available_move2 = true;
											isEnPassant2 = true;
										}
										else
										{
											available_move2 = AvailableMove(chosen_field2, become_field2, table2) &&
												!Check1(turn_move2, chosen_field2, become_field2, table2, pieces2);
										}
									}
									else
									{
										available_move2 = AvailableMove(chosen_field2, become_field2, table2) &&
											!Check1(turn_move2, chosen_field2, become_field2, table2, pieces2);
									}
									if (available_move2)
									{
										if (isCastling2) std::cout << "you castled" << "\n\n";
										else if (isEnPassant2) std::cout << "you captured en passant" << "\n\n";
										else std::cout << "you moved the " << table2[chosen_field2[0]][chosen_field2[1]] << "\n\n";

										if ((table2[become_field2[0]][become_field2[1]] == "--") && !isEnPassant2) Sound0("music/move.wav", isChunk);
										else Sound0("music/take.wav", isChunk);

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

										pieces2[num_chosen_piece2].x = dst_become_piece2[0];
										pieces2[num_chosen_piece2].y = dst_become_piece2[1];
										table2[become_field2[0]][become_field2[1]] = table2[chosen_field2[0]][chosen_field2[1]];
										table2[chosen_field2[0]][chosen_field2[1]] = "--";
										turn_move2 = !turn_move2;
										available_move2 = false;
										isCheck2 = false;
										num_last_moved_piece2 = num_chosen_piece2;

										if (turn_move2 && (num_last_moved_piece2 >= 8) && (num_last_moved_piece2 <= 16))
										{
											if (become_field2[0] == 7)
											{
												textures2[num_last_moved_piece2] = textures2[3];
												textures1_2[num_last_moved_piece2] = textures1_2[3];
												textures2_2[num_last_moved_piece2] = textures2_2[3];
												textures3_2[num_last_moved_piece2] = textures3_2[3];
												table2[become_field2[0]][become_field2[1]] = "bQ";
											}
										}
										else if (!turn_move2 && (num_last_moved_piece2 >= 24) && (num_last_moved_piece2 <= 32))
										{
											if (become_field2[0] == 0)
											{
												textures2[num_last_moved_piece2] = textures2[19];
												textures1_2[num_last_moved_piece2] = textures1_2[19];
												textures2_2[num_last_moved_piece2] = textures2_2[19];
												textures3_2[num_last_moved_piece2] = textures3_2[19];
												table2[become_field2[0]][become_field2[1]] = "wQ";
											}
										}

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
											Sound1("music/victory_draw.wav", isChunk);
											game_over2 = true;
										}

										else if (Check2(turn_move2, table2, pieces2))
										{
											isCheck2 = true;
											if (Checkmate(turn_move2, table2, pieces2, num_last_moved_piece2, moves_history2, moves_num2))
											{
												std::string winner = "white";
												if (turn_move2) winner = "black";
												std::cout << "checkmate, " << winner << " won" << "\n\n";
												Sound1("music/victory_draw.wav", isChunk);
												game_over2 = true;
											}
											else std::cout << "check" << "\n\n";
										}

										else if (Stalemate(turn_move2, table2, pieces2, num_last_moved_piece2, moves_history2, moves_num2))
										{
											std::cout << "stalemate" << "\n\n";
											Sound1("music/victory_draw.wav", isChunk);
											game_over2 = true;
										}

										if (moves_num2 < 90)
										{
											moves_num2++;
											MoveRecording(moves_num2, moves_history2, table2, pieces2, game_over2, num_last_moved_piece2, turn_move2, isCheck2, castling2);
											SaveTextures(moves_num2, textures2, textures1_2, textures2_2, textures3_2, moves_history2);
										}								
								#pragma endregion	

										if (!game_over2)		
										{
											std::thread computerThread(ComputerMove, chosen_field2, become_field2, dst_chosen_piece2, dst_become_piece2, 
												table2, &num_chosen_piece2, pieces2, &isCastling2, &isEnPassant2, &turn_move2, textures2, textures1_2,
												textures2_2, textures3_2, &available_move2, &num_last_moved_piece2, &isCheck2, &isChunk, &game_over2,
												&moves_num2, moves_history2, &isChosen2, castling2);
											computerThread.detach();										
										}
									}
									else
									{
										std::cout << "this move is impossible" << std::endl;
										pieces2[num_chosen_piece2].x = dst_chosen_piece2[0];
										pieces2[num_chosen_piece2].y = dst_chosen_piece2[1];;
									}
								}
								else if (isChosen2)
								{
									if ((dst_become_piece2[0] == dst_chosen_piece2[0]) && (dst_become_piece2[1] == dst_chosen_piece2[1]))
									{
										std::cout << "you put the " << table2[become_field2[0]][become_field2[1]] << " back" << std::endl;
									}
									else
									{
										std::cout << "this move is impossible" << std::endl;
									}
									pieces2[num_chosen_piece2].x = dst_chosen_piece2[0];
									pieces2[num_chosen_piece2].y = dst_chosen_piece2[1];
							}
								isChosen2 = false;					
							}
						}
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

		SDL_RenderClear(ren);

		if (mode == 0)
		{ 
			SDL_RenderCopy(ren, tex_settings, NULL, &rect_settings);
			SDL_RenderCopy(ren, tex_mode1, NULL, &rect_mode1);
			SDL_RenderCopy(ren, tex_mode2, NULL, &rect_mode2);
			SDL_RenderCopy(ren, tex_quit, NULL, &rect_quit);
		}
		else if (mode == 1)
		{
			switch (active_board)
			{
			case 1:
				SDL_RenderCopy(ren, tex_board_brown, NULL, &rect_board);
				SDL_SetRenderDrawColor(ren, 240, 217, 181, 128);
				break;
			case 2:
				SDL_RenderCopy(ren, tex_board_green, NULL, &rect_board);
				SDL_SetRenderDrawColor(ren, 255, 255, 221, 128);
				break;
			case 3:
				SDL_RenderCopy(ren, tex_board_blue, NULL, &rect_board);
				SDL_SetRenderDrawColor(ren, 222, 227, 230, 128);
				break;
			}

			SDL_RenderFillRect(ren, &rect_menu_bar);
			SDL_SetRenderDrawColor(ren, 0, 0, 0, 128);

			SDL_RenderCopy(ren, tex_logout, NULL, &rect_logout);

			if ((moves_num1 == 0) || (moves_num1 >= 90)) SDL_RenderCopy(ren, tex_return_gray, NULL, &rect_return);
			else SDL_RenderCopy(ren, tex_return, NULL, &rect_return);
				
			if (moves_num1 == 0) SDL_RenderCopy(ren, tex_restart_gray, NULL, &rect_restart);
			else SDL_RenderCopy(ren, tex_restart, NULL, &rect_restart);

			if ((moves_num1 == 0) || game_over1) SDL_RenderCopy(ren, tex_flag_gray, NULL, &rect_flag);
			else SDL_RenderCopy(ren, tex_flag, NULL, &rect_flag);

			for (int i = 0; i < 32; i++) SDL_RenderCopy(ren, textures1[i], NULL, &pieces1[i]);
			if (isChosen1) RenAvailMove(turn_move1, ren, tex_dot, tex_frame, tex_star, pieces1, table1, chosen_field1, num_last_moved_piece1, castling1, moves_history1, moves_num1);
			if (isCheck1) RenCheck(turn_move1, table1, tex_check);
			for (int i = 0; i < 32; i++) if (i == num_chosen_piece1) SDL_RenderCopy(ren, textures1[i], NULL, &pieces1[i]);
		}
		else if (mode == 2)
		{
			switch (active_board)
			{
			case 1:
				SDL_RenderCopy(ren, tex_board_brown, NULL, &rect_board);
				SDL_SetRenderDrawColor(ren, 240, 217, 181, 128);
				break;
			case 2:
				SDL_RenderCopy(ren, tex_board_green, NULL, &rect_board);
				SDL_SetRenderDrawColor(ren, 255, 255, 221, 128);
				break;
			case 3:
				SDL_RenderCopy(ren, tex_board_blue, NULL, &rect_board);
				SDL_SetRenderDrawColor(ren, 222, 227, 230, 128);
				break;
			}

			SDL_RenderFillRect(ren, &rect_menu_bar);
			SDL_SetRenderDrawColor(ren, 0, 0, 0, 128);

			SDL_RenderCopy(ren, tex_logout, NULL, &rect_logout);

			if (moves_num2 == 0) SDL_RenderCopy(ren, tex_restart_gray, NULL, &rect_restart);
			else SDL_RenderCopy(ren, tex_restart, NULL, &rect_restart);
				
			if ((moves_num2) > 1 && (moves_num2 < 90)) SDL_RenderCopy(ren, tex_return, NULL, &rect_return);
			else SDL_RenderCopy(ren, tex_return_gray, NULL, &rect_return);
				
			if ((moves_num2 == 0) || game_over2) SDL_RenderCopy(ren, tex_flag_gray, NULL, &rect_flag);
			else SDL_RenderCopy(ren, tex_flag, NULL, &rect_flag);

			for (int i = 0; i < 32; i++) SDL_RenderCopy(ren, textures2[i], NULL, &pieces2[i]);
			if (isChosen2) RenAvailMove(turn_move2, ren, tex_dot, tex_frame, tex_star, pieces2, table2, chosen_field2, num_last_moved_piece2, castling2, moves_history2, moves_num2);
			if (isCheck2) RenCheck(turn_move2, table2, tex_check);
			for (int i = 0; i < 32; i++) if (i == num_chosen_piece2) SDL_RenderCopy(ren, textures2[i], NULL, &pieces2[i]);
		}
		else if (mode == 3)
		{
			SDL_SetRenderDrawColor(ren, 105, 63, 181, 128);
			SDL_RenderDrawRects(ren, rects_s, 6);

			switch (active_board)
			{
				case 1: 
					SDL_RenderDrawRects(ren, rects_board_brown, 7);
					break;
				case 2:
					SDL_RenderDrawRects(ren, rects_board_green, 7);
					break;
				case 3:
					SDL_RenderDrawRects(ren, rects_board_blue, 7);
					break;
			}

			switch (active_pieces)
			{
			case 1:
				SDL_RenderDrawRects(ren, rects_pieces1, 7);
				break;
			case 2:
				SDL_RenderDrawRects(ren, rects_pieces2, 7);
				break;
			case 3:
				SDL_RenderDrawRects(ren, rects_pieces3, 7);
				break;
			}

			if (isMusic) SDL_RenderCopy(ren, tex_tick, NULL, &rects_s[2]);
			else SDL_RenderCopy(ren, tex_cross, NULL, &rects_s[2]);
			if (isChunk) SDL_RenderCopy(ren, tex_tick, NULL, &rects_s[5]);
			else SDL_RenderCopy(ren, tex_cross, NULL, &rects_s[5]);

			SDL_RenderCopy(ren, tex_back, NULL, &rect_back);
			SDL_RenderCopy(ren, tex_music, NULL, &rect_music);
			SDL_RenderCopy(ren, tex_sound_effects, NULL, &rect_sound_effects);

			SDL_RenderCopy(ren, tex_board_design, NULL, &rect_board_design);
			SDL_RenderCopy(ren, tex_pieces_design, NULL, &rect_pieces_design);

			SDL_RenderCopy(ren, tex_board_brown, NULL, &rect_board_brown);
			SDL_RenderCopy(ren, tex_board_green, NULL, &rect_board_green);
			SDL_RenderCopy(ren, tex_board_blue, NULL, &rect_board_blue);

			SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

			SDL_RenderFillRect(ren, &rect_pieces1);
			SDL_RenderFillRect(ren, &rect_pieces2);
			SDL_RenderFillRect(ren, &rect_pieces3);

			SDL_RenderCopy(ren, tex_wn1, NULL, &rect_pieces1);
			SDL_RenderCopy(ren, tex_wn2, NULL, &rect_pieces2);
			SDL_RenderCopy(ren, tex_wn3, NULL, &rect_pieces3);

			SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		}

		SDL_RenderPresent(ren);
		SDL_Delay(1000/fps);
	}

	#pragma region Destroy&Free&Close
	SDL_DestroyTexture(tex_dot);
	SDL_DestroyTexture(tex_frame);
	SDL_DestroyTexture(tex_check);
	SDL_DestroyTexture(tex_star);
	SDL_DestroyTexture(tex_logout);
	SDL_DestroyTexture(tex_return);
	SDL_DestroyTexture(tex_return_gray);
	SDL_DestroyTexture(tex_flag);
	SDL_DestroyTexture(tex_flag_gray);
	SDL_DestroyTexture(tex_restart);
	SDL_DestroyTexture(tex_restart_gray);
	SDL_DestroyTexture(tex_settings);
	SDL_DestroyTexture(tex_mode1);
	SDL_DestroyTexture(tex_mode2);
	SDL_DestroyTexture(tex_quit);
	SDL_DestroyTexture(tex_back);
	SDL_DestroyTexture(tex_music);
	SDL_DestroyTexture(tex_board_design);
	SDL_DestroyTexture(tex_pieces_design);
	SDL_DestroyTexture(tex_sound_effects);
	SDL_DestroyTexture(tex_tick);
	SDL_DestroyTexture(tex_cross);
	SDL_DestroyTexture(tex_bb1);
	SDL_DestroyTexture(tex_bk1);
	SDL_DestroyTexture(tex_bn1);
	SDL_DestroyTexture(tex_bp1);
	SDL_DestroyTexture(tex_bq1);
	SDL_DestroyTexture(tex_br1);
	SDL_DestroyTexture(tex_wb1);
	SDL_DestroyTexture(tex_wk1);
	SDL_DestroyTexture(tex_wn1);
	SDL_DestroyTexture(tex_wp1);
	SDL_DestroyTexture(tex_wq1);
	SDL_DestroyTexture(tex_wr1);
	SDL_DestroyTexture(tex_bb2);
	SDL_DestroyTexture(tex_bk2);
	SDL_DestroyTexture(tex_bn2);
	SDL_DestroyTexture(tex_bp2);
	SDL_DestroyTexture(tex_bq2);
	SDL_DestroyTexture(tex_br2);
	SDL_DestroyTexture(tex_wb2);
	SDL_DestroyTexture(tex_wk2);
	SDL_DestroyTexture(tex_wn2);
	SDL_DestroyTexture(tex_wp2);
	SDL_DestroyTexture(tex_wq2);
	SDL_DestroyTexture(tex_wr2);
	SDL_DestroyTexture(tex_bb3);
	SDL_DestroyTexture(tex_bk3);
	SDL_DestroyTexture(tex_bn3);
	SDL_DestroyTexture(tex_bp3);
	SDL_DestroyTexture(tex_bq3);
	SDL_DestroyTexture(tex_br3);
	SDL_DestroyTexture(tex_wb3);
	SDL_DestroyTexture(tex_wk3);
	SDL_DestroyTexture(tex_wn3);
	SDL_DestroyTexture(tex_wp3);
	SDL_DestroyTexture(tex_wq3);
	SDL_DestroyTexture(tex_wr3);
	SDL_DestroyTexture(tex_board_brown);
	SDL_DestroyTexture(tex_board_green);
	SDL_DestroyTexture(tex_board_blue);
	Mix_FreeMusic(background);
	Mix_FreeChunk(sound);
	TTF_CloseFont(font);
	Mix_CloseAudio();
	#pragma endregion

	DeInit(0);
	return 0;
}