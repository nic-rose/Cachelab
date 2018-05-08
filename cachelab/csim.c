#define _GNU_SOURCE

#include "cachelab.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

//Nicolas Rose and Chris Maxel

typedef struct line{
	char vaild;
	long tag;
} Line;

/*
Line[E] set;
set[pow(int 2, int s)] cache;
structPointer = (Line *)malloc(E*sizeof(Line));
*/

//void getStuffInArray(){	
//}

int readFromCMD(int argc, char * arg[]){ 
/* Reads input from the Command line. (eg. Gets arguments(4,1,4,"traces/yi.trace") 
	from ./csim -v -s 4 -E 1 -b 4 -t traces/yi.trace) */

	
	/*int numSets;  // The commented variables are initalized in cachelab.h
	int numLines;
	int bitSize;
	char* fileName;
	*/
	int i;
	verbose = 0;

	for(i = 1; i < argc; i++){		
		switch(arg[i][1]){
			case 'h':
				printf("usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file> \n");
				printf("Options: \n" );
				printf("-h         Print this help message. \n");
				printf("-v         Optional verbose flag. \n");
				printf("-s <num>   Number of set index bits. \n");
				printf("-E <num>   Number of lines per set. \n");
				printf("-b <num>   Number of block offset bits. \n");
				printf("-t <file>  Trace file. \n");
				printf(" \n");
				printf("Examples: \n");
				printf("linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace \n");
				printf("linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace \n");
				break;

			case 'v':
				verbose = 1;
				break;

			case 's':
				if(i+1 == argc) {return 1;}
				numSets = 1 << atoi(arg[i+1]);
				i++; break;

			case 'E':
				if(i+1 == argc) {return 1;}
				numLines = atoi(arg[i+1]);
				i++;  break;

			case 'b':
				if(i+1 == argc) {return 1;}
				bitSize = 1 << atoi(arg[i+1]);
				i++; break;

			case 't':
				if(i+1 == argc) {return 1;}
				fileName = arg[i+1];
				i++; break;

			default:
				printf("invalid flag: %s\n",arg[i]);
				return 1;
		}
	}
	if(verbose == 1){	
		if(numSets == 0) {printf("s is not set :");}
		 else {printf("\nnumber of sets  : %d\n",numSets);} 

		if(numLines == 0) {printf("E is not set:");} 
			else {printf("number of lines : %d\n",numLines);}

		if(bitSize == 0) {printf("b is not set:");} 
			else {printf("Size of block   : %d\n",bitSize);}

		if(fileName == NULL) {printf("file name not set");} 
		else {printf("fileName        : %s\n", fileName);}
	}
	return 0;
}

void print_array(char *array, int length){
	printf("\nHere is your array:\n");
    for (int i = 0; i < length; i++){
    	printf("%c ", array[i]);
    }
    printf("\nEnd of your array!\n");
}


void readInFile(int* addressList, char*opperationList){
	/* In this function we are reading the stuff that we want to input into the cache and array em up */

	FILE * fp; // File pointer
	char * line = NULL;
	size_t len = 0;
	ssize_t lengthOfLine; // Length of Each line, May not need this but could be helpful
	int numFileLines = 0; // Number of lines in a file


	fp = fopen(fileName,"r"); // Should put error handling into this
	// printf("heres the location of the file ==> %p\n", fp);


	while((lengthOfLine = getline(&line, &len, fp)) != -1){ // Counts the nubmer of lines in the file
		numFileLines++;
	}
	printf("\nTotal number of files in a line => %d\n",numFileLines);

	addressList[numFileLines];
	opperationList[numFileLines];  //-- int* addressList, char*opperationList --//

	fclose(fp);
	char * line2 = NULL;
	len = 0;
	int chPerLine = 0;
	int counter = 0;
	char * addr;
	fp = fopen(fileName,"r");
	while((lengthOfLine = getline(&line2, &len, fp)) != -1){ // Grabs each line and puts it in an array
		chPerLine = 0;
		//printf("a\n");
		if(line2[0] == ' '){
			addr = line2 + 3;
			strtok(addr, ",");
			char * size = addr + strlen(addr) + 1;
			int addr_l = strtol(addr, NULL, 16);
			int size_l = strtol(size, NULL, 16);
			printf("%d\n", addr_l);
			printf("%d\n", size_l);
			opperationList[counter] = line2[1];
			addressList[counter] = addr_l;
			counter++;
		}
		chPerLine++;
		
		// printf("Length of line : %zu \n", lengthOfLine);
	}

	for(int i = 0; i < 7 ; i++){
		// printf("This is the Address==> %x\n", addressList[i]);
	}

	// print_array(opperationList, 7);
	fclose(fp); // Close file
	/*if (line){
		free(line);
	}*/
	return *addressList, *opperationList;
}



Line* setUpCache(){
	/* This function sets up the cache, or creates the correct amount of spots for */

	int cacheSize = numLines * numSets * bitSize;
	printf("Size of theCache: %d\n", cacheSize);

	Line theCache[numLines*numSets] = (Line*)calloc(numLines * numSets,sizeof(Line));
	printf("\nHow many stucts are in our cache => %d\n", numLines * numSets);

	theCache[0].vaild = 'y';
	printf("\nhaha here is theCache[0].valid => %c\n", theCache[0].vaild);
	return *theCache;
}


int main(int argc, char * arg[]){
	readFromCMD(argc, arg);
	readInFile();
	setUpCache();


    printSummary(0, 0, 0);
    return 0;
}
