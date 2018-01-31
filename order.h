#ifndef _ORDER_H
#define _ORDER_H 1

typedef struct _order_item {
    char               *item;         // item name
    double              weight;        // how much each weighs in kilos
    int                quantity;      // how many
    struct _order_item *next;         // ptr to next item
} order_item;

typedef struct _order {
    double              total_weight;  // weight of order in kilos
    double              shipping_cost; // total calculated shipping cost in dollars
    order_item         *items;        // linked list of items
} order;

extern double calculate_shipping_cost(double weight, int quantity);
extern order  *add_to_order(order *my_order, char *name, int quantity, double weight);
extern order  *create_order();
extern void   print_order(order *my_order);
#endif
