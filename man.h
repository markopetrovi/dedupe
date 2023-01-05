/* Copyright (C) Marko Petrovic 2023
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or 
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program. 
* If not, see <https://www.gnu.org/licenses/>.
*/

// Documentation currently consists only of this help and comments in the code
#ifndef HELP_H
#define HELP_H
#include "helper.h"
#include <stdio.h>

// Print help and exit
void help(int status)
{
	printf("\nThis is the dedupe tool. On filesystems that support FIDEDUPERANGE ioctl \nthis tool compares 2 files and merges the same chunks in them ");
	printf("in the same \nphysical block by using filesystem's Copy-On-Write support. \nFor details look ioctl_fideduperange(2).\n\n");
	printf("Usage: dedupe [option] <file1> <file2>\n\n");
	printf("Options:\n");
	printf("\t--direct-cmp -d (default)\n\t\tCompare chunks at the same offset in destination and source \n\t");
	printf("Fast, but works efficiently only on files with the same beginning, \n\te.g. append-only files or same files\n\n");
	printf("\t--cross-cmp -c\n\t\tCompare every chunk from source with every chunk from \n\tdestination. Slow, but reaches maximum efficiency.\n\n");
	printf("\t--chunk-size=[size] (default: 4K)\n\t\tSet size to compare at one ioctl call. If even a single byte in \n\tthe chunk does not match, ");
	printf("deduplicaton of entire chunk fails. \n\tSmaller chunks achieve better efficiency at lower execution speed. \n\t");
	printf("Chunk must not be bigger than 16M. Possible suffixes: K, M\n\n");
	printf_sync("Author: \n\tWritten by Marko Petrovic\n\n");
	syscall(SYS_exit_group, status);
}
#endif
