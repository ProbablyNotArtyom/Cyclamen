#!/usr/bin/env ruby

require 'mrdialog'
require 'pp'

TYPE_INVALID = 0
TYPE_CHOICE = 1
TYPE_CHECKLIST = 2
TYPE_STRING = 3

class NConfig_Menu
	@@title = ""
	@@prompt = ""
	@@entries = []
	@@items = []
	@@selected = 0
	@@menu_data = nil

	def initialize(title, prompt)
		@@prompt = prompt
		@@title = title
		@@menu_data = Struct.new(:tag, :item)
	end

	def add(entry_obj)
		@@items.push(entry_obj)
		data = @@menu_data.new

		case entry_obj.type
			when TYPE_CHOICE
				data.tag = "[?]"
				data.item = "#{entry_obj.prompt}"
			when TYPE_CHECKLIST
				data.tag = "--->"
				data.item = "#{entry_obj.prompt}"
			when TYPE_STRING
				data.tag = "(#{entry_obj.init})"
				data.item = entry_obj.prompt
			else
				data.tag = "NULL"
				data.item = "NULL"
		end
		@@entries.push(data.to_a)
	end

	def update_entries
		@@entries.each_index do |index|
			item = @@items[index]
			case item.type
				when TYPE_CHOICE
					@@entries[index][0] = "[?]"
					@@entries[index][1] = "#{item.prompt}"
				when TYPE_CHECKLIST
					@@entries[index][0] = "--->"
					@@entries[index][1] = "#{item.prompt}"
				when TYPE_STRING
					@@entries[index][0] = "(#{item.init})"
					@@entries[index][1] = item.prompt
				else
					@@entries[index][0] = "NULL"
					@@entries[index][1] = "NULL"
			end
		end
	end

	def clear
		@@entries.clear
		@@items.clear
		@@selected = 0
	end

	def trigger()

		menu = MRDialog.new
	    menu.clear = true
	    menu.title = @@title
		menu.shadow = true

    	activated = menu.menu(@@prompt, @@entries, 0, 0)
		index = @@entries.find_index { |entry| entry[0] == activated }
		@@items[index].trigger
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
	include NConfig_Menu_Entry
	attr_accessor :type
	attr_accessor :prompt
	attr_accessor :title
	attr_accessor :init

	def initialize(title, prompt)
		@type = TYPE_STRING
		@prompt = prompt
		@title = title
		@init = ""
	end

	def trigger(init = "")
		if init != "" then @init = init end
		dialog = MRDialog.new
	    dialog.clear = true
	    dialog.title = @title
		dialog.shadow = true

	    @init = dialog.inputbox(@prompt, 0, 0, @init)
		return @init
	end
end

class NConfig_Choice
	include NConfig_Menu_Entry
	attr_accessor :type
	attr_accessor :prompt
	attr_accessor :title
	attr_accessor :items
	attr_accessor :choice_data


	def initialize(title, prompt)
		@type = TYPE_CHOICE
		@items = []
		@prompt = prompt
		@title = title
		@choice_data = Struct.new(:tag, :item, :select)
	end

	def add(name, text, state = false)
		data = @choice_data.new
		data.tag = name
		data.item = text
		data.select = state
		@items.push(data.to_a)
	end

	def trigger()
		dialog = MRDialog.new
	    dialog.clear = true
	    dialog.title = @title
		dialog.shadow = true

		begin
			activated = dialog.radiolist(@prompt, @items)
		rescue EOFError
			activated = [""]
		end
		index = @items.find_index { |item| item[0] == activated }
		@items.each { |item| item[2] = false }
		@items[index][2] = true
	end
end

class NConfig_Checklist
	include NConfig_Menu_Entry
	attr_accessor :type
	attr_accessor :prompt
	attr_accessor :title
	attr_accessor :items
	attr_accessor :checklist_data

	def initialize(title, prompt)
		@type = TYPE_CHECKLIST
		@items = []
		@prompt = prompt
		@title = title
		@checklist_data = Struct.new(:tag, :item, :select)
	end

	def add(name, text, state = false)
		data = @checklist_data.new
		data.tag = name
		data.item = text
		data.select = state
		@items.push(data.to_a)
	end

	def trigger()
		dialog = MRDialog.new
	    dialog.clear = true
	    dialog.title = @title
		dialog.shadow = true

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
				end
			end
		end
	end
end

mainmenu = NConfig_Menu.new("Base Menu", "A test of the newConfig menu system.")

checklist0 = NConfig_Checklist.new("Test checklist title", "test prompt")
checklist0.add("Checklist 0", "Test Checklist 0")
checklist0.add("Checklist 1", "Test Checklist 1")
checklist0.add("Checklist 2", "Test Checklist 2")
checklist0.add("Checklist 3", "Test Checklist 3")

choice0 = NConfig_Choice.new("Test choice title", "test prompt")
choice0.add("Choice 0", "Test choice 0")
choice0.add("Choice 1", "Test choice 1")
choice0.add("Choice 2", "Test choice 2")
choice0.add("Choice 3", "Test choice 3")

string0 = NConfig_Input.new("Test String", "test string menu entry")
string1 = NConfig_Input.new("Test String", "test string menu entry")
string2 = NConfig_Input.new("Test String", "test string menu entry")
string3 = NConfig_Input.new("Test String", "test string menu entry")

mainmenu.add(checklist0)
mainmenu.add(choice0)
mainmenu.add(string0)
mainmenu.add(string1)
mainmenu.add(string2)
mainmenu.add(string3)
mainmenu.add(checklist0)
mainmenu.add(checklist0)
mainmenu.add(checklist0)
mainmenu.add(checklist0)

until 0 == 1 do
	mainmenu.trigger
end
