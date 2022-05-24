/*============================================================================*/
/* Program: Go Fish Card Game in C                                           */
/*                                                                            */
/* Description: CS50: Introduction to Computer Science Final Project          */
/*                        Program that simulates a card game called Go Fish   */
/*                                                                            */
/* File: gofish.c                                                             */
/*                                                                            */
/* Author: Rui Pedro Canário Daniel                                           */
/*                                                     Date: Summer 2020      */
/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef enum { false, true } bool;
typedef struct cart
{
    char cardb[3];
    struct cart *next;
    int valuecard;
    int valnype;
} cart;

//alphabetically ordered fuctions

char **allocatevector2d(int lines, int cols);
int *allocatevectorint(int nelements);
char askrandom();
void atributetable(char cards[52][3]);
cart *attributecards(cart *head, int *p, int *l, char cards[52][3]);
cart *card_insertandsort(cart *Listhead, cart *newcardd);
cart *cardtolist(cart *head, char *str, int *p1);
int checkasked(cart *vect, char x, int p);
cart *checkfish(cart *head, int *points, int *p2, int playyer, int *next);
cart *createcard(char *strtemp);
int decoder(int next, int nplayers, int active);
void displayrules();
void error();
void free2d(char **vect, int lines);
void freelist(cart *Listhead);
void game(int wins[10], int random, char name1[200], char name2[200], char name3[200], char name4[200], int nplayers);
char getasked(cart *player, int p1);
void getnames(int n, char name1[200], char name2[200], char name3[200], char name4[200]);
char getrandomasked(cart *player, int p1);
void printlist(cart *Listhead);
void printstats(int nplayers, int wins[10]);
void printwinner(int *points, int wins[10], char name1[200], char name2[200], char name3[200], char name4[200]);
cart *removecard(cart *Listhead, char *cardd, int *p);
void retiranumerovect(char **vect, int posicao, int tamanho);
void shuffle(int *array, size_t n);
int valid(char x);
int valuecards(cart *carta);
int valuenype(cart *aux);

int main(void)
{
    char *line = (char *) calloc(256, sizeof(char));
    char name1[200] = "Player 1", name2[200] = "Player 2", name3[200] = "Player 3", name4[200] = "Player 4";
    int wins[10] = {0}, ngames = 1;
    time_t t;
    srand((unsigned) time(&t));
    int players = 10, i = 0;
    displayrules();
    while (players != 0 && players != 1 && players != 2 && players != 3 && players != 4 && players != 5 && players != 6 && players != 7
           && players != 8 && ngames < 2)
    {
        printf("Select Game Mode: ");
        if (fgets(line, sizeof(line), stdin))
        {
            if (1 != sscanf(line, "%d", &players))
            {
                error();
            }
        }
        if (players == 0 || players == 3 || players == 6)
        {
            printf("Number of games to simulate: ");
            if (fgets(line, sizeof(line), stdin))
            {
                if (1 != sscanf(line, "%d", &ngames))
                {
                    error();
                }
            }
        }
        if (ngames < 1)
        {
            printf("TRY AGAIN\n");
            players = 6;
        }
    }

    switch (players)
    {
        //2 players (bot vs bot)
        case 0:
            for (i = 0; i < ngames; i++)
	        {
    	        game(wins, 1, name1, name2, name3, name4, 2);
    	    }
            printstats(2, wins);
        	break;
        //2 players (player vs bot)
    	case 1:
    	    getnames(1, name1, name2, name3, name4);
            game(wins, 2, name1, name2, name3, name4, 2);
        	break;
        //2 players (player vs player)
    	case 2:
        	getnames(2, name1, name2, name3, name4);
            game(wins, 0, name1, name2, name3, name4, 2);
        	break;
        //3 players (bot vs bot vs bot)
        case 3:
            for (i = 0; i < ngames; i++)
    	    {
    	        game(wins, 1, name1, name2, name3, name4, 3);
    	    }
            printstats(3, wins);
        	break;
        //3 players (player vs bot vs bot)
    	case 4:
            getnames(1, name1, name2, name3, name4);
            game(wins, 2, name1, name2, name3, name4, 3);
        	break;
        //3 players (player vs player vs player)
    	case 5:
            getnames(3, name1, name2, name3, name4);
            game(wins, 0, name1, name2, name3, name4, 3);
        	break;
        //4 players (bot vs bot vs bot vs bot)
        case 6:
            for (i = 0; i < ngames; i++)
    	    {
    	        game(wins, 1, name1, name2, name3, name4, 4);
    	    }
            printstats(4, wins);
        	break;
        //4 players (player vs bot vs bot)
    	case 7:
            getnames(1, name1, name2, name3, name4);
            game(wins, 2, name1, name2, name3, name4, 4);
        	break;
        //4 players (player vs player vs player)
    	case 8:
            getnames(4, name1, name2, name3, name4);
            game(wins, 0, name1, name2, name3, name4, 4);
        	break;
    }
    free(line);
    return 0;
}

