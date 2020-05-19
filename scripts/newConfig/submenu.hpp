	#include <cppurses/cppurses.hpp>

	#include <iostream>
	#include <fstream>
	#include <filesystem>
	#include <stdio.h>

	#include "entry.hpp"
	#include "newConfig.hpp"

//----------------------------------------------------------------

using namespace cppurses;
namespace nConfig {
	class Submenu : public Entry {
	private:
		cppurses::Menu_stack *widget;
	public:
		Submenu(cppurses::Menu_stack *parent_menu);
		Submenu(cppurses::Menu_stack *parent_menu, std::string text);
		~Submenu();

		Widget *get_widget();
		void update_glyph(Glyph_string glyph);
	};
}
