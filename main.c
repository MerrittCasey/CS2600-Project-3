//#include <stdio_ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "address_book.h"
#include "address_book_fops.h"
#include "address_book_menu.h"

int main(void)
{
	AddressBook address_book;
	Status ret;

	ret = load_file(&address_book);

	for(int i = 0; i < address_book.count; i++){
		printf("%s : ", address_book.list[i].name);
		for(int j = 0; j < 5; j++){
			printf("%s %s ", address_book.list[i].phone_numbers[j], address_book.list[i].email_addresses[j]);
		}
		printf("\n");
	}



	// if (ret == e_success)
	// {
	// 	/* Show all the available menu */
	// 	ret = menu(&address_book);

	// 	if (ret == e_success)
	// 	{
	// 		/* Save the entries */
	// 		//save_prompt(&address_book);
	// 	}
	// }

	return 0;
}
