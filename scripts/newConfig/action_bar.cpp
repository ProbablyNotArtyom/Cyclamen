	#include <cppurses/cppurses.hpp>
	#include "newConfig.hpp"
	#include "action_bar.hpp"

//----------------------------------------------------------------

using namespace cppurses;
namespace nConfig {

	Action_bar::Action_bar() {
		this->initialize();
	    this->focus_policy = Focus_policy::Tab;
		this->set_name("Action_bar");
	    save_button.set_name("Action_bar - save_btn");
	    select_button.set_name("Action_bar - select_button");
	    exit_button.set_name("Action_bar - exit_button");
	    help_button.set_name("Action_bar - help_button");

	    pad_left.set_name("Action_bar - pad_left");
	    pad_right.set_name("Action_bar - pad_right");
		this->enable();
		this->install_event_filter(*this);

		auto& save_short = Shortcuts::add_shortcut(Key::s);
	    save_short.connect([this] { save_button.clicked(); });
		auto& exit_short = Shortcuts::add_shortcut(Key::e);
	    exit_short.connect([this] { exit_button.clicked(); });
		auto& help_short = Shortcuts::add_shortcut(Key::h);
	    help_short.connect([this] { help_button.clicked(); });

		exit_button.clicked.connect([this] { NewConfig::do_exit(); });

		selected = &save_button;
		selected_index = 0;
	}

	void Action_bar::initialize() {
		this->height_policy.fixed(3);
		this->width_policy.stretch(2);
		this->width_policy.type(Size_policy::Expanding);

		pad_left.width_policy.stretch(2);
		pad_left.height_policy.fixed(3);
	    pad_left.brush.set_background(Color::Blue);
	    pad_left.brush.set_foreground(Color::Blue);

	    save_button.width_policy.fixed(8);
	    save_button.brush.set_background(Color::Dark_blue);
	    save_button.brush.set_foreground(Color::White);
		save_button.border.enable();
		select_button.width_policy.fixed(8);
	    select_button.brush.set_background(Color::Dark_blue);
	    select_button.brush.set_foreground(Color::White);
		select_button.border.enable();
	    exit_button.width_policy.fixed(8);
	    exit_button.brush.set_background(Color::Dark_blue);
	    exit_button.brush.set_foreground(Color::White);
		exit_button.border.enable();
	    help_button.width_policy.fixed(8);
	    help_button.brush.set_background(Color::Dark_blue);
	    help_button.brush.set_foreground(Color::White);
		help_button.border.enable();

		pad_right.width_policy.stretch(2);
		pad_right.height_policy.fixed(3);
	    pad_right.brush.set_background(Color::Blue);
	    pad_right.brush.set_foreground(Color::Blue);

	}

	bool Action_bar::focus_in_event() {
		switch (selected_index) {
			case 0:	selected = &save_button; break;
			case 1:	selected = &select_button; break;
			case 2:	selected = &exit_button; break;
			case 3:	selected = &help_button; break;
		}

		auto& action_enter = Shortcuts::add_shortcut(Key::Enter);
		action_enter.connect([this] { selected->clicked(); });
	    selected->brush.set_background(Color::White);
	    selected->brush.set_foreground(Color::Blue);
	    selected->update();
	    return Widget::focus_in_event();
	}

	bool Action_bar::focus_out_event() {
	    selected->brush.set_background(Color::Dark_blue);
	    selected->brush.set_foreground(Color::White);
	    selected->update();

		Shortcuts::remove_shortcut(Key::Enter);
		if (selected_index >= 3) selected_index = 0;
		else selected_index++;

	    return Widget::focus_out_event();
	}

	bool Action_bar::focus_in_event_filter(Widget& receiver) {
	    Focus::set_focus_to(*this);
	    return true;
	}
}
