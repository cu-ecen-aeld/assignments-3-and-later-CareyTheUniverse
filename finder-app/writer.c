#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>


int main(int argc, char *argv[]) 
{
    openlog(NULL, 0, LOG_USER);
	
    // Check correct number of args
    if (argc != 3) 
    {
	printf("Error: Incorrect number of arguments %d", argc);
        syslog(LOG_ERR, "Error: Incorrect number of arguments %d", argc);
        return 1;
    }

    char *filename = argv[1];
    char *string = argv[2];
    FILE *openfile = fopen(filename, "w");
    
    // Check if file can open
    if (openfile == NULL) 
    {
    	printf("Error: Can't open file %s", filename);
        syslog(LOG_ERR, "Error: Can't open file %s", filename);
        return 1;
    }

    fprintf(openfile, "%s", string);
    fclose(openfile);
    syslog(LOG_DEBUG, "Wrote %s to %s", string, filename);

    return 0;
}
