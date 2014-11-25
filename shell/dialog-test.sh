#!/usr/bin/env bash

# run dialog
dialog --title "Dialog Test" --nocancel --radiolist "Choose OS:" 15 40 5 \
	1 Linux on \
	2 Solaris off \
	3 'HP UX' off \
	4 AIX off

# clear screen
clear

exit 0
