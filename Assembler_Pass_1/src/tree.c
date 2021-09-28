#include "headers.h"

TREE* root;

int PushLeaf(SYMBOL leaf){
    TREE* newNode;
    int cmpr;
    newNode = (TREE*)malloc(sizeof(TREE));
    newNode->node = leaf;

    if (root == NULL){
        root = newNode;
        newNode->left = NULL;
        newNode->right = NULL;
        return 1;
    }else{
        TREE* cur = root;
        TREE* par;
        int exit = 0;
        while(!exit){
            par = cur;
            //run compare of newNode against current
            cmpr = strcmp(newNode->node.Name, cur->node.Name);
            //goto right node
            if (cmpr > 0){
                cur = cur->right;
                if (cur == NULL){par->right = newNode;return 1;}
            }else if (cmpr < 0){
                cur = cur ->left;
                if (cur == NULL){par->left = newNode;return 1;}
            }else{//error case
                printf("\nERROR: DUPLICATE SYMBOL \"%s\"\n", newNode->node.Name);
                return 0;
            }
        }
    }
    return 1;
}

SYMBOL FindSymbol(char* leaf){
    TREE* cur = root;
    int test = 1;
    while(test != 0){
        test = strcmp(leaf, cur->node.Name);
        if (test != 0){
            if (test < 0){
                cur = cur->left;
            }else if (test > 0){
                cur = cur->right;
            }
        }else break;
    }
    return cur->node;
}

int PrintTree(){
    TraversInOrder(root);
    return 0;
}

TREE* TraversInOrder(TREE* localRoot){
    if (localRoot != NULL){
           TraversInOrder(localRoot->left);
           printf("\n%s\t%x", localRoot->node.Name, localRoot->node.Address);
           TraversInOrder(localRoot->right);
        }
    return localRoot;
}
