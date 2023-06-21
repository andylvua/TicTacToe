//
// Created by andrew on 17/06/23.
//

#include "backend/board.hpp"

Board::Board(GameMode gameMode) {
    int size;
    int win_length;
    if (gameMode == GameMode::TicTacToe) {
        size = 3;
        win_length = 3;
    } else {
        size = 15;
        win_length = 5;
    }
    board = std::vector<std::vector<int>>(size, std::vector<int>(size, 0));
    winLength = win_length;
}

bool Board::indexBoard(const std::function<bool(int)>& f) {
    bool result = true;
    for (auto & row : board) {
        for (auto & col : row) {
            if (f(col)) {
                result = false;
                break;
            }
        }
    }
    return result;
}

bool Board::isFull() {
    return indexBoard([](int cell) { return cell == 0; });
}

bool Board::isEmpty() {
    return indexBoard([](int cell) { return cell != 0; });
}

void Board::clear() {
    for (auto &row: board) {
        for (auto &col: row) {
            col = 0;
        }
    }

    winningLine.clear();
}

[[nodiscard]] int Board::getSize() const {
    return static_cast<int>(board.size());
}

[[nodiscard]] int Board::getCellCount() const {
    return static_cast<int>(board.size() * board.size());
}

[[nodiscard]] Board::line_t Board::getWinningLine() const {
    return winningLine;
}

GameStatus Board::getGameStatus(bool saveWinningLine) {
    auto boardSize = static_cast<int>(board.size());

    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col <= boardSize - winLength; ++col) {
            auto player = board[row][col];
            if (player == 0)
                continue;

            bool win = true;
            for (int i = 1; i < winLength; ++i) {
                if (board[row][col + i] != player) {
                    win = false;
                    break;
                }
            }

            if (win && saveWinningLine) {
                for (int i = 0; i < winLength; ++i) {
                    winningLine.emplace_back(row, col + i);
                }
                return GameStatus::Win;
            }
            if (win) {
                return GameStatus::Win;
            }
        }
    }

    for (int col = 0; col < boardSize; ++col) {
        for (int row = 0; row <= boardSize - winLength; ++row) {
            auto player = board[row][col];
            if (player == 0)
                continue;

            bool win = true;
            for (int i = 1; i < winLength; ++i) {
                if (board[row + i][col] != player) {
                    win = false;
                    break;
                }
            }

            if (win && saveWinningLine) {
                for (int i = 0; i < winLength; ++i) {
                    winningLine.emplace_back(row + i, col);
                }
                return GameStatus::Win;
            }
            if (win) {
                return GameStatus::Win;
            }
        }
    }

    for (int row = 0; row <= boardSize - winLength; ++row) {
        for (int col = 0; col <= boardSize - winLength; ++col) {
            auto player = board[row][col];
            if (player == 0)
                continue;

            bool win = true;
            for (int i = 1; i < winLength; ++i) {
                if (board[row + i][col + i] != player) {
                    win = false;
                    break;
                }
            }

            if (win && saveWinningLine) {
                for (int i = 0; i < winLength; ++i) {
                    winningLine.emplace_back(row + i, col + i);
                }
                return GameStatus::Win;
            }
            if (win) {
                return GameStatus::Win;
            }
        }
    }

    for (int row = winLength - 1; row < boardSize; ++row) {
        for (int col = 0; col <= boardSize - winLength; ++col) {
            auto player = board[row][col];
            if (player == 0)
                continue;

            bool win = true;
            for (int i = 1; i < winLength; ++i) {
                if (board[row - i][col + i] != player) {
                    win = false;
                    break;
                }
            }

            if (win && saveWinningLine) {
                for (int i = 0; i < winLength; ++i) {
                    winningLine.emplace_back(row - i, col + i);
                }
                return GameStatus::Win;
            }
            if (win) {
                return GameStatus::Win;
            }
        }
    }

    if (isFull()) {
        return GameStatus::Draw;
    }

    return GameStatus::InProgress;
}

Board::line_t Board::getEmptyCells() {
    std::vector<std::pair<int, int>> emptyCells;
    for (int row = 0; row < board.size(); ++row) {
        for (int col = 0; col < board.size(); ++col) {
            if (board[row][col] == 0) {
                emptyCells.emplace_back(row, col);
            }
        }
    }
    return emptyCells;
}

std::vector<int> &Board::operator[](int row) {
    return board[row];
}
