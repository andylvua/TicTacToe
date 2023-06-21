//
// Created by andrew on 17/06/23.
//

#include "backend/backend.hpp"

void TicTacToe::initGame(const Player &self, const Player &opponent, AIType aiType, GameMode gameMode) {
    m_opponent = opponent;
    m_self = self;

    m_board = Board(gameMode);
    m_gameMode = gameMode;

    m_aiType = aiType;
    m_ai = m_aiMap.value(aiType);
}

void TicTacToe::startGame() {
    m_status = GameStatus::InProgress;

    selectFirstPlayer();
}

void TicTacToe::selectFirstPlayer() {
    m_currentPlayer = m_self.symbol == Symbol::X ? m_self : m_opponent;
    emit turnChanged(m_currentPlayer);

    if (m_currentPlayer.type == PlayerType::AI) {
        makeAIMove();
    }
}

void TicTacToe::changeTurn() { // NOLINT
    m_currentPlayer = m_currentPlayer == m_self ? m_opponent : m_self;
    emit turnChanged(m_currentPlayer);

    if (m_currentPlayer.type == PlayerType::AI) {
        makeAIMove();
    }
}

void TicTacToe::makeMove(int row, int col) { // NOLINT
    m_board[row][col] = m_currentPlayer.symbol;
    emit move(row, col, m_currentPlayer);

    m_status = m_board.getGameStatus(true);

    if (m_status == GameStatus::InProgress) {
        changeTurn();
    } else {
        emit gameEnded(m_status);
    }
}

void TicTacToe::makeAIMove() { // NOLINT
    if (m_status != GameStatus::InProgress) {
        return;
    }

    auto cell = m_ai->makeMove(m_board, m_opponent.symbol);
    makeMove(cell.first, cell.second);
}

void TicTacToe::restartGame() {
    m_board.clear();
    m_status = GameStatus::InProgress;

    selectFirstPlayer();
}
