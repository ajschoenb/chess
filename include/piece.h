#include <string>
#include <vector>

#include "util.h"

#pragma once

class Piece {
 public:
  enum Type {
    PAWN = 0, BISHOP, KNIGHT, ROOK, QUEEN, KING
  };

  static Piece* make(char ch, int rank_idx, int file_idx);
  static Piece* make(Color color, Type type, int rank_idx, int file_idx);

  virtual ~Piece() {}

  std::string to_string();

  // returns possible moves on a theoretically empty board
  // these moves are culled later before actually being evaluated
  virtual std::vector<Position> get_moves() = 0;

 protected:
  Piece(Color color, Type type, int rank_idx, int file_idx);

  Color color;
  Type type;
  Position pos;
};

class Pawn : public Piece {
 public:
  Pawn(Color color, int rank_idx, int file_idx);

  std::vector<Position> get_moves();
};

class Bishop : public Piece {
 public:
  Bishop(Color color, int rank_idx, int file_idx);

  std::vector<Position> get_moves();
};

class Knight : public Piece {
 public:
  Knight(Color color, int rank_idx, int file_idx);

  std::vector<Position> get_moves();
};

class Rook : public Piece {
 public:
  Rook(Color color, int rank_idx, int file_idx);

  std::vector<Position> get_moves();
};

class Queen : public Piece {
 public:
  Queen(Color color, int rank_idx, int file_idx);

  std::vector<Position> get_moves();
};

class King : public Piece {
 public:
  King(Color color, int rank_idx, int file_idx);

  std::vector<Position> get_moves();
};
