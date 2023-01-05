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

// This header contains functions that do the actual traversing
// through files and perform deduplication

#ifndef WORK_H
#define WORK_H
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <unistd.h>
#include "helper.h"

extern __u64 chunk_size;
static __u64 check_length(off_t cur, off_t size)
{
	if(cur + chunk_size <= size)
		return chunk_size;
	return size - cur;
}
unsigned long long int do_direct(struct file_dedupe_range* fdr, int srcfd)
{
	off_t size = lseek(srcfd, 0, SEEK_END);
	off_t size1 = lseek(fdr->info->dest_fd, 0, SEEK_END);
	size = min(size, size1);
	unsigned long long int count = 0;
	
	for(off_t cur = 0; cur < size; cur+=chunk_size)
	{
		fdr->src_offset = cur;
		fdr->info->dest_offset = cur;
		fdr->src_length = check_length(cur, size);
		check( ioctl(srcfd, FIDEDUPERANGE, fdr) )
		if(fdr->info->status == FILE_DEDUPE_RANGE_SAME)
			count+= fdr->src_length;
	}
	
	return count;
}
unsigned int do_cross(struct file_dedupe_range* fdr, int srcfd)
{
	// TODO:
	printf_sync("Feature not implemented yet!\n");
	return 0;
}
#endif
