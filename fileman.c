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

/*
LIST OF CITATIONS:
https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
https://www.tutorialspoint.com/cprogramming/c_file_io.htm
https://stackoverflow.com/questions/19780919/read-write-from-file-descriptor-at-offset
https://linux.die.net/man/2/creat
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

	return 0;
}

/*
 * You need to implement this function, see fileman.h for details 
 */
int fileman_append(char *fname, char *buf, size_t size) {
	return 0;
}

/*
 * You need to implement this function, see fileman.h for details 
 */
int fileman_copy(char *fsrc, char *fdest) {
	// Remove, only here to fail code warning test
  
	return 0;
}

/*
 * You need to implement this function, see fileman.h for details 
 */
void fileman_dir(int fd, char *dname) {
}

/*
 * You need to implement this function, see fileman.h for details 
 */
void fileman_tree(int fd, char *dname) {
}

