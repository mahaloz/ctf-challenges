#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// gcc -no-pie breakfast_stacker.c  -o breakfast_stacker
// Based on CSAWQuals-20, BardsFail.

#define NUMFOOD 10
#define NAMELENGTH 32
#define FOODSIZE sizeof(pancake)
#define MAXINPUTSIZE 128
char foods[NUMFOOD];

typedef struct _pancake
{
    char topping;
    short tastiness;
    int corners;
    char name[NAMELENGTH];
    double saturation;
} pancake;

typedef struct _waffle
{
    char topping;
    double saturation;
    int corners;
    short tastiness;
    char name[NAMELENGTH];
} waffle;

// randomized function layout
void eatPancake(char * name);
char * eatFood(char * ptr, int pancakeNumber);
void createWaffle(waffle * this_waffle);
int getIntClean();
char getCharClean();
void eatWaffle(char * name);
void createPancake(pancake * this_pancake);
void getInput(int length, char * buffer);
size_t createFood(char * ptr, int i);
void stackBreakfast();
// end randomization

void getInput(int length, char * buffer)
{
    memset(buffer, 0, length);
    int count = 0;
    char c;
    while((c = getchar()) != '\n' && !feof(stdin))
    {
        if(count < (length-1))
        {
            buffer[count] = c;
            count++;
        }
    }
    buffer[count] = '\x00';
}

char getCharClean()
{
    char input[MAXINPUTSIZE];
    getInput(MAXINPUTSIZE, input);
    return input[0];
}

int getIntClean()
{
    char input[MAXINPUTSIZE];
    getInput(MAXINPUTSIZE, input);
    return atoi(input);
}

void createPancake(pancake * this_pancake)
{
    puts("What's on the pancake:");
    puts("1) Chocolate Chips");
    puts("2) Bananas");
    fflush(stdout);
    char topping = getCharClean();
    if (topping == '1')
    {
        this_pancake->topping = 'c';
    }
    else if (topping == '2')
    {
        this_pancake->topping = 'b';
    }
    else
    {
        printf("Error: invalid topping selection. Selection was %c\n",topping);
        exit(0);
    }
    this_pancake->tastiness = 5;
    this_pancake->corners = 0;
    this_pancake->saturation = 18;

    puts("Who get's to eat this delicious food? (give me a name!)");
    fflush(stdout);
    // XXX: changed NAMELENGTH for MAXINPUTSIZE
    read(0, this_pancake->name, MAXINPUTSIZE);
    // strip newline from end of name
    for(int i = 0; i < MAXINPUTSIZE-1; i++)
    {
        if (this_pancake->name[i]=='\n')
        {
            this_pancake->name[i]='\x00';
        }
    }
}

void createWaffle(waffle * this_waffle)
{
    puts("What's on the waffle:");
    puts("1) Chocolate Chips");
    puts("2) Bananas");
    fflush(stdout);
    char topping = getCharClean();
    if (topping == '1')
    {
        this_waffle->topping = 'c';
    }
    else if (topping == '2')
    {
        this_waffle->topping = 'b';
    }
    else
    {
        printf("Error: invalid topping selection. Selection was %c\n",topping);
        exit(0);
    }
    this_waffle->tastiness = 100;
    this_waffle->corners = 4;
    this_waffle->saturation = 5;

    puts("Who get's to eat this delicious food? (give me a name!)");
    fflush(stdout);
    read(0, this_waffle->name, NAMELENGTH);
    // strip newline from end of name
    for(int i = 0; i < NAMELENGTH-1; i++)
    {
        if (this_waffle->name[i]=='\n')
        {
            this_waffle->name[i]='\x00';
        }
    }
}

