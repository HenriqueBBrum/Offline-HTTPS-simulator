#pragma once


#include<../include/utils.hpp>
#include<../include/diffie_hellman.hpp>
#include<../include/aes.hpp>


#include <memory>


#include<string>


using std::string;



class HTTPS{

    private:
    
        std::unique_ptr<Utils> utils;
        std::unique_ptr<DiffieHellman> df;
        std::unique_ptr<AES> aes;

    public:
    
        HTTPS();

        void run(string input_file);

};