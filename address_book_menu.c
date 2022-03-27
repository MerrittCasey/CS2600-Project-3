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

	printf("#######  Address Book  #######\n");
	printf("####### Add Contact: \n");
	printf("\n");

	char arr[32];
	printf("1. Name       : ");
	scanf("%s", arr);
	
	address_book->list[address_book->count - 1].si_no = (address_book->count - 1);
	strcpy(address_book->list[address_book->count - 1].name[0], arr);

	for(int i = 0; i < 3; i++){
		printf("2. Phone No 1 : ", i);
		scanf("%s", arr);
		strcpy(address_book->list[address_book->count - 1].phone_numbers[i], arr);
		i=i+1;
	}

	for(int i = 0; i < 3; i++){
		printf("3. Email ID 1 : ", i);
		scanf("%s", arr);
		strcpy(address_book->list[address_book->count - 1].email_addresses[i], arr);
		i=i+1;
	}

	printf("\nSuccessfully added new contact!\n");


	return e_success;
}

/*Searchs Contacts I believe this is supposed to call the other search function*/
//doesn't work atm tho lmao
Status search_contact(AddressBook *address_book)
{
	int option = 0;

	menu_header("Search Contact By:\n");

	printf("0. Back\n");
	printf("1. Name\n");
	printf("2. Phone Number\n");
	printf("3. Email ID\n");
	printf("4. Serial Number\n");
	printf("\n");
	printf("Please select an option: ");
	scanf("%d", &option);
	getchar(); //prevents infinite loops from invalid inputs

	char arr[32];
	int pos = 0;
	int prevpos = -1;
	int namesFound = 0;
	switch (option)
	{
	case 0:
		return e_back;
		break;
	case 1:
		printf("Enter Name: ");
		scanf("%s", arr);
		getchar();
		for (int i = pos; i < address_book->count; i++)
		{
			pos = search(arr, address_book, i,0,"",e_search);
			//display index where search was found; will be replaced w/ a call to list contact function
			if(pos != e_fail && pos > prevpos){
				printf("found at index %d\n", pos);
				printf("=================================================================================================================\n");
				printf(": S.No : %-32s : %-32s : %-32s :\n", " Name", " Phone No", "Email ID");
				printf("=================================================================================================================\n");
				printf(": %d    : %-32s : ", address_book->list[pos].si_no, address_book->list[pos].name);
				for(int j = 0; j < 5; j++){
					if(j == 0){
						printf("%-32s : %-32s :\n", address_book->list[pos].phone_numbers[j], address_book->list[pos].email_addresses[j]);
					}else{
						printf(":      : %-32s : %-32s : %-32s :\n", " ", address_book->list[pos].phone_numbers[j], address_book->list[pos].email_addresses[j]);
					}
				}
				printf("=================================================================================================================\n");
				namesFound++;
			}
			prevpos = pos;
		}
		break;
	case 2:
		printf("Phone Number: ");
		scanf("%s", arr);
		getchar();
		pos = search(arr, address_book, PHONE_NUMBER_COUNT,1,"",e_search);
		break;
	case 3:
		printf("Enter Email: ");
		scanf("%s", arr);
		getchar();
		pos = search(arr, address_book, EMAIL_ID_COUNT,6,"",e_search);
		break;
	case 4:
		printf("Enter Serial Number: ");
		scanf("%s", arr);
		getchar();
		pos = search(arr, address_book, 1,11,"",e_search);
		break;
	default:
		break;
	}
	
	if (option != 1 || namesFound == 0)
	{
		if(pos == e_fail){
		printf("Not Found\n");
		}else{
		printf("found at index %d\n", pos);
		//display index where search was found
		printf("=================================================================================================================\n");
		printf(": S.No : %-32s : %-32s : %-32s :\n", " Name", " Phone No", "Email ID");
		printf("=================================================================================================================\n");
		printf(": %d    : %-32s : ", address_book->list[pos].si_no, address_book->list[pos].name);
		for(int j = 0; j < 5; j++){
			if(j == 0){
				printf("%-32s : %-32s :\n", address_book->list[pos].phone_numbers[j], address_book->list[pos].email_addresses[j]);
			}else{
				printf(":      : %-32s : %-32s : %-32s :\n", " ", address_book->list[pos].phone_numbers[j], address_book->list[pos].email_addresses[j]);
			}
		}
		printf("=================================================================================================================\n");
		}
	}

	return e_success;
}

