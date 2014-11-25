#!/usr/bin/env bash

# run dialog
dialog --backtitle "Dialog Test" --nocancel --radiolist "Choose OS:" 0 0 0 \
	1 Linux off \
	2 Solaris on \
	3 'HP UX' off \
	4 AIX off

# clear screen
clear

exit 0
