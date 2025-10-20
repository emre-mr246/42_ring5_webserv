#!/usr/bin/env python3

import sys

nonexistent_file = open('/this/path/does/not/exist.txt', 'r')
content = nonexistent_file.read()
print(content)
