//
// Created by andrew on 17/06/23.
//

#include <QApplication>
#include <QFile>
#include <QStyle>

#include "ui/ui.hpp"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QFile styleFile("../styles/style.qss");

    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        app.setStyleSheet(style);
        styleFile.close();
    }

    TicTacToeUI game;
    game.show();

    return QApplication::exec();
}
