//
// Created by andrew on 17/06/23.
//

#ifndef TIC_TAC_TOE_TEST_AI_HPP
#define TIC_TAC_TOE_TEST_AI_HPP

#include <vector>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <map>
#include <chrono>
#include <random>

#include "board.hpp"
#include "player.hpp"

enum class AIType {
    Random,
    Minimax,
    MTD
};

class AI {
public:
    virtual std::pair<int, int>
    makeMove(Board &board, Symbol symbol) = 0;

    virtual ~AI() = default;
};

class RandomAI : public AI {
public:
    std::pair<int, int>
    makeMove(Board &board, Symbol symbol) override;
};

class MinimaxAI : public AI {
public:
    static constexpr int MAX_DEPTH = 10;

    std::pair<int, int>
    makeMove(Board &board, Symbol symbol) override;

private:
    static int minimax(Board &board, int depth, int alpha, int beta, bool isMaximizing, Symbol symbol);
};

class MTDAI : public AI {
public:
    static constexpr int MAX_DEPTH = 4;

    std::pair<int, int>
    makeMove(Board &board, Symbol symbol) override;

private:
    using restrictions_t = std::array<int, 4>;
    using direction_t = std::array<int, 9>;
    using directions_t = std::array<direction_t, 4>;

    static constexpr int boardSize = 15;
    static constexpr int scoreThreshold = 800000;
    static constexpr int bestMoveThreshold = 1999900;
    static constexpr int winningScore = 2000000;

    struct Move {
        int i;
        int j;
        int score;

        static Move getRandomMove() {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(0, boardSize - 1);
            return {dist(mt), dist(mt), 0};
        }
    };

    struct Conditions {
        static constexpr int LiveOne = 10;
        static constexpr int DeadOne = 1;
        static constexpr int LiveTwo = 100;
        static constexpr int DeadTwo = 10;
        static constexpr int LiveThree = 1000;
        static constexpr int DeadThree = 100;
        static constexpr int LiveFour = 10000;
        static constexpr int DeadFour = 1000;
        static constexpr int Five = 100000;
        static constexpr int Win = std::numeric_limits<int>::min() + 1;
    };

    struct HashTable {
        std::array<std::array<std::array<int, 2>, boardSize>, boardSize> table{};

        HashTable() {
            init();
        }

        void init() {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
            for (auto &row: table) {
                for (auto &col: row) {
                    col[0] = dist(mt);
                    col[1] = dist(mt);
                }
            }
        }

        int hashBoard(Board &board) {
            int hash = 0;
            for (int i = 0; i < boardSize; ++i) {
                for (int j = 0; j < boardSize; ++j) {
                    int player = board.board[i][j];
                    if (player != 0) {
                        hash ^= table[i][j][player == 1 ? 0 : 1];
                    }
                }
            }
            return hash;
        }

        int updateHash(int hash, int i, int j, int player) {
            hash ^= table[i][j][player == 1 ? 0 : 1];
            return hash;
        }
    };

    struct CacheNode {
        int score;
        int depth;
        int Flag;
    };

    std::map<int, int> stateCache;
    std::map<int, CacheNode> cache;
    Move bestMove{};
    HashTable hashTable{};
    int depthReached = 0;

    static int evaluateBlock(int blocks, int pieces);

    static int evaluateBoard(Board &board, Symbol player, const restrictions_t &restrictions);

    static directions_t getDirections(Board &board, int x, int y);

    static bool remoteCell(Board &board, int row, int column);

    static restrictions_t getRestrictions(Board &board);

    static restrictions_t updateRestrictions(Board &board, const restrictions_t &restrictions, int i, int j);

    static int getSeq(int y, int e);

    static int evaluateSeq(int seq);

    static int evaluateDirection(const direction_t &direction_arr, Symbol player);

    static int evaluateMove(Board &board, int x, int y, Symbol player);

    static std::vector<Move> boardGenerator(const restrictions_t &restrictions, Board &board, Symbol player);

    int evaluateState(Board &board, Symbol player, int hash, const restrictions_t &restrictions);

    int negamax(Board &board, Symbol player, int depth, int alpha, int beta, int hash, const restrictions_t &restrictions);

    Move mtd(Board &board, int guess, int depth, const restrictions_t &restrictions);

    Move iterativeMtd(Board &board, int depth);
};

#endif //TIC_TAC_TOE_TEST_AI_HPP
