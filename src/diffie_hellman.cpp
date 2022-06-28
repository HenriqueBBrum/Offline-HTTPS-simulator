#include <../include/diffie_hellman.hpp>

#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <gmp.h>

#include <openssl/sha.h>
#include <openssl/evp.h>


#include <boost/filesystem.hpp>

const string diffie_hellman_1_output_path = "input/keys.txt";


DiffieHellman::DiffieHellman(){
    utils = std::make_unique<Utils>();
}


std::unordered_map<string, string> DiffieHellman::generate_public_key(string p_hex, string g_hex){
	mpz_t p,g,a, A;
	mpz_init_set_str(p, p_hex.c_str(), 16);
	mpz_init_set_str(g, g_hex.c_str(), 16);

	gmp_randstate_t state;
	gmp_randinit_mt(state);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	gmp_randseed_ui(state, seed);

	mpz_init(a);
	mpz_urandomm(a,state, p); // 0 - p-1

	mpz_init(A);
	// g^a mod p
	mpz_powm(A, g, a, p);

	// Write to file private and public keys
	std::fstream output_file;
	output_file.open(diffie_hellman_1_output_path,std::ios::out); 
	auto a_hex = mpz_get_str(nullptr, -16, a);
	auto A_hex = mpz_get_str(nullptr, -16, A);
	output_file<<Utils::file_delimiter+"a\n"<<a_hex<<"\n"<<Utils::file_delimiter+"A\n"<<A_hex<<"\n";
	
    return {{"a", a_hex}, {"A",A_hex}};
}


string DiffieHellman::find_diffie_hellman_secret(string B_hex, string a_hex, string p_hex){
	mpz_t V, B, a, p;
		
	mpz_init_set_str(B, B_hex.c_str(), 16);
	mpz_init_set_str(a, a_hex.c_str(), 16);
	mpz_init_set_str(p, p_hex.c_str(), 16);

	mpz_init(V);
	// V = B^a mod p
	mpz_powm(V, B, a, p);

	return mpz_get_str(nullptr, -16, V);
}


bool DiffieHellman::Openssl_SHA256hash(const byte* unhashed, const int unhashed_len, string& hashed){
    bool success = false;
    EVP_MD_CTX* context = EVP_MD_CTX_new();

	if(context != nullptr && EVP_DigestInit_ex(context, EVP_sha256(), nullptr)){
		if(EVP_DigestUpdate(context, unhashed, unhashed_len)){
			byte hash[EVP_MAX_MD_SIZE];
			unsigned int lengthOfHash = 0;

			if(EVP_DigestFinal_ex(context, hash, &lengthOfHash)) {
				std::stringstream ss;
				// COnvert to hexadecimal string
				for(unsigned int i = 0; i < lengthOfHash; ++i){
					ss << std::uppercase<<std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
				}

				hashed = ss.str();
				success = true;
			}
		}
		EVP_MD_CTX_free(context);
	}
        
    return success;
}



string DiffieHellman::find_aes128_key(string p_hex, string g_hex, string B_hex){
	std::unordered_map<string, string> my_diffie_values;

	boost::filesystem::path full_path( boost::filesystem::current_path());

	// Either read public and private key from file or create new pair
	if(boost::filesystem::exists(full_path.string()+"/"+diffie_hellman_1_output_path)){
		my_diffie_values = utils->read_hex_strs_from_file(diffie_hellman_1_output_path);
	}else{
		my_diffie_values = generate_public_key(p_hex, g_hex);
	}

	std::cout<<"\nA = "<<my_diffie_values["A"]<<"\n";

	auto V = find_diffie_hellman_secret(B_hex, my_diffie_values["a"], p_hex);

	string S;
	auto [V_byte, V_byte_len] = utils->turn_hex_str_to_byte_array(V);
	Openssl_SHA256hash(V_byte, V_byte_len, S);
	delete V_byte;

	return S.substr(0, KEY_SZ*2);
}
