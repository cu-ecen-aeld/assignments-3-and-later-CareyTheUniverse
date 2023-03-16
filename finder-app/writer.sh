#!/bin/bash

# First argument is a full path to a file on the filesystem, referred to as writefile
# Second argument is a text string to be written within this file, referred to as writestr

# Check that both arguments are provided, then get arguments
if [ $# -ne 2 ]; then
	echo "Error: $0 requried 2 arguments: writefile writestr"
	exit 1
else
	writefile="$1"
	writestr="$2"
fi

# Create a new path if it doesn't already exist
mkdir -p "$(dirname "$writefile")"

# Write writefile with conect writestr, overwriting any existing file
echo "$writestr" > "$writefile"

# Check that the file was created
if [ ! -f "$writefile" ]; then
	echo "Error: $writefile wasn't created"
	exit 1
fi

exit 0
