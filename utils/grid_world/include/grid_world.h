#pragma once

#include <grid_board.h>

class GridWorld {
 public:
  enum class Action { up, down, left, right };
  using Reward = int;

  GridWorld(size_t size = 4) : board_(size) {
    // Initial board state
    board_.addPiece("Player", 'P', 0, 0);
    board_.addPiece("Wall", 'W', 1, 2);
    board_.addPiece("Pit", '-', 2, 2);
    board_.addPiece("Goal", '+', 3, 3);
  };

  Reward step(Action action) {
    if (status_ != Status::ongoing) {
      throw std::runtime_error("cannot step a finished game.");
    }
    auto [row_idx_delta, col_idx_delta] = mapAction(action);
    if (isValid(row_idx_delta, col_idx_delta)) {
      board_.movePiece("Player", row_idx_delta, col_idx_delta);
    }
    return evaluate();
  }

  void display() const {
    switch (status_) {
      case Status::ongoing:
        board_.display();
        break;
      case Status::win:
        std::cout << "You won the game by reaching the goal\n";
        break;
      case Status::loss:
        std::cout << "You loss the game by falling in the pit\n";
        break;
    }
  }

 private:
  GridBoard board_;

  enum class Status { ongoing, loss, win };
  Status status_{Status::ongoing};

  bool isValid(int row_idx_delta, int col_idx_delta) {
    auto new_player_pos = std::pair<int, int>(board_.getPiecePos("Player").first + row_idx_delta,
                                              board_.getPiecePos("Player").second + col_idx_delta);
    return new_player_pos != board_.getPiecePos("Wall") && board_.isOnBoard(new_player_pos);
  }

  std::pair<int, int> mapAction(Action action) {
    int row_idx_delta = 0;
    int col_idx_delta = 0;
    switch (action) {
      case Action::up:
        row_idx_delta = -1;
        break;
      case Action::down:
        row_idx_delta = 1;
        break;
      case Action::left:
        col_idx_delta = -1;
        break;
      case Action::right:
        col_idx_delta = 1;
        break;
    }
    return {row_idx_delta, col_idx_delta};
  }

  Reward evaluate() {
    if (board_.getPiecePos("Player") == board_.getPiecePos("Pit")) {
      status_ = Status::loss;
      return -10;
    } else if (board_.getPiecePos("Player") == board_.getPiecePos("Goal")) {
      status_ = Status::win;
      return 10;
    } else {
      return -1;
    }
    return Reward();
  }
};
