#include <tcl.h>
#include <string.h>
#include "order.h"

static int CreateOrderCmd(
    ClientData clientData,
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj *CONST objv[])
{
    char orderStructKey[24];
    int  madeANewHashEntry;
    Tcl_HashEntry *orderEntry;

    // retrieve the order hashtable from the associated data
    Tcl_HashTable *orderHash = (Tcl_HashTable *)Tcl_GetAssocData(interp, "orders", NULL);
    if(orderHash == NULL) {
        Tcl_AddErrorInfo(interp, "Failed to retrieve order hash");
        return TCL_ERROR;
    }

    // create an order object and store a pointer to it in the hash table
    order *my_order = create_order();
    snprintf(orderStructKey, sizeof(orderStructKey), "order%p", my_order);
    orderEntry = Tcl_CreateHashEntry(orderHash, orderStructKey, &madeANewHashEntry);

    if(orderEntry == NULL) {
        Tcl_AddErrorInfo(interp, "Failed to create order hash entry");
        return TCL_ERROR;
    }
    Tcl_SetHashValue(orderEntry, (ClientData)my_order);

    // return the hash table key for our order
    Tcl_SetObjResult(interp, Tcl_NewStringObj(orderStructKey, strlen(orderStructKey)));

    return TCL_OK;
}

static int AddToOrderCmd(
    ClientData clientData,
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj *CONST objv[])
{
    if(objc != 5) {
        Tcl_AddErrorInfo(interp, 
            "add_to_order: wrong number of args, should be order_handle name quantity weight");
        return TCL_ERROR;
    }

    char    *orderKey     = Tcl_GetString(objv[1]);
    char    *orderName    = Tcl_GetString(objv[2]);
    int     orderQuantity;
    double  orderWeight;
    
    if(Tcl_GetIntFromObj(interp, objv[3], &orderQuantity) != TCL_OK) {
        Tcl_AddErrorInfo(interp, "add_to_order: argument 3 must be an integer");
        return TCL_ERROR;
    }
    
    if(Tcl_GetDoubleFromObj(interp, objv[4], &orderWeight) != TCL_OK) {
        Tcl_AddErrorInfo(interp, "add_to_order: argument 4 must be numeric");
        return TCL_ERROR;
    }

    // retrieve the order hashtable from the associated data
    Tcl_HashTable *orderHash = (Tcl_HashTable *)Tcl_GetAssocData(interp, "orders", NULL);
    if(orderHash == NULL) {
        Tcl_AddErrorInfo(interp, "Failed to retrieve order hash");
        return TCL_ERROR;
    }

    // retrieve the order
    Tcl_HashEntry *orderEntry = Tcl_FindHashEntry(orderHash, orderKey);
    if(orderEntry == NULL) {
        Tcl_AddErrorInfo(interp, "add_to_order: could not find order");
        return TCL_ERROR;
    }

    order *my_order = (order *)Tcl_GetHashValue(orderEntry);
    add_to_order(my_order, orderName, orderQuantity, orderWeight);
    
    Tcl_SetObjResult(interp, Tcl_NewStringObj(orderKey, strlen(orderKey)));

    return TCL_OK;
}

static int SetShippingCostCmd(
    ClientData clientData,
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj *CONST objv[])
{
    if(objc != 3) {
        Tcl_AddErrorInfo(interp, 
            "set_shipping_cost: wrong number of args, should be order_handle cost");
        return TCL_ERROR;
    }

    char   *orderKey     = Tcl_GetString(objv[1]);
    double cost;

    if(Tcl_GetDoubleFromObj(interp, objv[2], &cost) != TCL_OK) {
        Tcl_AddErrorInfo(interp, "set_shipping_cost: argument 1 must be numeric");
        return TCL_ERROR;
    }

    // retrieve the order hashtable from the associated data
    Tcl_HashTable *orderHash = (Tcl_HashTable *)Tcl_GetAssocData(interp, "orders", NULL);
    if(orderHash == NULL) {
        Tcl_AddErrorInfo(interp, "Failed to retrieve order hash");
        return TCL_ERROR;
    }

    // retrieve the order
    Tcl_HashEntry *orderEntry = Tcl_FindHashEntry(orderHash, orderKey);
    if(orderEntry == NULL) {
        Tcl_AddErrorInfo(interp, "add_to_order: could not find order");
        return TCL_ERROR;
    }
    order *my_order = (order *)Tcl_GetHashValue(orderEntry);
    my_order->shipping_cost = cost;

    Tcl_SetObjResult(interp, Tcl_NewStringObj(orderKey, strlen(orderKey)));

    return TCL_OK;
}