void eatPancake(char * name)
{
    puts("Do you let him/her eat it? Or do you sacrafice the food to Kyle?");
    puts("Options:");
    puts("(s)acrafice the food");
    puts("(l)et him/her eat the food");
    fflush(stdout);
    char selection = getCharClean();
    if (selection == 's')
    {
        puts("Kyle stares at the Pancake... a tear rolls from his eye in hapiness...");
        puts("suddenly, Kyle starts smashing the Pancake stack with his hands -- laughing");
        puts("as his hands get covered in syrup. He bumbles it until it dissinegrates.\n");
    }
    else if (selection == 'l')
    {
        printf("%s runs up to eat the Pancake, but before he can, Kyle, in anger, smashes\n", name);
        puts("the pancake until it is nothing more than particles... shucks...");
    }
    else
    {
        puts("Error: invalid selection.");
        exit(0);
    }
}

void eatWaffle(char * name)
{
    printf("Do you let %s eat it? Or do you sacrafice the food to Kyle?\n",name);
    puts("Options:");
    puts("(s)acrafice the food");
    puts("(l)et him/her eat the food");
    puts("(e)at the waffle before anyone notices");
    fflush(stdout);
    char selection = getCharClean();
    if (selection == 's')
    {
        puts("Kyle stares at the Waffle... a tear rolls from his eye in hapiness...");
        puts("suddenly, Kyle starts smashing the Waffle stack with his hands -- laughing");
        puts("as his hands get covered in syrup. He bumbles it until it dissinegrates.\n");
    }
    else if (selection == 'l')
    {
        printf("%s runs up to eat the Waffle, but before he can, Kyle, in anger, smashes\n", name);
        puts("the pancake until it is nothing more than particles... shucks...");
    }
    else if (selection == 'e')
    {
        puts("Before anyone can get to the Waffle, you inhale it. It's so good, a tear rolls");
        puts("down your eye. Kyle notices, and then devours you... Game Over.");
        exit(0);
    }
    else
    {
        puts("Error: invalid selection.");
        exit(0);
    }
}

char * eatFood(char * ptr, int pancakeNumber)
{
    char food = foods[pancakeNumber];
    char * name;
    printf("\n");
    if (food == 'p')
    {
        name = ((pancake *)ptr)->name;
        ptr += sizeof(pancake);
        printf("%s approaches the pancake... \n", name);
        eatPancake(name);
    }
    else if (food == 'w')
    {
        name = ((waffle *)ptr)->name;
        ptr += sizeof(waffle);
        printf("%s approaches the waffle... \n", name);
        eatWaffle(name);
    }
    else
    {
        puts("Error in reading foods.");
        exit(0);
    }
    return ptr;
}

size_t createFood(char * ptr, int i)
{
    printf("\n");
    printf("Breakfast Food #%d is a pancake or waffle? (p, w):\n", i+1);
    fflush(stdout);
    char food = getCharClean();
    if (food == 'p')
    {
        foods[i] = 'p';
        createPancake((pancake *) ptr);
        return sizeof(pancake);
    }
    else if (food == 'w')
    {
        foods[i] = 'w';
        createWaffle((waffle *) ptr);
        return sizeof(waffle);
    }
    else
    {
        printf("Invalid food: %c\n", food);
        exit(0);
    }
}

void stackBreakfast()
{
    char party[FOODSIZE * NUMFOOD];
    memset(party, 0, FOODSIZE * NUMFOOD);
    int offset = 0;
    for (int i = 0; i < NUMFOOD; i++)
    {
        offset += createFood((party + offset), i);
    }
    offset = 0;
    char * pancakePtr = party+offset;
    for (int i = 0; i < NUMFOOD; i++)
    {
        pancakePtr = eatFood(pancakePtr, i);
    }
}

int main(int argc, char**argv)
{
    puts("Hey, Hey! Wakeup! It's time for breakfest yells Kyle.");
    puts("*You walk downstairs to find food scattereted across the table*");
    puts("You look at the scatterted food in contempt, unsure what to do.");
    puts("Kyle stares at you... then says, \"Isen\'t obvious?, stack them...\"");
    puts("In confusion, you proceed, and start stacking food...");
    stackBreakfast();
    puts("Kyle, smirks, and says, \"You did it wrong.\"");
    return 0;
}

