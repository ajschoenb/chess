#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

#include "board.h"

Board::Board(std::string fen) {
  /* <FEN> ::=  <Piece Placement>
   ' ' <Side to move>
   ' ' <Castling ability>
   ' ' <En passant target square>
   ' ' <Halfmove clock>
   ' ' <Fullmove counter> */

  std::stringstream fen_stream(fen);
  std::string pieces_str, side_str, castle_str, ep_square_str;

  fen_stream >> pieces_str >> side_str >> castle_str >> ep_square_str >> halfmoves >> fullmoves;

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
        Piece* piece = new Piece(ch, rank_idx, file_idx);
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

  if (castle_str.find("K") != std::string::npos) {
    castling_ability[Color::WHITE].add_ability(CastleAbility::KINGSIDE);
  }
  if (castle_str.find("Q") != std::string::npos) {
    castling_ability[Color::WHITE].add_ability(CastleAbility::QUEENSIDE);
  }
  if (castle_str.find("k") != std::string::npos) {
    castling_ability[Color::BLACK].add_ability(CastleAbility::KINGSIDE);
  }
  if (castle_str.find("q") != std::string::npos) {
    castling_ability[Color::BLACK].add_ability(CastleAbility::QUEENSIDE);
  }

  if (ep_square_str.length() == 2) {
    char file = ep_square_str[0];
    char rank = ep_square_str[1];
    int file_idx = file - 'a';
    int rank_idx = rank - '1';
    ep_square = Position(rank_idx, file_idx);
  } else if (ep_square_str == "-") {
    ep_square = Position();
  } else {
    std::cerr << "Invalid FEN detected - invalid en passant square string: " << ep_square_str << std::endl;
    exit(1);
  }
}

