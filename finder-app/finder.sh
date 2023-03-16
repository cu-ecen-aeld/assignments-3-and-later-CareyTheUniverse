#!/bin/bash

# First argument is a path to a directory, referred to as filesdir
# Second argument is a text string referred, to as searchstr

# Check that both arguments are provided, then get arguments
if [ $# -ne 2 ]; then
	echo "Error: $0 required 2 arguments: filesdir searchstr"
	exit 1
else 
	filesdir="$1"
	searchstr="$2"
fi

# Check that filesdir represents a directory on the filesystem
if [ ! -d "$filesdir" ]; then
	echo "Error: $filesdir is not a directory"
	exit 1
fi

# Find number of files in the directory and all subdirectories
num_files=$(find "$filesdir" -type f | wc -l)

# Find number of matching lines found in respective files
num_lines=$(grep -r "$searchstr" "$filesdir" | wc -l)

echo "The number of files are $num_files and the number of matching lines are $num_lines"

exit 0