void game(int wins[10], int random, char name1[200], char name2[200], char name3[200], char name4[200], int nplayers)
{
    char cards[52][3], asked;
    atributetable(cards);
    cart *player1 = NULL, *player2 = NULL, *player3 = NULL, *player4 = NULL, *aux;
    int l = 0, x = 0; //l controls how many cards are already given
    int p1 = 0, p2 = 0, p3 = 0, p4 = 0; //p1 = cards of player1, p2 = cards of player2
    int points[4] = {0}; //points of the players
    int active = 1, received = 0, next = 0; //player active

    player1 = attributecards(player1, &p1, &l, cards); //atribute the initial cards to all the players
    player2 = attributecards(player2, &p2, &l, cards);
    if (nplayers > 2)
    {
        player3 = attributecards(player3, &p3, &l, cards);
        if (nplayers > 3)
        {
            player4 = attributecards(player4, &p4, &l, cards);
        }
    }

    do
    {
        active = decoder(next, nplayers, active); //get the next active player acording to the previous turn
        received = 0;
        if (active == 1) //player 1 turn
        {
            if (p1 == 0 && l < 52) //if the player have no cards receives one from table deck
            {
                printf("You got: %s\n", cards[l]);
                player1 = cardtolist(player1, cards[l], &p1);
                l++;
            }
            next = 1;
            printf("\n%s\n", name1);
            printlist(player1);
            if (random == 0 || random == 2) //get the asked card from a virtual or physical player
            {
                asked = getasked(player1, p1);
            }
            else
            {
                asked = getrandomasked(player1, p1);
            }
            //askingblock
            if (p2 > 0)
            {
                for (aux = player2; aux != NULL;)
                {
                    if (asked == aux->cardb[0])
                    {
                        printf("%s give you: %s\n", name2, aux->cardb);
                        player1 = cardtolist(player1, aux->cardb, &p1);
                        player2 = removecard(player2, aux->cardb, &p2);
                        received = 1;
                        next = 0;
                        aux = player2;
                    }
                    if (aux != NULL)
                    {
                        aux = aux->next;
                    }
                }
            }
            if (nplayers > 2)
            {
                if (p3 > 0)
                {
                    for (aux = player3; aux != NULL;)
                    {
                        if (asked == aux->cardb[0])
                        {
                            printf("%s give you: %s\n", name3, aux->cardb);
                            player1 = cardtolist(player1, aux->cardb, &p1);
                            player3 = removecard(player3, aux->cardb, &p3);
                            received = 1;
                            next = 0;
                            aux = player3;
                        }
                        if (aux != NULL)
                        {
                            aux = aux->next;
                        }
                    }
                }
                if (nplayers > 3 && p4 > 0)
                {
                    for (aux = player4; aux != NULL;)
                    {
                        if (asked == aux->cardb[0])
                        {
                            printf("%s give you: %s\n", name4, aux->cardb);
                            player1 = cardtolist(player1, aux->cardb, &p1);
                            player4 = removecard(player4, aux->cardb, &p4);
                            received = 1;
                            next = 0;
                            aux = player4;
                        }
                        if (aux != NULL)
                        {
                            aux = aux->next;
                        }
                    }
                }
            }
            //endaskingblock
            player1 = checkfish(player1, points, &p1, 0, &next);

            if (next != 0 && received != 1)//no fish and no receiving: get a card from the deck
            {
                if (asked == cards[l][0] && l < 52)
                {
                    printf("LUCK!! You got: %s\n", cards[l]);
                    player1 = cardtolist(player1, cards[l], &p1);
                    next = 0;
                    l++;
                    player1 = checkfish(player1, points, &p1, 0, &x);
                }
                else if (l < 52)
                {
                    printf("You got: %s\n", cards[l]);
                    player1 = cardtolist(player1, cards[l], &p1);
                    l++;
                    player1 = checkfish(player1, points, &p1, 0, &next);
                }
            }
            else
            {
                next = 0;
            }
        }
        else if (active == 2)//player 2 turn
        {

            if (p2 == 0 && l < 52)//if the player have no cards receives one from table deck
            {
                printf("You got: %s\n", cards[l]);
                player2 = cardtolist(player2, cards[l], &p2);
                l++;
            }
            next = 1;
            printf("\n%s\n", name2);
            printlist(player2);
            if (random == 0)//get the asked card from a virtual or physical player
            {
                asked = getasked(player2, p2);
            }
            else
            {
                asked = getrandomasked(player2, p2);
            }
            //askingblock
            if (p1 > 0)
            {
                for (aux = player1; aux != NULL;)
                {
                    if (asked == aux->cardb[0])
                    {
                        printf("%s give you: %s\n", name1, aux->cardb);
                        player2 = cardtolist(player2, aux->cardb, &p2);
                        player1 = removecard(player1, aux->cardb, &p1);
                        received = 1;
                        next = 0;
                        aux = player1;
                    }
                    if (aux != NULL)
                    {
                        aux = aux->next;
                    }
                }
            }
            if (nplayers > 2)
            {
                if (p3 > 0)
                {
                    for (aux = player3; aux != NULL;)
                    {
                        if (asked == aux->cardb[0])
                        {
                            printf("%s give you: %s\n", name3, aux->cardb);
                            player2 = cardtolist(player2, aux->cardb, &p2);
                            player3 = removecard(player3, aux->cardb, &p3);
                            received = 1;
                            next = 0;
                            aux = player3;
                        }
                        if (aux != NULL)
                        {
                            aux = aux->next;
                        }
                    }
                }
                if (nplayers > 3 && p4 > 0)
                {
                    for (aux = player4; aux != NULL;)
                    {
                        if (asked == aux->cardb[0])
                        {
                            printf("%s give you: %s\n", name4, aux->cardb);
                            player2 = cardtolist(player2, aux->cardb, &p2);
                            player4 = removecard(player4, aux->cardb, &p4);
                            received = 1;
                            next = 0;
                            aux = player4;
                        }
                        if (aux != NULL)
                        {
                            aux = aux->next;
                        }
                    }
                }
            }
            //endaskingblock
            player2 = checkfish(player2, points, &p2, 1, &next);

            if (next != 0 && received != 1)//no fish and no receiving: get a card from the deck
            {
                if (asked == cards[l][0] && l < 52)
                {
                    printf("LUCK!! You got: %s\n", cards[l]);
                    player2 = cardtolist(player2, cards[l], &p2);
                    next = 0;
                    l++;
                    player2 = checkfish(player2, points, &p2, 1, &x);
                }
                else if (l < 52)
                {
                    printf("You got: %s\n", cards[l]);
                    player2 = cardtolist(player2, cards[l], &p2);
                    l++;
                    player2 = checkfish(player2, points, &p2, 1, &next);
                }

            }
            else
            {
                next = 0;
            }
        }
        else if (nplayers > 2 && active == 3)  //player 3 turn
        {

            if (p3 == 0 && l < 52)//if the player have no cards receives one from table deck
            {
                printf("You got: %s\n", cards[l]);
                player3 = cardtolist(player3, cards[l], &p3);
                l++;
            }
            next = 1;
            printf("\n%s\n", name3);
            printlist(player3);
            if (random == 0)//get the asked card from a virtual or physical player
            {
                asked = getasked(player3, p3);
            }
            else
            {
                asked = getrandomasked(player3, p3);
            }
            //askingblock
            if (p1 > 0)
            {
                for (aux = player1; aux != NULL;)
                {
                    if (asked == aux->cardb[0])
                    {
                        printf("%s give you: %s\n", name1, aux->cardb);
                        player3 = cardtolist(player3, aux->cardb, &p3);
                        player1 = removecard(player1, aux->cardb, &p1);
                        received = 1;
                        next = 0;
                        aux = player1;
                    }
                    if (aux != NULL)
                    {
                        aux = aux->next;
                    }
                }
            }
            if (p2 > 0)
            {
                for (aux = player2; aux != NULL;)
                {
                    if (asked == aux->cardb[0])
                    {
                        printf("%s give you: %s\n", name2, aux->cardb);
                        player3 = cardtolist(player3, aux->cardb, &p3);
                        player2 = removecard(player2, aux->cardb, &p2);
                        received = 1;
                        next = 0;
                        aux = player2;
                    }
                    if (aux != NULL)
                    {
                        aux = aux->next;
                    }
                }
            }
            if (nplayers > 3 && p4 > 0)
            {
                for (aux = player4; aux != NULL;)
                {
                    if (asked == aux->cardb[0])
                    {
                        printf("%s give you: %s\n", name4, aux->cardb);
                        player3 = cardtolist(player3, aux->cardb, &p3);
                        player4 = removecard(player4, aux->cardb, &p4);
                        received = 1;
                        next = 0;
                        aux = player4;
                    }
                    if (aux != NULL)
                    {
                        aux = aux->next;
                    }
                }
            }
            //endaskingblock
            player3 = checkfish(player3, points, &p3, 2, &next);

            if (next != 0 && received != 1)//no fish and no receiving: get a card from the deck
            {
                if (asked == cards[l][0] && l < 52)
                {
                    printf("LUCK!! You got: %s\n", cards[l]);
                    player3 = cardtolist(player3, cards[l], &p3);
                    next = 0;
                    l++;
                    player3 = checkfish(player3, points, &p3, 2, &x);
                }
                else if (l < 52)
                {
                    printf("You got: %s\n", cards[l]);
                    player3 = cardtolist(player3, cards[l], &p3);
                    l++;
                    player3 = checkfish(player3, points, &p3, 2, &next);
                }

            }
            else
            {
                next = 0;
            }
        }
        else if (nplayers > 3 && active == 4)  //player 4 turn
        {

            if (p4 == 0 && l < 52)//if the player have no cards receives one from table deck
            {
                printf("You got: %s\n", cards[l]);
                player4 = cardtolist(player4, cards[l], &p4);
                l++;
            }
            next = 1;
            printf("\n%s\n", name4);
            printlist(player4);
            if (random == 0)//get the asked card from a virtual or physical player
            {
                asked = getasked(player4, p4);
            }
            else
            {
                asked = getrandomasked(player4, p4);
            }
            //askingblock
            if (p1 > 0)
            {
                for (aux = player1; aux != NULL;)
                {
                    if (asked == aux->cardb[0])
                    {
                        printf("%s give you: %s\n", name1, aux->cardb);
                        player4 = cardtolist(player4, aux->cardb, &p4);
                        player1 = removecard(player1, aux->cardb, &p1);
                        received = 1;
                        next = 0;
                        aux = player1;
                    }
                    if (aux != NULL)
                    {
                        aux = aux->next;
                    }
                }
            }
            if (p2 > 0)
            {
                for (aux = player2; aux != NULL;)
                {
                    if (asked == aux->cardb[0])
                    {
                        printf("%s give you: %s\n", name2, aux->cardb);
                        player4 = cardtolist(player4, aux->cardb, &p4);
                        player2 = removecard(player2, aux->cardb, &p2);
                        received = 1;
                        next = 0;
                        aux = player2;
                    }
                    if (aux != NULL)
                    {
                        aux = aux->next;
                    }
                }
            }
            if (p3 > 0)
            {
                for (aux = player3; aux != NULL;)
                {
                    if (asked == aux->cardb[0])
                    {
                        printf("%s give you: %s\n", name3, aux->cardb);
                        player4 = cardtolist(player4, aux->cardb, &p4);
                        player3 = removecard(player3, aux->cardb, &p3);
                        received = 1;
                        next = 0;
                        aux = player3;
                    }
                    if (aux != NULL)
                    {
                        aux = aux->next;
                    }
                }
            }
            //endaskingblock
            player4 = checkfish(player4, points, &p4, 3, &next);

            if (next != 0 && received != 1)
            {
                if (asked == cards[l][0] && l < 52)//no fish and no receiving: get a card from the deck
                {
                    printf("LUCK!! You got: %s\n", cards[l]);
                    player4 = cardtolist(player4, cards[l], &p4);
                    next = 0;
                    l++;
                    player4 = checkfish(player4, points, &p4, 3, &x);
                }
                else if (l < 52)
                {
                    printf("You got: %s\n", cards[l]);
                    player4 = cardtolist(player4, cards[l], &p4);
                    l++;
                    player4 = checkfish(player4, points, &p4, 3, &next);
                }

            }
            else
            {
                next = 0;
            }
        }
    }
    while (l < 52);

    printwinner(points, wins, name1, name2, name3, name4);

    freelist(player1);
    freelist(player2);
    freelist(player3);
    freelist(player4);
    return;
}

