//
// Created by andrew on 17/06/23.
//

#ifndef TIC_TAC_TOE_TEST_QT_UTILS_HPP
#define TIC_TAC_TOE_TEST_QT_UTILS_HPP

#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QGridLayout>
#include <QLabel>

namespace QtUtils {
    inline int showMessageBox(const QString &message, const QString &title, QMessageBox::StandardButtons buttons,
                              std::pair<int, int> size) {
        auto messageBox = new QMessageBox();
        messageBox->setText(message);
        messageBox->setWindowTitle(title);
        messageBox->setStandardButtons(buttons);
        messageBox->setModal(true);
        messageBox->setFixedSize(size.first, size.second);
        return messageBox->exec();
    }

    inline void animateButtonOpacity(QPushButton *button, const QList<std::pair<qreal, QVariant>> &keyValues,
                                     int duration, int loopCount = 1) {
        auto eff = new QGraphicsOpacityEffect();
        button->setGraphicsEffect(eff);

        auto a = new QPropertyAnimation(eff, "opacity");
        a->setKeyValues(keyValues);
        a->setDuration(duration);
        a->setLoopCount(loopCount);
        a->start(QPropertyAnimation::DeleteWhenStopped);
    }

    inline void addLabeledWidget(QGridLayout *layout, const QString &label, QWidget *widget, int position) {
        auto labelWidget = new QLabel(label);
        layout->addWidget(labelWidget, position, 0);
        layout->addWidget(widget, position, 1);
    }
}


#endif //TIC_TAC_TOE_TEST_QT_UTILS_HPP
