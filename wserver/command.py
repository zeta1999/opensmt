#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sserver
import sys
import optparse
import os.path

__author__ = 'Matteo Marescotti'

if __name__ == '__main__':
    parser = optparse.OptionParser("usage: %prog <host> [options]")
    parser.add_option('-p', '--port', dest='port', type='int',
                      default=5000, help='specify commands port number')
    (options, args) = parser.parse_args()
    if len(args) == 0:
        parser.error("missing <host> argument")

    s = sserver.Socket()
    s.connect((args[0], options.port))

    if len(args) == 1:
        s.write(sys.stdin.read())
    else:
        for filename in args[1:]:
            with open(filename, 'r') as f:
                print filename
                s.write('{}S{}\\{}'.format(
                    '!' if filename.endswith('.smt2') else '',
                    os.path.splitext(os.path.basename(filename))[0],
                    f.read())
                )

    s.close()