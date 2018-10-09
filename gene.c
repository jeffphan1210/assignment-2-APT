/******************************************************************************
** Student name: 	...
** Student number: 	...
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "gene.h"

int * create_pcbmill_chrom(int numAlleles){
	int *chrom = safeMalloc(sizeof(int*)*numAlleles);
	int i;
	for(i=0;i<numAlleles;i++){
		chrom[i] = i;
	}
	for ( i = 0; i < numAlleles; i++) {    
		int temp = chrom[i];
		int randomIndex = rand() % numAlleles;
		chrom[i]           = chrom[randomIndex];
		chrom[randomIndex] = temp;
	}
	return chrom;
}

int * create_minfn_chrom(int numAlleles){
	int *chrom = safeMalloc(sizeof(int*)*numAlleles);
	int i;
	for(i=0;i<numAlleles;i++){
		int j;
		while (TRUE){

			j = rand()% MINFN_MAX;
			if(chrom[i]==j) continue;
			else break;
		}
		chrom[i] = j;
	}

	return chrom;
}


Gene * mutate_pcbmill(Gene *g){
	int index1,index2;
	Gene *gene = safeMalloc(sizeof(Gene));
	gene->num_alleles = g->num_alleles;
	gene->chromosome = create_pcbmill_chrom(gene->num_alleles);
	memcpy(gene->chromosome,g->chromosome,sizeof(int*)*gene->num_alleles);
	gene->fitness = 0;
	gene->raw_score = 0;

    #ifdef DEBUG
        index1 = 2;
        index2 = 4;
    #else
        index1 = rand() % gene->num_alleles;
        while (TRUE){
            index2 = rand() % gene->num_alleles;
            if (index1 == index2) break;
        }
    #endif
	
	swap(&gene->chromosome[index1],&gene->chromosome[index2]);
	return gene;
}

Gene * mutate_minfn(Gene *g){
	int index,i;
	Gene *gene = safeMalloc(sizeof(Gene));
	gene->num_alleles = g->num_alleles;
	gene->chromosome = create_minfn_chrom(gene->num_alleles);
	memcpy(gene->chromosome,g->chromosome,sizeof(int *)*gene->num_alleles);
	gene->fitness = 0;
	gene->raw_score = 0;
	#ifdef DEBUG
		index = 2;
	#else
		index = rand() % g->num_alleles;
	#endif
	
	i = rand() % MINFN_MAX;
	gene->chromosome[index] = i;
	return gene;
}

Gene * crossover_pcbmill(Gene *g1, Gene *g2){
    int index1,index2,i,tmp;
    Gene *gene = safeMalloc(sizeof(Gene));
    gene->num_alleles = g1->num_alleles;
    gene->chromosome = safeMalloc(sizeof(int*)*gene->num_alleles);
    gene->fitness = 0;
    gene->raw_score = 0;
#ifdef DEBUG
    index1 = 2;
    index2 = 4;
#else
    index1 = rand() % gene->num_alleles;
    while (TRUE){
        index2 = rand() % gene->num_alleles;
        if(index1 <= index2) {
            printf("i1 : %d, i2: %d\n",index1,index2);
            break;
        }
    }
#endif
    tmp = index1;
    for (i = 0 ; i <= (index2-index1);i++){
        gene->chromosome[i] = g1->chromosome[tmp];
        tmp++;
    }
    tmp = 0;
    for(;i<gene->num_alleles;i++){
        while(TRUE){
            int tmpval = g2->chromosome[tmp];
            if(!checkduplicate(gene->chromosome,tmpval,i)) {
                tmp++;
                continue;
            }
            gene->chromosome[i] =  g2->chromosome[tmp];
            tmp++;
            break;
        }
    }
    return gene;
}

Boolean checkduplicate(int *a, int b,int size){
	int i;
	for (i=0;i < size ;i++){
		if(a[i]==b) return FALSE;
	}
	return TRUE;
}

Gene * crossover_minfn(Gene *g1, Gene *g2){
	int index,i;
	Gene *newGene = safeMalloc(sizeof(Gene));
	newGene->num_alleles = g1->num_alleles;
	newGene->chromosome = safeMalloc(sizeof(int)*newGene->num_alleles);
	newGene->fitness = 0;
	newGene->raw_score = 0;
	#ifdef DEBUG
		index = 2;
	#else
		index = rand() % newGene->num_alleles;
	#endif
	
	i=0;
	for(;i<index+1;i++){
		newGene->chromosome[i] = g1->chromosome[i];
	}

	for(;i<newGene->num_alleles;i++){
		newGene->chromosome[i] = g2->chromosome[i];
	}
	
	return newGene;
}
double eval_pcbmill(InVTable *invt, Gene *gene){
    int i,j;
    double sum=0;
    for(i = 0; i < gene->num_alleles-1;i++){
        j = i;
        j++;
        sum += distance(invt->table[i],invt->table[j]);
        i--;
    }
    return sum;
}

double distance(InVector a, InVector b){
    double tmp = sqrt((pow(b[0]-a[0],2)+pow(b[1]-a[1],2)));
    return tmp;
}

double eval_minfn(InVTable *invt, Gene *gene){
    int j,tmpsum=0;
    double sum=0;
    for(j=0;j<invt->width-1;j++){
        tmpsum += invt->table[0][j]*gene->chromosome[j];
    }
    sum += abs(tmpsum - invt->table[0][INVT_WIDTH-1]);
    return sum;
}

Gene * gene_create_rand_gene(int numAlleles, CreateFn create_chrom){
	Gene *newGene = safeMalloc(sizeof(Gene));
	newGene->chromosome = create_chrom(numAlleles);
	newGene->fitness = newGene->raw_score = 0;
	newGene->num_alleles = numAlleles;
	return newGene;
}

void gene_calc_fitness(Gene *gene, EvalFn evaluate_fn, InVTable *invTab){

    gene->raw_score = evaluate_fn(invTab,gene);
    gene->fitness = 1/(gene->raw_score + 1.0);
}


void gene_normalise_fitness(Gene *gene, double total_fitness){
    gene->fitness = gene_get_fitness(gene)/total_fitness;
}

void gene_free(Gene *gene){
	free(gene->chromosome);
	free(gene);
}



double gene_get_fitness(Gene *gene){
    return gene->fitness;
}

void gene_print(Gene *gene) {
    int i;
    printf("chrom:");
    for(i=0;i<gene->num_alleles;i++){
        printf("%d",gene->chromosome[i]);
        if(i!=gene->num_alleles-1){
            printf(",");
        }
    }
    printf(" fit: %.3f raw: %.3f\n",gene->fitness,gene->raw_score);
}

Gene *cloneGene(Gene *gene){
	Gene *clone = safeMalloc(sizeof(Gene));
	clone->chromosome = safeMalloc(sizeof(gene->chromosome));
	memcpy(clone->chromosome,gene->chromosome,sizeof(int*)*gene->num_alleles);
	clone->fitness = 0;
	clone->num_alleles = gene->num_alleles;
	clone->raw_score = 0;
	return clone;
}

