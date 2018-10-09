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

Pop_list *reproducePop(Pop_list *poplist){
    int counter = 1;
    Pop_node *newnode;
    Pop_list *newPoplist;
    pop_init(&newPoplist);
    newPoplist->count = poplist->count;
    if(poplist->head == NULL) return NULL;
    newnode = cloneNode(poplist->head);
    insertNode(newPoplist,newnode);
    while(counter<poplist->count-1){
        Gene *par1;
        par1 = rouletteSelection(poplist);
        if(randomNumber100()<=5) pop_list_add(newPoplist,poplist->mutate_gene(par1));
        else{
            Gene *par2 = rouletteSelection(poplist);
            pop_list_add(newPoplist,poplist->crossover_genes(par1,par2));
        }
        counter++;

    }
    return newPoplist;
}

void pop_list_add(Pop_list *poplist,Gene *gene){

    Pop_node *newchild = safeMalloc(sizeof(Pop_node));
    newchild->gene = gene;
    newchild->next = NULL;
    insertNode(poplist,newchild);
}

void copyPoplist(Pop_list *dest,Pop_list *poplist){
    pop_set_fns(dest,poplist->create_rand_chrom,poplist->mutate_gene,poplist->crossover_genes,poplist->evaluate_fn);
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
            return curr->gene;
        }
        curr = curr->next;
    }
    return NULL;
}

void popListFree(Pop_node *head){
    Pop_node *tmp ;
    while(head!=NULL){
        tmp = head;
        head = head->next;
        gene_free(tmp->gene);
        free(tmp);
    }
}

void freeNode(Pop_node *node){
    gene_free(node->gene);
    free(node);
}


/* TO DO - other functions as appropriate */
