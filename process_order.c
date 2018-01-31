#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "order.h"

#define OK        0
#define EMPTY     1
#define TOO_MUCH  2

static int readLine(char *prompt, char *buff, size_t sz) {
    int ch, extra;

    if(prompt != NULL) {
        printf("%s: ", prompt);
        fflush(stdout);
    }

    if(fgets(buff, sz, stdin) == NULL || buff[0] == '\n')
        return EMPTY;

    if(buff[strlen(buff) -1] != '\n') {
        extra = 0;
        while(((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_MUCH : OK;
    }
    
    buff[strlen(buff)-1] = '\0';
    return OK;
}

int main(int argc, char *argv[]) {
    order *my_order = create_order();
    char inbuf[80];
    int status = OK;

    while(status == OK) {
        char   name[80];
        float  quantity;
        float  weight;

        printf("\nNew Item\n");

        status = readLine("Item name", inbuf, sizeof(inbuf));
        if(status == OK) {
            strncpy(name, inbuf, 80);
        } else {
            continue;
        }

        status = readLine("Quantity", inbuf, sizeof(inbuf));
        if(status == OK) {
            quantity = atof(inbuf);
        } else {
            continue;
        }

        status = readLine("Unit weight", inbuf, sizeof(inbuf));
        if(status == OK) {
            weight = atof(inbuf);
        } else {
            continue;
        }
        add_to_order(my_order, name, quantity, weight);
    }

    print_order(my_order);
    exit(0);
}
