/* HTTPS simulator class. Exchange of secret aes 128bits key with diffie-hellman algorithm */


#pragma once

#include<../include/utils.hpp>
#include<../include/diffie_hellman.hpp>
#include<../include/aes.hpp>

#include <memory>
#include <string>


using std::string;


class HTTPS{
    private:
        std::unique_ptr<Utils> utils;
        std::unique_ptr<DiffieHellman> df;
        std::unique_ptr<AES> aes;

    public:
        HTTPS();

        // Execute HTTPS with path for input_file. This file needs to contain p, g, B and the encrypted message 
        void run(string input_file);
};