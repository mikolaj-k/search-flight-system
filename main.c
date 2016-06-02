//CLION SHIT

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-stack-address"

//dla maca
#include <stddef.h>
#include <stdbool.h>
#include <memory.h>
#include <ctype.h>

//dla windowsa
#include <stdio.h>
#include <stdlib.h>

/**********************************************************************
* STRUCTURES INITIALIZATION
**********************************************************************/

struct FLIGHT
{
    char *from;
    char *to;
    int distance;
    int skip; //for backtracking
};

typedef struct FLIGHT FLIGHT;

struct connections
{
    FLIGHT connection;
    struct connections *next;
};

typedef struct connections connections;

/**********************************************************************
* FUNCTION DECLARATIONS
**********************************************************************/

void showMenuChoice();

void toUpper(char *text);

void search(connections *l, char *from, char *to);

FLIGHT isDirect(connections *l, char *from, char *to);

void isConnected(connections *l, char *from, char *to);

connections *add(connections *l, char *from, char *to, int distance);

connections *addReverse(connections *l, FLIGHT new);

connections *delete(connections *l, int n);

void show(connections *l);

void exportData(connections *l);

void import(connections *l);

connections *initialize(connections *l); //for testing database


/**********************************************************************
* MAIN
**********************************************************************/

int main(void)
{
    char choice;
    char clean[1024];
    char from[20];
    char to[20];

    int distance;
    int toDelete;

    connections *database = NULL;

    database = initialize(database);

    while (true)
    {

        showMenuChoice();
        scanf(" %c", &choice);

        switch (choice)
        {
            case '1':

                gets(clean);

                printf("From: ");
                gets(from);

                toUpper(from);

                printf("To: ");
                gets(to);

                toUpper(to);

                search(database, from, to);

                break;

            case '2':

                gets(clean);

                printf("From: ");
                gets(from);

                toUpper(from);

                printf("To: ");
                gets(to);

                toUpper(to);


                printf("Distance:");
                scanf(" %i", &distance);

                database = add(database, from, to, distance);

                break;

            case '3':

                show(database);

                printf("Which filght you want to delete: ");
                scanf(" %i", &toDelete);

                toDelete--;

                database = delete(database, toDelete);

                break;

            case '4':
                import(database);

                break;

            case '5':
                exportData(database);

                break;

            case '6':
                show(database);

                break;

            case 'q':

                printf("Thank you for using our airlines system!\n");
                exit(2137);

            default:
                printf("WRONG CHOICE!");

                break;
        }
    }
}

/**********************************************************************
* FUNCTION DEFINITIONS
**********************************************************************/

void showMenuChoice()
{
    printf("\nWelcome in our airlines system.\n");
    printf("*******************************\n");
    printf("[1]. Search for connection\n");
    printf("[2]. Add connection\n");
    printf("[3]. Remove connection\n");
    printf("[4]. Import database\n");
    printf("[5]. Export database\n");
    printf("[6]. Show database\n");
    printf("[q]. Exit\n");
    printf("What do you want to do: ");
}

void toUpper(char text[20])
{
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        text[i] = (char) toupper(text[i]);
    }
}

void search(connections *l, char *from, char *to)
{
    FLIGHT found;

    printf("Searching connection from: %s to: %s...\n", from, to);

    found = isDirect(l, from, to);

    isConnected(l, from, to);

}

FLIGHT isDirect(connections *l, char *from, char *to)
{
    printf("Searching direct connection from: %s to: %s...\n", from, to);

    connections *tmp = l;
    FLIGHT found;
    found.from = "";
    found.to = "";
    found.distance = 0;

    while (tmp)
    {
        if (strcmp((*tmp).connection.from, from))
        {
            if (strcmp((*tmp).connection.to, to))
            {
                found.from = (*tmp).connection.from;
                found.to = (*tmp).connection.to;
                found.distance = (*tmp).connection.distance;

                printf("found");

                return found;
            }
        }

        tmp = (*tmp).next;
    }
    return found;
}

void isConnected(connections *l, char *from, char *to)
{
    printf("Searching non-direct connection from: %s to: %s...\n", from, to);
}

