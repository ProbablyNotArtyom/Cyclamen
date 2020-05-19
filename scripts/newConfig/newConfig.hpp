#ifndef _NEWCONFIG_H
#define _NEWCONFIG_H

	#include <cppurses/system/focus.hpp>
	#include <cppurses/widget/layouts/vertical.hpp>
	#include <cppurses/widget/widgets/menu_stack.hpp>
	#include <cppurses/widget/widgets/titlebar.hpp>
	#include <cppurses/painter/glyph.hpp>
	#include <cppurses/painter/color.hpp>
	#include <cppurses/system/system.hpp>

	#include <filesystem>
	#include <typeinfo>
	#include <type_traits>
	#include "action_bar.hpp"
	#include "parse.hpp"

//----------------------------------------------------------------

using namespace cppurses;
namespace nConfig {

	class NewConfig {
	public:
		static void do_exit() {
			System::exit(0);
		}

		inline static void init_glyphs(Widget *widget) {
			widget->height_policy.stretch(2);
			widget->width_policy.stretch(2);
			widget->brush.set_background(NewConfig::default_color_fg);
			widget->brush.set_foreground(NewConfig::default_color_bg);
		}

		inline static void init_glyphs(Menu_stack *widget) {
			widget->height_policy.stretch(2);
			widget->width_policy.stretch(2);
			widget->brush.set_background(NewConfig::default_color_fg);
			widget->brush.set_foreground(NewConfig::default_color_bg);
			widget->menu().height_policy.stretch(2);
			widget->menu().width_policy.stretch(2);
			widget->menu().width_policy.type(Size_policy::Expanding);
			widget->menu().wallpaper = Glyph{L' ', background(NewConfig::default_color_bg)};
			widget->menu().set_brush_paints_wallpaper();
			widget->menu().brush.set_background(NewConfig::default_color_bg);
			widget->menu().brush.set_foreground(NewConfig::default_color_fg);
			widget->menu().title.brush.set_background(NewConfig::default_color_bg);
			widget->menu().title.brush.set_foreground(NewConfig::default_color_fg);
			widget->menu().title.brush.add_attributes(Attribute::Bold);
			widget->menu().show_title();
			widget->menu().hide_line_break();
			if (widget->menu().size() > 0)
				widget->menu().set_selected_attribute(selected_attr);
		}

		inline static std::string master_filename;
		inline static std::filesystem::path current_dir = std::filesystem::current_path();
		inline static Parse parser;
		inline static const Attribute selected_attr{Attribute::Bold};
		inline static const Color default_color_fg = Color::Blue;
		inline static const Color default_color_bg = Color::White;
	};

	class Rootmenu : public layout::Vertical {
	public:
		Rootmenu();

		Label& title_label {this->make_child<Label>(
			Glyph_string{"Titlebar", foreground(Color::Light_blue)})};
		Menu_stack& main_menu {this->make_child<Menu_stack>(
			Glyph_string{"Initial Menu", foreground(Color::White)})};
		Action_bar& action_bar {this->make_child<Action_bar>()};
	};
}

//----------------------------------------------------------------

#endif
