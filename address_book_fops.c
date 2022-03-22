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
	/* 
	 * Check for file existance
	 */
	int ret = access(DEFAULT_FILE, F_OK);

	if (ret == 0)
	{
		address_book->fp = fopen(DEFAULT_FILE, "r");

		int size = 0;
		char line[356];

		while(fgets(line, sizeof(line), address_book->fp)){
			size++;
		}

		ContactInfo list[size];
		int count = 0;

		fclose(address_book->fp);
		address_book->fp = fopen(DEFAULT_FILE, "r");
		
		while(fgets(line, sizeof(line), address_book->fp)){
			int tracker = 0;
			char *token;
			token = strtok(line, ", ");

			while(token){
				if(tracker == 0){
					strcpy(list[count].name[tracker], token);
					//tracker++;
				}else if(tracker > 0 && tracker <= 5){
					strcpy(list[count].phone_numbers[tracker - 1], token);
					//tracker++;
				}else if(tracker > 5 && tracker <= 10){
					strcpy(list[count].email_addresses[tracker - 6], token);
					//tracker++;
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
		address_book->fp = fopen(DEFAULT_FILE, "w+");
	}

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
		//address_book->fp = fopen(DEFAULT_FILE, "w");
	}

	//fclose(address_book->fp);

	return e_success;
}
