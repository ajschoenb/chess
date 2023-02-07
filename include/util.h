#include <sstream>
#include <utility>

#pragma once

constexpr int BOARD_SIZE = 8;

enum Color {
  WHITE = 0, BLACK, SIZE
};

class Position {
 public:
  Position(int rank_idx, int file_idx) : rank_idx(rank_idx), file_idx(file_idx) {}

  int get_rank() { return rank_idx; }
  int get_file() { return file_idx; }

  bool is_in_bounds() {
    return rank_idx >= 0 && rank_idx < BOARD_SIZE
        && file_idx >= 0 && file_idx < BOARD_SIZE;
  }

  std::string to_string() {
    std::stringstream stream;

    stream << static_cast<char>('a' + file_idx);
    stream << rank_idx + 1;

    return stream.str();
  }

 private:
  int rank_idx;
  int file_idx;
};