/*Other Search Function*/
//Search based on search type and return position in array
int search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	switch (field)
	{
	case 0:
		for(int i = loop_count; i < address_book->count; i++){
			if(strcmp(address_book->list[i].name[0], str) == 0){
				return i;
			}
		}
		break;
	case 1:
		for(int i = 0; i < address_book->count; i++){
			for(int j = 0; j < 5; j++){
				if(strcmp(address_book->list[i].phone_numbers[j], str) == 0){
					return i;
				}
			}
		}
		break;
	case 6:
		for(int i = 0; i < address_book->count; i++){
			for(int j = 0; j < 5; j++){
				if(strcmp(address_book->list[i].email_addresses[j], str) == 0){
					return i;
				}
			}
		}
	break;
	case 11:
		for(int i = 0; i < address_book->count; i++){
			int si_no_input = 0;
			si_no_input = atoi(str); //converts input si_no to integer to compare w/ integer value in table
			if(address_book->list[i].si_no == si_no_input){
				return i;
			}
		}
		break;
	default:
		break;
	}

	return e_fail;
}

void addressEdit(AddressBook *address_book, int pos){
	// edit operations
	int yesOrNo = -1;
	char nameChange[32],phoneChange[32],emailChange[32];

	while(yesOrNo != 1 && yesOrNo != 2){
		printf("Edit the person's name?[1 for Yes, 2 for No]: ");
		scanf("%d",&yesOrNo);
		if(yesOrNo == 1){
			printf("Enter New Name: ");
			scanf("%s", nameChange);
			strcpy(address_book->list[pos].name[0], nameChange);
		}
	}
	yesOrNo = -1;

	while(yesOrNo != 1 && yesOrNo != 2){
		printf("Edit the person's phone number?[1 for Yes, 2 for No]: ");
		scanf("%d",&yesOrNo);
		if(yesOrNo == 1){
			printf("Enter new Phone Number: ");
			scanf("%s", phoneChange);
			strcpy(address_book->list[pos].phone_numbers[0], phoneChange);
		}
	}
	yesOrNo = -1;

	while(yesOrNo != 1 && yesOrNo != 2){
		printf("Edit the person's email address?[1 for Yes, 2 for No]: ");
		scanf("%d",&yesOrNo);
		if(yesOrNo == 1){
			printf("Enter new email address: ");
			scanf("%s", emailChange);
			strcpy(address_book->list[pos].email_addresses[0], emailChange);
		}
	}
			
}

/*Edits Contacts*/
//Search contacts find the correct one then edit the info based on user input
Status edit_contact(AddressBook *address_book)
{
	int option = -1, pos = -1;
	char arr[32];

	while(option < 0 || option > 4){
		printf("\n0. Back to the previous menu\n1. Edit by name.\n");
		printf("2. Edit by phone number.\n3. Edit by email address.\n");
		printf("4. Edit by serial number.\n");
		scanf("%d",&option);
	}


	switch (option){

		case 0:
			return e_back; 
			// back to the previous menus options
			
		case 1:
			// edit by name
			while(pos<0){
				printf("Enter Name: ");
				scanf("%s", arr);
				pos = search(arr, address_book, NAME_COUNT,0,"",e_search);
				if(pos < 0)
					printf("Name not found! Please try another name!\n");
			}

			addressEdit(address_book,pos);
			break;
			
		case 2:
			// edit by phone number
			while(pos<0){
				printf("Enter Phone Number: ");
				scanf("%s", arr);
				pos = search(arr, address_book, PHONE_NUMBER_COUNT,1,"",e_search);
				if(pos < 0)
					printf("Phone Number not found! Please try another number!\n");
			}

			addressEdit(address_book,pos);

			break;

		case 3:
			// edit by email
			while(pos<0){
				printf("Enter Email Address: ");
				scanf("%s", arr);
				pos = search(arr, address_book, EMAIL_ID_COUNT,6,"",e_search);
				if(pos < 0)
					printf("Email Address not found! Please try another email!\n");
			}

			addressEdit(address_book,pos);
			break;
		
		case 4:
			// edit by serial number
			while(pos<0){
				printf("Enter Serial Number: ");
				scanf("%s", arr);
				pos = search(arr, address_book, 1,11,"",e_search);
				if(pos < 0)
					printf("Serial Number not found! Please try another number!\n");
			}

			addressEdit(address_book,pos);
			break;

		default:
			break;
	}
	printf("Information Updated!\n");
	return e_success;
}

