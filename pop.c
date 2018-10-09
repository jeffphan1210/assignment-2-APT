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
    float randNum = (rand()/(float)RAND_MAX + 1);

}
void calculateFitness(Pop_list *poplist,InVTable *invt){
    Pop_node *node = poplist->head;
    double totalFitness = 0;
    int i =0;
    for(;i<invt->width;i++){
        printf("%d \n",invt->table[0][i]);
    }
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

Pop_node *rouletteSelection(Pop_list *poplist)
{
    /*/generate a random number between 0 & total fitness count*/
    double fitnessRandom = (double)(RAND_MAX * 1);
     
    /*go through the chromosones adding up the fitness so far*/
    double fitnessSoFar = 0.0;
    Pop_node * curr = poplist->head;
    if (curr == NULL) return NULL;
    while(curr!= NULL){
        fitnessSoFar += gene_get_fitness(curr->gene);
        if(fitnessSoFar >= fitnessRandom) return curr;
    }
    return NULL;
}

/* TO DO - other functions as appropriate */
