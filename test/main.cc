#include <iostream>
#include <cassert>

#include "board.h"

void test_position() {
  // test basic bounds checks
  int k = 2;
  for (int i = -k; i < BOARD_SIZE + k; i++) {
    for (int j = -k; j < BOARD_SIZE + k; j++) {
      Position p(i, j);
      bool is_oob = false;
      if (i < 0 || i >= BOARD_SIZE || j < 0 || j >= BOARD_SIZE) is_oob = true;
      assert(p.is_oob() == is_oob);
    }
  }
}

void test_board() {
  std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  Board b(fen);
  std::string s = "Pieces:\n\
Black rook on a8\n\
Black knight on b8\n\
Black bishop on c8\n\
Black queen on d8\n\
Black king on e8\n\
Black bishop on f8\n\
Black knight on g8\n\
Black rook on h8\n\
Black pawn on a7\n\
Black pawn on b7\n\
Black pawn on c7\n\
Black pawn on d7\n\
Black pawn on e7\n\
Black pawn on f7\n\
Black pawn on g7\n\
Black pawn on h7\n\
White pawn on a2\n\
White pawn on b2\n\
White pawn on c2\n\
White pawn on d2\n\
White pawn on e2\n\
White pawn on f2\n\
White pawn on g2\n\
White pawn on h2\n\
White rook on a1\n\
White knight on b1\n\
White bishop on c1\n\
White queen on d1\n\
White king on e1\n\
White bishop on f1\n\
White knight on g1\n\
White rook on h1\n\
\n\
Side to move: White\n\
\n\
Castling ability:\n\
White: both\n\
Black: both\n\
\n\
En passant square: -\n\
Halfmove counter: 0\n\
Fullmove counter: 1";
  assert(b.to_string() == s);
}

void test_moves() {
  // white pawn test
  std::string fen = "8/7P/8/3pPp2/2K2PP1/PK6/1PPP4/8 w - d6 0 1";
  Board* b = new Board(fen);
  std::vector<Move> moves = b->get_moves(Color::WHITE);
  assert(moves.size() == 23);
  delete b;

  // black pawn test
  fen = "8/4ppp1/6kp/1pp2k2/2PpP3/8/p7/8 w - c3 0 1";
  b = new Board(fen);
  moves = b->get_moves(Color::BLACK);
  assert(moves.size() == 23);
  delete b;

  // white knight test
  fen = "8/8/3pp3/1n4N1/3PP3/8/8/8 w - - 0 1";
  b = new Board(fen);
  moves = b->get_moves(Color::WHITE);
  assert(moves.size() == 7);

  // black knight test
  moves = b->get_moves(Color::BLACK);
  assert(moves.size() == 7);
  delete b;

  // white rook test
  fen = "8/1R4p1/1P4r1/8/8/8/8/8 w - - 0 1";
  b = new Board(fen);
  moves = b->get_moves(Color::WHITE);
  assert(moves.size() == 7);

  // black rook test
  moves = b->get_moves(Color::BLACK);
  assert(moves.size() == 11);
  delete b;

  // white bishop test
  fen = "8/8/4p3/1P3b2/8/3B4/8/8 w - - 0 1";
  b = new Board(fen);
  moves = b->get_moves(Color::WHITE);
  assert(moves.size() == 8);

  // black bishop test
  moves = b->get_moves(Color::BLACK);
  assert(moves.size() == 7);
  delete b;

  // white queen test
  fen = "8/8/4p3/1P3q2/3P4/3Q1p2/8/8 w - - 0 1";
  b = new Board(fen);
  moves = b->get_moves(Color::WHITE);
  assert(moves.size() == 16);

  // black queen test
  moves = b->get_moves(Color::BLACK);
  assert(moves.size() == 18);
  delete b;

  // white king test
  fen = "r3k2r/p3pp1p/8/8/8/8/P3PP1P/R3K2R w KQkq - 0 1";
  b = new Board(fen);
  moves = b->get_moves(Color::WHITE);
  assert(moves.size() == 18);

  // black king test
  moves = b->get_moves(Color::BLACK);
  assert(moves.size() == 18);
}

int main() {
  test_position();
  test_board();
  test_moves();
}
