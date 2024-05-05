#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coins.h"

int main(int argc, char *argv[]) {
	FILE *file;
    int Nsize = 0;7
    records *record = 0;	
	int menuinp; 
	int cent[4] = {0};
	
    if (CheckArgument(argc)){
		file=openinputfile(argv,file);	
	}
	
	scanfile(file,&record,&Nsize);
	
	//check stored values
	for (int i = 0; i < Nsize; ++i) {
	printf("%s %d %s\n", record[i].name, record[i].amount, record[i].currency);}
    
	fclose(file);
	int remainder = record->amount;
	/**menu
	input 1 searches name and displays change 
	input 2 exits loop and exports data to file "change.csv"**/
    do {
        printf("\n1. Enter name\n2. Exit\nEnter option (1/2):\n");
        scanf("%d", &menuinp);	
        menuchoice(menuinp,record, Nsize, cent, remainder);
    } while (menuinp == 1);
			
    free(record);
    return (0);
}