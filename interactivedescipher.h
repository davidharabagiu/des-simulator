#ifndef DES_SIMULATOR_INTERACTIVEDESCIPHER_H
#define DES_SIMULATOR_INTERACTIVEDESCIPHER_H

#include <vector>
#include <cstdint>
#include <string>

class InteractiveDESCipher
{
public:
    void set_key(const std::vector<bool>& key);
    std::vector<bool> effective_key() const;
    std::vector<std::vector<bool>> round_keys() const;
    void set_text(const std::vector<bool>& text);
    std::vector<bool> permuted_text() const;
    std::vector<std::vector<bool>> encryption_rounds() const;
    std::vector<bool> cipher_text() const;
    std::vector<bool> permuted_cipher_text() const;
    std::vector<std::vector<bool>> decryption_rounds() const;
    std::vector<bool> decrypted_text() const;

private:
    void ComputeEffectiveKey();
    void ComputeRoundKeys();
    void Encrypt(const std::vector<std::vector<bool>>& round_keys, const std::vector<bool>& original,
                 std::vector<bool>& permuted, std::vector<std::vector<bool>>& rounds, std::vector<bool>& result);

private:
    std::vector<bool> key_;
    std::vector<bool> effective_key_;
    std::vector<std::vector<bool>> round_keys_;
    std::vector<bool> plain_text_;
    std::vector<bool> permuted_text_;
    std::vector<bool> cipher_text_;
    std::vector<bool> permuted_cipher_text_;
    std::vector<bool> decrypted_text_;
    std::vector<std::vector<bool>> encryption_rounds_;
    std::vector<std::vector<bool>> decryption_rounds_;
};

#endif //DES_SIMULATOR_INTERACTIVEDESCIPHER_H
