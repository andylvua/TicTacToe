//
// Created by andrew on 17/06/23.
//

#include <limits>
#include <iostream>

#include "backend/ai.hpp"

std::pair<int, int> MTDAI::makeMove(Board &board, Symbol symbol) {
    if (board.isEmpty()) {
        Move randomMove = Move::getRandomMove();
        return {randomMove.i, randomMove.j};
    }

    iterativeMtd(board, MAX_DEPTH);
    return {bestMove.i, bestMove.j};
}

[[nodiscard]] int MTDAI::evaluateBlock(int blocks, int pieces) {
    if (blocks == 0) {
        switch (pieces) {
            case 1:
                return Conditions::LiveOne;
            case 2:
                return Conditions::LiveTwo;
            case 3:
                return Conditions::LiveThree;
            case 4:
                return Conditions::LiveFour;
            default:
                return Conditions::Five;
        }
    } else if (blocks == 1) {
        switch (pieces) {
            case 1:
                return Conditions::DeadOne;
            case 2:
                return Conditions::DeadTwo;
            case 3:
                return Conditions::DeadThree;
            case 4:
                return Conditions::DeadFour;
            default:
                return Conditions::Five;
        }
    } else {
        if (pieces >= 5) {
            return Conditions::Five;
        }

        return 0;
    }
}

int MTDAI::evaluateBoard(Board &board, Symbol player, const restrictions_t &restrictions) {
    int score = 0;
    int min_r = restrictions[0];
    int min_c = restrictions[1];
    int max_r = restrictions[2];
    int max_c = restrictions[3];

    for (int row = min_r; row < max_r + 1; row++) {
        for (int column = min_c; column < max_c + 1; column++) {
            if (board[row][column] == player) {
                int block = 0;
                int piece = 1;
                if (column == 0 || board[row][column - 1] != 0) {
                    block++;
                }
                for (column++; column < boardSize && board[row][column] == player; column++) {
                    piece++;
                }
                if (column == boardSize || board[row][column] != 0) {
                    block++;
                }
                score = score + evaluateBlock(block, piece);
            }
        }
    }

    for (int column = min_c; column < max_c + 1; column++) {
        for (int row = min_r; row < max_r + 1; row++) {
            if (board[row][column] == player) {
                int block = 0;
                int piece = 1;
                if (row == 0 || board[row - 1][column] != 0) {
                    block++;
                }
                for (row++; row < boardSize && board[row][column] == player; row++) {
                    piece++;
                }
                if (row == boardSize || board[row][column] != 0) {
                    block++;
                }
                score += evaluateBlock(block, piece);
            }
        }
    }

    for (int n = min_r; n < (max_c - min_c + max_r); n += 1) {
        int r = n;
        int c = min_c;
        while (r >= min_r && c <= max_c) {
            if (r <= max_r) {
                if (board[r][c] == player) {
                    int block = 0;
                    int piece = 1;
                    if (c == 0 || r == boardSize - 1 || board[r + 1][c - 1] != 0) {
                        block++;
                    }
                    r--;
                    c++;
                    for (; r >= 0 && board[r][c] == player; r--) {
                        piece++;
                        c++;
                    }
                    if (r < 0 || c == boardSize || board[r][c] != 0) {
                        block++;
                    }
                    score += evaluateBlock(block, piece);
                }
            }
            r -= 1;
            c += 1;
        }
    }

    for (int n = min_r - (max_c - min_c); n <= max_r; n++) {
        int r = n;
        int c = min_c;
        while (r <= max_r && c <= max_c) {
            if (r >= min_r && r <= max_r) {
                if (board[r][c] == player) {
                    int block = 0;
                    int piece = 1;
                    if (c == 0 || r == 0 || board[r - 1][c - 1] != 0) {
                        block++;
                    }
                    r++;
                    c++;
                    for (; r < boardSize && board[r][c] == player; r++) {
                        piece++;
                        c++;
                    }
                    if (r == boardSize || c == boardSize || board[r][c] != 0) {
                        block++;
                    }
                    score += evaluateBlock(block, piece);
                }
            }
            r += 1;
            c += 1;
        }

    }
    return score;
}

