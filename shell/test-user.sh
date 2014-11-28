#!/usr/bin/env bash

if [[ "$EUID" != 0 ]]; then
	echo "This is not root user"
else
	echo "This is root user"
fi

exit
