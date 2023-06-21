//
// Created by andrew on 17/06/23.
//

#ifndef TIC_TAC_TOE_TEST_BACKEND_HPP
#define TIC_TAC_TOE_TEST_BACKEND_HPP

#include <QSharedPointer>
#include <QMap>
#include <QObject>
#include <QMetaClassInfo>
#include <QList>

#include "board.hpp"
#include "player.hpp"
#include "ai.hpp"

class TicTacToe : public QObject {
Q_OBJECT

public:
    Board m_board;
    GameStatus m_status = GameStatus::Waiting;

    Player m_currentPlayer = {PlayerType::None, Symbol::X};
    Player m_self = {PlayerType::Human, Symbol::X};
    Player m_opponent = {PlayerType::AI, Symbol::O};

    AIType m_aiType = AIType::Random;
    GameMode m_gameMode = GameMode::TicTacToe;

private:
    QSharedPointer<AI> m_ai;

    QMap<AIType, QSharedPointer<AI>> m_aiMap = {
            {AIType::Random,  QSharedPointer<AI>(new RandomAI())},
            {AIType::Minimax, QSharedPointer<AI>(new MinimaxAI())},
            {AIType::MTD,     QSharedPointer<AI>(new MTDAI())}
    };

public:
    void initGame(const Player &self, const Player &opponent, AIType aiType, GameMode gameMode);

    void startGame();

    void selectFirstPlayer();

    void changeTurn();

    void makeMove(int row, int col);

    void makeAIMove();

    void restartGame();

signals:
#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
    void turnChanged(Player &player);

    void gameEnded(GameStatus status);

    void move(int row, int col, Player &player);
#pragma clang diagnostic pop
};

#endif //TIC_TAC_TOE_TEST_BACKEND_HPP