/*Delete Contact*/
//search by name and get position to delete
//Reallocate memory to a new array that is smaller
//copy from original array to new array skipping over position to delete
//free memory from original array
Status delete_contact(AddressBook *address_book)
{
	char confirm;
	int pos = 0;
	int prevpos = -1;
	int namesFound = 0;

	//Placeholders
	ContactInfo deleteRow,emptyRow,lastRow;


	strcpy(deleteRow.name[0], " ");
	strcpy(emptyRow.name[0], " ");

	for (int i = 0; i < 5; i++)
	{
		strcpy(deleteRow.phone_numbers[i], " ");
		strcpy(deleteRow.email_addresses[i], " ");

		strcpy(emptyRow.phone_numbers[i], " ");
		strcpy(emptyRow.email_addresses[i], " ");

		strcpy(lastRow.phone_numbers[i], " ");
		strcpy(lastRow.phone_numbers[i], " ");
	}

	printf("Enter Name: ");
	scanf("%s", deleteRow.name[0]);
	getchar();
	for (int i = pos; i < address_book->count; i++){
		pos = search(deleteRow.name[0], address_book, i,0,"",e_search);
		//display index where search was found; will be replaced w/ a call to list contact function
		if(pos != e_fail && pos > prevpos){
			printf("found at index %d\n", pos);
			printf("=================================================================================================================\n");
			printf(": S.No : %-32s : %-32s : %-32s :\n", " Name", " Phone No", "Email ID");
			printf("=================================================================================================================\n");
			printf(": %d    : %-32s : ", address_book->list[pos].si_no, address_book->list[pos].name);
			for(int j = 0; j < 5; j++){
				if(j == 0){
					printf("%-32s : %-32s :\n", address_book->list[pos].phone_numbers[j], address_book->list[pos].email_addresses[j]);
				}else{
					printf(":      : %-32s : %-32s : %-32s :\n", " ", address_book->list[pos].phone_numbers[j], address_book->list[pos].email_addresses[j]);
				}
			}
			printf("=================================================================================================================\n");
			namesFound++;
		}
		prevpos = pos;
	}

	int user_si_no;
	printf("Select a Serial Number of the name you'd like to delete: ");
	scanf("%d", &user_si_no);
	getchar();
	//assigning deletePerson to desired Serial Number for deletion
	deleteRow = address_book->list[user_si_no - 1];

	menu_header("Delete Contact:\n");
	printf("Name       : %s\n", deleteRow.name[0]);
	printf("Phone Number 1 : %s\n", deleteRow.phone_numbers[0]);
	printf("Phone Number 2 : %s\n", deleteRow.phone_numbers[1]);
	printf("Phone Number 3 : %s\n", deleteRow.phone_numbers[2]);
	printf("Phone Number 4 : %s\n", deleteRow.phone_numbers[3]);
	printf("Phone Number 5 : %s\n", deleteRow.phone_numbers[4]);
	printf("Email Address 1 : %s\n", deleteRow.email_addresses[0]);
	printf("Email Address 2 : %s\n", deleteRow.email_addresses[1]);
	printf("Email Address 3 : %s\n", deleteRow.email_addresses[2]);
	printf("Email Address 4 : %s\n", deleteRow.email_addresses[3]);
	printf("Email Address 5 : %s\n", deleteRow.email_addresses[4]);

	printf("\nPress 'Y or y' to delete, otherwise press any key: ");
	scanf("%s", &confirm);
	getchar();
	//Confirmation for deletion
	if (confirm == 'Y' || confirm == 'y'){
		//lastPerson gets assigned the last address in the book
		lastRow = address_book->list[address_book->count - 1];
		lastRow.si_no = user_si_no;

		//The last person will replace whoever is getting deleted
		address_book->list[user_si_no - 1] = lastRow;

		emptyRow.si_no = 0;
		address_book->list[address_book->count - 1] = emptyRow;

		address_book->count--;
		}
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