all: liborder.so libordertcl.so process_order

%.o : %.c
	gcc -c -I/opt/ActiveTcl-8.6/include -fPIC $< -o $@

liborder.so: order.o
	gcc -shared order.o -o liborder.so

libordertcl.so: order_tcl.o liborder.so
	gcc -shared order_tcl.o -o libordertcl.so -L. -lorder

process_order: process_order.o liborder.so
	gcc -o process_order process_order.o -L. -lorder

test: libordertcl.so
	LD_LIBRARY_PATH=`pwd` tclsh test.tcl

clean:
	rm -f *.o *.so process_order
