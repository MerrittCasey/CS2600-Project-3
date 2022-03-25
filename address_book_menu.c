#include <stdio.h>
//#include <stdio_ext.h> isnt included in my c libraries idk lmao I think its only used for e_exit which is just 0 i think
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "address_book_fops.h"
#include "address_book_log.h"
#include "address_book_menu.h"
#include "address_book.h"

/*Gets User Input Based on passed type*/
//probably needs some error checking
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

/*Called Every Time */
Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		//main_menu(); this was included in the skeleton code idk why tho we'll have to figure it out later

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

/*Menu Header*/
void menu_header(const char *str)
{
	fflush(stdout);

	//system("cls"); this literally just constantly clears the console so no other prints shows up weird idk why it was included in the skeleton

	printf("#######  Address Book  #######\n");
	if (str != '\0')
	{
		printf("#######  %s\n", str);
	}
}

/*Main Menu Prints*/
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

/*Main Menu Logic*/
//I changed it from a do while to just a while, I hate do whiles
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
				list_contacts(address_book,"",0,"",e_list_contacts);
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
//Works but needs change to fit IO
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

/*Searchs Contacts I believe this is supposed to call the other search function*/
//doesn't work atm tho lmao
Status search_contact(AddressBook *address_book)
{
	int option = 0;
	printf("Enter What Your Searching With: ");
	scanf("%d", option);

	char arr[32];
	int pos = 0;
	if(option == 0){
		printf("Enter Name: ");
		scanf("%s", arr);
		
		pos = search(arr, address_book, 0,0,"",e_search);
	}else if(option == 1){
		printf("Enter Phone #: ");
		scanf("%s", arr);
		
		pos = search(arr, address_book, 0,1,"",e_search);
	}else if(option == 2){
		printf("Enter Email: ");
		scanf("%s", arr);
		
		pos = search(arr, address_book, 0,2,"",e_search);
	}

	if(pos == e_fail){
		printf("Not Found\n");
	}else{
		printf("%d\n", pos);
	}
	return e_success;
}

/*Other Search Function*/
//Search based on search type and return position in array
int search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	if(field == 0){
		for(int i = 0; i < address_book->count; i++){
			if(strcmp(address_book->list[i].name[0], str) == 0){
				return i;
			}
		}
	}else if(field == 0){
		for(int i = 0; i < address_book->count; i++){
			for(int j = 0; j < 5; j++){
				if(strcmp(address_book->list[i].phone_numbers[j], str) == 0){
					return i;
				}
			}
		}
	}else if(field == 0){
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

/*Edits Contacts*/
//Search contacts find the correct one then edit the info based on user input
Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
	return e_success;
}

/*Delete Contact*/
//search by name and get position to delete
//Reallocate memory to a new array that is smaller
//copy from original array to new array skipping over position to delete
//free memory from original array
Status delete_contact(AddressBook *address_book)
{
	return e_success;
}

/*List Contacts*/
//Right now this just lists all the contacts in the book.
//I think its supposed to be called for anytime we list stuff like, edit and search and shit.
Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	printf("=================================================================================================================\n");
	printf(": S.No : %-32s : %-32s : %-32s :\n", " Name", " Phone No", "Email ID");
	for(int i = 0; i < address_book->count; i++){
		printf("=================================================================================================================\n");
		printf(": %d    : %-32s : ", address_book->list[i].si_no, address_book->list[i].name);
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