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
	class TextEntry : public Entry {
	private:
		cppurses::Label *widget;
	public:
		TextEntry(cppurses::Menu_stack *parent_menu);
		TextEntry(cppurses::Menu_stack *parent_menu, std::string text);
		~TextEntry();

		Widget *get_widget();
		void update_glyph(Glyph_string glyph);
	};
}
