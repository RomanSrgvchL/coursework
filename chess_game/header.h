#pragma once

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h> 
#include <SDL_mixer.h>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

struct move
{
	std::string table[8][8];
	SDL_Rect pieces[32];
	SDL_Texture* textures[32];
	SDL_Texture* textures1[32];
	SDL_Texture* textures2[32];
	SDL_Texture* textures3[32];
	bool game_over;
	int num_last_moved_piece;
	bool turn_move;
	bool isCheck;
	bool castling[6];
};

void DeInit(int error);
void Init();
void LoadMusic(bool isMusic);
void PrintTable(std::string table[8][8]);
void Sound0(std::string name_file, bool isChunk);
void Sound1(std::string name_file, bool isChunk);
SDL_Texture* LoadTextureFromFile(const char* filename);
bool AvailableMove(int* chosen_field, int* become_field, std::string table[8][8]);
bool BitField(bool turn_move, int* king_field, int* become_field, std::string table[8][8], SDL_Rect* pieces);
bool Check1(bool turn_move, int* piece_chosen_field, int* become_field, std::string table[8][8], SDL_Rect* pieces);
bool Check2(bool turn_move, std::string table[8][8], SDL_Rect* pieces);
bool CheckCastling1(bool turn_move, int* become_field, std::string table[8][8], SDL_Rect* pieces, int* chosen_field, bool* castling);
bool CheckCastling2(bool turn_move, int* become_field, std::string table[8][8], SDL_Rect* pieces, int* chosen_field, bool* castling);
bool CheckEnPassant1(bool turn_move, std::string table[8][8], SDL_Rect* pieces, int* chosen_field, int* become_field, int* dst_become_piece, int num, move* moves_history, int moves_num);
bool CheckEnPassant2(bool turn_move, std::string table[8][8], SDL_Rect* pieces, int* chosen_field, int* become_field, int* dst_become_piece, int num, move* moves_history, int moves_num);
bool Checkmate(bool turn_move, std::string table[8][8], SDL_Rect* pieces, int num, move* moves_history, int moves_num);
bool Stalemate(bool turn_move, std::string table[8][8], SDL_Rect* pieces, int num, move* moves_history, int moves_num);
bool Draw(std::string table[8][8]);
void RenAvailMove(bool turn_move, SDL_Renderer* ren, SDL_Texture* tex_dot, SDL_Texture* tex_frame, SDL_Texture* tex_star, SDL_Rect* pieces, 
	std::string table[8][8], int* chosen_field, int num, bool* castling, move* moves_history, int moves_num);
void RenCheck(bool turn_move, std::string table[8][8], SDL_Texture* tex_check);
SDL_Texture* get_text_texture(SDL_Renderer*& ren, char* text, TTF_Font* font);
void ReturnTextures(int moves_num, SDL_Texture* textures[32], SDL_Texture* textures1[32], SDL_Texture* textures2[32], SDL_Texture* textures3[32], move* moves_history);
void ReturnMove(int moves_num, bool* turn_move, std::string table[8][8], SDL_Rect* pieces, bool* game_over, int* num, bool* isCheck, move* moves_history, bool* castling);
void ChangeTextures(SDL_Texture* textures[32], SDL_Texture* textures1[32], SDL_Texture* textures2[32], SDL_Texture* textures3[32], int active_pieces);
void SaveSettings(bool isMusic, bool isChunk, int active_board, int active_pieces);
void SaveTextures(int moves_num, SDL_Texture* textures[32], SDL_Texture* textures1[32], SDL_Texture* textures2[32], SDL_Texture* textures3[32], move* moves_history);
void MoveRecording(int moves_num, move* moves_history, std::string table[8][8], SDL_Rect* pieces, bool game_over, int num_last_moved_piece, bool turn_move, bool isCheck, bool* castling);
void ComputerMove(int* chosen_field2, int* become_field2, int* dst_chosen_piece2, int* dst_become_piece2, std::string table2[8][8], int* num_chosen_piece2, SDL_Rect* pieces2,
	bool* isCastling2, bool* isEnPassant2, bool* turn_move2, SDL_Texture* textures2[32], SDL_Texture* textures1_2[32], SDL_Texture* textures2_2[32], SDL_Texture* textures3_2[32],
	bool* available_move2, int* num_last_moved_piece2, bool* isCheck2, bool* isChunk, bool* game_over2, int* moves_num2, move* moves_history2, bool* isChosen2, bool* castling2);