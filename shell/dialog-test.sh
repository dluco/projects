#!/usr/bin/env bash

# run dialog
<<<<<<< HEAD
dialog --title "Dialog Test" --nocancel --radiolist "Choose OS:" 15 40 5 \
	1 Linux on \
	2 Solaris off \
=======
dialog --backtitle "Dialog Test" --nocancel --radiolist "Choose OS:" 0 0 0 \
	1 Linux off \
	2 Solaris on \
>>>>>>> 204c95761ba2ac4cffc1408dd370d373f4f8c390
	3 'HP UX' off \
	4 AIX off

# clear screen
clear

exit 0
