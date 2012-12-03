#! /bin/bash

# configure source & cache devices
src='/dev/sdd1'
cac='/dev/sdc'

# dm-cache kernel object
tk="dm-cache.ko"

# parm 1: start index of source device
# parm 2: final index of source device
# parm 3: the word 'cache' which must be included
# parm 4: source device
# parm 5: cache device
# parm 6: load previous cache configuration; otherwise, start from scratch
# parm 7: block size used by the cache
# parm 8: capacity of the cache
# parm 9: cache associativity; doesn't matter what we put here
# parm 10: enable write back

vnm='node1' # name for virtual machine

insmod $tk
echo 0 8385898 cache $src $cac 0 8 1048237 1 1 $vnm | dmsetup create foodev
mount /dev/mapper/foodev /media/dmmount
