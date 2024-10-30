#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <sys/types.h>
#include <md5.h>
#include <fcntl.h>
#include <unistd.h>
#include "viktar.h"

void display_help(void);
void read_viktar(char *);
void toc(void);
struct Person {
    char name[50];
    int age;
    float height;
};
void writeToFile(viktar_header_t person, const char *filename);
void writeToFile(viktar_header_t person, const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to open file");
        exit(1);
    }

    // Write the data directly in binary format
    if (write(fd, &person, sizeof(struct Person)) == -1) {
        perror("Failed to write to file");
        close(fd);
        exit(1);
    }

    close(fd);
}
int main(int argc, char **argv)
{
    int opt;
	char str[1000];
/*
    char input[100];
    bool doEncrypt = true;
    int shift = 3;
    long shiftedResult;
    int base;
    char *endptr;
*/
	viktar_header_t test_data = {"Alice", 25, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, 2, 2, {3, 3}, {4,4}};

    // Write the structure to the file
    writeToFile(test_data, "person.dat");

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
		printf("create");
            break;
/*Short table of contents.
• Your short table of contents must match exactly the one
produced by my implementation of viktar.
• If the -f option is not used, read the archive file from stdin.
• Look at the output of my solution for the format of he output.
*/
        case 't':
		
            {
                perror("strtol");
                exit(EXIT_FAILURE);
            }

            break;
/*Long table of contents.
• Your long table of contents must match exactly the one
produced by my implementation of viktar.
• If the -f option is not used, read the archive file from stdin.
• Look at the output of my solution for the format of he output.
*/
	case 'T':
		printf("long table of contents");
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
//printf("specify filename");
                  strcpy(str, optarg);
		read_viktar(str);
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
		printf("Validate the content");
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
		printf("verbose processing");
		break;
        default:
            abort();
        }
    }

	return EXIT_SUCCESS;
}





/*
    if (shift < 0 || shift > 96)
    {
        perror("Shift value must be between 0 and 96");
        exit(EXIT_FAILURE);
    }
    while (fgets(input, sizeof(input), stdin) != NULL)
    {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
        {
            input[len - 1] = '\0';
        }

        // Output the stored string

        for (int i = 0; input[i] != '\0'; i++)
        {
            int shiftModifier;
            if (doEncrypt)
            {
                shiftModifier = 1;
            }
            else
            {
                shiftModifier = -1;
            }
            if (isprint(input[i]))
            {
                shiftedResult = input[i] + (shift * shiftModifier);
                if (shiftedResult > 126)
                {
                    shiftedResult -= 95;
                }
                else if (shiftedResult < 32)
                {
                    shiftedResult += 95;
                }
                input[i] = shiftedResult;

                printf("%c", input[i]);
            }
        }
        printf("\n");
    }

    return 0;
}

*/

void display_help(void) {
    printf("Usage: ./viktar [OPTIONS]\n");
    printf("Options: xctTf:Vhv\n");
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



void read_viktar(char* filename) {
	int iarch = STDIN_FILENO;
	printf("file descriptor: %d", iarch);
	//char buf[100] = {'\0'};
	printf("%s", filename);

	if (filename != NULL ) {
		iarch = open(filename, O_RDONLY);
	}
	printf("file descriptor: %d", iarch);

}

void toc(void) {

	char *filename = NULL;
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

	// process the archive file metadata
	while (read(iarch, &md, sizeof(viktar_header_t )) > 0) {
		// print archive member name
	}
	printf("Contents of viktar file: \"%s\"\n", filename != NULL ? filename : "stdin");
	memset(buf, 0, 100);
	strncpy(buf, md.viktar_name, VIKTAR_MAX_FILE_NAME_LEN);
	printf("\tfile name: %s\n", buf);


	// process the archive file metadata
	printf("Contents of viktar file: \"%s\"\n", filename != NULL ? filename : "stdin");
	while (read(iarch, &md, sizeof(viktar_header_t )) > 0) {
	// print archive member name
		memset(buf, 0, 100);
		strncpy(buf, md.viktar_name, VIKTAR_MAX_FILE_NAME_LEN);
		printf("\tfile name: %s\n", buf);
	}
	lseek(iarch, md.st_size + sizeof(viktar_footer_t), SEEK_CUR);

		if (filename != NULL ) {
		close(iarch);
		}

}
