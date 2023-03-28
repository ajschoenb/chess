#include <sstream>
#include <utility>

#pragma once

constexpr int BOARD_SIZE = 8;

enum Color {
  WHITE = 0, BLACK
};

class Position {
 public:
  Position(int rank_idx, int file_idx) {
    pos0x88 = 16 * rank_idx + file_idx;
  }

  Position() : Position(-1, -1) {}

  int get_rank() { return pos0x88 >> 4; }
  int get_file() { return pos0x88 & 7; }

  bool is_oob() { return pos0x88 & 0x88; }

  bool operator==(Position p) {
    return get_rank() == p.get_rank() && get_file() == p.get_file();
  }

  std::string to_string() {
    if (is_oob()) return "-";

    std::stringstream stream;

    stream << static_cast<char>('a' + get_file());
    stream << get_rank() + 1;

    return stream.str();
  }

 private:
  char pos0x88;
};

class CastleAbility {
 public:
  static constexpr char NONE      = 0;
  static constexpr char KINGSIDE  = 1;
  static constexpr char QUEENSIDE = 2;
  static constexpr char BOTH      = KINGSIDE | QUEENSIDE;

  CastleAbility(char ability) : ability(ability) {}
  CastleAbility() : ability(NONE) {}

  void add_ability(char a) { ability |= a; }
  void remove_ability(char a) { ability &= ~a; }
  bool has_ability(char a) { return ability & a; }

  std::string to_string() {
    if (has_ability(BOTH)) return "both";
    if (has_ability(QUEENSIDE)) return "queenside";
    if (has_ability(KINGSIDE)) return "kingside";
    return "none";
  }

 private:
  char ability;
};
