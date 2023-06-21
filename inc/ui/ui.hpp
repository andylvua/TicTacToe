//
// Created by andrew on 17/06/23.
//

#ifndef TIC_TAC_TOE_TEST_GUI_HPP
#define TIC_TAC_TOE_TEST_GUI_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QWidget>

#include "backend/player.hpp"
#include "backend/ai.hpp"
#include "backend/board.hpp"
#include "backend/backend.hpp"

class TicTacToeUI : public QMainWindow {
Q_OBJECT

public:
    explicit TicTacToeUI(QWidget *parent = nullptr);

private:
    static constexpr std::pair<int, int> m_resolution = {300, 200};

    struct ButtonsBoard {
        std::vector<std::vector<QSharedPointer<QPushButton>>> buttons;

        ButtonsBoard() = default;

        ButtonsBoard(TicTacToeUI &parent, QGridLayout *layout);

        std::vector<QSharedPointer<QPushButton>> &operator[](int i);

        void blockSignals(bool block);

        void refresh();
    };

    TicTacToe m_backend;

    ButtonsBoard m_buttons;

private:
    void initGUI();

    void initMenu();

    void startGame(const Player &self, const Player &opponent, AIType ai_type, GameMode mode);

    void animateMove(int row, int col);

    void animateWinningLine(const Board::line_t &line);

    void handleButtonClick(int row, int col);

    void restartGame();

public slots:
    void showPlayerTurn(const Player &player);

    void endGame(GameStatus status);

    void makeMove(int row, int col, const Player &player);
};

#endif //TIC_TAC_TOE_TEST_GUI_HPP
