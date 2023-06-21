//
// Created by andrew on 17/06/23.
//

#include <QInputDialog>
#include <QLabel>
#include <QRandomGenerator>
#include <QComboBox>
#include <QTimer>
#include <QScreen>

#include "ui/ui.hpp"
#include "ui/qt_utils.hpp"

namespace {
    QMap<QString, PlayerType> playerTypes = {
            {"Human", PlayerType::Human},
            {"AI",    PlayerType::AI},
    };

    QMap<QString, AIType> aiTypes = {
            {"Random",  AIType::Random},
            {"Minimax", AIType::Minimax},
            {"MTD",     AIType::MTD},
    };

    QMap<QString, GameMode> gameModes = {
            {"Five in a Row", GameMode::Gomoku},
            {"Tic-tac-toe",   GameMode::TicTacToe},
    };

    QMap<QString, QStringList> compatibleAIs = {
            {"Five in a Row", {"Random", "MTD"}},
            {"Tic-tac-toe",   {"Random", "Minimax"}},
    };

    QMap<QString, Symbol> playerSymbols = {
            {"X", Symbol::X},
            {"O", Symbol::O},
    };
}

TicTacToeUI::TicTacToeUI(QWidget *parent) : QMainWindow(parent) {
    connect(&m_backend, &TicTacToe::turnChanged, this, &TicTacToeUI::showPlayerTurn);
    connect(&m_backend, &TicTacToe::gameEnded, this, &TicTacToeUI::endGame);
    connect(&m_backend, &TicTacToe::move, this, &TicTacToeUI::makeMove);

    move(screen()->geometry().center() - frameGeometry().center());
    initMenu();
}

void TicTacToeUI::initGUI() {
    auto centralWidget = new QWidget(this);
    auto gridLayout = new QGridLayout(centralWidget);

    setWindowTitle("Tic-Tac-Toe");
    setCentralWidget(centralWidget);

    auto restartButton = new QPushButton(centralWidget);
    restartButton->setFixedHeight(50);
    restartButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    restartButton->setIconSize(QSize(30, 30));

    auto turnLabel = new QPushButton(centralWidget);
    turnLabel->setObjectName("turnLabel");
    turnLabel->setFixedHeight(50);
    turnLabel->setDisabled(true);
    turnLabel->setIconSize(QSize(20, 20));

    auto backButton = new QPushButton(centralWidget);
    backButton->setFixedHeight(50);
    backButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    backButton->setIconSize(QSize(30, 30));

    auto restartButtonWidth = std::clamp(m_backend.m_board.getSize() / 3, 1, 2);
    gridLayout->addWidget(restartButton, 0, 0, 1, restartButtonWidth);
    gridLayout->addWidget(turnLabel, 0, m_backend.m_board.getSize() / 2, 1, 1);
    auto backButtonWidth = std::clamp(m_backend.m_board.getSize() / 3, 1, 2);
    gridLayout->addWidget(backButton, 0, m_backend.m_board.getSize() - backButtonWidth, 1, backButtonWidth);

    connect(restartButton, &QPushButton::clicked, this, &TicTacToeUI::restartGame);
    connect(backButton, &QPushButton::clicked, this, &TicTacToeUI::initMenu);

    m_buttons = ButtonsBoard(*this, gridLayout);

    setFixedSize(centralWidget->sizeHint());
}

