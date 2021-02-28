	#include <cppurses/cppurses.hpp>
	#include <cppurses/system/focus.hpp>
	#include <cppurses/system/system.hpp>
	#include <cppurses/widget/widgets/menu.hpp>
	#include <cppurses/system/events/key.hpp>
	#include <cppurses/system/shortcuts.hpp>
	#include <cppurses/system/system.hpp>
	#include <cppurses/widget/focus_policy.hpp>
	#include <cppurses/widget/layouts/vertical.hpp>
	#include <cppurses/widget/widgets/menu.hpp>
	#include <cppurses/painter/glyph.hpp>
	#include <cppurses/painter/color.hpp>

	#include <iostream>
	#include <fstream>
	#include <filesystem>
	#include <typeinfo>
	#include <stdio.h>

	#include "entry.hpp"
	#include "newConfig.hpp"
	#include "textentry.hpp"

//----------------------------------------------------------------

using namespace cppurses;
namespace nConfig {
	TextEntry::~TextEntry() { widget->close(); }
	TextEntry::TextEntry(cppurses::Menu_stack *parent_menu) {
		set_title("Label");
		widget = &(parent_menu->make_page<cppurses::Label>(Entry::title_fmt()));
		NewConfig::init_glyphs(dynamic_cast<Widget*>(widget));
		widget->focus_policy = Focus_policy::None;
	}

	TextEntry::TextEntry(cppurses::Menu_stack *parent_menu, std::string text) {
		set_title(text);
		widget = &(parent_menu->make_page<cppurses::Label>(Entry::title_fmt()));
		NewConfig::init_glyphs(dynamic_cast<Widget*>(widget));
		widget->focus_policy = Focus_policy::None;
	}

	Widget *TextEntry::get_widget() {
		return dynamic_cast<Widget*>(widget);
	}

	void TextEntry::update_glyph(Glyph_string glyph) {
		widget->set_contents(glyph);
	}
}
