#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "address_book_fops.h"
#include "address_book_log.h"
#include "address_book_menu.h"
#include "address_book.h"

int get_option(int type, const char *msg)
{
	printf("%s",msg);
	if(type == NUM){
		int option;
		scanf("%d", &option);
		return option;
	}else if(type == CHAR){
		char option;
		scanf("%c", &option);
		return (int)option;
	}else{
		return 0;
	}
}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		//main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	//system("cls");

	printf("#######  Address Book  #######\n");
	if (str != '\0')
	{
		printf("#######  %s\n", str);
	}
}

void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	int option = 1;
	int con = 1;

	while(option != 0){
		main_menu();

		option = get_option(NUM, "");

		if ((address_book->count == 0) && (option != e_add_contact))
		{
			printf("No entries found!!. Would you like to add? Use Add Contacts\n");

			continue;
		}

		switch (option){
			case e_add_contact:
				add_contacts(address_book);
				break;
			case e_search_contact:
				search_contact(address_book);
				break;
			case e_edit_contact:
				edit_contact(address_book);
				break;
			case e_delete_contact:
				delete_contact(address_book);
				break;
			case e_list_contacts:
				list_contacts(address_book);
				break;
			case e_save:
				save_file(address_book);
				break;
			case 0:
				con = 0;
				break;
		}	
	}

	return e_success;
}

/*Add Contacts*/
//Reallocs memory and increases size by one
//Works but needs change to fit io
Status add_contacts(AddressBook *address_book)
{
	ContactInfo* newList = realloc(address_book->list, sizeof(ContactInfo) * (address_book->count + 1));
	address_book->list = newList;
	address_book->count = address_book->count + 1;

	char arr[32];
	printf("Enter Name: ");
	scanf("%s", arr);
	
	address_book->list[address_book->count - 1].si_no = (address_book->count - 1);
	strcpy(address_book->list[address_book->count - 1].name[0], arr);

	for(int i = 0; i < 5; i++){
		printf("Enter Phone Number %d: ", i + 1);
		scanf("%s", arr);
		strcpy(address_book->list[address_book->count - 1].phone_numbers[i], arr);
	}

	for(int i = 0; i < 5; i++){
		printf("Enter Email Number %d: ", i + 1);
		scanf("%s", arr);
		strcpy(address_book->list[address_book->count - 1].email_addresses[i], arr);
	}

	printf("Successfully added new contact!\n");

	return e_success;
}

/**/
//
Status search_contact(AddressBook *address_book)
{
	int option = 0;
	printf("Enter What Your Searching With: ");
	scanf("%d", option);

	char arr[32];
	int pos = 0;
	if(option == e_name){
		printf("Enter Name: ");
		scanf("%s", arr);
		
		pos = search(arr, address_book, e_name);
	}else if(option == e_phone){
		printf("Enter Phone #: ");
		scanf("%s", arr);
		
		pos = search(arr, address_book, e_phone);
	}else if(option == e_email){
		printf("Enter Email: ");
		scanf("%s", arr);
		
		pos = search(arr, address_book, e_email);
	}

	if(pos == e_fail){
		printf("Not Found\n");
	}else{
		//print
	}
	return e_success;
}

/**/
Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
	return e_success;
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
	return e_success;
}

Status list_contacts(AddressBook *address_book)//, const char *title, int *index, const char *msg, Modes mode
{
	printf("=================================================================================================================\n");
	printf(": S.No : %-32s : %-32s : %-32s :\n", " Name", " Phone No", "Email ID");
	for(int i = 0; i < address_book->count; i++){
		printf("=================================================================================================================\n");
		printf(": %d    : %-32s : ", address_book->list[i].si_no + 1, address_book->list[i].name);
		for(int j = 0; j < 5; j++){
			if(j == 0){
				printf("%-32s : %-32s :\n", address_book->list[i].phone_numbers[j], address_book->list[i].email_addresses[j]);
			}else{
				printf(":      : %-32s : %-32s : %-32s :\n", " ", address_book->list[i].phone_numbers[j], address_book->list[i].email_addresses[j]);
			}
		}
	}
	printf("=================================================================================================================\n");

	return e_success;
}

int search(const char *str, AddressBook *address_book, Searchtype type)
{
	if(type == e_name){
		for(int i = 0; i < address_book->count; i++){
			if(strcmp(address_book->list[i].name[0], str) == 0){
				return i;
			}
		}
	}else if(type == e_phone){
		for(int i = 0; i < address_book->count; i++){
			for(int j = 0; j < 5; j++){
				if(strcmp(address_book->list[i].phone_numbers[j], str) == 0){
					return i;
				}
			}
		}
	}else if(type == e_email){
		for(int i = 0; i < address_book->count; i++){
			for(int j = 0; j < 5; j++){
				if(strcmp(address_book->list[i].email_addresses[j], str) == 0){
					return i;
				}
			}
		}
	}

	return e_fail;
}