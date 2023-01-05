# dedupe
Btrfs file deduplication tool.

This tool uses ioctl_fideduperange(2) to ask Linux kernel to compare regions of 2 files and merge them into the same physical block.
Further writes to either of those 2 files will again create separate copy of data for each file. (Copy-On-Write)

This was tested with btrfs, but it should work with any filesystem that supports used ioctl call.

# INSTALL
Program is quite simple so it doesn't have a Makefile. 

Just type 'gcc main.c -o dedupe' to compile it or download pre-build 'dedupe' binary file and run it.

There are no special dependencies for running nor for compiling. Only glibc (libc.so.6) is needed as for any dynamically linked program.
