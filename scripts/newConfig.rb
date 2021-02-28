#!/usr/bin/env ruby

require 'mrdialog'
require 'io/console'
require 'pp'
require 'ox'

TYPE_INVALID = 0
TYPE_CHOICE = 1
TYPE_CHECKLIST = 2
TYPE_STRING = 3
TYPE_BOOL = 4

#----------------------------------------------------------------

def main_tester
	if ARGV.length < 1
		puts "[.] usage: newConfig.rb <master-nConfig-file>\n\n"
		exit 1
	elsif ARGV.length >= 1
		configurator = NConfig.new(ARGV[0])
		if ARGV.length >= 2
			puts Ox.dump(configurator.config)
			exit 0
		end
	end

end

#----------------------------------------------------------------

class NConfig_Menu
	def initialize(title, prompt)
		@prompt = prompt
		@title = title
		@entries = []
		@items = []
		@selected = 0
		@menu_data = Struct.new(:tag, :item)

		@tty_width = IO.console.winsize[1]
		@tty_height = IO.console.winsize[0]
	end

	def add(entry_obj)
		@items.push(entry_obj)
		data = @menu_data.new

		case entry_obj.type
			when TYPE_CHOICE
				data.tag = "#{entry_obj.title}"
				data.item = "#{entry_obj.prompt} <#{entry_obj.init}>"
			when TYPE_CHECKLIST
				data.tag = "#{entry_obj.title}"
				data.item = "#{entry_obj.prompt} --->"
			when TYPE_STRING
				data.tag = "#{entry_obj.title}"
				data.item = "#{entry_obj.prompt} (#{entry_obj.init})"
			when TYPE_BOOL
				data.tag = "#{entry_obj.title}"
				data.item = "#{entry_obj.prompt} [#{entry_obj.init}]"
			else
				data.tag = "NULL"
				data.item = "NULL"
		end
		@entries.push(data.to_a)
	end

	def update_entries
		@entries.each_index do |index|
			item = @items[index]
			case item.type
				when TYPE_CHOICE
					@entries[index][0] = "#{item.title}"
					@entries[index][1] = "#{item.prompt} <#{item.init}>"
				when TYPE_CHECKLIST
					@entries[index][0] = "#{item.title}"
					@entries[index][1] = "#{item.prompt} --->"
				when TYPE_STRING
					@entries[index][0] = "#{item.title}"
					@entries[index][1] = "#{item.prompt} (#{item.init})"
				when TYPE_BOOL
					@entries[index][0] = "#{item.title}"
					@entries[index][1] = "#{item.prompt} [#{item.init}]"
				else
					@entries[index][0] = "NULL"
					@entries[index][1] = "NULL"
			end
		end
	end

	def clear
		@entries.clear
		@items.clear
		@selected = 0
	end

	def trigger()
		menu = MRDialog.new
	    menu.clear = true
	    menu.title = @title
		menu.shadow = true
		menu.ok_label = "Select"
		menu.cancel_label = "Exit"
		menu.help_button = true
		menu.help_label = "Help"
		menu.extra_button = true
		menu.extra_label = "Save"
		menu.dialog_options = "--no-tags --column-separator \"\t\""

    	activated = menu.menu(@prompt, @entries, @tty_height/2, @tty_width/2)
		case menu.exit_code
			when menu.dialog_ok
				# Do nothing
			when menu.dialog_cancel, menu.dialog_esc
				puts "Exit activated"
				exit 0
			when menu.dialog_extra
				puts "Save activated"
				exit 1
			when menu.dialog_help
				puts "Help activated"
				exit 2
			else
				exit 0
		end
			index = @entries.find_index { |entry| entry[0] == activated }
			@items[index].trigger
			@selected = index
			self.update_entries
	end
end

module NConfig_Menu_Entry
	attr_accessor :type, :title, :prompt

	include Comparable
	def <=>(other)
    	self.type <=> other.type
	end
end

class NConfig_Input
	#include NConfig_Menu_Entry
	attr_accessor :type
	attr_accessor :prompt
	attr_accessor :title
	attr_accessor :init

	def initialize(title, prompt, init = "")
		@type = TYPE_STRING
		@prompt = prompt
		@title = title
		@init = init
	end

	def trigger(init = "")
		if init != "" then @init = init end
		dialog = MRDialog.new
	    dialog.clear = true
	    dialog.title = @title
		dialog.shadow = true
		dialog.nocancel = true
		dialog.help_button = true

	    @init = dialog.inputbox(@prompt, 0, 0, @init)
		return @init
	end
end

