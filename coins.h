#ifndef COINS_H
#define COINS_H

#define MAX_SIZE 128   

typedef struct {
    char name[MAX_SIZE];
    int amount;
    char currency[5];
} records;

int CheckArgument(int argc);
FILE *openinputfile (char *argv[], FILE *file);
int scanfile(FILE *file, records **record, int *Nsize);
int storeRecord(records **record, int *acsize, char *name, int amount, char *currency, int lineNumber);
int validateinput(char *name, int amount, char *currency, int lineNumber);
void menuchoice(int menuinp,records *record, int Nsize, int cent[], int remainder);
void displaymenuchange(records *record,int Nsize,int cent[],int remainder);
void writefile(const records *record, int Nsize,int cent[], int remainder);

#endif // COINS_H