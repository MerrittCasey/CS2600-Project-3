//#include <stdio_ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "address_book.h"
#include "address_book_fops.h"
#include "address_book_menu.h"

int main(void)
{
	AddressBook* address_book = malloc(sizeof(address_book));
	Status ret;

	ret = load_file(address_book);

	if (ret == e_success)
	{
		/* Show all the available menu */
		ret = menu(address_book);

		if (ret == e_success)
		{
			/* Save the entries */
			save_prompt(address_book);//test this later
		}
	}

	free(address_book);
	return 0;
}
