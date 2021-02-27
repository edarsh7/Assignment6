/*********************************************************************
 *
 * Copyright (C) 2020-2021 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 *
 ***********************************************************************/

#include "fileman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>




/*
LIST OF CITATIONS:
https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
https://www.tutorialspoint.com/cprogramming/c_file_io.htm

*used for figuring out pwrite() and pread() in fileman_read and fileman_write*
https://stackoverflow.com/questions/19780919/read-write-from-file-descriptor-at-offset

https://linux.die.net/man/2/creat

*used for figuring out stats() function in fileman_write
https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c

https://profile.iiita.ac.in/bibhas.ghoshal/lab_files/System%20calls%20for%20files%20and%20directories%20in%20Linux.html
*/

/*
 * Read at most SIZE bytes from FNAME starting at FOFFSET into BUF starting 
 * at BOFFSET.
 *
 * RETURN number of bytes read from FNAME into BUFF, -1 on error.
 */
int fileman_read(char *fname, size_t foffset, char *buf, size_t boffset, size_t size) 
{
	int file = open(fname, O_RDONLY);

	if(file == -1)
		return file;
	
	for(;boffset > 0 ; boffset--)
	{
		buf++;
	}
	int bytes_read = pread(file, buf, (int)size, foffset);
	return bytes_read;
}

/*
 * Create FILE and Write SIZE bytes from BUF starting at BOFFSET into FILE 
 * starting at FOFFSET.
 * 
 * RETURN number of bytes from BUFF written to FNAME, -1 on error or if FILE 
 * already exists
 */
int fileman_write(char *fname, size_t foffset, char *buf, size_t boffset, size_t size) 
{
	struct stat sb;
	if(stat(fname, &sb) == 0)
		return -1;

	int file = creat(fname, 0777);

	for(;boffset > 0 ; boffset--)
	{
		buf++;
	}

	int bytes_read = pwrite(file, buf, size, foffset);
	return bytes_read;
}


/*
 * Append SIZE bytes from BUF to existing FILE.
 * 
 * RETURN number of bytes from BUFF appended to FNAME, -1 on error or if FILE 
 * does not exist
 */
int fileman_append(char *fname, char *buf, size_t size) {

	int file = open(fname, O_APPEND|O_RDWR);

	if(file == -1)
		return file;

	int bytes_read = write(file, buf, size);
	return bytes_read;
}

/*
 * Copy existing file FSRC to new file FDEST.
 * 
 * RETURN number of bytes from FSRC written to FDEST, -1 on error, or if FSRC 
 * does not exists, or if SDEST already exists
 */
int fileman_copy(char *fsrc, char *fdest) {

	struct stat stat_dest;
	int f_src = open(fsrc, O_RDWR);

	if(f_src == -1 || stat(fdest, &stat_dest) == 0)
		return -1;

	int f_dest = creat(fdest, 0777);

	struct stat stat_src;
	stat(fsrc, &stat_src);
	char *buf = malloc((int)stat_src.st_size);

	int x = read(f_src, buf, (int)stat_src.st_size);
	int y = write(f_dest, buf, x);
	free(buf);
	return y;

	
}

/*
 * You need to implement this function, see fileman.h for details 
 */
void fileman_dir(int fd, char *dname)
{
	DIR *dir;
	struct dirent *dp;

	char fn_q[100];
	char nn_q[100];
	dir = opendir(dname);
	while((dp = readdir(dir)) != NULL)
	{
		struct stat stbuf;
		sprintf(fn_q,"%s/%s", dname, dp->d_name);
		if(stat(fn_q, &stbuf) == -1)
		{
			printf("idk \n");
			continue;
		}
		if((stbuf.st_mode & S_IFMT) == S_IFDIR)
		{
			continue;
		}
	
	}

	
}

/*
 * You need to implement this function, see fileman.h for details 
 */
void fileman_tree(int fd, char *dname) {
}

