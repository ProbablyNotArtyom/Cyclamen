#!/bin/bash

if [[ $1 == "" ]]; then
	./newConfig.rb ../nConfig.xml
else
	./newConfig.rb ../nConfig.xml DUMMY
fi
