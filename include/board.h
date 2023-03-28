#include <vector>
#include <string>

#include "piece.h"
#include "util.h"

#pragma once

class Board {
 public:
  Board(std::string fen);
  ~Board();

  Piece* at(Position p);

  // pseudo-legal moves, not accounting for checks
  std::vector<Move> get_moves(Color color);

  void make_move(Move move);
  void unmake_move(Move move);

  Color get_side_to_move() { return side_to_move; }
  CastleAbility get_castling_ability(Color color) { return castling_ability[color]; }
  Position get_ep_square() { return ep_square; }
  int get_halfmoves() { return halfmoves; }
  int get_fullmoves() { return fullmoves; }

  std::string to_string();

 private:
  std::vector<Piece*> pieces;
  Piece* board[BOARD_SIZE][BOARD_SIZE];

  Color side_to_move;
  CastleAbility castling_ability[2];
  Position ep_square;
  int halfmoves;
  int fullmoves;
};
