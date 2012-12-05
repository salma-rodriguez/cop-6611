#! /bin/bash

# configure cache device
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

# name of virtual machines
vnm1='node1' 
vnm2='node2'

function start {
insmod $tk
echo 0 8387584 cache /dev/sdb1 $cac 0 8 1048576 1 1 $vnm1 | dmsetup create cache1
mount /dev/mapper/cache1 /media/dmmnt1
}

function addc {
echo 0 8387584 cache /dev/sdd1 $cac 0 8 1048576 1 1 $vnm2 | dmsetup create cache2
mount /dev/mapper/cache2 /media/dmmnt2
}

function destroy {
umount /dev/mapper/cache1 > /dev/null 2>&1
umount /dev/mapper/cache2 > /dev/null 2>&1
dmsetup remove /dev/mapper/cache1
dmsetup remove /dev/mapper/cache2
rmmod dm_cache > /dev/null 2>&1
}

case "$1" in
	start)
		start
		;;
	addc)
		addc
		;;
	stop)
		destroy
		;;
	*)
		echo "$) create/destroy"
		;;
esac
