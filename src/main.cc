#include <iostream>

#include "board.h"
#include "piece.h"

int main(int argc, char** argv) {
  if (argc > 1) {
    std::string fen(argv[1]);
    BoardState b(fen);
    std::cout << b.to_string() << std::endl;
  }
  return 0;
}
