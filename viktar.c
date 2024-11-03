
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
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
void createArchive(char * filename);
void writeToFile(viktar_header_t person, const char *filename);
void readFile(const char *filename);
void extractFile(const char *filename);
void strmode(mode_t mode, char * buf);
void compute_md5(char * filename, uint8_t * digest);
void compute_data_md5(viktar_header_t *header, unsigned char *output);


int main(int argc, char **argv)
{
    int opt;
	viktar_action_t action;
	//char str[1000] = {'\0'};
	char archiveFilename[1000] = {'\0'};
    const size_t MAX_ARG_LENGTH = 1000;

/*
    char input[100];
    bool doEncrypt = true;
    int shift = 3;
    
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
		action = ACTION_EXTRACT;
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
                  strcpy(archiveFilename, optarg);
		//read_viktar(str);
              }
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

	if (optind < argc) {
	int optCount = 0;
	    char remaining_args[argc-optind][MAX_ARG_LENGTH];

        fprintf(stderr, "\nThis is what remains on the command line:\n%x", optCount);

        for(int j = optind; j < argc; j++) {
            // If you want to do something with these values on the command line
            //   you'll need to use something other than getopt().
	strncpy(remaining_args[optCount],argv[j],MAX_ARG_LENGTH - 1);
	remaining_args[optCount][MAX_ARG_LENGTH - 1] = '\0';
            printf("\t%s\n", remaining_args[optCount]);
optCount++;
        }
    }

    switch(action) {
        case ACTION_NONE:
            printf("No action selected.\n");
            break;
        case ACTION_CREATE:
		createArchive(archiveFilename);
            break;
        case ACTION_EXTRACT:
		extractFile(archiveFilename);
            break;
        case ACTION_TOC_SHORT:
		toc(archiveFilename);
            break;
        case ACTION_TOC_LONG:
		showLongToC(archiveFilename);
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
		exit(1);
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
		exit(1);
	}
	printf("Contents of viktar file: \"%s\"\n", filename != NULL ? filename : "stdin");

	// process the archive file metadata
	while (read(iarch, &md, sizeof(viktar_header_t )) > 0) {
		struct passwd *pw; 	// print archive member name
		struct group *grp; 	// print archive member name
		char modeBuf[11] = {'\0'};
		mode_t fileMode = md.st_mode; //& 0777;
		strmode(fileMode, modeBuf);
		//struct timespec ts;
		memset(buf, 0, 100);
		strncpy(buf, md.viktar_name, VIKTAR_MAX_FILE_NAME_LEN);
		printf("\tfile name: %s\n", buf);
    		printf("\t\tmode:\t\t%s\n", modeBuf);
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




void strmode(mode_t mode, char * buf) {
  const char chars[] = "-rwxrwxrwx";
  for (size_t i = 0; i < 10; i++) {
    buf[i] = (mode & (1 << (9-i))) ? chars[i] : '-';
  }
  buf[10] = '\0';
}


void compute_md5(char * filename, uint8_t * digest) {



int ifd = 0;
    unsigned char buffer[1000] = {'\0'};
    ssize_t bytes_read = 0;
    MD5_CTX context;

        ifd = open(filename, O_RDONLY);
        if (ifd < 0) {
            perror("cannot open file");
			fprintf(stderr, "Unable to open input file %s\n", "1-tasdf");
            exit(EXIT_FAILURE);
        }
        
        MD5Init(&context);
        for( ; (bytes_read = read(ifd, buffer, 1000)) > 0; ) {
            MD5Update(&context, buffer, bytes_read);
        }
        MD5Final(digest, &context);
        close(ifd);
/*
        for(int j = 0; j < MD5_DIGEST_LENGTH; ++j) {
            printf("%02x", digest[j]);
        }
        printf("\n");
 */   
    return;
}

/*


void compute_data_md5(const viktar_header_t * filename, uint8_t * digest) {
    unsigned char buffer[5000] = {'\0'};
    MD5_CTX context;
   	//printf("compute_data: %s", filename.viktar_name); 
	
memcpy(buffer, filename, sizeof(viktar_header_t));  // Serialize the struct data

        MD5Init(&context);
        MD5Update(&context, (const uint8_t*)filename, sizeof(viktar_header_t));
        MD5Final(digest, &context);
    return;
}

*/

void compute_data_md5(viktar_header_t *header, unsigned char *buffer) {
//unsigned char *ptr = buffer;
    MD5_CTX md5Context;
    MD5Init(&md5Context);
	MD5Update(&md5Context, (const uint8_t *)header, sizeof(viktar_header_t));

    // Finalize the MD5 hash
    MD5Final(buffer, &md5Context);
}




