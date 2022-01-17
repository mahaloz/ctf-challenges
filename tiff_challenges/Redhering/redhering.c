#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Car {
	char name[0x20];
};

struct Car *only_car;
char dealership[0x1000];
int usage = 0;

void readn(char *buf, unsigned len)
{
	int num_read = read(0, buf, len);
	if(num_read <= 0) {
		puts("read error");
		exit(-1);
	}
}

int read_num()
{
	char buf[0x10];
	memset(buf, 0, sizeof(buf));
	readn(buf, sizeof(buf)-1);
	return atoi(buf);
}

void create_car()
{
	puts("\n[!] Creating new car...");
	only_car = (struct Car *) malloc(sizeof(struct Car));
	puts("Name: ");
	readn(only_car->name, 0x10);
}

void destroy_car()
{
	puts("\nDestroying the only car in the lot...");
	free(only_car);
}

void edit_car()
{
	puts("\nSorry... our developer never got to this.");
	puts("Just destroy the car and make a new one.");
}

void name_dealership()
{
	puts("\nDealership Name: ");
	readn(dealership, 0x1000);
}

void show_dealership()
{	
	int answer; 
	if(usage < 100)
	{	
		puts("Show dealership?");
		puts("1. Yes");
		puts("2. No");
		
		char buf[0x10];
		memset(buf, 0, sizeof(buf));
		readn(buf, sizeof(buf)-1);
		answer = atoi(buf);

		if(answer == 1)
			printf(dealership);
		
		usage++;
	}
	else
	{
		puts("Sorry this interface is offline");
	}
		

}

void print_menu(void)
{
	puts("\n-------------------------");
	puts("What do you want to do?");
	puts("1. Create a new car");
	puts("2. Destroy a car");
	puts("3. Edit a car");
	puts("4. Name the dealrship");
	puts("5. Show dealership");
	printf("Choice: ");
}

int vuln()
{
	puts("Welcome to the virtual car dealrship and creator by");
	puts("Judis the Mallocer. This new platform will allow you");
	puts("to make the car of your dreams. Aplogies for bugs...");
	puts("we've recently had a shortage on food which has made us");
	puts("a little loopy.");

	int choice;
	while(1) {
		print_menu();
		choice = read_num();

		switch(choice) {
		case 1:
			create_car();
			break;
		case 2:
			destroy_car();
			break;
		case 3:
			edit_car();
			break;
		case 4:
			name_dealership();
			break;
		case 5:
			show_dealership();
			break;
		default:
			puts("Unknown choice");
		}
	}

    return 0;
}

int main(int argc, char**argv)
{
    vuln();
    return 0;
}



