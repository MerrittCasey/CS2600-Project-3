#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif
//this is for windows ^^

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h> //uncomment if ur on linux
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

Status load_file(AddressBook *address_book)
{
	int ret = access(DEFAULT_FILE, F_OK); // returns 0 if file exists

	if (ret == 0)//file exists
	{
		address_book->fp = fopen(DEFAULT_FILE, "r");
		
		/*Getting Size*/
		int size = 0;
		char line[356];

		while(fgets(line, sizeof(line), address_book->fp)){
			size++;
		}

		fclose(address_book->fp);//resetting back to start of file
		address_book->fp = fopen(DEFAULT_FILE, "r");

		ContactInfo* list = malloc(sizeof(ContactInfo) * size); //creating list with size that we got above
		int count = 0;
		
		while(fgets(line, sizeof(line), address_book->fp)){
			int tracker = 0;
			char *token;
			token = strtok(line, ", ");

			while(token){
				if(tracker == 0){
					strcpy(list[count].name[tracker], token);
				}else if(tracker > 0 && tracker <= 5){
					strcpy(list[count].phone_numbers[tracker - 1], token);
				}else if(tracker > 5 && tracker <= 10){
					strcpy(list[count].email_addresses[tracker - 6], token);
				}
				
				token = strtok(NULL, ", ");
				tracker++;
			}

			list[count].si_no = count;
			
			count++;
		}

		address_book->count = count;
		address_book->list = list;
	}
	else
	{
		address_book->fp = fopen(DEFAULT_FILE, "w+");//creating file if it doesnt exist.
	}

	fclose(address_book->fp);//yeet gotta close that bisch
	return e_success;
}

Status save_file(AddressBook *address_book)
{
	/*
	 * Write contacts back to file.
	 * Re write the complete file currently
	 */ 
	if (address_book->fp == NULL)
	{
		return e_fail;
	}else{
		address_book->fp = fopen(DEFAULT_FILE, "w");

		for(int i = 0; i < address_book->count; i++){
			fprintf(address_book->fp, "%s,", address_book->list[i].name[0]);
			for(int j = 0; j < 5; j++){
				fprintf(address_book->fp, " %s,", address_book->list[i].phone_numbers[j]);
			}
			for(int j = 0; j < 5; j++){
				fprintf(address_book->fp, " %s,\n", address_book->list[i].email_addresses[j]);
			}
		}
	}

	fclose(address_book->fp);
	free(address_book->list);
	free(address_book);

	return e_success;
}
