/******************************************************************************
** Student name: 	...
** Student number: 	...
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "drive.h"
#include "invector.h"
#include "pop.h"
#include "gene.h"

#define MAXINPUT 50
void test_pcbmill(void){
	Gene *gene = gene_create_rand_gene(TEST_ALLELE_LEN,create_pcbmill_chrom);
	Gene *gene2,*gene1,*newGene;
	printf("PCBMILL gene:\n");
	gene_print(gene);
	printf("\nMutate index 2 and 4: \n");
	gene1 = mutate_pcbmill(gene);
	gene_print(gene1);
	printf("\n");
	gene_free(gene);
    gene_free(gene1);


	gene1 = gene_create_rand_gene(TEST_ALLELE_LEN,create_pcbmill_chrom);
	gene2 = gene_create_rand_gene(TEST_ALLELE_LEN,create_pcbmill_chrom);
	printf("PCBMILL genes:\n");
	gene_print(gene1);
	gene_print(gene2);
	printf("\nCrossover index 2 and 4: \n");
	newGene = crossover_pcbmill(gene1,gene2);
	gene_print(newGene);
	gene_free(gene1);
	gene_free(gene2);
	gene_free(newGene);
	
	printf("\n");
}

void test_minfn(void){
	Gene *gene = gene_create_rand_gene(TEST_ALLELE_LEN,create_minfn_chrom);
	Gene *gene2,*gene1,*newGene;
	printf("MINFN gene:\n");
	gene_print(gene);
	printf("\nMutate index 2: \n");
	gene1 = mutate_minfn(gene);
	
	gene_print(gene1);
	gene_free(gene);
    
    gene_free(gene1);
	printf("\n");
	gene1 = gene_create_rand_gene(TEST_ALLELE_LEN,create_minfn_chrom);
	gene2 = gene_create_rand_gene(TEST_ALLELE_LEN,create_minfn_chrom);
	printf("MINFN genes:\n");
	gene_print(gene1);
	printf("\n");
	gene_print(gene2);
	printf("\nCrossover index 2: \n");
	newGene = crossover_minfn(gene1,gene2);

	gene_print(newGene);
	gene_free(gene1);
	gene_free(gene2);
	gene_free(newGene);
	
	printf("\n");
}

int main(int argc, char *argv[]){
#ifdef DEBUG
    test_minfn();
    test_pcbmill();
#else
    srand(SRAND_SEED);
    mainFunction(argc,argv);
#endif
    
    return EXIT_SUCCESS;
}

int validArgu(int argc, char *argv[],FILE *file,InVTable *invt ){
    char input[MAXINPUT];
    int i=0,inputI, counter=0;
    int size = checkStrtol(argv[alleleSize]);
    if(!(argc == CMD_ARG_MAX || argc == CMD_ARG_MAX - 1)){
        printf("main: incorrect number of arguments\n");
        return EXIT_FAILURE;
    }
    if(file == NULL) {
        printf("main: incorrect number of arguments\n");
        return EXIT_FAILURE;
    }
    if((strcmp(argv[geneType],CMD_ARG_PCBMILL)==0)){
        if(size != 2) {
            printf("incorrect 2nd argument\n");
            return EXIT_FAILURE;
        }
    }
    while (fgets(input,INVT_MAX,file)!= NULL){
        int tmpCounter = counter;
        checkOverflow(input);
        input[strlen(input) - 1] = '\0';
        inputI = convertInput(input,invt->table[i],&counter,size);
        if( i != inputI /*TODO*/){
            printf("I'm here i = %d, inputI = %d, tmpCounter = %d, counter = %d",i,inputI,tmpCounter,counter);
            printf("allelesize mismatch with vector size\n");
            return EXIT_FAILURE;
        }
        invt->tot +=1;
        i++;
    }
    invt->width = counter;
    if(!checkType(argv[geneType],inputI,size,counter)){
        printf("allelesize mismatch with vector size\n");
        return EXIT_FAILURE;
    };
    return EXIT_SUCCESS;
}

int mainFunction(int argc, char *argv[]){
    Pop_list *poplist,*tmpPopList;
    /*Pop_node *curr;*/
    InVTable *invt;
    int count = 0;
    int size = checkStrtol(argv[alleleSize]);
    int populationSize = checkStrtol(argv[popSize]);
    int numberGen = checkStrtol(argv[numGen]);
    FILE *file = fopen(argv[inputFile],"r");
    invt = safeMalloc(sizeof(InVTable));
    invector_init(invt);
    if(validArgu(argc,argv,file,invt)) return EXIT_FAILURE;
    fclose(file);
    while(count<numberGen){
        if(count == 0){
            pop_init(&poplist);
            pop_setup(argv[geneType],poplist);
            pop_create_gene(populationSize,poplist,size);
            tmpPopList = poplist;
        }
        else {
            tmpPopList = reproducePop(poplist);
            
        }
    
        printf("\n");
        printPopList(tmpPopList);
        for(int i = 0;i<4;i++){
            printf("invt %d ",invt->table[0][i]);
        }
        calculateFitness(tmpPopList,invt);
        bubbleSortPop(tmpPopList);
        printPopList(tmpPopList);
        poplist = tmpPopList;
        count++;
    }
    return EXIT_SUCCESS;
}


void pop_setup(char *type,Pop_list *poplist){
    if(strcmp(type,CMD_ARG_MINFN)==0){
        pop_set_fns(poplist,create_minfn_chrom,mutate_minfn,crossover_minfn,eval_minfn);
    }
       
    else if (strcmp(type,CMD_ARG_PCBMILL)==0)
        pop_set_fns(poplist,create_pcbmill_chrom,mutate_pcbmill,crossover_pcbmill,eval_pcbmill);
}

int checkType(char *type,int i,int size,int counter){
    if(strcmp(type,CMD_ARG_MINFN)==0 && size < INVT_WIDTH && size >0) {
        if (i==0 && counter == size) {
            return 1;
        }
        return 0;
    }
    else if(strcmp(type,CMD_ARG_PCBMILL)==0  && i>0 && size == 2) return 1;
    else return 0;
}

/*
Boolean validStringTok(char *str){
	if (str == NULL){
		printf("printInvalidInput");
		return FALSE;
	}
	return TRUE;
}

Boolean validInput(char *argv[],int argc,int alleSize){
	int i=1;
	if((strcmp(argv[i],CMD_ARG_MINFN) !=0 && strcmp(argv[i],CMD_ARG_PCBMILL)) !=0 ) {
		printf("incorrect 2nd argument");
		return FALSE;
	}
	i++;
	if(argv[i]!= alleleSize){
		printf("allelesize mismatch with vector size");
		return FALSE;
	}
	return FALSE;
} */
