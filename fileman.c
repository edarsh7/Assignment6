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

//used for recursively getting number of tabs to print
int tab_ct = 1;
int set = 1;


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
	//this code blocks writes the initial directory to file descriptor
	char temp2[20] = "";
	if(set)
	{
		strcpy(temp2, dname);
		strcat(temp2, "\n");
		write(fd, temp2, strlen(temp2));
		set = 0;
	}

	char next_file[100] = "";
	char name_buf[100] = "";
	DIR *dir = NULL;
	struct dirent **dir_info = NULL;
	int i = 0;

	dir = opendir(dname);
	if(!dir)
		return;	
	
	int n = scandir(dname, &dir_info, NULL, alphasort);
	int hold = n-1;

	while(n--)
	{
		strcpy(name_buf, "");
		if(strcmp(dir_info[i]->d_name, ".") == 0   || strcmp(dir_info[i]->d_name, "..") == 0)
		{	
			i++;
			continue;
		}
		int temp = tab_ct;
		
		//creating a buffer that holds the correct line to print
		while(temp != 0)
		{
			strcat(name_buf, "    ");
			temp--;
		}
		strcat(name_buf, dir_info[i]->d_name);
		strcat(name_buf, "\n");
		
		//writing buffer to fd
		write(fd, name_buf, strlen(name_buf));

		//creating buffer for the next files name
		strcpy(next_file, dname);
		strcat(next_file, "/");
		strcat(next_file, dir_info[i]->d_name);

		//recursive call into next subdirectory
		tab_ct++;
		fileman_dir(fd, next_file);
		tab_ct--;
		i++;

		//VALGRIND ERROR REMOVER
		if(n == 0)
		{
			while(hold >= 0)
			{
				free(dir_info[hold]);
				hold--;
			}
		}
	}
	//VALGRIND ERROR REMOVER
	free(dir_info);
	closedir(dir);
}

/*
 * You need to implement this function, see fileman.h for details 
 */
void fileman_tree(int fd, char *dname) {
}