Board::~Board() {
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

Piece* Board::at(Position p) {
  return board[p.get_rank()][p.get_file()];
}

void get_pawn_moves(Board* board, Piece* piece, std::vector<Move>& moves) {
  assert(piece->get_type() == Piece::Type::PAWN);
  Color color = piece->get_color();
  Position pos = piece->get_position();
  Position ep_square = board->get_ep_square();

  if (color == Color::WHITE) {
    if (pos.get_rank() == 4) {
      // pawn on 5th rank can maybe do en passant
      Position ep_left(5, pos.get_file() - 1);
      Position ep_right(5, pos.get_file() + 1);
      Piece* captured = board->at(Position(4, ep_square.get_file()));
      if (!ep_left.is_oob() && ep_left == ep_square) {
        assert(captured != nullptr);
        moves.push_back(Move(pos, ep_left, captured, Move::FLAG_EP));
      }
      if (!ep_right.is_oob() && ep_right == ep_square) {
        assert(captured != nullptr);
        moves.push_back(Move(pos, ep_right, captured, Move::FLAG_EP));
      }
    }
    Position up(pos.get_rank() + 1, pos.get_file());
    if (board->at(up) == nullptr) {
      if (pos.get_rank() == 6) {
        moves.push_back(Move(pos, up, nullptr, Move::FLAG_PROMOTE_B));
        moves.push_back(Move(pos, up, nullptr, Move::FLAG_PROMOTE_N));
        moves.push_back(Move(pos, up, nullptr, Move::FLAG_PROMOTE_R));
        moves.push_back(Move(pos, up, nullptr, Move::FLAG_PROMOTE_Q));
      } else {
        moves.push_back(Move(pos, up, nullptr, 0));
        if (pos.get_rank() == 1) {
          // pawn on 2nd rank can move two forward if squares empty
          Position up2(3, pos.get_file());
          if (board->at(up2) == nullptr) {
            moves.push_back(Move(pos, up2, nullptr, 0));
          }
        }
      }
    }
    Position left(pos.get_rank() + 1, pos.get_file() - 1);
    Position right(pos.get_rank() + 1, pos.get_file() + 1);
    if (!left.is_oob()) {
      Piece* captured = board->at(left);
      if (captured != nullptr && captured->get_color() != color) {
        moves.push_back(Move(pos, left, captured, 0));
      }
    }
    if (!right.is_oob()) {
      Piece* captured = board->at(right);
      if (captured != nullptr && captured->get_color() != color) {
        moves.push_back(Move(pos, right, captured, 0));
      }
    }
  } else if (color == Color::BLACK) {
    if (pos.get_rank() == 3) {
      // pawn on 4th rank can maybe do en passant
      Position ep_left(2, pos.get_file() - 1);
      Position ep_right(2, pos.get_file() + 1);
      Piece* captured = board->at(Position(3, ep_square.get_file()));
      if (!ep_left.is_oob() && ep_left == ep_square) {
        assert(captured != nullptr);
        moves.push_back(Move(pos, ep_left, captured, Move::FLAG_EP));
      }
      if (!ep_right.is_oob() && ep_right == ep_square) {
        assert(captured != nullptr);
        moves.push_back(Move(pos, ep_right, captured, Move::FLAG_EP));
      }
    }
    Position up(pos.get_rank() - 1, pos.get_file());
    if (board->at(up) == nullptr) {
      if (pos.get_rank() == 1) {
        moves.push_back(Move(pos, up, nullptr, Move::FLAG_PROMOTE_B));
        moves.push_back(Move(pos, up, nullptr, Move::FLAG_PROMOTE_N));
        moves.push_back(Move(pos, up, nullptr, Move::FLAG_PROMOTE_R));
        moves.push_back(Move(pos, up, nullptr, Move::FLAG_PROMOTE_Q));
      } else {
        moves.push_back(Move(pos, up, nullptr, 0));
        if (pos.get_rank() == 6) {
          // pawn on 7th rank can move two forward if squares empty
          Position up2(4, pos.get_file());
          if (board->at(up2) == nullptr) {
            moves.push_back(Move(pos, up2, nullptr, 0));
          }
        }
      }
    }
    Position left(pos.get_rank() - 1, pos.get_file() - 1);
    Position right(pos.get_rank() - 1, pos.get_file() + 1);
    if (!left.is_oob()) {
      Piece* captured = board->at(left);
      if (captured != nullptr && captured->get_color() != color) {
        moves.push_back(Move(pos, left, captured, 0));
      }
    }
    if (!right.is_oob()) {
      Piece* captured = board->at(right);
      if (captured != nullptr && captured->get_color() != color) {
        moves.push_back(Move(pos, right, captured, 0));
      }
    }
  }
}

void get_bishop_moves(Board* board, Piece* piece, std::vector<Move>& moves) {
  assert(piece->get_type() == Piece::Type::BISHOP ||
         piece->get_type() == Piece::Type::QUEEN);
  Color color = piece->get_color();
  Position pos = piece->get_position();
  int rank = pos.get_rank();
  int file = pos.get_file();

  bool up_left = true, up_right = true, down_left = true, down_right = true;
  for (int i = 1; i < BOARD_SIZE; i++) {
    if (!up_left && !up_right && !down_left && !down_right) break;
    if (up_left) {
      Position p(rank + i, file - i);
      if (p.is_oob()) {
        up_left = false;
      } else {
        Piece* captured = board->at(p);
        if (captured != nullptr) {
          up_left = false;
          if (captured->get_color() != color) {
            moves.push_back(Move(pos, p, captured, 0));
          }
        } else {
          moves.push_back(Move(pos, p, nullptr, 0));
        }
      }
    }
    if (up_right) {
      Position p(rank + i, file + i);
      if (p.is_oob()) {
        up_right = false;
      } else {
        Piece* captured = board->at(p);
        if (captured != nullptr) {
          up_right = false;
          if (captured->get_color() != color) {
            moves.push_back(Move(pos, p, captured, 0));
          }
        } else {
          moves.push_back(Move(pos, p, nullptr, 0));
        }
      }
    }
    if (down_left) {
      Position p(rank - i, file - i);
      if (p.is_oob()) {
        down_left = false;
      } else {
        Piece* captured = board->at(p);
        if (captured != nullptr) {
          down_left = false;
          if (captured->get_color() != color) {
            moves.push_back(Move(pos, p, captured, 0));
          }
        } else {
          moves.push_back(Move(pos, p, nullptr, 0));
        }
      }
    }
    if (down_right) {
      Position p(rank - i, file + i);
      if (p.is_oob()) {
        down_right = false;
      } else {
        Piece* captured = board->at(p);
        if (captured != nullptr) {
          down_right = false;
          if (captured->get_color() != color) {
            moves.push_back(Move(pos, p, captured, 0));
          }
        } else {
          moves.push_back(Move(pos, p, nullptr, 0));
        }
      }
    }
  }
}

void get_knight_moves(Board* board, Piece* piece, std::vector<Move>& moves) {
  assert(piece->get_type() == Piece::Type::KNIGHT);
  Color color = piece->get_color();
  Position pos = piece->get_position();

  // format is (rank, file)
  int transforms[][2] = {
    { 2, -1 },
    { 2, 1 },
    { 1, -2 },
    { -1, -2 },
    { 1, 2 },
    { -1, 2 },
    { -2, -1 },
    { -2, 1 }
  };
  int N = 8;
  for (int i = 0; i < N; i++) {
    Position p(pos.get_rank() + transforms[i][0], pos.get_file() + transforms[i][1]);
    if (!p.is_oob()) {
      Piece* captured = board->at(p);
      if (captured != nullptr && captured->get_color() == color) continue;
      moves.push_back(Move(pos, p, captured, 0));
    }
  }
}

void get_rook_moves(Board* board, Piece* piece, std::vector<Move>& moves) {
  assert(piece->get_type() == Piece::Type::ROOK ||
         piece->get_type() == Piece::Type::QUEEN);
  Color color = piece->get_color();
  Position pos = piece->get_position();
  int rank = pos.get_rank();
  int file = pos.get_file();

  bool left = true, right = true, up = true, down = true;
  for (int i = 1; i < BOARD_SIZE; i++) {
    if (!left && !right && !up && !down) break;
    if (left) {
      Position p(rank, file - i);
      if (p.is_oob()) {
        left = false;
      } else {
        Piece* captured = board->at(p);
        if (captured != nullptr) {
          left = false;
          if (captured->get_color() != color) {
            moves.push_back(Move(pos, p, captured, 0));
          }
        } else {
          moves.push_back(Move(pos, p, nullptr, 0));
        }
      }
    }
    if (right) {
      Position p(rank, file + i);
      if (p.is_oob()) {
        right = false;
      } else {
        Piece* captured = board->at(p);
        if (captured != nullptr) {
          right = false;
          if (captured->get_color() != color) {
            moves.push_back(Move(pos, p, captured, 0));
          }
        } else {
          moves.push_back(Move(pos, p, nullptr, 0));
        }
      }
    }
    if (up) {
      Position p(rank + i, file);
      if (p.is_oob()) {
        up = false;
      } else {
        Piece* captured = board->at(p);
        if (captured != nullptr) {
          up = false;
          if (captured->get_color() != color) {
            moves.push_back(Move(pos, p, captured, 0));
          }
        } else {
          moves.push_back(Move(pos, p, nullptr, 0));
        }
      }
    }
    if (down) {
      Position p(rank - i, file);
      if (p.is_oob()) {
        down = false;
      } else {
        Piece* captured = board->at(p);
        if (captured != nullptr) {
          down = false;
          if (captured->get_color() != color) {
            moves.push_back(Move(pos, p, captured, 0));
          }
        } else {
          moves.push_back(Move(pos, p, nullptr, 0));
        }
      }
    }
  }
}

void get_queen_moves(Board* board, Piece* piece, std::vector<Move>& moves) {
  get_bishop_moves(board, piece, moves);
  get_rook_moves(board, piece, moves);
}

void get_king_moves(Board* board, Piece* piece, std::vector<Move>& moves) {
  assert(piece->get_type() == Piece::Type::KING);
  Color color = piece->get_color();
  Position pos = piece->get_position();

  // format is (rank, file)
  int transforms[][2] = {
    { -1, 0 },
    { 1, 0 },
    { 0, -1 },
    { 0, 1 },
    { -1, -1 },
    { 1, -1 },
    { -1, 1 },
    { 1, 1 }
  };
  int N = 8;
  for (int i = 0; i < N; i++) {
    Position p(pos.get_rank() + transforms[i][0], pos.get_file() + transforms[i][1]);
    if (!p.is_oob()) {
      Piece* captured = board->at(p);
      if (captured != nullptr && captured->get_color() == color) continue;
      moves.push_back(Move(pos, p, captured, 0));
    }
  }

  CastleAbility castling_ability = board->get_castling_ability(color);
  if (castling_ability.has_ability(CastleAbility::KINGSIDE)) {
    // kingside castle always goes to g-file
    Position p(pos.get_rank(), 6);
    moves.push_back(Move(pos, p, nullptr, Move::FLAG_CASTLE));
  }
  if (castling_ability.has_ability(CastleAbility::QUEENSIDE)) {
    // queenside castle aways goes to c-file
    Position p(pos.get_rank(), 2);
    moves.push_back(Move(pos, p, nullptr, Move::FLAG_CASTLE));
  }
}

std::vector<Move> Board::get_moves(Color color) {
  std::vector<Move> moves;
  for (Piece* piece : pieces) {
    if (piece->get_color() != color) continue;

    switch (piece->get_type()) {
    case Piece::Type::PAWN:
      get_pawn_moves(this, piece, moves);
      break;
    case Piece::Type::BISHOP:
      get_bishop_moves(this, piece, moves);
      break;
    case Piece::Type::KNIGHT:
      get_knight_moves(this, piece, moves);
      break;
    case Piece::Type::ROOK:
      get_rook_moves(this, piece, moves);
      break;
    case Piece::Type::QUEEN:
      get_queen_moves(this, piece, moves);
      break;
    case Piece::Type::KING:
      get_king_moves(this, piece, moves);
      break;
    default:
      break;
    }
  }
  return moves;
}

void Board::make_move(Move move) {
}

void Board::unmake_move(Move move) {
  // TODO how to undo capture?
}

std::string Board::to_string() {
  std::stringstream stream;
  stream << "Pieces:" << std::endl;
  for (Piece* p : pieces) {
    stream << p->to_string() << std::endl;
  }
  stream << std::endl;

  stream << "Side to move: " << (side_to_move == Color::WHITE ? "White" : "Black") << std::endl;
  stream << std::endl;

  stream << "Castling ability:" << std::endl;
  stream << "White: " << castling_ability[Color::WHITE].to_string() << std::endl;
  stream << "Black: " << castling_ability[Color::BLACK].to_string() << std::endl;
  stream << std::endl;

  stream << "En passant square: " << ep_square.to_string() << std::endl;

  stream << "Halfmove counter: " << halfmoves << std::endl;
  stream << "Fullmove counter: " << fullmoves;

  return stream.str();
}
