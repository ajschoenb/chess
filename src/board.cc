#include <iostream>
#include <sstream>
#include <string>

#include "board.h"

BoardState::BoardState(std::string fen) {
  /* <FEN> ::=  <Piece Placement>
   ' ' <Side to move>
   ' ' <Castling ability>
   ' ' <En passant target square>
   ' ' <Halfmove clock>
   ' ' <Fullmove counter> */

  std::stringstream fen_stream(fen);
  std::string pieces_str, side_str, castle_str;

  fen_stream >> pieces_str >> side_str >> castle_str >> en_passant_square >> halfmoves >> fullmoves;

  // rank 8->1, col a->h
  std::stringstream pieces_stream(pieces_str);
  for (int rank_idx = BOARD_SIZE - 1; rank_idx >= 0; rank_idx--) {
    for (int file_idx = 0; file_idx < BOARD_SIZE; file_idx++) {
      board[rank_idx][file_idx] = nullptr;
    }

    std::string rank;
    getline(pieces_stream, rank, '/');
    int file_idx = 0;
    for (char ch : rank) {
      if (isdigit(ch)) file_idx += ch - '0';
      else if (isalpha(ch)) {
        Piece* piece = Piece::make(ch, rank_idx, file_idx);
        pieces.push_back(piece);
        board[rank_idx][file_idx] = piece;
        file_idx++;
      }
    }
  }

  if (side_str == "w") {
    side_to_move = Color::WHITE;
  } else if (side_str == "b") {
    side_to_move = Color::BLACK;
  } else {
    std::cerr << "Invalid FEN detected - invalid side to move string: " << side_str << std::endl;
    exit(1);
  }

  castling_ability[Color::WHITE] = 0;
  castling_ability[Color::BLACK] = 0;
  if (castle_str.find("K") != std::string::npos) {
    castling_ability[Color::WHITE] += CASTLE_KINGSIDE;
  }
  if (castle_str.find("Q") != std::string::npos) {
    castling_ability[Color::WHITE] += CASTLE_QUEENSIDE;
  }
  if (castle_str.find("k") != std::string::npos) {
    castling_ability[Color::BLACK] += CASTLE_KINGSIDE;
  }
  if (castle_str.find("q") != std::string::npos) {
    castling_ability[Color::BLACK] += CASTLE_QUEENSIDE;
  }
}

BoardState::~BoardState() {
  pieces.clear();
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j] != nullptr) {
        delete board[i][j];
        board[i][j] = nullptr;
      }
    }
  }
}

std::string BoardState::to_string() {
  std::stringstream stream;
  stream << "Pieces:" << std::endl;
  for (Piece* p : pieces) {
    stream << p->to_string() << std::endl;
  }
  stream << std::endl;

  stream << "Side to move: " << (side_to_move == Color::WHITE ? "White" : "Black") << std::endl;
  stream << std::endl;

  stream << "Castling ability:" << std::endl;
  stream << "White: " << castling_ability[Color::WHITE] << std::endl;
  stream << "Black: " << castling_ability[Color::BLACK] << std::endl;
  stream << std::endl;

  stream << "Halfmove counter: " << halfmoves << std::endl;
  stream << "Fullmove counter: " << fullmoves;

  return stream.str();
}
