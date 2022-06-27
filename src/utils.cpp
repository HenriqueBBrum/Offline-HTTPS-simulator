#include<../include/utils.hpp>

#include <fstream>
#include <sstream>

#include <boost/algorithm/string.hpp>



std::unordered_map<string, string> Utils::read_hex_strs_from_file(string filename){
	std::fstream input_file;
	input_file.open(filename,std::ios::in); 
	
	std::unordered_map<string, string> map;
	if (input_file.is_open()){ 
		string line, aux = "", key = "";
		while(getline(input_file, line)){ 
			std::stringstream ss(line);
			if(file_delimiter==line[0]){
				if(key.length()>0){
					boost::erase_all(aux, " ");
					map[key] = aux;
					aux = "";
				}
				key = line.substr(1);
			}else{
				aux=aux+line;
			}
		}
		input_file.close();
	}

	return map;
}



std::tuple<byte*, int> Utils::turn_hex_str_to_byte_array(const string &hex_str){
	byte* byte_array =  new byte[hex_str.length()/2]; 
	int j = 0;
	for(size_t i = 0; i<hex_str.length();i+=2){
		auto b = hex_str.substr(i, 2);
		byte_array[j] = (char) (int)strtol(b.c_str(), nullptr, 16);
		j++;
	}
	return {byte_array, j};
}


void Utils::reverse( char *start, char *end ){
    while( start < end ){
        char c = *start;
        *start++ = *end;
        *end-- = c;
    }
}


char *Utils::reverse_char( char *start ){
    char *end = start;
    while( (end[1] & 0xC0) == 0x80 ) end++; // In case of multibyte char
    reverse( start, end );
    return( end+1 );
}

void Utils::reverse_string( char *string ){
    char *end = string;
    while( *end ) end = reverse_char( end );
    reverse( string, end-1 );
}