void createArchive(char * archiveFilename) {
    //unsigned char md5_output[MD5_DIGEST_LENGTH];
viktar_header_t header;


	int oarch;
	//int iarch = STDIN_FILENO;
          struct stat sb;
	viktar_footer_t footer_data;
	//char buf[100] = {'\0'};
	//viktar_header_t md;
 //unsigned char md5_result[MD5_DIGEST_LENGTH];	
    char *filenames[] = {
        "3-s.txt",
        "4-s.txt",
        "5-s.txt",
        "6-s.txt"
    };

printf("just checking");
printf("%s", archiveFilename);

	oarch = open(archiveFilename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (oarch == -1) {
        perror("Error opening file");
	oarch = STDOUT_FILENO;
    }
    	if (write(oarch, VIKTAR_TAG, sizeof(VIKTAR_TAG)-1) == -1) {
       		perror("Failed to write to file");
        	close(oarch);
        	exit(1);
    	}

	for (int i = 0; i < 4; i++) {
    	uint8_t digest[MD5_DIGEST_LENGTH];
    /*char readBuffer[header.st_size]; // Buffer to hold the read data
ssize_t bytesRead;

    // Open the file for reading
   int fd =open(filenames[i], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
    }
        printf("Filename %d: %s\n", i + 1, filenames[i]);
    bytesRead = read(fd, readBuffer, header.st_size);
	printf("read: %lx", bytesRead);
    if (bytesRead == -1) {
        perror("Error reading file");
        close(fd);
    }
	
    readBuffer[bytesRead-1] = '\0';

    // Print the read content
    printf("Read from file: %s", readBuffer);
*/	
           if (lstat(filenames[i], &sb) == -1) {
               perror("lstat");
               exit(EXIT_FAILURE);
           }

	
	//printf("I-node number:            %ju\n", (uintmax_t) sb.st_ino);

strncpy(header.viktar_name, filenames[i], VIKTAR_MAX_FILE_NAME_LEN);
header.viktar_name[sizeof(header.viktar_name) - 1] = '\0'; // Ensure null termination

header.st_size = sb.st_size;
header.st_mode = sb.st_mode;
header.st_uid = sb.st_uid;
header.st_gid = sb.st_gid;
header.st_atim = sb.st_atim;
header.st_mtim = sb.st_mtim;

    	if (write(oarch, &header, sizeof(viktar_header_t)-1) == -1) {
       		perror("Failed to write to file");
        	close(oarch);
        	exit(1);
    	}
/*
    // Read the exact number of bytes from the file

        perror("Error reading file");
    // Null-terminate the string
*/


















	compute_data_md5(&header,digest);
	memcpy(footer_data.md5sum_header, digest, sizeof(footer_data.md5sum_header)); // Copy the entire array
	
	compute_md5(filenames[i], digest);
	memcpy(footer_data.md5sum_data, digest, sizeof(footer_data.md5sum_data)); // Copy the entire array

    	if (write(oarch, &footer_data, sizeof(viktar_footer_t)-1) == -1) {
       		perror("Failed to write to file");
        	close(oarch);
        	exit(1);
    	}
/*
    printf("MD5 hash header: ");
    for (int j = 0; j < MD5_DIGEST_LENGTH; j++) {
        printf("%x", digest[j]);
    }
    printf("MD5 hash data: ");
    for (int j = 0; j < MD5_DIGEST_LENGTH; j++) {
        printf("%02x", footer_data.md5sum_data[j]);
    }
    printf("\n");
           printf("Mode:                     %jo (octal)\n",
                  (uintmax_t) test_data.st_mode);

           printf("Ownership:                UID=%ju   GID=%ju\n",
                  (uintmax_t) test_data.st_uid, (uintmax_t) test_data.st_gid);

           printf("File size:                %jd bytes\n",
                  (intmax_t) test_data.st_size);
           printf("Blocks allocated:         %jd\n",
                  (intmax_t) sb.st_blocks);

           printf("Last status change:       %s", ctime(&sb.st_ctime));
           printf("Last file access:         %s", ctime(&test_data.st_atime));
           printf("Last file modification:   %s", ctime(&sb.st_mtime));

*/
    }
/*
	if (strlen(archFilename) < 1) {
    		iarch = open(archFilename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		perror("no filename given");
	}
*/
	

/*
*/

    close(oarch);


           exit(EXIT_SUCCESS);
       }



















	
/*	
    if (write(iarch, &VIKTAR_TAG, sizeof(char)*strlen(VIKTAR_TAG)) == -1) {
        perror("Failed to write to file");
        close(iarch);
        exit(1);
    }
}
*/










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



void extractFile(const char *filename) {
	char test[strlen(filename)];	
    //int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd = open(filename, O_RDONLY);
printf("filename: %lu", strlen(filename));
    if (fd == -1) {
        perror("Failed to open file");
        exit(1);
    }

    // Write the data directly in binary format
    if (read(fd, test, strlen(VIKTAR_TAG) == -1)) {
	perror("bad viktar file");
        close(fd);
        exit(1);
    }

    close(fd);

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