//if got a fish return 1
cart *checkfish(cart *head, int *points, int *p2, int playyer, int *next)
{
    cart *aux1, *aux2, *aux3, *aux4;
    aux1 = head;
    int check = 0, x = 0;
    char change[4][3];
    if (*p2 < 4) //less than 4 cards = nofish
    {
        return head;
    }
    else if (*p2 == 4) //4 cards, fish if all the cards have the same value
    {
        aux2 = aux1->next;
        aux3 = aux2->next;
        aux4 = aux3->next;
        if (aux1->cardb[0] == aux2->cardb[0] && aux3->cardb[0] == aux4->cardb[0] && aux1->cardb[0] == aux3->cardb[0])
        {
            printf("FISH OF %c\n", aux1->cardb[0]);
            strcpy(change[0], aux1->cardb);
            strcpy(change[1], aux2->cardb);
            strcpy(change[2], aux3->cardb);
            strcpy(change[3], aux4->cardb);
            head = removecard(head, change[0], &(*p2));
            head = removecard(head, change[1], &(*p2));
            head = removecard(head, change[2], &(*p2));
            head = removecard(head, change[3], &(*p2));
            //*p2 = *p2 - 4;
            points[playyer]++;
            x = 1;
        }
        return head;
    }
    // more than 4 cards: iterative checking if there are a fish, knowing that the cards are sorted
    for (aux1 = head, aux2 = aux1->next, aux3 = aux2->next, aux4 = aux3->next; aux4 != NULL && aux3 != NULL && aux2 != NULL
         && aux1 != NULL; aux1 = aux1->next, aux2 = aux1->next, aux3 = aux2->next, aux4 = aux3->next)
    {
        check = 0;

        if (aux1->valuecard == aux4->valuecard)//easy to check because the cards are sorted
        {
            strcpy(change[0], aux1->cardb);
            strcpy(change[1], aux2->cardb);
            strcpy(change[2], aux3->cardb);
            strcpy(change[3], aux4->cardb);
            check = 3;
            break;
        }
    }
    if (check == 3) //removing the fish from the player deck
    {
        printf("FISH OF %c\n", change[0][0]);
        head = removecard(head, change[0], &(*p2));
        head = removecard(head, change[1], &(*p2));
        head = removecard(head, change[2], &(*p2));
        head = removecard(head, change[3], &(*p2));
        points[playyer]++; //one more point to that player
        x = 1;
    }

    if (x == 1) //control block to secure that a fish means another turn for that player
    {
        *next = 0;
    }
    else
    {
        *next = 1;
    }
    return head;
}

