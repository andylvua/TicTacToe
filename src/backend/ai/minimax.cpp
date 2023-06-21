//
// Created by andrew on 17/06/23.
//

#include <limits>
#include <iostream>

#include "backend/ai.hpp"

std::pair<int, int> MinimaxAI::makeMove(Board &board, Symbol symbol) {
    int bestScore = std::numeric_limits<int>::min();
    int bestRow = -1;
    int bestCol = -1;

    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();


    for (int row = 0; row < board.getSize(); ++row) {
        for (int col = 0; col < board.getSize(); ++col) {
            if (board[row][col] == 0) {
                board[row][col] = symbol;
                int score = minimax(board, 0, alpha, beta, false, symbol);
                board[row][col] = 0;

                if (score > bestScore) {
                    bestScore = score;
                    bestRow = row;
                    bestCol = col;
                }

                alpha = std::max(alpha, bestScore);
                if (beta <= alpha) {
                    break;
                }
            }
        }
    }

    return {bestRow, bestCol};
}

int MinimaxAI::minimax(Board &board, int depth, int alpha, int beta, bool isMaximizing, Symbol symbol) { // NOLINT
    GameStatus status = board.getGameStatus();
    if (status == GameStatus::Win || depth >= MAX_DEPTH) {
        return isMaximizing ? -1 : 1;
    } else if (status == GameStatus::Draw) {
        return 0;
    }

    int bestScore = isMaximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    for (int row = 0; row < board.getSize(); ++row) {
        for (int col = 0; col < board.getSize(); ++col) {
            if (board[row][col] == 0) {
                board[row][col] = isMaximizing ? symbol : -symbol;
                int score = minimax(board, depth + 1, alpha, beta, !isMaximizing, symbol);
                board[row][col] = 0;

                if (isMaximizing) {
                    bestScore = std::max(score, bestScore);
                    alpha = std::max(alpha, bestScore);
                } else {
                    bestScore = std::min(score, bestScore);
                    beta = std::min(beta, bestScore);
                }

                if (beta <= alpha) {
                    break;
                }
            }
        }
    }
    return bestScore;
}
