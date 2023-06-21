//
// Created by andrew on 17/06/23.
//

#include <QRandomGenerator>
#include <iostream>

#include "backend/ai.hpp"

std::pair<int, int> RandomAI::makeMove(Board &board, Symbol symbol) {
    auto emptyCells = board.getEmptyCells();
    int randomIndex = QRandomGenerator::global()->bounded(static_cast<int>(emptyCells.size()));
    return emptyCells[randomIndex];
}
