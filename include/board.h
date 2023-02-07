#include <vector>
#include <string>

#include "piece.h"
#include "util.h"

#pragma once

class BoardState {
 public:
  static constexpr int CASTLE_NONE = 0;
  static constexpr int CASTLE_KINGSIDE = 1;
  static constexpr int CASTLE_QUEENSIDE = 2;
  static constexpr int CASTLE_BOTH = 3;

  BoardState(std::string fen);
  ~BoardState();

  std::string to_string();

 private:
  std::vector<Piece*> pieces;
  Piece* board[BOARD_SIZE][BOARD_SIZE];

  Color side_to_move;
  int castling_ability[Color::SIZE];
  std::string en_passant_square;
  int halfmoves;
  int fullmoves;
};
