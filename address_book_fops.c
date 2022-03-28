#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

/*Load File*/
//Works very unreadable atm tho lol
Status load_file(AddressBook *address_book)
{
	int ret = -1;
	address_book->fp = fopen(DEFAULT_FILE, "r");
	if(address_book->fp == NULL){
		fclose(address_book->fp);
		ret = 1;
	}else{
		ret = 0;
	}

	if (ret == 0)//file exists
	{
		/*Getting Size*/
		int size = 0;
		char line[356];

		while(fgets(line, sizeof(line), address_book->fp)){
			size++;
		}

		fclose(address_book->fp);//resetting back to start of file
		address_book->fp = fopen(DEFAULT_FILE, "r");

		ContactInfo* list = malloc(sizeof(ContactInfo) * size); //creating list with size that we got above
		int count = 0;//outer loop this is used to go through list[]
		
		while(fgets(line, sizeof(line), address_book->fp)){
			int tracker = 0;//inner loop used to traverse phone_numbers[] and email_address[]
			char *token;
			token = strtok(line, ", ");

			while(token){
				if(tracker == 0){
					strcpy(list[count].name[tracker], token);//copying into array
				}else if(tracker > 0 && tracker <= 5){
					strcpy(list[count].phone_numbers[tracker - 1], token);//copying into array
				}else if(tracker > 5 && tracker <= 10){
					strcpy(list[count].email_addresses[tracker - 6], token);//copying into array
				}
				
				token = strtok(NULL, ", ");//iterating through token
				tracker++;
			}

			list[count].si_no = count + 1;
			
			count++;
		}

		address_book->count = count;
		address_book->list = list;
	}
	else
	{
		printf("File Not File Creating File\n");
		address_book->fp = fopen(DEFAULT_FILE, "w+");
		address_book->count = 0;//creating file if it doesnt exist.
	}

	fclose(address_book->fp);//yeet gotta close that bisch
	return e_success;
}

/*Saves Contents of the array to the file*/
//Untested but should work
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
			for(int j = 0; j < 10; j++){
				if(j < 5){
					fprintf(address_book->fp, " %s,", address_book->list[i].phone_numbers[j]);
				}else if(j >= 5 && j < 10){
					fprintf(address_book->fp, " %s,", address_book->list[i].email_addresses[j - 5]);
				}
			}
			if(i != address_book->count - 1){
				fprintf(address_book->fp, "\n");//making sure we dont get an extra line in the csv file
			}
		}
	}

	fclose(address_book->fp);
	free(address_book->list);

	return e_success;
}