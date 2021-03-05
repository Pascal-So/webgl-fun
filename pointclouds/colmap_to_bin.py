#!/usr/bin/env python3
import struct
from sys import stdin, stdout

for l in stdin:
	if l[0] == '#':
		continue

	coords = list(map(float, l.split()[1:4]))
	binary = struct.pack('fff', *coords)
	stdout.buffer.write(binary)
