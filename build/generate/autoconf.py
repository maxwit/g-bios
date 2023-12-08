#!/usr/bin/env python3
#
# Usage:
#   $0 .config autoconf.h

import os, sys

if __name__ == '__main__':
	if len(sys.argv) != 3:
		print('usage: ' + sys.argv[0] + ' .config autoconf.h')
		sys.exit()

	fd_dot = open(sys.argv[1], 'r')
	fd_auto = open(sys.argv[2], 'w')

	fd_auto.write('#pragma once\n\n')

	# TODO: filter out some options, i.e. CONFIG_COMPILE
	arch = None
	plat = None
	for str in fd_dot:
		length = len(str)
		if length > 1 and str[0] != '#':
			str = str.strip()
			str = str.replace(' ', '')
			(key, value) = str.split('=')

			if key == 'CONFIG_ARCH':
				arch = value
			elif key == 'CONFIG_PLAT':
				plat = value
			elif key in ['CONFIG_CROSS_COMPILE', 'CONFIG_DEBUG']:
				continue

			if value == 'y':
				fd_auto.write('#define ' + key + '\n')
			else:
				fd_auto.write('#define ' + key + ' ' + value + '\n')
		elif length == 1:
			fd_auto.write(str)

	if arch is None or plat is None:
		raise Exception('pls check CONFIG_ARCH/CONFIG_PLAT definition!')

	fd_auto.write('// device specific headers\n')
	fd_auto.write('#include <cpu.h>\n')
	# fd_auto.write('#include <' + arch + '/' + plat + '.h>\n')
	fd_auto.close()
	fd_dot.close()
