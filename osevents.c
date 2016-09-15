/*
   CITS2002 Project 1 2016
   Name(s):     Louis White, Ben Robinson
   Student number(s):   21500209, (student-number2)
   Date:        date-of-submission
*/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define BUFFSIZE    256

// Create structure to store the information on each line
typedef struct LineInfo {
    int time;
    int processno;
    char action;
    int sectorreq;
} LineInfo;

double sectorspermicrosec(int diskrpm, int disksectors) {

    // Calculate the number of disk sectors are accessed per microsecond
    return (diskrpm / 60)*0.000001*disksectors;

}    

struct LineInfo getinfofromline(char line[], char delimiter) {
    
    // Declare variables
    int time;
    int processno;
    char *state;
    int sectorreq;
        
    char *token;
    const char d = ' ';
    
    // Seperate each part of the string at the delimiter and save into it's respective field
    token = strtok(line, &d);
    time = atoi(token);
    token = strtok(NULL, &d);
    processno = atoi(token);
    token = strtok(NULL, &d);
    state = token;
    token = strtok(NULL, &d);
    if(token == NULL) {           // Check if token is null, this will be the case if the action were to be a read or write operation
        sectorreq = -1;           // Set sector required to number that does not make sense
    }
    else {
        sectorreq = atoi(token);
    }
    
    // Add variables to the structure created to hold these variables
    LineInfo info = { .time = time, .processno = processno, .action = state[0], .sectorreq = sectorreq };
    return info;
}

void processfile(char scenario[]) {
    
    FILE *fp;
    char line[BUFFSIZE];
    
    char *token;
    const char d = ' ';
    
    int countlines = 0;
    
    int timequantum = 0;
    int diskrpm = 0;
    int disksectors = 0;
    
    LineInfo info;
        
    // Open the file for read-access
    fp = fopen(scenario, "r");
    
    // Check if anything went wrong
    if(fp == NULL) {
        printf("Cannot open file\n");
        exit(EXIT_FAILURE);
    }
    
    while( fgets(line, sizeof line, fp) != NULL) {
        
        // Check if anything went wrong
        if(line == NULL) {
            printf("Line does not contain anything\n");
            exit(EXIT_FAILURE);
        }
        else {
            if(countlines == 0) {           // Read line 1 and retrieve the value for time quantum
                token = strtok(line, &d);
                token = strtok(NULL, &d);   // Split the line and get the 2nd string as token
                timequantum = atoi(token);
                printf("Time Quantum: %i\n", timequantum);      // Debugging print statement
            }
            else if(countlines == 1) {      // Read line 2 and retrieve the value for disk rpm
                token = strtok(line, &d);
                token = strtok(NULL, &d);
                diskrpm = atoi(token);
                printf("Disk RPM: %i\n", diskrpm);      // Debugging print statement
            }
            else if(countlines == 2) {      // Read line 3 and retrieve the value for disk sectors
                token = strtok(line, &d);
                token = strtok(NULL, &d);
                disksectors = atoi(token);
                printf("Disk Sectors: %i\n", disksectors);      // Debugging print statement
                printf("\n");                                   // Debugging print statement
            }
            else {
                    // This will be filled in when we figure out how to actually process the lines and do stuff with them
                    info = getinfofromline(line, d);
                    
                    // Debugging print statements
                    printf("Time taken: %i\n", info.time);
                    printf("Process number: %i\n", info.processno);
                    printf("Operation: %c\n", info.action);                     // a = admit, r = read, w = write, e = exit
                    printf("Disk sector required: %i\n", info.sectorreq);       // Currently prints 1000 if no sector required
                    printf("\n");
                    
            }
            countlines++;
        }

    }
     
    double sectorsper = sectorspermicrosec(diskrpm, disksectors);
    printf("Sectors per microsecond: %f\n", sectorsper);
    
    fclose(fp);
}

int main(int argc, char *argv[]) {
    
    // Check the number of inputs the program has
    if(argc != 2) {
        fprintf(stderr, "Usage: %s scenariofile\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else {
        processfile(argv[1]);
        exit(EXIT_SUCCESS);
    }

}
