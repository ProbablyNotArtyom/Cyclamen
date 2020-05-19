#ifndef _ACTIONBAR_H
#define _ACTIONBAR_H

	#include <cppurses/system/focus.hpp>
	#include <cppurses/widget/layouts/vertical.hpp>
	#include <cppurses/widget/widgets/menu_stack.hpp>
	#include <cppurses/widget/widgets/titlebar.hpp>
	#include <cppurses/painter/glyph.hpp>
	#include <cppurses/painter/color.hpp>

//----------------------------------------------------------------

namespace nConfig {
	
	class Action_bar : public cppurses::layout::Horizontal {
	public:
		Action_bar();

		cppurses::Push_button *selected;
		int selected_index = 0;

		bool focus_in_event() override;
		bool focus_out_event() override;
		bool focus_in_event_filter(Widget& receiver) override;

		cppurses::Label& pad_left {this->make_child<cppurses::Label>()};
		cppurses::Push_button& save_button {this->make_child<cppurses::Push_button>("Save")};
		cppurses::Push_button& select_button {this->make_child<cppurses::Push_button>("Select")};
		cppurses::Push_button& exit_button {this->make_child<cppurses::Push_button>("Exit")};
		cppurses::Push_button& help_button {this->make_child<cppurses::Push_button>("Help")};
		cppurses::Label& pad_right {this->make_child<cppurses::Label>()};

	private:
		void initialize();
	};
}

//----------------------------------------------------------------

#endif
