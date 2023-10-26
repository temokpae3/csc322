#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// set up the line
typedef struct Line{
	int valid; // valid bit
	long l_tag; // line tag
	int t_LFU; // tracking the least frequently used
	int t_LRU; // tracking the least recently used
}Line;

void LRUOrg(Line *setLine, int i, int E){ // method to oragize the LRU
	int oldLRU = setLine[i].t_LRU;
	setLine[i].t_LRU = E-1;
	int j = 0;
	while (j < E){
		if ((j != i) && (setLine[j].t_LRU > oldLRU)){
			setLine[j].t_LRU--;
		}
		j++;
	}
}

int main(){
	int S, E, B, m, hT, mP; // set, lines, block size, m bit address, hit time, and miss penalty
	char replacementPolicy[3]; // replacement policy (LRU or LFU)

	// takes the input from stdin to set up the cache sim
	printf("Welcome to the Cache Simulation!\n");
	printf("Enter the set, lines, block size, m bit address, replacement policy, hit time and miss penalty respectively,\n");
	printf("from there, enter integer addresses.\n");
	scanf("%d %d %d %d %s %d %d", &S, &E, &B, &m, replacementPolicy, &hT, &mP);

	// setIndex bits s
	int s = (int)log2(S);
	// offset bits b
	int b = (int)log2(B);
	// tag bits t
	int t = m - (s + b);

	// the cache size C
	int C = B * E * S;

	// misses and hits
	int hits = 0;
	int misses = 0;

	// cycles
	int cycles = 0;

	// cache simulators/setup
	Line **set = malloc(sizeof(Line) *S);
	for(int i = 0; i < S; i++){
		set[i] = malloc(sizeof(Line) *E);
		for(int j = 0; j < E; j++){
			// initializing setline
			set[i][j].valid = 0;
			set[i][j].l_tag = 0;
			set[i][j].t_LRU = j;
			set[i][j].t_LFU = 0;
		}
	}

	// user input
	long userInput = 0;

	// if userInput != -1, go through the loop
	while (userInput != -1){
		scanf("%lx", &userInput);
		if (userInput == -1){
			break;
		}

		// get setIndex
		long setIndex = (userInput >> b) & ((1 << s) - 1); // right shift b to get rid of offset bits.

		// get tag
		long tag = (userInput >> (b + s)) & ((1 << t) - 1); // right shift b + s to get rid of offset and setIndex bits.

		// work with this particular set[setIndex]
		Line *setLine = set[setIndex];

		// direct-mapped cache
		if (E == 1){
			// Miss cases
			if (setLine->valid == 0 || (setLine->valid == 1 && setLine->l_tag != tag)){
				printf("%lx M\n", userInput);
				setLine->valid = 1;
				setLine->l_tag = tag;
				misses++;
				cycles += (hT + mP); // each miss adds miss penalty and hit time to the cycles
			}else{ // Hit cases
				printf("%lx H\n", userInput);
				setLine->l_tag = tag;
				hits++;
				cycles += hT; // each hit adds hit time to the cycles
			}
		}else if((E > 1 && E < C/B) || E == C/B){ // Set associative and Fully associative
			// LRU policy
			int setCapacity = 0;
			for (int i = 0; i < E; i++) {
				// Miss cases
				if (setLine[i].valid == 0){
					printf("%lx M\n", userInput);
					setLine[i].valid = 1;
					setLine[i].l_tag = tag;
					setLine[i].t_LFU = 0; // sets the LFU to 0 on this line
					LRUOrg(setLine, i, E); // Organizes the LRU trackers by updating MRU and LRU everytime 
					misses++;
					cycles += (hT + mP);
					break;
				} else if (setLine[i].l_tag != tag){
					setCapacity++;
				} else { /* hit cases */
					printf("%lx H\n", userInput);
					setLine[i].t_LFU++; // increments the LFU after every hit so that its tracker would be more frequenced
					LRUOrg(setLine, i, E); // Organizes the LRU trackers by updating MRU and LRU everytime a reference is made
					hits++;
					cycles += hT;
					break;
				} // miss cases when eviction is neccessary

				if (setCapacity == E){ // no invalid lines
					misses++;
					cycles += (hT + mP);
					printf("%lx M\n", userInput); // lineTag != tag --> miss
					/*LRU policy*/
					if (strcmp(replacementPolicy, "LRU") == 0){
						int index = 0;
						/*find the LRU cache line*/
						for (int j = 0; j < E; j++){
							if (setLine[j].t_LRU == 0){ // LRU line is the line that has its tracker = 0
								index = j;
							}
						}
						LRUOrg(setLine, index, E); // organizing the LRU cache lines to turn LRU into MRU and update the others
						setLine[index].l_tag = tag; // evicts
					}else if (strcmp(replacementPolicy, "LFU") == 0){
						int leastFreq = setLine[0].t_LFU;
						int index = 0;
						for (int k = 0; k < E; k++){
							if (setLine[k].t_LFU < leastFreq){ // find the minimum tracked LFU
								leastFreq = setLine[k].t_LFU;
								index = k;
							}
						}
						setLine[index].l_tag = tag;
						setLine[index].t_LFU = 0;
					}
				}
			}
		}
	}

	// results line
	double missRate = (double) (misses * 100) / (hits + misses);
	printf("%.f %d \n", missRate, cycles);

	// free memory
	for (int i = 0; i < S; i++){
		free(set[i]); // free all the setLine that got malloc
	}
	free(set); // free the top level set
	return 0;
}