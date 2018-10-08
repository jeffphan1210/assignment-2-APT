/******************************************************************************
 ** Student name:     ...
 ** Student number:     ...
 ** Course:             Advanced Programming Techniques - S2 2018
 ******************************************************************************/

#include "pop.h"

Boolean pop_init(Pop_list **pop){
    *pop = safeMalloc(sizeof(Pop_list));
    (*pop)->head = NULL;
    (*pop)->count = 0;
    return TRUE;
}

void pop_set_fns(Pop_list *p,CreateFn cf,MutateFn mf,CrossOverFn cof,EvalFn ef){
    p->create_rand_chrom = cf;
    p->mutate_gene = mf;
    p->crossover_genes = cof;
    p->evaluate_fn = ef;
}

void pop_print_fittest(Pop_list *p){
    Pop_node *cur = p->head;
    double min;
    if(cur == NULL) return;
    min = cur->gene->fitness;
    while(cur!=NULL){
        if(cur->gene->fitness <= min) min = cur->gene->fitness;
        cur = cur->next;
    }
    printf("Fittest is : %f", min);
}

Pop_node *create_Node(Pop_list *p,int numAllele){
    Pop_node *newNode = safeMalloc(sizeof(Pop_node));
    newNode->gene = gene_create_rand_gene(numAllele,create_minfn_chrom);
    newNode->next = NULL;
    return newNode;
}
void pop_create_gene(int popsize, Pop_list *poplist, int width){
    int i;
    for(i = 0; i < popsize; i++){
        Pop_node *newNode = create_Node(poplist,width);
        insertNode(poplist,newNode);
    }
}

void insertNode(Pop_list *p,Pop_node *node){
    if(p->head == NULL){
        p->head = node;
        p->count++;
        return;
    }
    node->next = p->head;
    p->head = node;
}

void bubbleSortPop(Pop_list *p){
    int swap;
    Pop_node *tmp,*tmp2 = NULL;
    if( p->head == NULL) return;
    do{
        swap = 0;
        tmp = p->head;
        
        while(tmp->next != tmp2){
            if(tmp->gene->fitness > tmp->next->gene->fitness )
            {
                swapNode(tmp,tmp->next);
                swap = 1;
            }
        }
        tmp2 = tmp;
    }
    while(swap);
}

void swapNode(Pop_node *a, Pop_node *b){
    Gene *tmp = a->gene;
    a->gene = b->gene;
    b->gene = tmp;
}



void printPopList(Pop_list *poplist){
    Pop_node *curr;
    if( poplist->head == NULL){
        printf("null list");
        return;
    }
    curr = poplist->head;
    while(curr->next!= NULL){
        gene_print(curr->gene);
        curr = curr->next;
    }
}


/* TO DO - other functions as appropriate */