class NConfig_Choice
	#include NConfig_Menu_Entry
	attr_accessor :type
	attr_accessor :prompt
	attr_accessor :title
	attr_accessor :items
	attr_accessor :choice_data
	attr_accessor :init

	def initialize(title, prompt)
		@type = TYPE_CHOICE
		@items = []
		@prompt = prompt
		@title = title
		@choice_data = Struct.new(:tag, :item, :select)
		@init = ""
	end

	def add(name, text, state = false)
		data = @choice_data.new
		data.tag = name
		data.item = text
		data.select = state
		@items.push(data.to_a)
	end

	def trigger(init = "")
		if init != "" then @init = init end
		dialog = MRDialog.new
	    dialog.clear = true
	    dialog.title = @title
		dialog.shadow = true
		dialog.nocancel = true
		dialog.help_button = true

		begin
			activated = dialog.radiolist(@prompt, @items)
		rescue EOFError
			activated = [""]
		end
		@items.each { |item| item[2] = false }
		@items.each do |item|
			if item[0] == activated
				item[2] = true
				@init = item[0]
			end
		end
	end
end

class NConfig_Checklist
	#include NConfig_Menu_Entry
	attr_accessor :type
	attr_accessor :prompt
	attr_accessor :title
	attr_accessor :items
	attr_accessor :checklist_data
	attr_accessor :init

	def initialize(title, prompt)
		@type = TYPE_CHECKLIST
		@items = []
		@prompt = prompt
		@title = title
		@checklist_data = Struct.new(:tag, :item, :select)
		@init = ""
	end

	def add(name, text, state = false)
		data = @checklist_data.new
		data.tag = name
		data.item = text
		data.select = state
		@items.push(data.to_a)
	end

	def trigger(init = "")
		if init != "" then @init = init end
		dialog = MRDialog.new
	    dialog.clear = true
	    dialog.title = @title
		dialog.shadow = true
		dialog.nocancel = true
		dialog.help_button = true

		begin
			activated = dialog.checklist(@prompt, @items)[0].split("\" \"")
		rescue EOFError
			activated = [""]
		end
		@items.each { |item| item[2] = false }
		@items.each do |item|
			activated.each do |select|
				if item[0] == select.delete("\"")
					item[2] = true
					@init = item[0]
				end
			end
		end
	end
end

class NConfig_Bool
	#include NConfig_Menu_Entry
	attr_accessor :type
	attr_accessor :prompt
	attr_accessor :title
	attr_accessor :init

	def initialize(title, prompt, state = false)
		@type = TYPE_BOOL
		@prompt = prompt
		@title = title
		@init = format_bool(state)
	end

	def trigger(init = "")
		if init != "" then @init = init end
		dialog = MRDialog.new
	    dialog.clear = true
	    dialog.title = @title
		dialog.shadow = true

	    @init = format_bool(dialog.yesno(@prompt))
		return @init
	end

	private def format_bool(bool)
		if (bool)
			return "*"
		else
			return " "
		end
	end
end

#----------------------------------------------------------------

class Entry_Builder
	def self.config(node)
		type = "NULL"
		title = "Title"
		prompt = "Prompt"
		depends = ""
		help = "There is no help text for this option."
		default = ""
		node.each do |key, value|
			case key
			when :varname
				title = value
			when :prompt
				prompt = value
			when :type
				type = value
			when :depends
				depends = value
			when :help
				help = value
			when :default
				default = value
			end
		end
		case type
		when "bool"
			return NConfig_Bool.new(title, "#{prompt}\t", default)
		when "string"
			return NConfig_Input.new(title, "#{prompt}\t", default)
		when "int"
			return NConfig_Input.new(title, "#{prompt}\t", default)
		else

		end
	end

	def self.submenu(node)

	end

	def self.include(node)

	end
end

#----------------------------------------------------------------

class NConfig
	attr_accessor :config
	attr_accessor :rootmenu
	def initialize(filename)
		begin
			@config = Ox.load_file(filename, mode: :hash)
		rescue
			case $!
			when Ox::InvalidPath
				puts "[!] Failed to find nConfig file"
				exit 0
			when Ox::ParseError
				puts "[!] Failed to parse nConfig file: Error at Line #{$!.to_s[/(?<=line )[0-9]+/]}"
				exit 0
			end
		end

		if !@config[:value] == "newConfig"
			puts "[!] nConfig does not start with '<newConfig>' header"
			exit 0
		else
			@rootmenu = @config[:newConfig].find { |x| x[:rootmenu] }
			@submenu = @rootmenu[:rootmenu]
		end

		menu = NConfig_Menu.new(@config[:newConfig][0][:title], @rootmenu[:rootmenu][0][:title])
		@submenu.each do |node|
			if node.member? :config
				newconfig = Entry_Builder.config(node[:config])
				menu.add(newconfig)
			elsif node.member? :submenu
				newconfig = Entry_Builder.submenu(node[:submenu])
				menu.add(newconfig)
			end
		end
		menu.trigger
	end
end

#----------------------------------------------------------------

main_tester()
