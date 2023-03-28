#include <iostream>
#include <sstream>
#include <cassert>

#include "piece.h"

Piece::Piece(Color color, Type type, int rank_idx, int file_idx)
  : color(color), type(type), pos(rank_idx, file_idx)
{}

Piece::Piece(char ch, int rank_idx, int file_idx)
  : pos(rank_idx, file_idx)
{
  color = isupper(ch) ? Color::WHITE : Color::BLACK;
  switch (tolower(ch)) {
  case 'p':
    type = Type::PAWN;
    break;
  case 'b':
    type = Type::BISHOP;
    break;
  case 'n':
    type = Type::KNIGHT;
    break;
  case 'r':
    type = Type::ROOK;
    break;
  case 'q':
    type = Type::QUEEN;
    break;
  case 'k':
    type = Type::KING;
    break;
  default:
    std::cerr << "Invalid piece type detected: " << tolower(ch) << std::endl;
    exit(1);
  }
}

std::string Piece::to_string() {
  std::stringstream stream;
  switch (color) {
  case Color::WHITE:
    stream << "White ";
    break;
  case Color::BLACK:
    stream << "Black ";
    break;
  }

  switch (type) {
  case Type::PAWN:
    stream << "pawn ";
    break;
  case Type::BISHOP:
    stream << "bishop ";
    break;
  case Type::KNIGHT:
    stream << "knight ";
    break;
  case Type::ROOK:
    stream << "rook ";
    break;
  case Type::QUEEN:
    stream << "queen ";
    break;
  case Type::KING:
    stream << "king ";
    break;
  }

  stream << "on ";
  stream << pos.to_string();

  return stream.str();
}

/*std::vector<Position> Pawn::get_moves() {
  std::vector<Position> moves;
  if (color == Color::WHITE) {
    if (pos.get_rank() == 1) {
      // pawn on 2nd rank can move two forward
      moves.push_back(Position(3, pos.get_file()));
    } else if (pos.get_rank() == 4) {
      // pawn on 5th rank can maybe do en passant
      Position ep_left(5, pos.get_file() - 1);
      Position ep_right(5, pos.get_file() + 1);
      if (!ep_left.is_oob()) moves.push_back(ep_left);
      if (!ep_right.is_oob()) moves.push_back(ep_right);
    }
    moves.push_back(Position(pos.get_rank() + 1, pos.get_file()));
  }
  return moves;
  }*/

std::string Move::to_string() {
  std::stringstream stream;
  stream << from.to_string() << "-" << to.to_string();
  return stream.str();
}
