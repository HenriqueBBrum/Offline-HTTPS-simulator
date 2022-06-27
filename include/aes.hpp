#pragma once


#include <../include/utils.hpp>

class AES{
    private:


    public:

        /*  Encrypts <plaintext> using AES_128 CBC mode  with <iv> and <key>
            Returns the encrypted message */
        string encrypt_aes_128(const byte* key, const byte* iv, const string plaintext, const size_t &cipher_len);

         /*  Decrypts <plaintext> using AES_128 CBC mode  with <iv> and <key>
             Returns the decrypted message without padding */
        string decrypt_aes_128(const byte* key, const byte* iv, const byte* ciphertext, const size_t &cipher_len);

};