void atributetable(char cards[52][3])
{
    int *arr = (int *) calloc(52, sizeof(int)); //indexes

    for (int i = 0; i < 52; i++)
    {
        arr[i] = i;
    }

    shuffle(arr, 52); //some shuffle random
    //random atribute the deck to the cards vector
    strcpy(cards[arr[0]], "2C");
    strcpy(cards[arr[1]], "2D");
    strcpy(cards[arr[2]], "2P");
    strcpy(cards[arr[3]], "2O");
    strcpy(cards[arr[4]], "3C");
    strcpy(cards[arr[5]], "3D");
    strcpy(cards[arr[6]], "3H");
    strcpy(cards[arr[7]], "3S");
    strcpy(cards[arr[8]], "4C");
    strcpy(cards[arr[9]], "4D");
    strcpy(cards[arr[10]], "4H");
    strcpy(cards[arr[11]], "4S");
    strcpy(cards[arr[12]], "5C");
    strcpy(cards[arr[13]], "5D");
    strcpy(cards[arr[14]], "5H");
    strcpy(cards[arr[15]], "5S");
    strcpy(cards[arr[16]], "6C");
    strcpy(cards[arr[17]], "6D");
    strcpy(cards[arr[18]], "6H");
    strcpy(cards[arr[19]], "6S");
    strcpy(cards[arr[20]], "7C");
    strcpy(cards[arr[21]], "7D");
    strcpy(cards[arr[22]], "7H");
    strcpy(cards[arr[23]], "7S");
    strcpy(cards[arr[24]], "8C");
    strcpy(cards[arr[25]], "8D");
    strcpy(cards[arr[26]], "8H");
    strcpy(cards[arr[27]], "8S");
    strcpy(cards[arr[28]], "9C");
    strcpy(cards[arr[29]], "9D");
    strcpy(cards[arr[30]], "9H");
    strcpy(cards[arr[31]], "9S");
    strcpy(cards[arr[32]], "TC");
    strcpy(cards[arr[33]], "TD");
    strcpy(cards[arr[34]], "TH");
    strcpy(cards[arr[35]], "TS");
    strcpy(cards[arr[36]], "JC");
    strcpy(cards[arr[37]], "JD");
    strcpy(cards[arr[38]], "JH");
    strcpy(cards[arr[39]], "JS");
    strcpy(cards[arr[40]], "QC");
    strcpy(cards[arr[41]], "QD");
    strcpy(cards[arr[42]], "QH");
    strcpy(cards[arr[43]], "QS");
    strcpy(cards[arr[44]], "KC");
    strcpy(cards[arr[45]], "KD");
    strcpy(cards[arr[46]], "KH");
    strcpy(cards[arr[47]], "KS");
    strcpy(cards[arr[48]], "AC");
    strcpy(cards[arr[49]], "AD");
    strcpy(cards[arr[50]], "AH");
    strcpy(cards[arr[51]], "AS");
    free(arr);
    return;
}

