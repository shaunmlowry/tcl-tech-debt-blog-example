lappend auto_path [pwd]
package require ordertcl

set my_order [create_order]
add_to_order $my_order "widgets" 5 11.3
add_to_order $my_order "flubbits" 7 10.0 

array set details [get_order_details $my_order]
parray details

set_shipping_cost $my_order [shipping_cost $details(total_weight) 1]

array set details [get_order_details $my_order]
parray details
