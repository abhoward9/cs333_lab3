#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <sys/types.h>
#include <inttypes.h>

#include <md5.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "viktar.h"

void display_help(void);
void read_viktar(char *);
void toc(char * goodTest);
void showLongToC(char * goodTest);
void writeToFile(viktar_header_t person, const char *filename);
void readFile(const char *filename);


void readFile(const char *filename) {
	char test[50];	
    //int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        exit(1);
    }

    // Write the data directly in binary format
    if (read(fd, test, 10)) {
	printf("%s", test);
        close(fd);
        exit(1);
    }

    close(fd);


}
int main(int argc, char **argv)
{
    int opt;
	viktar_action_t action;
	char str[1000];
	//bool show_ToC = false;
	//bool long_ToC = false;
/*
    char input[100];
    bool doEncrypt = true;
    int shift = 3;
    long shiftedResult;
    int base;
    char *endptr;
*/
	//viktar_header_t test_data = {"Alice", 25, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, 2, 2, {3, 3}, {4,4}};

    // Write the structure to the file

    //writeToFile(test_data, "person.dat");

	//readFile("goodTest");








    while ((opt = getopt(argc, argv, OPTIONS)) != -1)
    {
        switch (opt)
        {
	/*Extract members from viktar file.
• Names of files to extract are given on the command line,
unless…
• If no filenames are given on the command line, extract all files.
• If the -f command line option is not used, read the archive
file from stdin.
	When extracting files from the archive, the file timestamps
(atime and mtime) must be restored as stored in the
archive.
• When extracting files from the archive, the file permissions
(mode) must be restored as stored in the archive.
• You cannot restore file ownership as stored in the archive, so
don’t spend time on it. Extracted files will always be owned by
you.
• You must validate the archive member data with the 32-bit
CRC value in the footer when you extract a file. If the CRC
values do not match, issue a warning (to stderr) and extract
the file anyway.
• If the extracted file already exists, overwrite it.
*/
        case 'x':
		printf("extract");		
            break;
/*Create a viktar style archive file.
• The names of the files to place in the archive are given on the
command line.
• If no filenames are given on the command line, create a
viktar file that only contains the VIKTAR_FILE header
line.
• If the -f command line option is not used, write the archive
file to stdout.
• If the archive file does not exist, put the following permissions
on it "
-rw--r—r"
.
• You cannot set the permissions on an archive file sent to
stdout. It will just have the permissions specified by your
umask.
• If the archive file already exists, overwrite it.
*/
        case 'c':
		action = ACTION_CREATE;
            break;
        case 't':
		action = ACTION_TOC_SHORT;

            break;
	case 'T':
		action = ACTION_TOC_LONG;
		break;
/*Specify the name of the viktar file on which to operate.
• If the -f command line option is not used, you will read/write
stdio as necessary for the operation.*/
	case 'f':
              if (optopt == 'f')
              {
                  fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                  printf("%s", optarg);
              }
              else
              {
                  strcpy(str, optarg);
		//read_viktar(str);
              }
              break;
  
		break;	
/*
	Validate the content of the archive member with the CRC values
stored in the archive file.
• Your validation must match exactly the one produced by my
implementation of viktar.
• If the -f option is not used, read the archive file from stdin.
Look at the output of my solution for the format of the output.
*/
	case 'V':
		//printf("Validate the content");
		break;
//Show the help text and exit. Your help text must match mine.
	case 'h':
		display_help();
		break;
/*
Verbose processing.
• This is really to help you follow what your code is doing.
• You need to accept this switch, have your code emit some
diagnostics with this set. If your code simply prints a message
that verbose is enabled, it will meet this objective.
• All the messages from the verbose output should be sent to
stderr, NOT stdout. Do not comingle output and
diagnostics.
*/
	case 'v':
		perror("verbose is enabled");
		break;
        default:
            abort();
        }
    }
    switch(action) {
        case ACTION_NONE:
            printf("No action selected.\n");
            break;
        case ACTION_CREATE:
            break;
        case ACTION_EXTRACT:
            printf("Performing extract action.\n");
            break;
        case ACTION_TOC_SHORT:
		toc(str);
            break;
        case ACTION_TOC_LONG:
			showLongToC(str);
            break;
        case ACTION_VALIDATE:
            printf("Performing validation.\n");
            break;
        default:
            printf("Unknown action.\n");
            break;
    }
	return EXIT_SUCCESS;
}





void display_help(void) {
printf("help text\n");
    printf("\tUsage: ./viktar [OPTIONS]\n");
    printf("\tOptions: xctTf:Vhv\n");
    printf("  -x           extract file/files from archive\n");
    printf("  -c           create an archive file\n");
    printf("  -t           display a short table of contents of the archive file\n");
    printf("  -T           display a long table of contents of the archive file\n");
    printf("               Only one of xctTV can be specified\n");
    printf("  -f filename  use filename as the archive file\n");
    printf("  -V           validate the MD5 values in the viktar file\n");
    printf("  -v           give verbose diagnostic messages\n");
    printf("  -h           display this AMAZING help message\n");
}


