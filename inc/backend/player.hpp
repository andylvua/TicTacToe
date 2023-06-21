//
// Created by andrew on 17/06/23.
//

#ifndef TIC_TAC_TOE_TEST_ENUMS_HPP
#define TIC_TAC_TOE_TEST_ENUMS_HPP

enum class PlayerType {
    None,
    Human,
    AI
};

enum Symbol : int {
    X = 1,
    O = -1,
};


struct Player {
    PlayerType type;
    Symbol symbol;

    bool operator==(const Player &other) const {
        return type == other.type && symbol == other.symbol;
    }

    [[nodiscard]] char getSymbolChar() const {
        if (symbol == Symbol::X) {
            return 'X';
        } else if (symbol == Symbol::O) {
            return 'O';
        } else {
            return ' ';
        }
    }
};

#endif //TIC_TAC_TOE_TEST_ENUMS_HPP
