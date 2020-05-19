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
	#include <stdio.h>

	#include "entry.hpp"
	#include "submenu.hpp"

//----------------------------------------------------------------

using namespace cppurses;
namespace nConfig {
	Submenu::~Submenu() { widget->close(); }
	Submenu::Submenu(cppurses::Menu_stack *parent_menu) {
		set_format("{} --->");
		set_title("Submenu");
		widget = &(parent_menu->make_page<cppurses::Menu_stack>(Entry::title_fmt()));
		NewConfig::init_glyphs(widget);
	}

	Submenu::Submenu(cppurses::Menu_stack *parent_menu, std::string text) {
		set_format("{} --->");
		set_title(text);
		widget = &(parent_menu->make_page<cppurses::Menu_stack>(Entry::title_fmt()));
		NewConfig::init_glyphs(widget);
	}

	Widget *Submenu::get_widget() {
		return dynamic_cast<Widget*>(widget);
	}

	void Submenu::update_glyph(Glyph_string glyph) {
		//widget->menu_.title.set_contents(glyph);
	}
}
