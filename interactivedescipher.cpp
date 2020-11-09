//
// Created by david on 05.11.2020.
//

#include "interactivedescipher.h"

#include <algorithm>

namespace
{
// Parity bit drop table

int kParityBitDropTable[56] = { 57, 49, 41, 33, 25, 17, 9,
                                1, 58, 50, 42, 34, 26, 18,
                                10, 2, 59, 51, 43, 35, 27,
                                19, 11, 3, 60, 52, 44, 36,
                                63, 55, 47, 39, 31, 23, 15,
                                7, 62, 54, 46, 38, 30, 22,
                                14, 6, 61, 53, 45, 37, 29,
                                21, 13, 5, 28, 20, 12, 4 };

int kShiftTable[16] = { 1, 1, 2, 2,
                        2, 2, 2, 2,
                        1, 2, 2, 2,
                        2, 2, 2, 1 };

int kKeyCompressionTable[48] = { 14, 17, 11, 24, 1, 5,
                                 3, 28, 15, 6, 21, 10,
                                 23, 19, 12, 4, 26, 8,
                                 16, 7, 27, 20, 13, 2,
                                 41, 52, 31, 37, 47, 55,
                                 30, 40, 51, 45, 33, 48,
                                 44, 49, 39, 56, 34, 53,
                                 46, 42, 50, 36, 29, 32 };

int kInitialPermutationTable[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
                                     60, 52, 44, 36, 28, 20, 12, 4,
                                     62, 54, 46, 38, 30, 22, 14, 6,
                                     64, 56, 48, 40, 32, 24, 16, 8,
                                     57, 49, 41, 33, 25, 17, 9, 1,
                                     59, 51, 43, 35, 27, 19, 11, 3,
                                     61, 53, 45, 37, 29, 21, 13, 5,
                                     63, 55, 47, 39, 31, 23, 15, 7 };

int kExpansionDBox[48] = { 32, 1, 2, 3, 4, 5, 4, 5,
                                6, 7, 8, 9, 8, 9, 10, 11,
                                12, 13, 12, 13, 14, 15, 16, 17,
                                16, 17, 18, 19, 20, 21, 20, 21,
                                22, 23, 24, 25, 24, 25, 26, 27,
                                28, 29, 28, 29, 30, 31, 32, 1 };

int kSBox[8][4][16] = { { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },
                        { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },
                        { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },
                        { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },
                        { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },
                        { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },
                        { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },
                        { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 } };

int kStraightPermutationTable[32] = { 16, 7, 20, 21,
                                      29, 12, 28, 17,
                                      1, 15, 23, 26,
                                      5, 18, 31, 10,
                                      2, 8, 24, 14,
                                      32, 27, 3, 9,
                                      19, 13, 30, 6,
                                      22, 11, 4, 25 };

int kFinalPermutationTable[64] = { 40, 8, 48, 16, 56, 24, 64, 32,
                                   39, 7, 47, 15, 55, 23, 63, 31,
                                   38, 6, 46, 14, 54, 22, 62, 30,
                                   37, 5, 45, 13, 53, 21, 61, 29,
                                   36, 4, 44, 12, 52, 20, 60, 28,
                                   35, 3, 43, 11, 51, 19, 59, 27,
                                   34, 2, 42, 10, 50, 18, 58, 26,
                                   33, 1, 41, 9, 49, 17, 57, 25 };

std::vector<bool> ShiftLeft(const std::vector<bool>& k, int shifts)
{
    std::vector<bool> r(k.size());
    std::copy_n(k.cbegin(), shifts, std::copy(k.cbegin() + shifts, k.cend(), r.begin()));
    return r;
}
} //namespace

void InteractiveDESCipher::ComputeEffectiveKey()
{
    effective_key_.clear();
    effective_key_.reserve(56);
    for (int x : kParityBitDropTable)
    {
        effective_key_.push_back(key_[x - 1]);
    }
}

void InteractiveDESCipher::ComputeRoundKeys()
{
    round_keys_.clear();
    std::vector<bool> left(effective_key_.cbegin(), effective_key_.cbegin() + effective_key_.size() / 2);
    std::vector<bool> right(effective_key_.cbegin() + effective_key_.size() / 2, effective_key_.cend());
    for (int i = 0; i < 16; i++) {
        // Shifting
        left = ShiftLeft(left, kShiftTable[i]);
        right = ShiftLeft(right, kShiftTable[i]);

        // Combining
        std::vector<bool> combine(effective_key_.size());
        std::copy(right.cbegin(), right.cend(), std::copy(left.cbegin(), left.cend(), combine.begin()));

        // Key Compression
        std::vector<bool> round_key;
        round_key.reserve(48);
        for (int x : kKeyCompressionTable)
        {
            round_key.push_back(combine[x - 1]);
        }

        round_keys_.push_back(round_key);
    }
}

