#include <stddef.h>
#include <stdbool.h>
#include <memory.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/**********************************************************************
* STRUCTURES INITIALIZATION
**********************************************************************/

struct FLIGHT
{
    char from[20];
    char to[20];
    int distance;
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

void toUpper(char *text);

void showMenuChoice();

void exportData(connections *l);

void show(connections *l);

connections *import(connections *l);

connections *search(connections *l, char *from, char *to);

connections *direct(connections *l, char *from, char *to);

connections *nonDirect(connections *l, char from[20], char to[20]);

connections *add(connections *l, char *from, char *to, int distance);

connections *addOneWay(connections *l, char *from, char *to, int distance);

connections *insert(connections *l, char *from, char *to, int distance);

connections *delete(connections *l, int n);

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
    connections *searchResult = NULL;

    database = import(database);

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

                searchResult = search(database, from, to);

                if (searchResult == NULL)
                {
                    printf("There is no connection between %s and %s", from, to);

                    getchar();

                }
                else
                {
                    show(searchResult);
                }
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

                printf("Which flight you want to delete: ");
                scanf(" %i", &toDelete);

                toDelete--;

                database = delete(database, toDelete);

                break;

            case '4':
                database = import(database);

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

void toUpper(char text[20])
{
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        text[i] = (char) toupper(text[i]);
    }
}

void showMenuChoice()
{
    //system("cls"); //windows
    system("clear"); //mac
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

void exportData(connections *l)
{
    int lineNumber = 1;

    printf("Exporting connections....\n");

    getchar();

    FILE *file = fopen("/Users/mikolajklimas/Desktop/search-flight-system/flights.txt", "w+");

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

void show(connections *l)
{
    int numberOfFlights = 1;

    connections *tmp = l;

    printf("AVAILABLE CONNECTIONS: \n");

    if (tmp == NULL)
    {
        printf("No connections available.");
    }
    else
    {
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
    getchar();
}

connections *import(connections *l)
{
    int lineNumber = 1;

    char *fromIn;
    char *toIn;
    int distanceIn;

    fromIn = (char *) malloc(sizeof(char *));
    toIn = (char *) malloc(sizeof(char *));

    if ((fromIn == NULL) || (toIn == NULL))
    {
        printf("MEMORY PROBLEM. CONTACT IT SUPPORT!");
        exit(-1);
    }

    char buf[256];
    char bufTransfer[256];

    FILE *file = fopen("/Users/mikolajklimas/Desktop/search-flight-system/flights.txt", "r");

    while (fgets(buf, sizeof(buf), file))
    {
        if ((lineNumber % 3) == 1)
        {
            strcpy(bufTransfer, buf);
            strcpy(fromIn, bufTransfer);
            strtok(fromIn, "\n");
        }
        if ((lineNumber % 3) == 2)
        {
            strcpy(bufTransfer, buf);
            strcpy(toIn, bufTransfer);
            strtok(toIn, "\n");
        }
        if ((lineNumber % 3) == 0)
        {
            distanceIn = atoi(buf);
            printf("Adding connection from %s to %s with distance %i\n", fromIn, toIn, distanceIn);
            l = addOneWay(l, fromIn, toIn, distanceIn);
        }

        lineNumber++;
    }

    fclose(file);

    getchar();

    return l;
}

connections *search(connections *l, char *from, char *to)
{
    connections *found = NULL;

    printf("Searching connection from: %s to: %s...\n", from, to);

    found = direct(l, from, to);

    if (found == NULL)
    {
        found = nonDirect(l, from, to);
    }

    return found;
}

connections *direct(connections *l, char *from, char *to)
{
    connections *tmp = l;
    connections *found = NULL;

    int decision;

    while (tmp)
    {
        decision = strcmp((*tmp).connection.from, from);

        if (!decision)
        {
            decision = strcmp((*tmp).connection.to, to);

            if (!decision)
            {
                found = addOneWay(found, (*tmp).connection.from, (*tmp).connection.to, (*tmp).connection.distance);

                return found;
            }
        }

        tmp = (*tmp).next;
    }
    return NULL;
}

connections *nonDirect(connections *l, char from[20], char to[20])
{
    connections *tmp = l;
    connections *found = NULL;
    char anywhere[20];

    int decision;

    while (tmp)
    {
        decision = strcmp((*tmp).connection.from, from);

        if (!decision)
        {
            strcpy(anywhere, (*tmp).connection.to);

            found = direct(l, anywhere, to);

            if (found != NULL)
            {
                found = addOneWay(found, (*tmp).connection.from, (*tmp).connection.to, (*tmp).connection.distance);

                return found;
            }
        }
        tmp = (*tmp).next;
    }

    return NULL;
}

connections *add(connections *l, char *from, char *to, int distance)
{
    char choice;

    printf("From: %s To: %s Distance:%i\n", from, to, distance);

    printf("Two side connection [y/n]: ");
    scanf(" %c", &choice);

    if (choice == 'y')
    {
        l = addOneWay(l, from, to, distance);
        l = insert(l, to, from, distance);
    }
    else if (choice == 'n')
    {
        l = addOneWay(l, from, to, distance);
    }

    getchar();

    return l;
}

connections *addOneWay(connections *l, char *from, char *to, int distance)
{
    FLIGHT new;

    strcpy(new.from, from);
    strcpy(new.to, to);
    new.distance = distance;

    connections *tmp;

    tmp = (connections *) malloc(sizeof(connections));

    if (tmp == NULL)
    {
        printf("MEMORY PROBLEM. CONTACT IT SUPPORT!");
        exit(-1);
    }

    strcpy((*tmp).connection.from, new.from);
    strcpy((*tmp).connection.to, new.to);
    (*tmp).connection.distance = new.distance;
    (*tmp).next = l;


    return tmp;
}

connections *insert(connections *l, char *from, char *to, int distance)
{
    if (l == NULL)
    {
        return addOneWay(l, from, to, distance);
    }

    connections *wsk;

    wsk = l;
    while ((*wsk).next != NULL)
    {
        wsk = (*wsk).next;
    }

    connections *newConnection;
    newConnection = (connections *) malloc(sizeof(connections));

    if (newConnection == NULL)
    {
        printf("Not enough memory.\n");
        exit(-1);
    }

    strcpy((*newConnection).connection.from, from);
    strcpy((*newConnection).connection.to, to);
    (*newConnection).connection.distance = distance;
    (*newConnection).next = NULL;

    (*wsk).next = newConnection;

    return l;
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

    getchar();

    return l;
}