MTDAI::directions_t MTDAI::getDirections(Board &board, int x, int y) {
    direction_t a{};
    direction_t b{};
    direction_t c{};
    direction_t d{};
    int a_i = 0;
    int b_i = 0;
    int c_i = 0;
    int d_i = 0;

    for (int i = -4; i < 5; i++) {
        if (x + i >= 0 && x + i <= boardSize - 1) {
            a[a_i] = board[x + i][y];
            a_i++;
            if (y + i >= 0 && y + i <= boardSize - 1) {
                b[b_i] = board[x + i][y + i];
                b_i++;
            }
        }
        if (y + i >= 0 && y + i <= boardSize - 1) {
            c[c_i] = board[x][y + i];
            c_i++;
            if (x - i >= 0 && x - i <= boardSize - 1) {
                d[d_i] = board[x - i][y + i];
                d_i++;
            }
        }

    }
    if (a_i != 9) {
        a[a_i] = 2;
    }
    if (b_i != 9) {
        b[b_i] = 2;
    }
    if (c_i != 9) {
        c[c_i] = 2;
    }
    if (d_i != 9) {
        d[d_i] = 2;
    }

    return {a, b, c, d};
}


bool MTDAI::remoteCell(Board &board, int row, int column) {
    for (int i = row - 2; i <= row + 2; i++) {
        if (i < 0 || i >= boardSize) continue;
        for (int j = column - 2; j <= column + 2; j++) {
            if (j < 0 || j >= boardSize) continue;
            if (board[i][j] != 0) return false;
        }
    }
    return true;
}

MTDAI::restrictions_t MTDAI::getRestrictions(Board &board) {
    int min_r = std::numeric_limits<int>::max() - 1;
    int min_c = std::numeric_limits<int>::max() - 1;
    int max_r = std::numeric_limits<int>::min() + 1;
    int max_c = std::numeric_limits<int>::min() + 1;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] != 0) {
                min_r = std::min(min_r, i);
                min_c = std::min(min_c, j);
                max_r = std::max(max_r, i);
                max_c = std::max(max_c, j);
            }
        }
    }
    if (min_r - 2 < 0) {
        min_r = 2;
    }
    if (min_c - 2 < 0) {
        min_c = 2;
    }
    if (max_r + 2 >= boardSize) {
        max_r = boardSize - 3;
    }
    if (max_c + 2 >= boardSize) {
        max_c = boardSize - 3;
    }

    return {min_r, min_c, max_r, max_c};
}

MTDAI::restrictions_t MTDAI::updateRestrictions(Board &board, const restrictions_t &restrictions, int i, int j) {
    int min_r = restrictions[0];
    int min_c = restrictions[1];
    int max_r = restrictions[2];
    int max_c = restrictions[3];
    if (i < min_r) {
        min_r = i;
    } else if (i > max_r) {
        max_r = i;
    }
    if (j < min_c) {
        min_c = j;
    } else if (j > max_c) {
        max_c = j;
    }
    if (min_r - 2 < 0) {
        min_r = 2;
    }
    if (min_c - 2 < 0) {
        min_c = 2;
    }
    if (max_r + 2 >= boardSize) {
        max_r = boardSize - 3;
    }
    if (max_c + 2 >= boardSize) {
        max_c = boardSize - 3;
    }

    return {min_r, min_c, max_r, max_c};
}

int MTDAI::getSeq(int y, int e) {
    if (y + e == 0) {
        return 0;
    }
    if (y != 0 && e == 0) {
        return y;
    }
    if (y == 0 && e != 0) {
        return -e;
    }

    return 5;
}

int MTDAI::evaluateSeq(int seq) {
    switch (seq) {
        case 0:
            return 7;
        case 1:
            return 35;
        case 2:
            return 800;
        case 3:
            return 15000;
        case 4:
            return 800000;
        case -1:
            return 15;
        case -2:
            return 400;
        case -3:
            return 1800;
        case -4:
            return 100000;
        default:
            return 0;
    }
}

int MTDAI::evaluateState(Board &board, Symbol player, int hash, restrictions_t const &restrictions) {
    int x_score = evaluateBoard(board, Symbol::X, restrictions);
    int o_score = evaluateBoard(board, Symbol::O, restrictions);
    int score = 0;
    if (player == Symbol::O) {
        score = (o_score - x_score);
    } else {
        score = (x_score - o_score);
    }
    stateCache[hash] = score;
    return score;
}


int MTDAI::evaluateDirection(const direction_t &direction_arr, Symbol player) {
    int score = 0;
    size_t arr_size = direction_arr.size();
    for (int i = 0; (i + 4) < arr_size; i++) {
        int you = 0;
        int enemy = 0;
        if (direction_arr[i] == 2) {
            return score;
        }
        for (int j = 0; j <= 4; j++) {
            if (direction_arr[i + j] == 2) {
                return score;
            }
            if (direction_arr[i + j] == player) {
                you++;
            } else if (direction_arr[i + j] == -player) {
                enemy++;
            }
        }
        score += evaluateSeq(getSeq(you, enemy));
        if ((score >= scoreThreshold)) {
            return Conditions::Win;
        }
    }
    return score;
}


