#pragma once


#include <../include/utils.hpp>


#include <memory>


class DiffieHellman{


    private:
        std::unique_ptr<Utils> utils;

        /*  Creates a (private) and A(public) values from p and g
            Returns a map (could've been a tuple..) with the following format: {{a, <hex_value>}, {A, <hex_value}} */
        std::unordered_map<string, string> generate_public_key(string p_hex, string g_hex);

        /*  Finds diffie-hellman secret with another user public key (B), my private key (a) and p
            Retunrs hexadecimal value */
        string find_diffie_hellman_secret(string B_hex, string a_hex, string p_hex);

        /*  Uses OpenSSL SHA 256 to hash a byte array. Hashed byte array is written to 'hashed'
            Returns true for sucess */
        bool Openssl_SHA256hash(const byte* unhashed, const int unhashed_len, string& hashed);


    public:

        DiffieHellman();

        /*  Discovers diffie-hellman secret and calculates SHA256 from it. First 128 bits of the hash is the aes key 
            Return hexadecimal value of aes key */
        string find_aes128_key(string p_hex, string g_hex, string B_hex);




};