/******************************************************************************
 ** Student name:     ...
 ** Student number:     ...
 ** Course:             Advanced Programming Techniques - S2 2018
 ******************************************************************************/

#include "pop.h"
#include <unistd.h>

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
    newNode->gene = gene_create_rand_gene(numAllele,p->create_rand_chrom);
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
    p->count++;

}

void bubbleSortPop(Pop_list *p){
    Boolean swap = FALSE;
    Pop_node *curr,*tmp2 = NULL;
    curr = tmp2 = p->head;

    if( curr == NULL || curr->next == NULL) return;
    while(!swap){
        swap = TRUE;
        while(curr->next!= NULL){
            if(gene_get_fitness(curr->gene) < gene_get_fitness(curr->next->gene)){
                swap = FALSE;
                swapGene(curr,curr->next);
            }
            curr = curr->next;
        }
        curr = p->head;
    }
}

void swapGene(Pop_node *a, Pop_node *b){
    Gene *tmpGene = a->gene;
    a->gene = b->gene;
    b->gene = tmpGene;
}



void printPopList(Pop_list *poplist){
    Pop_node *curr = poplist->head;
    if( curr == NULL){
        printf("null list");
        return;
    }
    while(curr!= NULL){
        gene_print(curr->gene);
        curr = curr->next;
    }
}

void reproducePop(Pop_list *gen0,Pop_list *gen1){
    int counter = 1;
    Pop_node *newnode;
    gen1->count = gen0->count;

    if(gen0->head == NULL) return;
    newnode = cloneNode(gen0->head);
    insertNode(gen1,newnode); 
    while(counter<gen0->count-1){
        Gene *par1;
        par1 = rouletteSelection(gen0);
        if(randomNumber100()<=5) pop_list_add(gen1,gen1->mutate_gene(par1));
        else{
            Gene *par2 = rouletteSelection(gen0);

            pop_list_add(gen1,gen1->crossover_genes(par1,par2));
        }
        counter++;

    }
}

void pop_list_add(Pop_list *poplist,Gene *gene){
    Pop_node *newchild = safeMalloc(sizeof(Pop_node));
    newchild->gene = gene;
    newchild->next = NULL;
    insertNode(poplist,newchild);
}
Pop_node *cloneNode(Pop_node *node){
    Pop_node *new = safeMalloc(sizeof(Pop_node));
    new->gene = cloneGene(node->gene);
    new->next = NULL;
    return new;
}
void calculateFitness(Pop_list *poplist,InVTable *invt){
    Pop_node *node = poplist->head;
    double totalFitness = 0;
    while(node!=NULL){

        gene_calc_fitness(node->gene,poplist->evaluate_fn,invt);
        totalFitness += node->gene->fitness;
        node = node->next;
    }
    node = poplist->head;
    while(node!=NULL){
        gene_normalise_fitness(node->gene,totalFitness);
        node = node->next;
    }
}

Gene *rouletteSelection(Pop_list *poplist)
{
    Pop_node * curr = poplist->head;
    double fitnessSoFar = 0.0;
    /*/generate a random number between 0 & total fitness count*/
    double fitnessRandom = RANGE * ((((double) rand()) / (double) RAND_MAX)) + MIN_RAND ;

    /*go through the chromosones adding up the fitness so far*/
    if (curr == NULL) return NULL;
    while(curr!= NULL){
        fitnessSoFar += gene_get_fitness(curr->gene);
        if(fitnessSoFar >= fitnessRandom) {
            Gene *clone = cloneGene(curr->gene);
            return clone;
        }
        curr = curr->next;
    }
    return NULL;
}
void swapPopList(Pop_list *p,Pop_list *p1){
    Pop_node *tmphead = p->head;
    p->head = p1->head;
    p1->head = tmphead;
}

void popListFree(Pop_list *poplist){
    freeList(poplist->head);
    poplist->head = NULL;
    poplist->create_rand_chrom = NULL;
    poplist->crossover_genes = NULL;
    poplist->mutate_gene = NULL;
    poplist->evaluate_fn =NULL;
    free(poplist);
}

void freeList(Pop_node *currentNode){
    if(currentNode->next != NULL) freeList(currentNode->next);
    freeNode(currentNode);
}
void freeNode(Pop_node *node){
    gene_free(node->gene);
    free(node);
}


/* TO DO - other functions as appropriate */
