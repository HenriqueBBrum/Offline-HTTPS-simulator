#include<../include/aes.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>

// Openssl EVP: The EVP cipher routines are a high-level interface to certain symmetric ciphers. 
#include <openssl/evp.h>

string AES::encrypt_aes_128(const byte* key, const byte* iv, const string plaintext, const size_t &cipher_len){
	EVP_CIPHER_CTX *ctx;
	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) 
		throw std::runtime_error("EVP_CIPHER_CTX_new init failed");

	int len;
    int ciphertext_len;

	// Initializes  cipher
    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1)
      throw std::runtime_error("EVP_EncryptInit_ex failed");

   	byte ciphertext[cipher_len];

	// Encrypts 'plaintext' with size 'plaintext.length()'. Encrypted text is written to 'ciphertext"
    if (EVP_EncryptUpdate(ctx, ciphertext, &len, reinterpret_cast<byte*>(const_cast<char*>(plaintext.c_str())), plaintext.length()) != 1)
      throw std::runtime_error("EVP_EncryptUpdate failed");
	  
    ciphertext_len = len;

	// Padding
    if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1)
      throw std::runtime_error("EVP_EncryptFinal_ex failed");

	ciphertext_len += len;

	EVP_CIPHER_CTX_free(ctx);  /* Clean up */

	std::stringstream ss;
	for(unsigned int i = 0; i < cipher_len; ++i)	
		ss << std::uppercase<< std::hex << std::setw(2) << std::setfill('0') << (int)ciphertext[i];

	return ss.str();
}


string AES::decrypt_aes_128(const byte* key, const byte* iv, const byte* ciphertext, const size_t &cipher_len){
	EVP_CIPHER_CTX *ctx;
	 /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) 
		throw std::runtime_error("EVP_CIPHER_CTX_new init failed");

	int len, plaintext_len;

    if(EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv) != 1)
		throw std::runtime_error("EVP_DecryptInit_ex failed");

	byte plaintext[cipher_len];
	// Decrypts a 'ciphertex' with len 'cipher_len'. Result is written to 'plaintex'.
    if(EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext , cipher_len) != 1)
        throw std::runtime_error("EVP_DecryptUpdate failed");

    plaintext_len = len;

	// Verifies for correct padding but does not remove it(?)
    if(EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1)
        throw std::runtime_error("EVP_DecryptFinal_ex failed");

    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);  /* Clean up */

	string ascii_plaintext;
	
	for(auto c: plaintext)
		if(!(c>=0 && c<=BLOCK_SZ))
			ascii_plaintext.push_back(c);
	
    return ascii_plaintext;
}



