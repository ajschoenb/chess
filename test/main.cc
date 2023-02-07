#include <iostream>
#include <cassert>

#include "board.h"

int main() {
  std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  BoardState b(fen);
  std::cout << b.to_string() << std::endl;
}
