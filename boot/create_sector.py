#!/usr/bin/env python2

f = open('boot', 'r+b')

inbuf = f.read(512)

outbuf = '\x00' * (510 - len(inbuf))
outbuf += '\x55\xAA'    # boot sector flag

f.write(outbuf)
f.close()
