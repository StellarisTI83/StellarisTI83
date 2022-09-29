#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"

#include "pathfinding.h"
#include "systemes.h"

typedef struct NodeStruct Node;

struct NodeStruct {
    int g;
    int h;
    int f;

    int x;
    int y;

    int numero;

    int parent;
    Node* suivant;
};

typedef struct{
    Node *firstNode;
}NodeArray;

static NodeArray *CreateNodeArray(){
    NodeArray *array = NULL;
    array = calloc_count(1, sizeof(NodeArray));
    array->firstNode = NULL;
    return array;
}

static void DestroyNodeArray(NodeArray *array){
    Node *actual_node = array->firstNode;
    Node *precedent_node = NULL;
    while(actual_node != NULL){
        precedent_node = actual_node;
        actual_node = actual_node->suivant;
        free_count(precedent_node);
    }
    free_count(array);
}

static Node* CreateNode(NodeArray *array){
    Node *node = NULL;
    if(array->firstNode == NULL){
        array->firstNode = calloc_count(1, sizeof(Node));
        node = array->firstNode;
    } else {
        node = array->firstNode;
        while(node->suivant != NULL){
            node = node->suivant;
        }
        node->suivant = calloc_count(1, sizeof(Node));
        node = node->suivant;
    }
    node->suivant = NULL;
    return node;
}

static void DestroyNode(NodeArray *array, int place){
    Node *node = NULL;
    Node *precedent_node = NULL;
    int index = 0;
    node = array->firstNode;

    if(place == 0){
        array->firstNode = node->suivant;

        free_count(node);
    } else {
        while((node->suivant != NULL) && (index < place)){
            precedent_node = node;
            node = node->suivant;
            index++;
        }
        precedent_node->suivant = node->suivant;
        free_count(node);
    }
}

static Node* NodeNumber(NodeArray *array, int number){
    Node *node = NULL;
    int index = 0;
    node = array->firstNode;
    while((index < number) && (node->suivant != NULL)){
        node = node->suivant;
        index++;
    }
    return node;
}

static int ArraySize(NodeArray *array){
    Node *node = NULL;
    int index = 0;
    node = array->firstNode;
    while((node != NULL) && (index < 100)){
        node = node->suivant;
        index++;
    }
    return index;
}

static Node* CopyNode(Node *destination, Node *source){
    Node *node;
    node = destination->suivant;
    memcpy(destination, source, sizeof(Node));
    destination->suivant = node;
    return destination;
}

static void ReverseArray(int array[], int size){
    int *aux = NULL, i = 0, j = size - 1;
    aux = calloc_count(1, size * sizeof(int));
    while(i < size){
        aux[j] = array[i];
        j--;
        i++;
    }
    i = 0;
    j = 0;
    while(aux[j] == 0){
        j++;
    }
    while(j < size){
        array[i] = aux[j];
        i++;
        j++;
    }
}

int PathFinding(StarSystem **galaxy, int *path, int debut, int fin, int taillePath){
    NodeArray *openList = NULL;
    NodeArray *closedList = NULL;
    NodeArray *childrenList = NULL;
    Node *start = malloc_count(sizeof(Node));
    Node *end = malloc_count(sizeof(Node));
    Node *current_node = NULL;
    Node *temporary_node = NULL;
    Node *children_node = NULL;

    int index = 0, current_index = 0, children_index = 0, nombre_de_boucles = 0;
    int children[4], parent;


    start->x = starSystem_XGet(galaxy[debut]);
    start->y = starSystem_YGet(galaxy[debut]);
    start->g = 0;
    start->h = 0;
    start->f = 0;
    start->numero = debut;
    start->suivant = NULL;
    start->parent = 0;

    end->x = starSystem_XGet(galaxy[fin]);
    end->y = starSystem_YGet(galaxy[fin]);
    end->g = 0;
    end->h = 0;
    end->f = 0;
    end->numero = fin;

    memset(path, 0, sizeof(int) * taillePath);

    openList = CreateNodeArray();
    closedList = CreateNodeArray();
    childrenList = CreateNodeArray();
    CopyNode(CreateNode(openList), start);

    while((ArraySize(openList) > 0) && (nombre_de_boucles < taillePath)){
        nombre_de_boucles++;
        current_node = openList->firstNode;
        current_index = 0;
        index = 0;

        //recupere l'element le plus petit
        while(index < ArraySize(openList)){
            temporary_node = NodeNumber(openList, index);
            if(current_node->f > temporary_node->f){
                current_node = temporary_node;
                current_index = index;
            }
            index++;
        }

        current_node = CopyNode(CreateNode(closedList), current_node);
        DestroyNode(openList, current_index);

        DestroyNodeArray(childrenList);
        childrenList = CreateNodeArray();

        //écire resultat
        if(current_node->numero == fin){
                current_index = 1;
                path[0] = fin;
                while(current_node->numero != debut){
                    path[current_index] = current_node->parent;
                    parent = current_node->parent;
                    index = 0;
                    while((current_node->numero != parent) && (index < 50)){
                        current_node = NodeNumber(closedList, index);
                        index++;
                    }
                    current_index++;
                }
                while(current_index < 50){
                    path[current_index] = 0;
                    current_index++;
                }
                ReverseArray(path, 50);
                return 0;
        }

        memset(children, 0, sizeof(int) * 4);
        children_index = 0;
        while(children_index < 4){
            children[children_index] = current_node->numero;
            index = hyperlane_DestinationGet(galaxy[current_node->numero], children_index);
            if(index != 255){
                children_node = CreateNode(childrenList);
                children_node->numero = index;
                children_node->x = starSystem_XGet(galaxy[index]);
                children_node->y = starSystem_YGet(galaxy[index]);
                children_node->parent = current_node->numero;
            }
            children_index++;
        }

        children_index = 0;
        index = 0;
        while(children_index < ArraySize(childrenList)){
            while(index < ArraySize(closedList)){
                if(NodeNumber(childrenList, children_index)->numero == NodeNumber(closedList, index)->numero){
                    goto finBoucleEnfants;
                }
                index++;
            }

            NodeNumber(childrenList, children_index)->g = current_node->g + 1;
            NodeNumber(childrenList, children_index)->h = pow(NodeNumber(childrenList, children_index)->x - end->x, 2.0) + pow(NodeNumber(childrenList, children_index)->y - end->y, 2.0);
            NodeNumber(childrenList, children_index)->f = NodeNumber(childrenList, children_index)->g + NodeNumber(childrenList, children_index)->h;

            index = 0;
            while(index < ArraySize(openList)){
                if(NodeNumber(childrenList, children_index)->numero == NodeNumber(openList, index)->numero){
                       if(NodeNumber(childrenList, children_index)->g > NodeNumber(openList, index)->g){
                            goto finBoucleEnfants;
                        }
                }
                index++;
            }
            CopyNode(CreateNode(openList), NodeNumber(childrenList, children_index));
            finBoucleEnfants:
            children_index++;
        }
    }
    return 1;
}
