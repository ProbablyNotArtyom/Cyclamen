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

	#include "newConfig.hpp"
	#include "action_bar.hpp"
	#include "textentry.hpp"
	#include "submenu.hpp"

//----------------------------------------------------------------

using namespace cppurses;
using namespace nConfig;

int main(int argc, const char *argv[]) {
	if (argc < 2) {
		std::cout << "[.] usage: newconfig <master-nConfig-file>\n\n";
		exit(false);
	} else {
		NewConfig::current_dir = NewConfig::master_filename = std::string(argv[1]);
		NewConfig::current_dir.remove_filename();
		bool valid = NewConfig::parser.open(NewConfig::master_filename);
		if (!valid) exit(false);
		if (argc == 3) {		// Debug mode
			NewConfig::parser.test();
			exit(false);
		}
	}

    System sys;
    Rootmenu base_menu;
	TextEntry testlabel = nConfig::TextEntry(&base_menu.main_menu, "Test Label");
	Submenu testsubmenu = nConfig::Submenu(&base_menu.main_menu, "Test Submenu");

	NewConfig::init_glyphs(&base_menu.main_menu);
    System::set_initial_focus(&base_menu.main_menu);
    return sys.run(base_menu);
}

namespace nConfig {

	Rootmenu::Rootmenu() {
	    this->focus_policy = Focus_policy::None;
	    this->set_name("Main_menu - head widget");
	    this->brush.set_background(Color::Blue);
	    this->brush.set_foreground(Color::Blue);
		this->border.enable();

		title_label.focus_policy = Focus_policy::None;
	    title_label.set_name("Titlebar in Main_menu");
	    title_label.brush.set_background(Color::Dark_blue);
	    title_label.brush.set_foreground(Color::White);
		title_label.brush.add_attributes(Attribute::Bold);
		title_label.set_alignment(Alignment::Center);
		title_label.enable();

		main_menu.border.segments.north_west = L'╭';
	    main_menu.border.segments.north_east = L'╮';
	    main_menu.border.segments.south_west = L'╰';
	    main_menu.border.segments.south_east = L'╯';
	    main_menu.border.enable();
	    main_menu.set_name("Menu_stack in Main_menu");
	    main_menu.menu().set_name("Menu in Main Widget Stack Menu");
		main_menu.enable();

	    auto& esc_short = Shortcuts::add_shortcut(Key::Escape);
	    esc_short.connect([this] { main_menu.goto_menu(); });
	}
}
