#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "order.h"

order_item *create_order_item(char *name, int quantity, double weight, order_item *next) {
    order_item *new_order_item = (order_item *)malloc(sizeof(order_item));
    if(new_order_item == NULL) {
        fprintf(stderr, "Error creating a new order_item\n");
        exit(0);
    }

    new_order_item->item     = (char *)malloc(strlen(name) + 1);
    if(new_order_item->item == NULL) {
        fprintf(stderr, "Error creating a new order item name\n");
        exit(0);
    }
    new_order_item->quantity = quantity;
    new_order_item->weight   = weight;
    new_order_item->next     = next;
    strcpy(new_order_item->item, name);

    return new_order_item;
}

order_item *prepend(order_item *head, char *name, int quantity, double weight) {
    order_item *new_order_item = create_order_item(name, quantity, weight, head);
    head                       = new_order_item;
    return head;
}

order_item *append(order_item *head, char *name, int quantity, double weight) {
    order_item *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;

    order_item *new_order_item = create_order_item(name, quantity, weight, NULL);
    cursor->next               = new_order_item;

    return head;
}

double calculate_shipping_cost(double weight, int quantity) {
    double total_weight = (double)quantity * weight;

    if(total_weight <= 50.0) {
        return 5.5;
    } else if(total_weight <= 100.0) {
        return 10.5;
    } else {
        return 20.5;
    }
}

order *add_to_order(order *my_order, char *name, int quantity, double weight) {
    double shipping_cost      = calculate_shipping_cost(weight, quantity);

    my_order->total_weight  += (double)quantity * weight;
    my_order->shipping_cost += shipping_cost;

    my_order->items          = prepend(my_order->items, name, quantity, weight);

    return my_order;
}

order *create_order() {
    order *my_new_order = (order *)malloc(sizeof(order));
    if(my_new_order == NULL) {
        fprintf(stderr, "Error creating a new order\n");
        exit(0);
    }
    my_new_order->total_weight  = 0.0;
    my_new_order->shipping_cost = 0.0;
    my_new_order->items         = NULL;
    return my_new_order;
}

void print_order(order *my_order) {
    printf("ORDER:\n");
    printf("Total weight: %4.1fkg\n", my_order->total_weight);
    printf("Shipping cost: $%4.2f\n", my_order->shipping_cost);

    order_item *cursor = my_order->items;
    while(cursor != NULL) {
        printf("Item:\n");
        printf("  name: %s\n", cursor->item);
        printf("  quantity: %d\n", cursor->quantity);
        printf("  unit weight: %4.2fkg\n", cursor->weight);
        cursor = cursor->next;
    }
}
