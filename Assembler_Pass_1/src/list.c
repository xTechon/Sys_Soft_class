#include "headers.h"

int printLink(LIST table){
    LIST current = table;
    while (table.next != NULL){
        printf("%s\t%d\n", current.node.Name, current.node.Address);
//        current = current.next;
    }
    return 0;
}