void shuffle(int *array, size_t n) //random mixing from Stack Overflow, Asmodiel's link to Ben Pfaff's Writings
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int testvalidcard(char *caard)
{
    //first character must be A,2,3,4,5,6,7,8,9,T,J,Q,K
    //second character must be C(Clubs), D(Diamonds), H(Hearts) e S(Spades)
    if (!((caard[0] <= 57 && caard[0] >= 50) || caard[0] == 65 || caard[0] == 84 || caard[0] == 75 || caard[0] == 74
          || caard[0] == 81) || (!(caard[1] == 'C' || caard[1] == 'D' || caard[1] == 'H' || caard[1] == 'S')))
    {
        return 0;
    }
    return 1;
}

char **allocatevector2d(int lines, int cols) //memory allocation 2dchar
{
    int i = 0;
    char **vect;
    if ((vect = (char **)calloc(lines, sizeof(char *))) == NULL)
    {
        error();
    }
    for (i = 0; i < lines; i++)
    {
        if ((vect[i] = (char *)calloc(cols, sizeof(char))) == NULL)
        {
            error();
        }
    }
    return vect;
}

void free2d(char **vect, int lines) //free memory 2d char
{
    int i = 0;
    for (i = 0; i < lines; i++)
    {
        free(vect[i]);
    }
    free(vect);
    return;
}

