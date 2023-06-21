//
// Created by andrew on 17/06/23.
//

#include "ui/ui.hpp"

TicTacToeUI::ButtonsBoard::ButtonsBoard(TicTacToeUI &parent, QGridLayout *layout) {
    buttons.resize(parent.m_backend.m_board.getSize());
    for (auto &row: buttons) {
        row.resize(parent.m_backend.m_board.getSize());
    }

    for (int i = 0; i < parent.m_backend.m_board.getCellCount(); ++i) {
        auto button = QSharedPointer<QPushButton>(new QPushButton(parent.centralWidget()));
        if (parent.m_backend.m_gameMode == GameMode::Gomoku) {
            button->setFixedSize(50, 50);
        } else {
            button->setFixedSize(100, 100);
        }
        auto boardSize = parent.m_backend.m_board.getSize();
        layout->addWidget(button.data(), i / boardSize + 1, i % boardSize);
        connect(button.data(), &QPushButton::clicked, &parent, [boardSize, i, &parent]() {
            parent.handleButtonClick(i / boardSize, i % boardSize);
        });

        buttons[i / boardSize][i % boardSize] = button;
    }
}

std::vector<QSharedPointer<QPushButton>> &TicTacToeUI::ButtonsBoard::operator[](int i) {
    return buttons[i];
}

void TicTacToeUI::ButtonsBoard::blockSignals(bool block) {
    for (auto &row: buttons) {
        for (auto &button: row) {
            button->blockSignals(block);
        }
    }
}

void TicTacToeUI::ButtonsBoard::refresh() {
    for (auto &row: buttons) {
        for (auto &button: row) {
            button->setIcon(QIcon());
            button->setChecked(false);
            button->blockSignals(false);
            button->setDisabled(false);
            button->setGraphicsEffect(nullptr);
            button->setStyleSheet("");
        }
    }
}
