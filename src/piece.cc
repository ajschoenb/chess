#include <iostream>
#include <sstream>
#include <cassert>

#include "piece.h"

Piece* Piece::make(Color color, Type type, int rank_idx, int file_idx)
{
  switch(type) {
  case PAWN:
    return new Pawn(color, rank_idx, file_idx);
  case BISHOP:
    return new Bishop(color, rank_idx, file_idx);
  case KNIGHT:
    return new Knight(color, rank_idx, file_idx);
  case ROOK:
    return new Rook(color, rank_idx, file_idx);
  case QUEEN:
    return new Queen(color, rank_idx, file_idx);
  case KING:
    return new King(color, rank_idx, file_idx);
  default:
    assert(false);
    return nullptr;
  }
}

Piece* Piece::make(char ch, int rank_idx, int file_idx)
{
  Color color = isupper(ch) ? Color::WHITE : Color::BLACK;
  switch (tolower(ch)) {
  case 'p':
    return new Pawn(color, rank_idx, file_idx);
  case 'b':
    return new Bishop(color, rank_idx, file_idx);
  case 'n':
    return new Knight(color, rank_idx, file_idx);
  case 'r':
    return new Rook(color, rank_idx, file_idx);
  case 'q':
    return new Queen(color, rank_idx, file_idx);
  case 'k':
    return new King(color, rank_idx, file_idx);
  default:
    std::cerr << "Invalid piece type detected: " << tolower(ch) << std::endl;
    exit(1);
  }
}

Piece::Piece(Color color, Type type, int rank_idx, int file_idx)
  : color(color), type(type), pos(rank_idx, file_idx)
{}

std::string Piece::to_string() {
  std::stringstream stream;
  switch (color) {
  case Color::WHITE:
    stream << "White ";
    break;
  case Color::BLACK:
    stream << "Black ";
    break;
  default:
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
  stream << " -- ";

  std::vector<Position> moves = get_moves();
  for (Position move : moves) {
    stream << move.to_string() << " ";
  }

  return stream.str();
}

Pawn::Pawn(Color color, int rank_idx, int file_idx)
  : Piece(color, Type::PAWN, rank_idx, file_idx)
{}

std::vector<Position> Pawn::get_moves() {
  std::vector<Position> moves;
  if (color == Color::WHITE) {
    if (pos.get_rank() == 1) {
      // pawn on 2nd rank can move two forward
      moves.push_back(Position(3, pos.get_file()));
    } else if (pos.get_rank() == 4) {
      // pawn on 5th rank can maybe do en passant
      Position ep_left(5, pos.get_file() - 1);
      Position ep_right(5, pos.get_file() + 1);
      if (ep_left.is_in_bounds()) moves.push_back(ep_left);
      if (ep_right.is_in_bounds()) moves.push_back(ep_right);
    }
    moves.push_back(Position(pos.get_rank() + 1, pos.get_file()));
  }
  return moves;
}

Bishop::Bishop(Color color, int rank_idx, int file_idx)
  : Piece(color, Type::BISHOP, rank_idx, file_idx)
{}

std::vector<Position> Bishop::get_moves() {
  std::vector<Position> moves;
  return moves;
}

Knight::Knight(Color color, int rank_idx, int file_idx)
  : Piece(color, Type::KNIGHT, rank_idx, file_idx)
{}

std::vector<Position> Knight::get_moves() {
  std::vector<Position> moves;
  return moves;
}

Rook::Rook(Color color, int rank_idx, int file_idx)
  : Piece(color, Type::ROOK, rank_idx, file_idx)
{}

std::vector<Position> Rook::get_moves() {
  std::vector<Position> moves;
  return moves;
}

Queen::Queen(Color color, int rank_idx, int file_idx)
  : Piece(color, Type::QUEEN, rank_idx, file_idx)
{}

std::vector<Position> Queen::get_moves() {
  std::vector<Position> moves;
  return moves;
}

King::King(Color color, int rank_idx, int file_idx)
  : Piece(color, Type::KING, rank_idx, file_idx)
{}

std::vector<Position> King::get_moves() {
  std::vector<Position> moves;
  return moves;
}
