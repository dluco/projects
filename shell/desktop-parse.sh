#!/usr/bin/env bash

FILES=/usr/share/xsessions/*.desktop

for F in $FILES
do
	echo "Filename: $F"
	echo "WM Name:" $(grep -Po "(?<=^Name=).*" $F)
	echo "Exec:" $(grep -Po "(?<=^Exec=).*" $F)
done

exit 0
