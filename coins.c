#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coins.h"

//check if the right number of arguments has been inputted
int CheckArgument(int argc) {
	if(argc<=1){
		printf("Too few command line arguments\n");
	}
	else if(argc==2){
		return (argc == 2);
	}
	else if(argc>2){
		printf("Too many command line arguments\n");
	}
}

//opens file and returns statement confirming if opened or not and returns file opened to main
FILE *openinputfile (char *argv[],FILE *file){
	file = fopen(argv[1], "r");
	if(file == NULL){
		perror("file opening unsuccessful");
		exit(EXIT_FAILURE);
	}else{
		printf("File %s is open for reading\n",argv[1]);
		return file;
	}
}

//Checks if input is within program bounds and if not informs user of invalid input 
int validateinput(char *name, int amount, char *currency, int lineNumber) {
	
	if (strcmp(currency, "US$") != 0 && strcmp(currency, "AU$") != 0 && strcmp(currency, "EUR") != 0) {
        printf("\nError: Invalid currency for %s at line %d.\n", name, lineNumber+1); //line 1 of file holds size so linenumber+1 displays the errors actual linenumber
		return 0;	
	}
	else if (amount < 1 || amount > 95) {
        printf("\nError: Invalid amount for %s at line %d. Amount must be between 1 and 95.\n", name, lineNumber+1);
		return 0;
	}
	else if(strcmp(currency,"AU$")==0 && amount%5!=0){
		printf("\nError: Invalid amount for %s AU$ currency must be divisible by 5 at line %d\n",name, lineNumber+1);
		return 0;
	}	
	return 1;
}

//scans input file 
int scanfile(FILE *file, records **record, int *Nsize) {
    char name[MAX_SIZE], currency[10];
    int amount, lineNumber = 0, acsize = 0,errorFound=0;

    fscanf(file, "%d", Nsize); 
    *record = realloc(*record, *Nsize * sizeof(**record));

	while (fscanf(file, "%s %d cents in %s", name, &amount, currency) == 3) {
        lineNumber++;
        if (!validateinput(name, amount, currency, lineNumber)) {
            errorFound = 1; 
            // Continue processing, to find all errors
        } else {
            // If no errors are found, proceed to store the record
            storeRecord(record, &acsize, name, amount, currency, lineNumber);
        }
    }

    if (errorFound) {
        printf("Errors were found in input. Terminating program.\n");
        free(*record); 
        *record = NULL;
        exit(EXIT_FAILURE); // Exit if any errors were found
    }

    *Nsize = acsize; // Update actual size
    return 0;
}

// Function to store records into the struct
int storeRecord(records **record, int *acsize, char *name, int amount, char *currency, int lineNumber) {

    int found = 0;
    for (int i = 0; i < *acsize; i++) {
        if (strcmp((*record)[i].name, name) == 0 && strcmp((*record)[i].currency, currency) == 0) {
            (*record)[i].amount += amount;
            found = 1;
            break;
        }
    }
    if (!found) {
        strncpy((*record)[*acsize].name, name, MAX_SIZE - 1);
        (*record)[*acsize].name[MAX_SIZE - 1] = '\0'; 
        (*record)[*acsize].amount = amount;
        strncpy((*record)[*acsize].currency, currency, 10);
        (*record)[*acsize].currency[10] = '\0'; 
        (*acsize)++;
    }

    return 1;
}	

//processes users menu choice and directs program to either path- display change or export data
void menuchoice(int menuinp,records *record, int Nsize, int cent[], int remainder) {
    if (menuinp==1){
		displaymenuchange(record, Nsize, cent, remainder);
    }
	else if (menuinp==2){
		writefile(record, Nsize, cent, remainder);
	}
	else {
		printf("Invalid option\n");
	}
}

void displaymenuchange(records *record, int Nsize,int cent[],int remainder) {
    char name[MAX_SIZE]; 
	printf("Enter name:\n"); //users name input to be accessed
    scanf("%s", name);

    int found_name = 0; //flag 
    for (int i = 0; i < Nsize; i++) {
        if (strcasecmp(record[i].name, name) == 0) {
			found_name = 1;
			 
            int *cointypes = NULL;
			
            static int US[] = {50, 25, 10, 1};
            static int AU[] = {50, 20, 10, 5};
            static int EUR[] = {20, 10, 5, 1};
			
			int remainder = record[i].amount;
            if (strcmp(record[i].currency, "US$") == 0) {
                cointypes = US;
            } else if (strcmp(record[i].currency, "AU$") == 0) {
                cointypes = AU;
            } else if(strcmp(record[i].currency, "EUR") == 0) {
                cointypes = EUR;
            } 

            for (int j = 0; j < 4; j++) {
                cent[j] = remainder / cointypes[j];
                remainder -= cent[j] * cointypes[j];
            }

            printf("\nCustomer:\n%s %d cents in %s\n", record[i].name, record[i].amount, record[i].currency);
            printf("\nChange:\n");
            for (int j = 0; j < 4; j++) {
                if (cent[j] > 0) {
                    printf("%d cents: %d\n", cointypes[j], cent[j]);
                }
            }
         
        }
    }

    if (!found_name) {
        printf("\nName: %s not found.\n", name);
    }
}

//

void writefile(const records *record, int Nsize,int cent[], int remainder){
	FILE *dstFile;	
	
   dstFile = fopen("change.csv", "w");
		if (dstFile == NULL) {
        perror("File opening failed!");
		return;
		}
	
	for (int i = 0; i < Nsize; i++) {
		
		int *cointypes=NULL;
		static int US[] = {50, 25, 10, 1};
		static int AU[] = {50, 20, 10, 5};
		static int EUR[] = {20, 10, 5, 1};
		
		
		if (strcmp(record[i].currency, "US$") == 0){
			cointypes=US;
		} else if (strcmp(record[i].currency, "AU$") == 0){
			cointypes=AU;
		} else if(strcmp(record[i].currency, "EUR") == 0) {
			cointypes=EUR;
    }
	int remainder = record[i].amount;
	for (int j = 0; j< 4; j++) {
        cent[j] = remainder / cointypes[j];
        remainder -= cent[j] * cointypes[j];
	}
		fprintf(dstFile, "%s, the change for %d cents in %s is", record[i].name, record[i].amount, record[i].currency);
		for (int k = 0; k < 4; k++) {
        fprintf(dstFile, " %d",cent[k]);
			if (k<3) {
				fprintf(dstFile,","); 
			}
		}
		fprintf(dstFile, "\n"); 
		}
	
    fclose(dstFile);
}