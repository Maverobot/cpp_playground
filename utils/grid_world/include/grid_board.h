#pragma once

#include <where.h>

#include <fort.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct BoardPiece {
  std::string name;
  char code;
  int row_idx;
  int col_idx;
};

class GridBoard {
 public:
  GridBoard(size_t size = 4) : size_(size){};

  void addPiece(std::string name, char code, int row_idx, int col_idx) {
    pieces.push_back({std::move(name), code, row_idx, col_idx});
  }

  auto getPiecePos(std::string name) {
    if (auto iter = std::find_if(pieces.begin(), pieces.end(),
                                 [&name](const auto& piece) { return piece.name == name; });
        iter != pieces.cend()) {
      return std::pair<int, int>(iter->row_idx, iter->col_idx);
    } else {
      std::ostringstream oss;
      oss << "Piece with name " << name << " does not exist.";
      throw std::runtime_error(oss.str());
    }
  }

  void movePiece(std::string name, int row_idx_delta, int col_idx_delta) {
    if (auto iter = std::find_if(pieces.begin(), pieces.end(),
                                 [&name](const auto& piece) { return piece.name == name; });
        iter != pieces.cend()) {
      iter->row_idx += row_idx_delta;
      iter->col_idx += col_idx_delta;
    } else {
      std::ostringstream oss;
      oss << "Piece with name " << name << " does not exist.";
      throw std::runtime_error(oss.str());
    }
  }

  void display() const {
    fort::char_table table;
    for (size_t row_idx = 0; row_idx < size_; row_idx++) {
      for (size_t col_idx = 0; col_idx < size_; col_idx++) {
        const auto& piece = where(pieces, [row_idx, col_idx](const auto& piece) {
          return piece.row_idx == row_idx && piece.col_idx == col_idx;
        });

        if (piece.size() == 0) {
          table << " ";
        } else if (piece.size() == 1) {
          table << piece.at(0).get().code;
        } else {
          throw std::runtime_error("more than one pieces at a pos");
        }
      }
      table << fort::endr << fort::separator;
    }
    std::cout << table.to_string() << std::endl;
  };
  bool isOnBoard(std::pair<int, int> pos) { return isOnBoard(pos.first, pos.second); }

  bool isOnBoard(int row_idx, int col_idx) {
    return row_idx >= 0 && row_idx < size_ && col_idx >= 0 && col_idx < size_;
  }

 private:
  size_t size_;
  std::vector<BoardPiece> pieces;
};
