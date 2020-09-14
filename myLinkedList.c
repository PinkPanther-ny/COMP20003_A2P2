/** ***********************
* Program written by Alvin Chen (nuoyanc@student.unimelb.edu.au) 9/2020
* 2020s2 UNIMELB COMP20003 assignment 2
*/

#include "myLinkedList.h"


/******************************* Constructors *********************************/
void
Node(Node_t *this, Clue_t *data, Node_t *next){

    assert(this != NULL);
    this -> data = data;
    this -> next = next;
}

Point_t * Point(double x, double y){
    Point_t * this = (Point_t *)malloc(sizeof(Point_t));
    this -> x = x;
    this -> y = y;
    return this;
}

KDT_t * KDT(Clue_t * data, int depth){
    KDT_t * this = (KDT_t *)malloc(sizeof(KDT_t));
    assert(this != NULL);

    this -> parent = NULL;
    this -> left = NULL;
    this -> right = NULL;
    this -> listData = creatLinkedList();
    pushToLinearList(this->listData, data);
    this -> depth = depth;
    return this;
}


/************************** LinkedList operation ******************************/
/* Ceate empty new linked list */
List_t *
creatLinkedList(){
    List_t * list = (List_t *)malloc(sizeof(List_t));
    assert(list != NULL);
    list -> head = NULL;
    list -> foot = NULL;
    return list;
}

/* Write linked list to a given file pointer */
void
fwriteLinkedList(List_t *src, FILE * fp, Point_t keyPoint){
    Node_t * ptr = src->head;

    while(ptr != NULL){
        putClueInfo(ptr->data, fp, keyPoint);
        ptr = ptr->next;
    }
}

/* Push an CLUE item to the head of the list */
List_t *
pushToLinearList(List_t *list, Clue_t *data){
    Node_t *newNode = (Node_t*)malloc(sizeof(Node_t));
    assert(newNode != NULL);

    if(list->head == NULL){
        Node(newNode, data, NULL);
        list -> foot = newNode;
    }else{
        Node(newNode, data, list -> head);
    }

    list -> head = newNode;
    return list;

}


/*************************** KD tree operation ********************************/
/* Add an item from the root */
KDT_t * addToKDT(KDT_t * root, Clue_t * newNode, int * depth){

    *depth += 1;
    if (root == NULL){

        *depth -= 1;
        return KDT(newNode, *depth);
    }

    if(cmp(newNode, root, (*depth)%2) == -1){
        pushToLinearList(root -> listData, newNode);

    }else if (cmp(newNode, root, (*depth)%2)){
        root->left = addToKDT(root->left, newNode, depth);
        root->left->parent = root;

    }else{
        root->right = addToKDT(root->right, newNode, depth);
        root->right->parent = root;

    }
    return root;

}

// Return the parent node of the key point as if
// the key point is in the tree(but not).
// or Return the node if it is exactly the same to the key point
KDT_t * searchKDT(KDT_t * root, Point_t key){

    assert(root!=NULL);

    if(point_cmp(root, key) == 0){
        if(DEBUG){printf("Search direction: EQUAL\n");}
        return root;

    }else if (point_cmp(root, key) < 0){
        if(root->left == NULL){
            if(DEBUG) {printf("Search direction: left == NULL\n");}
            return root;
        }
        if(DEBUG) {printf("Search direction: LEFT\n");}
        return searchKDT(root->left, key);

    }else{
        if(root->right == NULL){
            if(DEBUG) {printf("Search direction: right == NULL\n");}
            return root;
        }
        if(DEBUG) {printf("Search direction: RIGHT\n");}
        return searchKDT(root->right, key);

    }
}

KDT_t * compute_nearest(KDT_t * keyParent, Point_t key, double *nearest){

    while( (keyParent->parent)!=NULL ){
        double absDistance;
        
        Point_t curRoot_p = getClueLocation(
                  keyParent->parent->listData->head->data
        );

        if ( !(keyParent->parent->depth%2) ){ // x axis
            absDistance = fabs(curRoot_p.x - key.x);
        }else{
            absDistance = fabs(curRoot_p.y - key.y);
        }

        if ( absDistance <= (*nearest) ){
            keyParent = keyParent->parent;

        }else{
            break;
        }

    }
    return keyParent;

}

