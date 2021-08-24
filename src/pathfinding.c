#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"

#include "systemes.h"
#include "pathfinding.h"

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
    array = calloc(1, sizeof(NodeArray));
    array->firstNode = NULL;
    return array;
}

static void DestroyNodeArray(NodeArray *array){
    Node *actual_node = array->firstNode;
    Node *precedent_node = NULL;
    while(actual_node != NULL){
        precedent_node = actual_node;
        actual_node = actual_node->suivant;
        free(precedent_node);
    }
    free(array);
}

static Node* CreateNode(NodeArray *array){
    Node *node = NULL;
    if(array->firstNode == NULL){
        array->firstNode = calloc(1, sizeof(Node));
        node = array->firstNode;
    } else {
        node = array->firstNode;
        while(node->suivant != NULL){
            node = node->suivant;
        }
        node->suivant = calloc(1, sizeof(Node));
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

        free(node);
    } else {
        while((node->suivant != NULL) && (index < place)){
            precedent_node = node;
            node = node->suivant;
            index++;
        }
        precedent_node->suivant = node->suivant;
        free(node);
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
    aux = calloc(1, size * sizeof(int));
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

int PathFinding(StarSystem **galaxie, int *path, int debut, int fin, int taillePath){
    NodeArray *listeOuverte = NULL;
    NodeArray *listeFermee = NULL;
    NodeArray *listeEnfants = NULL;
    Node *depart = malloc(sizeof(Node));
    Node *arrivee = malloc(sizeof(Node));
    Node *current_node = NULL;
    Node *temporary_node = NULL;
    Node *children = NULL;

    int index = 0, current_index = 0, children_index = 0, nombre_de_boucles = 0;
    int enfant[4], parent;


    depart->x = starSystem_XGet(galaxie[debut]);
    depart->y = starSystem_YGet(galaxie[debut]);
    depart->g = 0;
    depart->h = 0;
    depart->f = 0;
    depart->numero = debut;
    depart->suivant = NULL;
    depart->parent = 0;

    arrivee->x = starSystem_XGet(galaxie[fin]);
    arrivee->y = starSystem_YGet(galaxie[fin]);
    arrivee->g = 0;
    arrivee->h = 0;
    arrivee->f = 0;
    arrivee->numero = fin;

    memset(path, 0, sizeof(int) * taillePath);

    listeOuverte = CreateNodeArray();
    listeFermee = CreateNodeArray();
    listeEnfants = CreateNodeArray();
    CopyNode(CreateNode(listeOuverte), depart);

    while((ArraySize(listeOuverte) > 0) && (nombre_de_boucles < taillePath)){
        nombre_de_boucles++;
        current_node = listeOuverte->firstNode;
        current_index = 0;
        index = 0;

        //recupere l'element le plus petit
        while(index < ArraySize(listeOuverte)){
            temporary_node = NodeNumber(listeOuverte, index);
            if(current_node->f > temporary_node->f){
                current_node = temporary_node;
                current_index = index;
            }
            index++;
        }

        current_node = CopyNode(CreateNode(listeFermee), current_node);
        DestroyNode(listeOuverte, current_index);

        DestroyNodeArray(listeEnfants);
        listeEnfants = CreateNodeArray();

        //écire resultat
        if(current_node->numero == fin){
                current_index = 1;
                path[0] = fin;
                while(current_node->numero != debut){
                    path[current_index] = current_node->parent;
                    parent = current_node->parent;
                    index = 0;
                    while((current_node->numero != parent) && (index < 50)){
                        current_node = NodeNumber(listeFermee, index);
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

        memset(enfant, 0, sizeof(int) * 4);
        children_index = 0;
        while(children_index < 4){
            enfant[children_index] = current_node->numero;
            index = hyperlane_DestinationGet(galaxie[current_node->numero], children_index);
            if(index != 255){
                children = CreateNode(listeEnfants);
                children->numero = index;
                children->x = starSystem_XGet(galaxie[index]);
                children->y = starSystem_YGet(galaxie[index]);
                children->parent = current_node->numero;
            }
            children_index++;
        }

        children_index = 0;
        index = 0;
        while(children_index < ArraySize(listeEnfants)){
            while(index < ArraySize(listeFermee)){
                if(NodeNumber(listeEnfants, children_index)->numero == NodeNumber(listeFermee, index)->numero){
                    goto finBoucleEnfants;
                }
                index++;
            }

            NodeNumber(listeEnfants, children_index)->g = current_node->g + 1;
            NodeNumber(listeEnfants, children_index)->h = pow(NodeNumber(listeEnfants, children_index)->x - arrivee->x, 2.0) + pow(NodeNumber(listeEnfants, children_index)->y - arrivee->y, 2.0);
            NodeNumber(listeEnfants, children_index)->f = NodeNumber(listeEnfants, children_index)->g + NodeNumber(listeEnfants, children_index)->h;

            index = 0;
            while(index < ArraySize(listeOuverte)){
                if(NodeNumber(listeEnfants, children_index)->numero == NodeNumber(listeOuverte, index)->numero){
                       if(NodeNumber(listeEnfants, children_index)->g > NodeNumber(listeOuverte, index)->g){
                            goto finBoucleEnfants;
                        }
                }
                index++;
            }
            CopyNode(CreateNode(listeOuverte), NodeNumber(listeEnfants, children_index));
            finBoucleEnfants:
            children_index++;
        }
    }
    return 1;
}
