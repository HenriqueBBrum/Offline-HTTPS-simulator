
/* Generic class for methods not used by AES or Diffie-hellman class */


#pragma once


#include <unordered_map>
#include <string>

#define BLOCK_SZ 16
#define KEY_SZ 16

using std::string;

typedef unsigned char byte;


class Utils{
    private:
        // Reverse multibyte char
        void reverse( char *start, char *end );

        char *reverse_char( char *start );


    public:
        Utils(){}

        static const char file_delimiter = '%';

        /*  Reads hex strings separeted by '%<name>' in a txt file. 
            Returns a map with the following format: {<name>, <hex_string>} */
        std::unordered_map<string, string> read_hex_strs_from_file(string filename);

        /*  Turns a hex string (e.g F24A00) to a byte array. 
            Returns the array and its size  */
        std::tuple<byte*, int> turn_hex_str_to_byte_array(const string &hex_str);

        /* Reverses an UTF-8 string */
        void reverse_string( char *string );
};