void InteractiveDESCipher::set_key(const std::vector<bool> &key)
{
    key_ = key;
    ComputeEffectiveKey();
    ComputeRoundKeys();
    if (plain_text_.size() == 64)
    {
        Encrypt(round_keys_, plain_text_, permuted_text_, encryption_rounds_, cipher_text_);
        auto round_keys2 = round_keys_;
        std::reverse(round_keys2.begin(), round_keys2.end());
        Encrypt(round_keys2, cipher_text_, permuted_cipher_text_, decryption_rounds_, decrypted_text_);
    }
}

std::vector<bool> InteractiveDESCipher::effective_key() const
{
    return effective_key_;
}

std::vector<std::vector<bool>> InteractiveDESCipher::round_keys() const
{
    return round_keys_;
}

void InteractiveDESCipher::Encrypt(const std::vector<std::vector<bool>>& round_keys, const std::vector<bool>& original,
                                   std::vector<bool>& permuted, std::vector<std::vector<bool>>& rounds,
                                   std::vector<bool>& result)
{
    rounds.clear();

    // Initial Permutation
    permuted.clear();
    permuted.reserve(64);
    for (int x : kInitialPermutationTable)
    {
        permuted.push_back(original[x - 1]);
    }

    // Splitting
    std::vector<bool> left(permuted.cbegin(), permuted.cbegin() + permuted.size() / 2);
    std::vector<bool> right(permuted.cbegin() + permuted.size() / 2, permuted.cend());

    std::vector<bool> partial_cipher_text(64);
    for (int i = 0; i < 16; i++) {
        // Expansion D-box
        std::vector<bool> exp;
        exp.reserve(48);
        for (int y : kExpansionDBox)
        {
            exp.push_back(right[y - 1]);
        }

        // XOR RoundKey[i] and exp
        std::vector<bool> x;
        x.reserve(48);
        for (int j = 0; j != 48; ++j)
        {
            x.push_back(round_keys[i][j] ^ exp[j]);
        }

        // S-boxes
        std::vector<bool> op;
        op.reserve(32);
        for (int j = 0; j < 8; j++) {
            int row = 2 * int(x[j * 6]) + int(x[j * 6 + 5]);
            int col = 8 * int(x[j * 6 + 1]) + 4 * int(x[j * 6 + 2]) + 2 * int(x[j * 6 + 3]) + int(x[j * 6 + 4]);
            int val = kSBox[j][row][col];
            op.push_back(val / 8);
            val = val % 8;
            op.push_back(val / 4);
            val = val % 4;
            op.push_back(val / 2);
            val = val % 2;
            op.push_back(val);
        }

        // Straight D-box
        std::vector<bool> op2;
        op2.reserve(32);
        for (int y : kStraightPermutationTable)
        {
            op2.push_back(op[y - 1]);
        }

        // XOR left and op2
        std::vector<bool> x2;
        x2.reserve(32);
        for (int j = 0; j != 32; ++j)
        {
            x2.push_back(left[j] ^ op2[j]);
        }

        left = x2;

        // Swapper
        if (i != 15) {
            swap(left, right);
        }

        // Combine
        std::copy(right.cbegin(), right.cend(), std::copy(left.cbegin(), left.cend(), partial_cipher_text.begin()));
        rounds.push_back(partial_cipher_text);
    }

    // Final Permutation
    result.clear();
    result.reserve(64);
    for (int y : kFinalPermutationTable)
    {
        result.push_back(partial_cipher_text[y - 1]);
    }
}

void InteractiveDESCipher::set_text(const std::vector<bool>& text)
{
    plain_text_ = text;
    if (key_.size() == 64)
    {
        Encrypt(round_keys_, plain_text_, permuted_text_, encryption_rounds_, cipher_text_);
        auto round_keys2 = round_keys_;
        std::reverse(round_keys2.begin(), round_keys2.end());
        Encrypt(round_keys2, cipher_text_, permuted_cipher_text_, decryption_rounds_, decrypted_text_);
    }
}

std::vector<bool> InteractiveDESCipher::permuted_text() const
{
    return permuted_text_;
}

std::vector<std::vector<bool>> InteractiveDESCipher::encryption_rounds() const
{
    return encryption_rounds_;
}

std::vector<bool> InteractiveDESCipher::cipher_text() const
{
    return cipher_text_;
}

std::vector<bool> InteractiveDESCipher::permuted_cipher_text() const
{
    return permuted_cipher_text_;
}

std::vector<std::vector<bool>> InteractiveDESCipher::decryption_rounds() const
{
    return decryption_rounds_;
}

std::vector<bool> InteractiveDESCipher::decrypted_text() const
{
    return decrypted_text_;
}