/* Search the item with nearest location recursively */
KDT_t * VLR_search(KDT_t * keyParent, Point_t key, 
                   double *nearest, KDT_t * result, int *compareTime){
    
    if (keyParent==NULL){
        // no compare here.
        return result;
    }
    double curDistance = distanceTo(
        key,
        getClueLocation(keyParent->listData->head->data)
    );

    *compareTime += 1;
    if ( curDistance - *nearest <= ROUNDING_ERROR_MARGIN){

        *nearest = curDistance;
        result = keyParent;

    }
    if(fabs(point_cmp(keyParent->left, key))<*nearest){
        result = VLR_search(keyParent->left, key, nearest, result, compareTime);
    }
    if(fabs(point_cmp(keyParent->right, key))<*nearest) {
        result = VLR_search(keyParent->right, key, nearest, result, compareTime);
    }
    return result;
}


/************************ Basic compare functions *****************************/
int cmp(Clue_t * newData, KDT_t * curRoot, int axis){
    Point_t new_p = getClueLocation(newData);
    Point_t curRoot_p = getClueLocation(curRoot->listData->head->data);

    if(new_p.x == curRoot_p.x && new_p.y == curRoot_p.y){
        return -1;
    }

    if(axis){
        return new_p.x < curRoot_p.x;
    }else{
        return new_p.y < curRoot_p.y;
    }
}

double point_cmp(KDT_t * curRoot, Point_t key){
    if(curRoot==NULL){return INFINITY;}
    int axis = curRoot->depth%2;
    Point_t curRoot_p = getClueLocation(curRoot->listData->head->data);

    if((key.x == curRoot_p.x) && (key.y == curRoot_p.y)){
        return 0;
    }

    if(!axis){
        return (key.x - curRoot_p.x);
    }else{
        return (key.y - curRoot_p.y);
    }

}


/********************* Free data that stored in heap **************************/
void
freeLinkedList(List_t *list) {
    assert(list!=NULL);
    Node_t *cur, *pre;

    cur = list->head;
    while (cur) {
        pre = cur;
        cur = cur->next;

        free(pre->data->censusYear);
        free(pre->data->blockID);
        free(pre->data->propertyID);
        free(pre->data->basePropertyID);

        free(pre->data->clueSmallArea);
        free(pre->data->tradingName);
        free(pre->data->industryCode);
        free(pre->data->industryDescription);

        free(pre->data->xCoord);
        free(pre->data->yCoord);
        free(pre->data->location);
        free(pre->data);

        free(pre);
    }
    free(list);

}

void
freeListNode(List_t *list) {
    assert(list!=NULL);
    Node_t *cur, *pre;

    cur = list->head;
    while (cur) {
        pre = cur;
        cur = cur->next;

        free(pre);
    }
    free(list);
}

void freeKDT_Node(KDT_t * root){
    // No need to free parent,
    // since it will be freed as parent node call free
    freeLinkedList(root->listData);
    free(root->left);
    free(root->right);
}

void LRV_Free(KDT_t * root){
    if (root == NULL){ return; }
    LRV_Free(root->left);
    LRV_Free(root->right);
    freeKDT_Node(root);
}

/********************************************************************/


/* Search the item with nearest location recursively */
void STG2VLR_search(KDT_t * keyParent, Point_t key, 
                   double *nearest, int *compareTime, FILE * fp){
    
    if (keyParent==NULL){
        // no compare here.
        return;
    }
    double curDistance = distanceTo(
        key,
        getClueLocation(keyParent->listData->head->data)
    );

    *compareTime += 1;
    if ( curDistance - *nearest <= ROUNDING_ERROR_MARGIN){
        fwriteLinkedList(keyParent->listData, fp, key);

    }
    if(fabs(point_cmp(keyParent->left, key))<*nearest){
        STG2VLR_search(keyParent->left, key, nearest, compareTime, fp);
    }
    if(fabs(point_cmp(keyParent->right, key))<*nearest) {
        STG2VLR_search(keyParent->right, key, nearest, compareTime, fp);
    }
}