connections *add(connections *l, char *from, char *to, int distance)
{
    FLIGHT new;
    char choice;

    new.from = from;
    new.to = to;
    new.distance = distance;
    new.skip = 0;

    printf("Adding connection from: %s to: %s with distance: %i...\n", new.from, new.to, new.distance);

    connections *tmp;

    tmp = (connections *) malloc(sizeof(connections));

    if (tmp == NULL)
    {
        printf("MEMORY PROBLEM. CONTACT IT SUPPORT!");
        exit(-1);
    }

    printf("Two side connection [y/n]: ");
    scanf(" %c", &choice);

    if (choice == 'y')
    {
        l = addReverse(l, new);
    }

    (*tmp).connection.from = new.from;
    (*tmp).connection.to = new.to;
    (*tmp).connection.distance = new.distance;
    (*tmp).next = l;


    return tmp;
}

connections *addReverse(connections *l, FLIGHT new)
{
    connections *tmp;
    tmp = (connections *) malloc(sizeof(connections));

    (*tmp).connection.from = new.to;
    (*tmp).connection.to = new.from;
    (*tmp).connection.distance = new.distance;
    (*tmp).next = l;

    return tmp;
}


connections *delete(connections *l, int n)
{
    connections *pom, *poprz = NULL;

    int k = 0;

    if (l == NULL)
    {
        printf("EMPTY LIST OF FLIGHTS!\n");
    }
    else if (n < 0)
    {
        printf("Number must be bigger than 1\n");
    }

    else
    {
        pom = l;

        while ((*pom).next != NULL && k < n)
        {
            poprz = pom;
            pom = (*pom).next;
            ++k;
        }

        if (k == n && poprz != NULL)
        {
            printf("Deleting flight form: %s to %s with distance %i\n", (*pom).connection.from, (*pom).connection.to,
                   (*pom).connection.distance);
            (*poprz).next = (*pom).next;
            free(pom);
        }
        else if (k == n && poprz == NULL)
        {
            printf("Deleting flight form: %s to %s with distance %i\n", (*pom).connection.from, (*pom).connection.to,
                   (*pom).connection.distance);
            l = (*pom).next;
            free(pom);
        }
    }

    return l;
}


void show(connections *l)
{
    int numberOfFlights = 1;

    printf("Showing connections...\n");

    connections *tmp = l;

    printf("AVAILABLE CONNECTIONS: \n");

    while (tmp)
    {
        printf("%i.\n", numberOfFlights);
        printf("\tFrom: %s\n", (*tmp).connection.from);
        printf("\tTo: %s\n", (*tmp).connection.to);
        printf("\tDistance: %i\n", (*tmp).connection.distance);
        printf("\n");

        numberOfFlights++;

        tmp = (*tmp).next;
    }
}

void exportData(connections *l)
{
    int lineNumber = 1;

    printf("Exporting connections....\n");

    FILE *file = fopen("flights.txt", "w+");

    connections *tmp = l;

    while (tmp)
    {
        if ((lineNumber % 3) == 1)
        {
            fprintf(file, "%s\n", (*tmp).connection.from);
            lineNumber++;
        }
        if ((lineNumber % 3) == 2)
        {
            fprintf(file, "%s\n", (*tmp).connection.to);
            lineNumber++;
        }
        if ((lineNumber % 3) == 0)
        {
            fprintf(file, "%i\n", (*tmp).connection.distance);
            lineNumber++;
        }

        tmp = (*tmp).next;
    }

    fclose(file);

}

void import(connections *l)
{
    int lineNumber = 1;

    char *fromIn;
    char *toIn;
    int distanceIn;

    char buf[256];

    FILE *file = fopen("flights.txt", "r");


    printf("Importing connectionns....\n");

    while (fgets(buf, sizeof(buf), file))
    {

        if ((lineNumber % 3) == 1)
        {
            fromIn = buf;
            strtok(fromIn, "\n");
            printf("from: %s\n", fromIn);
        }
        if ((lineNumber % 3) == 2)
        {
            toIn = buf;
            strtok(toIn, "\n");
            printf("to: %s\n", toIn);
        }
        if ((lineNumber % 3) == 0)
        {
            distanceIn = atoi(buf);
            printf("distance: %i\n", distanceIn);
//TODO: adding
            //printf("Adding connection from %s, to %s with distance %i", fromIn, toIn, distanceIn);
            //l = add(l, fromIn, toIn, distanceIn);
        }

        lineNumber++;
    }

    fclose(file);
}

connections *initialize(connections *l)
{
    printf("Initializing database....\n");

    l = add(l, "NEW YORK", "LAS VEGAS", 1000);
    l = add(l, "WARSAW", "GDANSK", 600);
    l = add(l, "WARSAW", "KRAKOW", 800);
    l = add(l, "KRAKOW", "KATOWICE", 200);

    return l;
}


#pragma clang diagnostic pop