int MTDAI::evaluateMove(Board &board, int x, int y, Symbol player) {
    int score = 0;
    directions_t directions = getDirections(board, x, y);
    int temp_score;
    for (int i = 0; i < 4; i++) {
        temp_score = evaluateDirection(directions[i], player);
        if (temp_score == Conditions::Win) {
            return Conditions::Win;
        } else {
            score += temp_score;
        }
    }
    return score;
}

std::vector<MTDAI::Move> MTDAI::boardGenerator(const restrictions_t &restrictions, Board &board, Symbol player) {
    std::vector<Move> availSpots_score;
    int min_r = restrictions[0];
    int min_c = restrictions[1];
    int max_r = restrictions[2];
    int max_c = restrictions[3];
    for (int i = min_r - 2; i <= max_r + 2; i++) {
        for (int j = min_c - 2; j <= max_c + 2; j++) {
            if (board[i][j] == 0 && !remoteCell(board, i, j)) {
                Move move{
                        .i = i,
                        .j = j,
                        .score = evaluateMove(board, i, j, player)
                };
                if (move.score == Conditions::Win) {
                    return {move};
                }
                availSpots_score.push_back(move);
            }
        }
    }

    std::sort(availSpots_score.begin(), availSpots_score.end(),
              [](Move const &a, Move const &b) { return a.score > b.score; });

    return availSpots_score;
}

int MTDAI::negamax(Board &board, Symbol player, int depth, int alpha, int beta, int hash, // NOLINT
                   const restrictions_t &restrictions) {
    const int alphaOrig = alpha;
    if (cache.count(hash) && cache[hash].depth >= depth) {
        int score = cache[hash].score;
        int flag = cache[hash].Flag;
        if (flag == 0) {
            return score;
        }

        if (flag == 1) {
            alpha = std::max(alpha, score);
        }
        if (flag == -1) {
            beta = std::min(beta, score);
        }

        if (alpha >= beta) {
            return score;
        }
    }

    if (board.getGameStatus() == GameStatus::Win) {
        return -winningScore + (depthReached - depth);
    }
    if (depth == 0) {
        if (stateCache.count(hash)) {
            return stateCache[hash];
        }
        return evaluateState(board, player, hash, restrictions);
    }
    std::vector<Move> availSpots = boardGenerator(restrictions, board, player);

    size_t availSpots_size = availSpots.size();
    if (availSpots_size == 0) {
        return 0;
    }

    int i, j;
    int newHash;
    int bestValue = std::numeric_limits<int>::min() + 1;
    int value;
    for (int y = 0; y < availSpots_size; y++) {
        i = availSpots[y].i;
        j = availSpots[y].j;

        newHash = hashTable.updateHash(hash, i, j, player);
        board[i][j] = player;
        restrictions_t new_restrictions = updateRestrictions(board, restrictions, i, j);
        value = -negamax(board, static_cast<Symbol>(-player), depth - 1, -beta, -alpha, newHash, new_restrictions);
        board[i][j] = 0;
        if (value > bestValue) {
            bestValue = value;
            if (depth == depthReached) {
                bestMove = {i, j, value};
            }
        }
        alpha = std::max(alpha, value);
        if (alpha >= beta) {
            break;
        }
    }

    CacheNode node{
            .score = bestValue,
            .depth = depth,
            .Flag = 0
    };

    if (bestValue <= alphaOrig) {
        node.Flag = 1;
    } else if (bestValue >= beta) {
        node.Flag = -1;
    } else {
        node.Flag = 0;
    }
    cache[hash] = node;
    return bestValue;
}

MTDAI::Move MTDAI::mtd(Board &board, int guess, int depth, restrictions_t const &restrictions) {
    int upper_bound = std::numeric_limits<int>::max() - 1;
    int lower_bound = std::numeric_limits<int>::min() + 1;
    int beta;

    Move last_successful{};
    do {
        if (guess == lower_bound) {
            beta = guess + 1;
        } else {
            beta = guess;
        }

        int result = negamax(board, Symbol::X, depth, beta - 1, beta, hashTable.hashBoard(board), restrictions);

        guess = result;
        last_successful = bestMove;

        if (guess < beta) {
            upper_bound = guess;
        } else {
            lower_bound = guess;
        }
    } while (lower_bound < upper_bound);

    return last_successful;
}

MTDAI::Move MTDAI::iterativeMtd(Board &board, int depth) {
    int i = 2;
    restrictions_t x = {0, 0, boardSize - 1, boardSize - 1};
    int guess = evaluateState(board, Symbol::X, hashTable.hashBoard(board), x);
    while (i != depth + 2) {
        depthReached = i;
        guess = (mtd(board, guess, i, getRestrictions(board))).score;
        if (guess > bestMoveThreshold) {
            break;
        }
        i += 2;
    }
    return bestMove;
}
