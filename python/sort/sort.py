#!/usr/bin/env python2

import fileinput

def sort_sequential(str_list):
    temp = list(str_list)
    #result = []

    for i in temp:
        for j in temp:
            test = cmp(i, j)
            if test > 0:
                # String i comes after String j
                # Update i to be equal to j
                i = j
        print i


def main():
    lines = []

    for line in fileinput.input():
        # Remove trailing newlines
        line = line.rstrip('\n\r')
        lines.append(line)

    # No more input - echo input
    for line in lines:
        print line

    sort_sequential(lines)

if __name__ == "__main__":
    main()
