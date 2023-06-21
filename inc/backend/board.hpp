//
// Created by andrew on 17/06/23.
//

#ifndef TIC_TAC_TOE_TEST_BOARD_HPP
#define TIC_TAC_TOE_TEST_BOARD_HPP

#include <vector>
#include <functional>

enum class GameStatus {
    Waiting,
    InProgress,
    Draw,
    Win
};

enum class GameMode {
    TicTacToe,
    Gomoku
};


struct Board {
    using pair_t = std::pair<int, int>;
    using line_t = std::vector<pair_t>;

    std::vector<std::vector<int>> board;
    line_t winningLine;
    int winLength;

    explicit Board(GameMode gameMode = GameMode::TicTacToe);

    bool isFull();

    bool isEmpty();

    void clear();

    [[nodiscard]] int getSize() const;

    [[nodiscard]] int getCellCount() const;

    [[nodiscard]] line_t getWinningLine() const;

    line_t getEmptyCells();

    GameStatus getGameStatus(bool saveWinningLine = false);

    std::vector<int> &operator[](int index);

private:
    bool indexBoard(const std::function<bool(int)>& f);

};


#endif //TIC_TAC_TOE_TEST_BOARD_HPP
