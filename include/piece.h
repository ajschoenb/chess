#include <string>
#include <vector>

#include "util.h"

#pragma once

class Piece {
 public:
  enum Type {
    PAWN = 0, BISHOP, KNIGHT, ROOK, QUEEN, KING
  };

  Piece(char ch, int rank_idx, int file_idx);
  Piece(Color color, Type type, int rank_idx, int file_idx);

  Color get_color() { return color; }
  Type get_type() { return type; }
  Position get_position() { return pos; }

  std::string to_string();

 private:
  Color color;
  Type type;
  Position pos;
};

class Move {
public:
  static constexpr int FLAG_EP = 1;
  static constexpr int FLAG_CASTLE = 2;
  static constexpr int FLAG_PROMOTE_B = 3;
  static constexpr int FLAG_PROMOTE_N = 4;
  static constexpr int FLAG_PROMOTE_R = 5;
  static constexpr int FLAG_PROMOTE_Q = 6;

  Move(Position from, Position to, Piece* captured, int flags)
    : from(from), to(to), captured(captured)
  {}

  std::string to_string();

private:
  Position from;
  Position to;
  Piece* captured;
  int flags;
};
