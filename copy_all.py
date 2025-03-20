"""copy all code .c and .h to a string and save to a file with headers
like

--- main.c ---
#include <stdio.h>

(...)

--- binary_tree.c ---
#include "binary_tree.h"

(...)

--- binary_tree.h ---
#ifndef BINARY_TREE_H
#define BINARY_TREE_H
...
#endif

"""

import os
import sys

if len(sys.argv) != 2:
	print("usage: python copy_all.py <output_file>")
	sys.exit(1)

output_file = sys.argv[1]

with open(output_file, "w") as f:
	for file in os.listdir("."):
		if file.endswith(".c") or file.endswith(".h"):
			with open(file, "r") as f2:
				f.write(f"--- {file} ---\n")
				f.write(f2.read())
				f.write("\n")

	print(f"All .c and .h files have been copied to {output_file}")
	