int *allocatevectorint(int nelements)//allocate a vector of ints
{
    int *vect;
    if ((vect = (int *)calloc(nelements, sizeof(int))) == NULL)
    {
        error();
    }
    return vect;
}

int valuecards(cart *aux) //functions to atribute a value to a card=value
{
    int value = 0;
    switch (aux->cardb[0])
    {
        case 'A':
            value = 14;
            break;
        case 'K':
            value = 13;
            break;
        case 'Q':
            value = 12;
            break;
        case 'J':
            value = 11;
            break;
        case 'T':
            value = 10;
            break;
        case '9':
            value = 9;
            break;
        case '8':
            value = 8;
            break;
        case '7':
            value = 7;
            break;
        case '6':
            value = 6;
            break;
        case '5':
            value = 5;
            break;
        case '4':
            value = 4;
            break;
        case '3':
            value = 3;
            break;
        case '2':
            value = 2;
            break;
    }
    return value;
}

//atributes a value to a card by nype
int valuenype(cart *aux)
{
    int result = 0;
    switch (aux->cardb[1])
    {
        case 'C':
            result = 4;
            break;
        case 'D':
            result = 3;
            break;
        case 'H':
            result = 2;
            break;
        case 'S':
            result = 1;
            break;
    }

    return result;
}

void error() //global errorr mensage
{
    fprintf(stdout, "errorr, read the rules!\n");
    exit(0);
}

