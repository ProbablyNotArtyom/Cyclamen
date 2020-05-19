#ifndef _ENTRY_H
#define _ENTRY_H

	#include <cppurses/system/focus.hpp>
	#include <cppurses/widget/layouts/vertical.hpp>
	#include <cppurses/widget/widgets/menu_stack.hpp>
	#include <cppurses/widget/widgets/titlebar.hpp>
	#include <cppurses/painter/glyph.hpp>
	#include <cppurses/painter/color.hpp>
	#include <cppurses/system/system.hpp>

	#include <filesystem>
	#include <fstream>
	#include <string>
	#include <typeinfo>
	#include <fmt/format.h>

	#include "newConfig.hpp"

//----------------------------------------------------------------

using namespace cppurses;
namespace nConfig {

	// Abstract base class for all menu entries
	class Entry {
	private:
		inline static std::string _entry_format = "{}";
		// entry_format defines the formatting string used to represent the entry in a menu
		// an entry_format should contain exactly one "{}", and no other formatting commands
		// this is formatted with entry_title to produce the final string displayed by the menu

		inline static std::string _entry_title = "";
		// entry_title defines the string used as to generate the menu entry label
		// it is formatted with entry_format before use
		// It must contain no newlines or line returns

		Color color_fg = NewConfig::default_color_fg;
		Color color_bg = NewConfig::default_color_bg;

	public:
		void set_title(std::string title) { _entry_title = title; }
		void set_format(std::string format) { _entry_format = format; }
		std::string title() { return _entry_title; }
		std::string format() { return _entry_format; }
		Glyph_string title_fmt() {
			std::string label_fmt = fmt::v6::format(_entry_format, _entry_title);
			Glyph_string glyph = {label_fmt, foreground(color_fg), background(color_bg)};
			return glyph;
		}

		void set_color_fg(Color fg) {
			color_fg = fg;
			update();
		}

		void set_color_bg(Color bg) {
			color_bg = bg;
			update();
		}

		void update() {
			std::string label_fmt = fmt::v6::format(_entry_format, _entry_title);
			Glyph_string glyph = {label_fmt, foreground(color_fg), background(color_bg)};
			update_glyph(glyph);
		}

	public:
		virtual ~Entry() = default;

		virtual Widget *get_widget() = 0;
		virtual void update_glyph(Glyph_string) = 0;
	};
}

//----------------------------------------------------------------

#endif
