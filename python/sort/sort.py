#!/usr/bin/env python2

import fileinput

def main():
    lines = []

    # Read in input, line by line
    for line in fileinput.input():
        # Remove trailing newlines
        line = line.rstrip('\n\r')
        lines.append(line)
    
    # Sort input
    lines.sort()

    # Echo input
    for line in lines:
        print line

if __name__ == "__main__":
    main()