int valid(char x) //checks if the asked card is valid
{
    if (x == 'A' || x == 'K' || x == 'J' || x == 'Q' || x == 'T' || x == '9' || x == '8' ||
        x == '7' || x == '6' || x == '5' || x == '4' || x == '3' || x == '2')
    {
        return 0;
    }
    return 1;
}

int checkasked(cart *head, char x, int p) //check if the card asked by a player is in their deck
{
    cart *aux;
    for (aux = head; aux != NULL; aux = aux->next)
    {
        if (aux->cardb[0] == x)
        {
            return 0;
        }
    }
    return 1;
}

cart *createcard(char *strtemp) //creates a new card
{
    cart *card1;
    if ((card1 = (cart *) calloc(1, sizeof(cart))) == NULL)
    {
        error();
    }
    strncpy(card1->cardb, strtemp, 3);
    card1->next = NULL;
    card1->valuecard = valuecards(card1);
    card1->valnype = valuenype(card1);
    return card1;
}

cart *card_insertandsort(cart *Listhead, cart *newcardd) //insert a card in a list, autosorting the list as it inserts
{
    cart *AuxH, *AuxT;
    if (Listhead == NULL)
    {
        Listhead = newcardd;
    }
    else
    {
        if (Listhead->valuecard < newcardd->valuecard || (Listhead->valuecard == newcardd->valuecard
                && Listhead->valnype < newcardd->valnype))
        {
            newcardd->next = Listhead;
            Listhead = newcardd;
        }
        else
        {
            AuxH = Listhead;
            AuxT = Listhead->next;
            while ((AuxT != NULL) && (AuxT->valuecard > newcardd->valuecard))
            {
                AuxH = AuxT;
                AuxT = AuxT->next;
            }
            newcardd->next = AuxT;
            AuxH->next = newcardd;
        }
    }
    return Listhead;
}

void freelist(cart *Listhead) //free a list of cards
{
    if (Listhead == NULL)
    {
        return;
    }
    cart *Aux;
    Aux = Listhead;
    while (Listhead != NULL)
    {
        Aux = Listhead;
        Listhead = (Listhead->next);
        free(Aux);
    }
    return;
}

cart *removecard(cart *Listhead, char *cardd, int *p) //remove a card from a list
{
    cart *AuxH, *AuxT;
    AuxH = Listhead;
    AuxT = Listhead->next;
    if (AuxH == NULL)
    {
        return Listhead;
    }
    (*p)--;
    if (strcmp(AuxH->cardb, cardd) == 0)
    {
        Listhead = Listhead->next;
        free(AuxH);
    }
    else
    {
        while (AuxT != NULL && strcmp(AuxT->cardb, cardd) != 0)
        {
            AuxH = AuxT;
            AuxT = AuxT->next;
        }
        if (AuxH->next != NULL)
        {
            AuxH->next = AuxT->next;
            free(AuxT);
        }
    }
    return Listhead;
}

void printlist(cart *Listhead) //outputs the list card values
{
    if (Listhead == NULL)
    {
        printf("\n");
        return;
    }
    cart *Aux;

    for (Aux = Listhead; Aux != NULL; Aux = Aux->next)
    {
        printf("%s ", Aux->cardb);
    }
    printf("\n");
}

char askrandom() //functions to atribute a value to a card=value
{
    int value = (rand() % 13) + 2;
    switch (value)
    {
        case 14:
            return 'A';
        case 13:
            return 'K';
        case 12:
            return 'Q';
        case 11:
            return 'J';
        case 10:
            return 'T';
        case 9:
            return '9';
        case 8:
            return '8';
        case 7:
            return '7';
        case 6:
            return '6';
        case 5:
            return '5';
        case 4:
            return '4';
        case 3:
            return '3';
        default:
            return '2';
    }
}

cart *cardtolist(cart *head, char *str, int *p1)//adds a card str to a list (player deck)
{
    cart *newcar;
    newcar = createcard(str);
    head = card_insertandsort(head, newcar);
    (*p1)++;
    return head;
}

//displays the winner of some round/ game
void printwinner(int *points, int wins[10], char name1[200], char name2[200], char name3[200], char name4[200])
{
    int i = 0, max = 0;
    for (i = 1; i < 4; i++)
    {
        if (points[i] > points[max])
        {
            max = i;
        }
    }
    switch (max)
    {
        case 0:
            printf("\n%s wins\n", name1);
            break;
        case 1:
            printf("\n%s wins\n", name2);
            break;
        case 2:
            printf("\n%s wins\n", name3);
            break;
        case 3:
            printf("\n%s wins\n", name4);
            break;
    }
    wins[max]++;
}

