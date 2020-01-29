#!/bin/bash
#usage: ./blktrace <device>

blktrace $1 -a complete -o - | blkparse -f "%T.%t %d %S %N\n" -i -
