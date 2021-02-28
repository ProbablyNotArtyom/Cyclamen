#ifndef _PARSE_H
#define _PARSE_H

	#include <cppurses/system/focus.hpp>
	#include <cppurses/widget/layouts/vertical.hpp>
	#include <cppurses/widget/widgets/menu_stack.hpp>
	#include <cppurses/widget/widgets/titlebar.hpp>
	#include <cppurses/painter/glyph.hpp>
	#include <cppurses/painter/color.hpp>
	#include <cppurses/system/system.hpp>

	#include <filesystem>
	#include <iostream>
	#include <string>
	#include "pugixml/pugixml.hpp"

//----------------------------------------------------------------
namespace nConfig {
	class Parse {
	public:
		typedef std::vector<ptrdiff_t> offset_data_t;
	public:
		Parse();
		Parse(std::string filename);
		bool open(std::string filename);
		void test();
		bool build_offset_data(offset_data_t& result, const std::string file);
		std::pair<int, int> get_location(const offset_data_t& data, ptrdiff_t offset);

		inline static pugi::xml_document master_config;

	private:
		struct config_walker : pugi::xml_tree_walker {
			pugi::xml_node_type last_type = pugi::node_null;
			virtual bool for_each(pugi::xml_node& node) {
				if (node.type() == pugi::node_element) {
					if (last_type == pugi::node_element) std::cout << "\n";
					for (int i = 0; i < depth(); ++i) std::cout << "  ";
				}
				switch (node.type()) {
					case pugi::node_element:
						std::cout << "<" << node.name();
						for (pugi::xml_attribute attr: node.attributes())
							std::cout << " " << attr.name() << "=" << attr.value();
						std::cout << ">";
						break;
					case pugi::node_pcdata:
						std::cout << node.value() << "\n";
						break;
					case pugi::node_cdata:
					case pugi::node_comment:
						std::cout << "<!-- " << node.text() << " -->\n";
						break;
					case pugi::node_pi:
						std::cout << "<?" << node.name() << "?>\n";
						break;
					case pugi::node_declaration:
					case pugi::node_document:
					case pugi::node_null:
					case pugi::node_doctype:
					default:
						break;
				}
				last_type = node.type();
				return true; 		// Continue traversal
			}

			virtual bool end(pugi::xml_node& node) {
				if (last_type == pugi::node_pcdata) std::cout << "\n";
				return true;
			}
		};
	};
}

//----------------------------------------------------------------

#endif