//prints the stats of all rounds
void printstats(int nplayers, int wins[10])
{
    int i = 0;
    int games = 0;
    if (nplayers > 9)
    {
        return;
    }
    for (i = 0; i < nplayers; i++)
    {
        games += wins[i];
    }
    printf("\nGames: %d\n", games);
    for (i = 0; i < nplayers; i++)
    {
        printf("Player %d: %d wins\n", i + 1, wins[i]);
    }
    return;
}

//get from physical user input for the asked card
char getasked(cart *player, int p1)
{
    char *line = (char *) calloc(256, sizeof(char));
    char asked;
    do
    {
        printf("Choose a card to ask: ");
        scanf("%s", line);
        asked = line[0];
        asked = toupper(asked);
    }
    while (valid(asked) || checkasked(player, asked, p1));
    free(line);
    return asked;
}
//get from virtual user input for the asked card
char getrandomasked(cart *player, int p1)
{
    char asked;
    do
    {
        asked = askrandom();
        asked = toupper(asked);
    }
    while (valid(asked) || checkasked(player, asked, p1));
    return asked;
}

//gets the names of the players
void getnames(int n, char name1[200], char name2[200], char name3[200], char name4[200])
{
    if (n > 0)
    {
        printf("Name Player1: ");
        scanf("%200s", name1);
        if (n > 1)
        {
            printf("Name Player2: ");
            scanf("%200s", name2);
            if (n > 2)
            {
                printf("Name Player 3: ");
                scanf("%200s", name3);
                if (n > 3)
                {
                    printf("Name Player 4: ");
                    scanf("%200s", name4);
                }
            }
        }
    }
}

//atribute the initial cards to the players
cart *attributecards(cart *head, int *p, int *l, char cards[52][3])
{
    int i;
    for (*l = *l, i = 0; i < 4; (*l)++, i++)
    {
        head = cardtolist(head, cards[*l], &*p);
    }
    return head;
}

//checks who is the next player acording to the previous turn and the number or players
int decoder(int next, int nplayers, int active)
{
    if (active == 1)
    {
        if (next == 1)
        {
            return 2;
        }
        else
        {
            return 1;
        }
    }
    else if (active == 2)
    {
        if (nplayers == 2)
        {
            if (next == 1)
            {
                return 1;
            }
            else
            {
                return 2;
            }
        }
        else
        {
            if (next == 1)
            {
                return 3;
            }
            else
            {
                return 2;
            }
        }
    }
    else if (active == 3)
    {
        if (nplayers == 3)
        {
            if (next == 1)
            {
                return 1;
            }
            else
            {
                return 3;
            }
        }
        else
        {
            if (next == 1)
            {
                return 4;
            }
            else
            {
                return 3;
            }
        }
    }
    else if (active == 4)
    {
        if (next == 1)
        {
            return 1;
        }
        else
        {
            return 4;
        }
    }
    return 1;
}

//shows to the user how this C program works
void displayrules()
{
    printf("Welcome to Go Fish Card Game!\nRules:\n");
    printf("- Objective: the player with most points wins;\n");
    printf("- Players win a point each time they have 4 cards of the same value, ex: 8C, 8D, 8H, 8S;\n");
    printf("- One at a time asks for a card, ex: 8. If someone have a card of that value must give it to the caller;\n");
    printf("- If the caller receives a card from an other player, it's his turn again;\n");
    printf("- If noone has that card, the player gets a card from the table deck;\n");
    printf("- If the card from the deck has the same value that the player asked, it's his turn again\n");
    printf("0 - 2 players (bot vs bot)\n");
    printf("1 - 2 players (player vs bot)\n");
    printf("2 - 2 players (player vs player)\n");
    printf("3 - 3 players (bot vs bot vs bot)\n");
    printf("4 - 3 players (player vs bot vs bot)\n");
    printf("5 - 3 players (player vs player vs player)\n");
    printf("6 - 4 players (bot vs bot vs bot vs bot)\n");
    printf("7 - 4 players (player vs bot vs bot vs bot)\n");
    printf("8 - 4 players (player vs player vs player vs player)\n\n");
}