/*
void read_viktar(char* filename) {
	int iarch = STDIN_FILENO;
	char buf[100] = {'\0'};
	viktar_header_t md;
	

	if (filename != NULL ) {
		iarch = open(filename, O_RDONLY);
	}
	// validate tag
	read(iarch, buf, strlen(VIKTAR_TAG));
	if(strncmp(buf, VIKTAR_TAG, strlen(VIKTAR_TAG)) != 0) {
	// not a valid viktar file
	// print snarky message and exit(1).
		fprintf(stderr, "snarky message\n");
		exit(EXIT_FAILURE);
	}
	printf("Contents of viktar file: \"%s\"\n", filename != NULL ? filename : "stdin");

	// process the archive file metadata
	while (read(iarch, &md, sizeof(viktar_header_t )) > 0) {
		// print archive member name
	memset(buf, 0, 100);
	strncpy(buf, md.viktar_name, VIKTAR_MAX_FILE_NAME_LEN);
	printf("\tfile name: %s\n", buf);
	lseek(iarch, md.st_size + sizeof(viktar_footer_t), SEEK_CUR);
	}



		if (filename != NULL ) {
		close(iarch);
		}


}*/

void toc(char * filename) {
	int iarch = STDIN_FILENO;
	char buf[100] = {'\0'};
	viktar_header_t md;
	

	if (filename != NULL ) {
		iarch = open(filename, O_RDONLY);
	}
	// validate tag
	read(iarch, buf, strlen(VIKTAR_TAG));
	if(strncmp(buf, VIKTAR_TAG, strlen(VIKTAR_TAG)) != 0) {
	// not a valid viktar file
	// print snarky message and exit(1).
		fprintf(stderr, "snarky message\n");
		exit(EXIT_SUCCESS);
	}
	printf("Contents of viktar file: \"%s\"\n", filename != NULL ? filename : "stdin");

	// process the archive file metadata
	while (read(iarch, &md, sizeof(viktar_header_t )) > 0) {
		// print archive member name
	memset(buf, 0, 100);
	strncpy(buf, md.viktar_name, VIKTAR_MAX_FILE_NAME_LEN);
	printf("\tfile name: %s\n", buf);
	lseek(iarch, md.st_size + sizeof(viktar_footer_t), SEEK_CUR);
	}



		if (filename != NULL ) {
		close(iarch);
		}
}










void writeToFile(viktar_header_t person, const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to open file");
        exit(1);
    }

    // Write the data directly in binary format
    if (write(fd, &person, sizeof(viktar_header_t)) == -1) {
        perror("Failed to write to file");
        close(fd);
        exit(1);
    }

    close(fd);


}

void showLongToC(char * filename) {

	int iarch = STDIN_FILENO;
	char buf[100] = {'\0'};
    	char buff[100] = {'\0'};
	viktar_header_t md;
	viktar_footer_t ft;
	

	if (filename != NULL ) {
		iarch = open(filename, O_RDONLY);
	}
	// validate tag
	read(iarch, buf, strlen(VIKTAR_TAG));
	if(strncmp(buf, VIKTAR_TAG, strlen(VIKTAR_TAG)) != 0) {
	// not a valid viktar file
	// print snarky message and exit(1).
		fprintf(stderr, "snarky message\n");
		exit(EXIT_SUCCESS);
	}
	printf("Contents of viktar file: \"%s\"\n", filename != NULL ? filename : "stdin");

	// process the archive file metadata
	while (read(iarch, &md, sizeof(viktar_header_t )) > 0) {
		struct passwd *pw; 	// print archive member name
		struct group *grp; 	// print archive member name
		mode_t fileMode = md.st_mode; //& 0777;
		//struct timespec ts;
		memset(buf, 0, 100);
		strncpy(buf, md.viktar_name, VIKTAR_MAX_FILE_NAME_LEN);
		printf("\tfile name: %s\n", buf);
    		printf("\t\tmode:\t\t%o\n", fileMode);
		pw = getpwuid(md.st_uid);  // Look up the user information	
    		if (pw) {
    			printf("\t\tuser:\t\t%s\n", pw->pw_name);
        	//printf("Username: %s\n", pw->pw_name);  // Print the username
    		} else {
        		perror("getpwuid");  // Error handling if the UID is not found
    		}

		grp = getgrgid(md.st_gid);  // Look up the user information	
    		if (grp) {
    			printf("\t\tgroup:\t\t%s\n", grp->gr_name);
        	//printf("Username: %s\n", pw->pw_name);  // Print the username
    		} else {
        		perror("getpwuid");  // Error handling if the UID is not found
    }
/*
    printf("    md5 sum header: %s\n", file.md5Header);
    printf("    md5 sum data:   %s\n\n", file.md5Data);
*/
	printf("\t\tsize:\t\t\%ld\n", md.st_size);

    //timespec_get(&md.st_atim, TIME_UTC);
    	strftime(buff, sizeof buff, "%Y-%m-%d %H:%M:%S %Z", localtime(&md.st_mtim.tv_sec));
    	printf("\t\tmtime:\t\t%s\n", buff);
    	strftime(buff, sizeof buff, "%Y-%m-%d %H:%M:%S %Z", localtime(&md.st_atim.tv_sec));
    	printf("\t\tatime:\t\t%s\n", buff);

	lseek(iarch, md.st_size, SEEK_CUR);
	if (read(iarch, &ft, sizeof(viktar_footer_t )) > 0) {
printf("\t\tmd5 sum header:\t");
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        printf("%02" PRIx8, ft.md5sum_header[i]);
    }
    printf("\n");

printf("\t\tmd5 sum data:\t");
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        printf("%02" PRIx8, ft.md5sum_data[i]);
    }
    printf("\n");
	}



	}



		if (filename != NULL ) {
		close(iarch);
		}

}













