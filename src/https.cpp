#include <../include/https.hpp>

#include <iostream>
#include<algorithm>



HTTPS::HTTPS(){
    utils = std::make_unique<Utils>();
	df = std::make_unique<DiffieHellman>();
	aes = std::make_unique<AES>();
}

void HTTPS::run(string input_file){
	// Read input strings: p, g, B and encrypted message
    auto input_map = utils->read_hex_strs_from_file(input_file);

	// 128 bits key from 256 sha hash of diffie hellman secret
	auto aes_key_128 = df->find_aes128_key(input_map["p"], input_map["g"], input_map["B"]);

	std::cout<<"\nMessage received: "<<input_map["ciphertext"]<<"\n";

	auto ct_len = input_map["ciphertext"].length();

	auto [key, key_len] = utils->turn_hex_str_to_byte_array(aes_key_128);
	auto [iv, iv_len] = utils->turn_hex_str_to_byte_array(input_map["ciphertext"].substr(0, KEY_SZ*2));
	auto [ciphertext, ciphertext_len] = utils->turn_hex_str_to_byte_array(input_map["ciphertext"].substr(KEY_SZ*2, ct_len));

	// Decrypts aes message
	auto plaintext = aes->decrypt_aes_128(key, iv, ciphertext, ciphertext_len);
	std::cout<<"\nPlaintex sem padding: "<<plaintext<<"\n";

	// Reverses message
	string rev_plaintext(plaintext);

	utils->reverse_string(const_cast<char*>(rev_plaintext.c_str()));

	// Encrypts reversed message
	auto final_message = aes->encrypt_aes_128(key, iv, rev_plaintext, ciphertext_len);
	std::cout<<"\nReversed message ciphertexted with iv: "<<input_map["ciphertext"].substr(0, KEY_SZ*2)+final_message<<"\n\n";


}