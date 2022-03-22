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

Status add_contacts(AddressBook *address_book)
{
	/* Add the functionality for adding contacts here */
	return e_success;
}

// Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
// {
// 	/* Add the functionality for adding contacts here */
// }

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
	return e_success;
}

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
	printf("=========================================================================\n");
	printf(": S.No : Name \t\t: Phone No \t\t: Email ID \t\t:\n");
	for(int i = 0; i < address_book->count; i++){
		printf("=========================================================================\n");
		printf(": %d\t: %s \t\t: ", address_book->list[i].si_no + 1, address_book->list[i].name);

		for(int j = 0; i < 5; j++){
			if(j == 0){
				printf("%s\t\t: %s\t\t:\n", address_book->list[i].phone_numbers[j], address_book->list[i].email_addresses[j]);
			}else{
				printf(":\t: \t\t: %s\t\t: %s\t\t:\n", address_book->list[i].phone_numbers[j], address_book->list[i].email_addresses[j]);
			}
		}

		printf("=========================================================================\n");
	}

	printf("=========================================================================\n");
	printf("\n");

	return e_success;
}
