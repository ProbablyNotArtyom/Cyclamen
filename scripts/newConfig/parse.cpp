
	#include <filesystem>
	#include <string>
	#include <iostream>
	#include <fstream>
	#include <filesystem>
	#include <yaml-cpp/yaml.h>

	#include "parse.hpp"

//----------------------------------------------------------------

using namespace pugi;
namespace nConfig {
	Parse::Parse() {}
	Parse::Parse(std::string filename) { Parse::open(filename); }

	bool Parse::open(std::string filename) {
		if (!std::filesystem::exists(filename)) {
			std::cerr << "[!] Cannot find nConfig file\n";
			return false;
		} else {
			xml_parse_result res = Parse::master_config.load_file(filename.c_str(), parse_minimal);
			if (!res) {
				offset_data_t offs;
				build_offset_data(offs, filename);
				std::cerr << "[!] Error at Line " << get_location(offs, res.offset).first
					<< ": " << res.description() <<"\n";
				return false;
			}
			return true;
		}
	}

//----------------------------------------------------------------

	void Parse::test() {
		xml_node node = Parse::master_config.child("newConfig");
		if (node.empty()) {
			std::cerr << "[!] nConfig does not start with 'newConfig:' header\n";
			return;
		}

		config_walker walker;
		Parse::master_config.traverse(walker);
	}

//----------------------------------------------------------------

	bool Parse::build_offset_data(offset_data_t& result, const std::string file) {
	    FILE* f = fopen(file.c_str(), "rb");
	    if (!f) return false;

	    ptrdiff_t offset = 0;
	    char buffer[1024];
	    size_t size;

	    while ((size = fread(buffer, 1, sizeof(buffer), f)) > 0) {
		    for (size_t i = 0; i < size; ++i)
		        if (buffer[i] == '\n') result.push_back(offset + i);

		    offset += size;
	    }

	    fclose(f);
	    return true;
	}

	std::pair<int, int> Parse::get_location(const offset_data_t& data, ptrdiff_t offset) {
	    offset_data_t::const_iterator it = std::lower_bound(data.begin(), data.end(), offset);
	    size_t index = it - data.begin();

	    return std::make_pair(1 + index, index == 0 ? offset + 1 : offset - data[index - 1]);
	}
}
