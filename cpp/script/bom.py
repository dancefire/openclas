#!/usr/bin/env python2.5

"""
Strip or append leading byte-order-mark from utf-8 files.
"""

import sys
import os

def usage(prog):
	print >>sys.stderr, 'usage: %s <option>' % prog
	print >>sys.stderr, '-a,    --append : Append UTF-8 BOM as the prefix of all C/C++ files recursivly from current directory'
	print >>sys.stderr, '-r,    --remove : Remove UTF-8 BOM prefix of all C/C++ files recursivly from current directory'
	sys.exit(1)

def bomer(filename, encoding):
	import codecs
	infile = codecs.open( filename, "r", "utf-8-sig" )	# 'utf-8-sig' encoding requires Python 2.5 or higher
	u = infile.read() # Returns a Unicode string from the UTF-8 bytes in the file
	infile.close()
	outfile = codecs.open( filename, "w", encoding)
	outfile.write( u )
	outfile.close()

def main(prog, *args):
	from getopt import getopt, GetoptError
	try:
		opts, args = getopt(args, 'ar', ['append', 'remove'])
	except GetoptError:
		usage(prog)

	encoding = "utf-8-sig"
	for o, a  in opts:
		if o in ("-a", "--append"):
			encoding = "utf-8-sig"
		elif o in ("-r", "--remove"):
			encoding = "utf-8"
		else:
			assert False, "unhandled option"

	for root, dirs, files in os.walk('.'):
		for name in files:
			base, ext = os.path.splitext(name)
			if ext in (".h", ".hpp", ".hxx", ".c", ".cpp", ".cxx"):
				filename = os.path.join(root, name)
				bomer(filename, encoding)
				print 'Converted ', filename
		
if __name__ == '__main__':
	try:
		main(*sys.argv)
	except KeyboardInterrupt:
		sys.exit(1)
