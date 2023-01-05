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

// This header defines some generic helper functions
// that are used across the code.

#ifndef HELPER_H
#define HELPER_H

#include <errno.h>
#include <sys/syscall.h>
#define printf_sync(...)				\
	printf(__VA_ARGS__);				\
	fflush(stdout);
#define check(function)					\
	if((function) == -1)				\
	{						\
		printf(#function);			\
		printf("\n%i %m\n", errno);		\
		fflush(stdout);				\
		syscall(SYS_exit_group, 1);		\
	}
#define struct_size sizeof(struct file_dedupe_range) + sizeof(struct file_dedupe_range_info)

off_t min(off_t a, off_t b)
{
	if(a < b)
		return a;
	return b;
}
int open_check(const char *pathname, int flags)
{
	int fd = open(pathname, flags);
	if(fd < 0)
	{
		printf_sync("open(%s) failed with error: %i %m\n", pathname, errno);
		syscall(SYS_exit_group, 0);
	}
	return fd;
}
#endif