static int ShippingCostCmd(
    ClientData clientData,
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj *CONST objv[])
{
    double weight;
    int    quantity;

    if(objc != 3) {
        Tcl_AddErrorInfo(interp, 
            "shipping_cost: wrong number of args, should be: unit_weight quantity");
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[1], &weight) != TCL_OK) {
        Tcl_AddErrorInfo(interp, "shipping_cost: argument 1 must be numeric");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &quantity) != TCL_OK) {
        Tcl_AddErrorInfo(interp, "shipping_cost: argument 2 must be an integer");
        return TCL_ERROR;
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(calculate_shipping_cost(weight, quantity)));

    return TCL_OK;
}

static int GetOrderDetailCmd(
    ClientData clientData,
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj *CONST objv[])
{
    if(objc != 2) {
        Tcl_AddErrorInfo(interp, 
            "get_order_details: wrong number of args, should be order_handle");
        return TCL_ERROR;
    }

    char   *orderKey     = Tcl_GetString(objv[1]);
    // retrieve the order hashtable from the associated data
    Tcl_HashTable *orderHash = (Tcl_HashTable *)Tcl_GetAssocData(interp, "orders", NULL);
    if(orderHash == NULL) {
        Tcl_AddErrorInfo(interp, "Failed to retrieve order hash");
        return TCL_ERROR;
    }

    // retrieve the order
    Tcl_HashEntry *orderEntry = Tcl_FindHashEntry(orderHash, orderKey);
    if(orderEntry == NULL) {
        Tcl_AddErrorInfo(interp, "get_order_details: could not find order");
        return TCL_ERROR;
    }
    order *my_order = (order *)Tcl_GetHashValue(orderEntry);

    Tcl_Obj *result_dict = Tcl_NewDictObj();

    Tcl_DictObjPut(
        interp,
        result_dict,
        Tcl_NewStringObj("total_weight", 12),
        Tcl_NewDoubleObj(my_order->total_weight)
    );

    Tcl_DictObjPut(
        interp,
        result_dict,
        Tcl_NewStringObj("shipping_cost", 13),
        Tcl_NewDoubleObj(my_order->shipping_cost)
    );

    Tcl_SetObjResult(interp, result_dict);

    return TCL_OK;
}

int Ordertcl_Init(Tcl_Interp *interp) {
    /* create a hash table to store pointers to any order structs we create */
    Tcl_HashTable *tblPtr = (Tcl_HashTable *)Tcl_Alloc(sizeof(Tcl_HashTable));
    Tcl_InitHashTable(tblPtr, TCL_STRING_KEYS);

    /* store a pointer to this hashtable in the extensions associated data
     * so we can get at it from anywhere in the extension */

    Tcl_SetAssocData(interp, "orders", NULL, (ClientData)tblPtr);

    Tcl_CreateObjCommand(
        interp, 
        "create_order",
        CreateOrderCmd, 
        (ClientData) 0, 
        (Tcl_CmdDeleteProc *) NULL
    );

    Tcl_CreateObjCommand(
        interp, 
        "add_to_order",
        AddToOrderCmd, 
        (ClientData) 0, 
        (Tcl_CmdDeleteProc *) NULL
    );

    Tcl_CreateObjCommand(
        interp, 
        "shipping_cost",
        ShippingCostCmd, 
        (ClientData) 0, 
        (Tcl_CmdDeleteProc *) NULL
    );

    Tcl_CreateObjCommand(
        interp, 
        "set_shipping_cost",
        SetShippingCostCmd, 
        (ClientData) 0, 
        (Tcl_CmdDeleteProc *) NULL
    );

    Tcl_CreateObjCommand(
        interp, 
        "get_order_details",
        GetOrderDetailCmd, 
        (ClientData) 0, 
        (Tcl_CmdDeleteProc *) NULL
    );

    return(Tcl_PkgProvide(interp, "ordertcl", "0.1"));
}

int Ordertcl_SafeInit(Tcl_Interp *interp) {
    return(Ordertcl_Init(interp));
}