void TicTacToeUI::initMenu() {
    auto layout = new QGridLayout();
    auto dialog = new QDialog(this);

    auto humanAICombo = new QComboBox(dialog);
    humanAICombo->addItems(playerTypes.keys());
    humanAICombo->setCurrentText(playerTypes.key(m_backend.m_opponent.type));

    auto aiTypeCombo = new QComboBox(dialog);
    aiTypeCombo->addItems(compatibleAIs.value(gameModes.key(m_backend.m_gameMode)));
    aiTypeCombo->setCurrentText(aiTypes.key(m_backend.m_aiType));
    aiTypeCombo->setEnabled(m_backend.m_opponent.type == PlayerType::AI);

    auto playerCombo = new QComboBox(dialog);
    playerCombo->addItems(playerSymbols.keys());
    playerCombo->setCurrentText(playerSymbols.key(m_backend.m_self.symbol));

    auto modeCombo = new QComboBox(dialog);
    modeCombo->addItems(gameModes.keys());
    modeCombo->setCurrentText(gameModes.key(m_backend.m_gameMode));

    auto startButton = new QPushButton("New game", dialog);
    startButton->setObjectName("startButton");

    connect(humanAICombo, &QComboBox::currentTextChanged, [aiTypeCombo](const QString &text) {
        aiTypeCombo->setEnabled(playerTypes.value(text) == PlayerType::AI);
    });

    auto updateAIOptions = [aiTypeCombo](const QString &gameMode) {
        aiTypeCombo->clear();
        aiTypeCombo->addItems(compatibleAIs.value(gameMode));
    };

    connect(modeCombo, &QComboBox::currentTextChanged, updateAIOptions);

    connect(startButton, &QPushButton::clicked,
            [this, humanAICombo, playerCombo, aiTypeCombo, dialog, modeCombo]() {
                auto opponentType = playerTypes.value(humanAICombo->currentText());
                auto selfSymbol = playerSymbols.value(playerCombo->currentText());

                startGame({PlayerType::Human, static_cast<Symbol>(selfSymbol)},
                          {opponentType, static_cast<Symbol>(-selfSymbol)},
                          aiTypes.value(aiTypeCombo->currentText()),
                          gameModes.value(modeCombo->currentText()));

                dialog->close();
            });

    QtUtils::addLabeledWidget(layout, "Opponent:", humanAICombo, 0);
    QtUtils::addLabeledWidget(layout, "Type of AI:", aiTypeCombo, 1);
    QtUtils::addLabeledWidget(layout, "Your symbol:", playerCombo, 2);
    QtUtils::addLabeledWidget(layout, "Game mode:", modeCombo, 3);
    QtUtils::addLabeledWidget(layout, "", startButton, 4);

    dialog->setLayout(layout);
    dialog->setWindowTitle("Tic-Tac-Toe Settings");
    dialog->setFixedSize(m_resolution.first, m_resolution.second);
    dialog->exec();

    if (m_backend.m_status != GameStatus::InProgress) {
        exit(0);
    }
}

void TicTacToeUI::startGame(const Player &self, const Player &opponent, AIType aiType, GameMode mode) {
    m_backend.initGame(self, opponent, aiType, mode);
    initGUI();

    m_backend.startGame();
}

void TicTacToeUI::animateMove(int row, int col) {
    QtUtils::animateButtonOpacity(m_buttons[row][col].data(), {{0,   1},
                                                               {0.4, 0.6},
                                                               {0.6, 1},
                                                               {0.8, 0.7},
                                                               {1,   1}}, 300);
}

void TicTacToeUI::animateWinningLine(const Board::line_t &line) {
    for (auto [row, col]: line) {
        QtUtils::animateButtonOpacity(m_buttons[row][col].data(), {{0,   1},
                                                                   {0.5, 0.55},
                                                                   {1,   1}}, 500, -1);
    }
}

void TicTacToeUI::makeMove(int row, int col, const Player &player) {
    auto button = m_buttons[row][col];
    auto iconSize = QSize(static_cast<int>(button->width() * 0.8), static_cast<int>(button->height() * 0.8));

    button->setIcon(QIcon(QString("../assets/%1_symbol.png").arg(player.getSymbolChar())));
    button->setIconSize(QSize(iconSize));
    button->setChecked(true);
    button->blockSignals(true);

    if (player.type == PlayerType::AI) {
        animateMove(row, col);
    }
}

void TicTacToeUI::handleButtonClick(int row, int col) {
    if (!m_buttons[row][col]->isChecked() && m_backend.m_currentPlayer.type == PlayerType::Human) {
        m_backend.makeMove(row, col);
    }
}

void TicTacToeUI::endGame(GameStatus status) {
    m_buttons.blockSignals(true);

    QString message;
    if (status == GameStatus::Win) {
        if (m_backend.m_opponent.type == PlayerType::AI) {
            message = m_backend.m_currentPlayer == m_backend.m_self ? "You win!" : "You lose!";
        } else {
            message = QString("%1 wins!").arg(m_backend.m_currentPlayer.getSymbolChar());
        }
        animateWinningLine(m_backend.m_board.getWinningLine());
    } else {
        message = "It's a draw!";
    }

    QTimer::singleShot(1000, this, [message]() {
        QtUtils::showMessageBox(message, "Game Over", QMessageBox::Ok, m_resolution);
    });
}

void TicTacToeUI::restartGame() {
    if (m_backend.m_status == GameStatus::InProgress) {
        auto response = QtUtils::showMessageBox("Progress will be lost. Are you sure?",
                                                "Restart Game",
                                                QMessageBox::Yes | QMessageBox::No,
                                                m_resolution);
        if (response == QMessageBox::No) {
            return;
        }
    }

    m_buttons.refresh();
    m_backend.restartGame();
}

void TicTacToeUI::showPlayerTurn(const Player &player) {
    auto turnLabels = findChildren<QPushButton *>("turnLabel");
    turnLabels.last()->setIcon(QIcon(QString("../assets/%1_symbol.png").arg(player.getSymbolChar())));
}
