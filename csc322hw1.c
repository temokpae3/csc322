#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Room {
	int roomID;
	int roomState; //0 = clean, 1 = half dirty, 2 = dirty
	struct Room *roomNorth;   	
	struct Room *roomSouth;
	struct Room *roomEast;
	struct Room *roomWest;
	int creatureLocation; //locates the creatures in the room
	struct Creature *creature[10];
} Room;

typedef struct Creature{
	int creatureID;
	int creatureType; //0 = PC, 1 = Animal, 2 = NPC
	struct Room *r_Location;
} Creature;

int respect = 40;
Creature *PC;
Creature *a_creature;
Room* room;

void look(void);

void clean(Creature *cleanCre);

void dirty(Creature *dirtyCre);

void reaction(Creature *reactCre, int cleanDirty);

int move(Creature *moveCre, Room* nextRoom);

void cleanRemains(Creature* remainCre, Room *r);

void action(Creature *actCre, char token[]);

int checkRoomEmpty(Room *room);

void forceClean(Creature *forceCleanCre, Room *r);

void forceDirty(Creature *forceDirtyCre, Room *r);

//Starts the game
void linkStart(void){
	// Room Creation
	int numOfRooms;
	int state, north, south, east, west;

	printf("Please enter how many rooms you want to have?: ");
	scanf("%d", &numOfRooms);

	room = (Room*) malloc(numOfRooms*sizeof(Room)); //allocates rooms to heap

	printf("Please enter 5 integers for each room in order to indicate: State, North, South, East, West\n");
	printf("Acceptable integers: 0 for clean, 1 for half-dirty, 2 for dirty, and -1 for no neighbor\n");
	for (int i = 0; i < numOfRooms; i++){
		//initialize creatureLocation
		room[i].creatureLocation = 0;

		printf("Room# %d: ",i);
		scanf("%d %d %d %d %d", &state, &north, &south, &east, &west);
		room[i].roomID = i; //set roomID to i so the roomID gets a 0-based index
		room[i].roomState = state;

		//checks for neighbors in the north, south, east and west directions
		if (north == -1){
			room[i].roomNorth = NULL; //if -1 for no neighbor then it's null
		} else {
			room[i].roomNorth = &room[north]; //else, room[i].roomNorth goes to room[north]
		}

		if (south == -1){
			room[i].roomSouth = NULL; //if -1 for no neighbor then it's null
		} else {
			room[i].roomSouth = &room[south]; //else, room[i].roomSouth goes to room[south]
		}

		if (east == -1){
			room[i].roomEast = NULL; //if -1 for no neighbor then it's null
		} else {
			room[i].roomEast = &room[east]; //else, room[i].roomEast goes to room[east]
		}

		if (west == -1){
			room[i].roomWest = NULL; // if -1 for no neighbor then it's null
		} else {
			room[i].roomWest = &room[west]; // else, room[i].roomWest goes to room[west]
		}
	}

	//Creature Creation
	int numOfCreatures;
	int type, roomLocation;

	printf("Please enter how many creatures would you like to have in the game?: ");
	scanf("%d", &numOfCreatures);

	a_creature = (Creature *) malloc(numOfCreatures * sizeof(Creature)); //allocates creatures to heap

	printf("Please enter 2 integers that describe the creatures and the room they're in, respectively.\n");
	printf("Acceptable integers: 0 for PC, 1 for Animal, 2 for NPC.\n");

	for (int i = 0; i < numOfCreatures; i++){
		printf("Creature# %d: ", i);
		scanf("%d %d", &type, &roomLocation); //the type of creature and its location
		a_creature[i].creatureID = i; //assigns names to the creatures
		a_creature[i].creatureType = type; //points a_Creature to int type
		a_creature[i].r_Location = &room[roomLocation]; //r_Location points to room[roomLocation]

		//if creature is PC, call them PC
		if(a_creature[i].creatureType == 0){
			PC = &a_creature[i];
		}
	}

	//puts creatures to their rooms
	for (int i = 0; i < numOfRooms; i++){
		for (int j = 0; j < numOfCreatures; j++){
			if (a_creature[j].r_Location == &room[i]){
				room[i].creature[room[i].creatureLocation] = &a_creature[j]; //creatureLocation goes with a creature array size
				room[i].creatureLocation++; //keeping track of creatureLocation so we know the amount of creatures in the room
			}
		}
	}
}

//gets command inputs from user
void commands(void){
	//user's typed commands section
	char input[15] = "";

	while (strcmp(input, "exit") != 0){ //If user types exit, they exit the game

		if (respect >= 80) { //If the user earned more than 80 points, they win the game
			printf("Congrats, you won the game! Respect points earned: %d\n", respect);
			break;
		} else if (respect <= 0){ //If the user earned less than 0 points, they lose the game
			printf("Sorry, you lost the game :c Respect points earned: %d\n", respect);
			break;
		} else {

			printf("Enter a command: ");
			scanf("%s", input);


			if(strcmp(input, "look") == 0){ //user types look command
				look();
			} else if (strcmp(input, "clean") == 0){ //user types clean command
				clean(PC);
			} else if (strcmp(input, "dirty") == 0){ //user types dirty command
				dirty(PC);
			} else if (strcmp(input, "north") == 0){ //moves Player to the North direction
				if(!checkRoomEmpty(PC->r_Location->roomNorth)){
					printf("Room %d at the north direction is already full! \n", PC->r_Location->roomNorth->roomID);
				} else {
					if(move(PC, PC->r_Location->roomNorth)){
						printf("You left your current room for north\n");
					} else {
						printf("There's no room at that direction.\n");
					}
				}
			} else if (strcmp(input, "south") == 0){ //moves Player to the South direction
				if(!checkRoomEmpty(PC->r_Location->roomSouth)){
					printf("Room %d at the south direction is already full! \n", PC->r_Location->roomSouth->roomID);
				} else {
					if(move(PC, PC->r_Location->roomSouth)){
						printf("You left your current room for south\n");
					} else {
						printf("There's no room at that direction.\n");
					}
				}
			} else if (strcmp(input, "east") == 0){ //moves Pleayer to the East direction
				if(!checkRoomEmpty(PC->r_Location->roomEast)){
					printf("Room %d at the east direction is already full! \n", PC->r_Location->roomEast->roomID);
				} else {
					if(move(PC, PC->r_Location->roomEast)){
						printf("You left your current room for east\n");
					} else {
						printf("There's no room at that direction.\n");
					}
				}
			} else if (strcmp(input, "west") == 0){ //moves Player to the West direction
				if(!checkRoomEmpty(PC->r_Location->roomWest)){
					printf("Room %d at at the west direction is already full! \n", PC->r_Location->roomWest->roomID);
				} else {
					if(move(PC, PC->r_Location->roomWest)){
						printf("You left your current room for west\n");
					} else {
						printf("There's no room at that direction.\n");
					}
				}
			} else if (strstr(input, ":")) { //if player uses ":" to force an action
				char *token = strtok(input, ":"); //token for creatureID number
				int ID = atoi(token); //atoi reads integer elements
				token = strtok(NULL, ":"); //token is element after :
				for (int i = 0; i < PC->r_Location->creatureLocation; i++){
					Creature *cre = PC->r_Location->creature[i];
					if (cre->creatureID == ID){
						action(cre, token);
					}
				}
			} else if (strcmp(input, "help") == 0){ //user types help command for a list of commands
				printf("look: Displays the information of the room you're in\n");
				printf("clean: Cleans the current room you're in and changes the room's state (Dirty -> Half-Dirty -> Clean)\n");
				printf("dirty: Dirties the current room which change the room's state from clean to half-dirty, half-dirty  to dirty\n");
				printf("north: moves player to the north direction\n");
				printf("south: moves player to the south direction\n");
				printf("east: moves player to the east direction\n");
				printf("west: moves player to the west direction\n");
				printf("creatureID:action: forces NPCs and animals with the same ID to clean or dirty the room and moves them to a different direction\n");
				printf("exit: Exit the game\n");
			} else {
				if(strcmp(input, "exit") == 0){ //exit out
					continue;
				}
				printf("Invalid input. Type help for the commands of this game.\n");
			}
		}
	}
       	printf("Thanks for playing, Goodbye!\n");
}

//creature actions
void action(Creature *actCre, char token[10]){
	if (strcmp(token, "look") == 0){
		look(); //user types creatureID:look, displays room information
	} else if (strcmp(token, "clean") == 0){ //user types creatureID:clean, force the creatures to clean
		forceClean(actCre, actCre->r_Location);
	} else if (strcmp(token, "dirty") == 0){ //user types creatureID:dirty, force the creatures to dirty
		forceDirty(actCre, actCre->r_Location);
	} else if (strcmp(token, "north") == 0){ //user types creatureID:north, force the creatures to the north direction
		if (move(actCre, actCre->r_Location->roomNorth)){
			cleanRemains(actCre, PC->r_Location); //if move returns 1 then clean remaining creatures
			printf("You moved %d to room %d at North\n", actCre->creatureID, actCre->r_Location->roomID);
		} else { //creatures would get mad for not moving
			respect -=1;
			if (actCre->creatureType == 1) {
				printf("%d growls at you. Respect is now %d\n", actCre->creatureID, respect);
			} else if (actCre->creatureType == 2) {
				printf("%d grumbles at you. Respect is now %d\n", actCre->creatureID, respect);
			}
		}
	} else if (strcmp(token, "south") == 0){ //user types creatureID:north, force the creatures to the south direction
		if (move(actCre, actCre->r_Location->roomSouth)){
			cleanRemains(actCre, PC->r_Location); //if move returns 1 then clean remaining creatures
			printf("You moved %d to room %d at South\n", actCre->creatureID, actCre->r_Location->roomID);
		} else { //creatures would get mad for not moving
			respect -=1;
			if (actCre->creatureType == 1) {
				printf("%d growls at you. Respect is now %d\n", actCre->creatureID, respect);
			} else if (actCre->creatureType == 2) {
				printf("%d grumbles at you. Respect is now %d\n", actCre->creatureID, respect);
			}
		}
	} else if (strcmp(token, "east") == 0){
		if (move(actCre, actCre->r_Location->roomEast)){
			cleanRemains(actCre, PC->r_Location); //if move returns 1 then clean remaining creatures
			printf("You moved %d to room %d at East\n", actCre->creatureID, actCre->r_Location->roomID);
		} else { //creatures would get mad for not moving
			respect -=1;
			if (actCre->creatureType == 1) {
				printf("%d growls at you. Respect is now %d\n", actCre->creatureID, respect);
			} else if (actCre->creatureType == 2) {
				printf("%d grumbles at you. Respect is now %d\n", actCre->creatureID, respect);
			}
		}
	} else if (strcmp(token, "west") == 0){
		if (move(actCre, actCre->r_Location->roomWest)){
			cleanRemains(actCre, PC->r_Location); //if move returns 1 then clean remaining creatures
			printf("You moved %d to room %d at West\n", actCre->creatureID, actCre->r_Location->roomID);
		} else { //creatures would get mad for not moving
			respect -=1;
			if (actCre->creatureType == 1) {
				printf("%d growls at you. Respect is now %d\n", actCre->creatureID, respect);
			} else if (actCre->creatureType == 2) {
				printf("%d grumbles at you. Respect is now %d\n", actCre->creatureID, respect);
			}
		}
	}
}

char *roomInformation(int roomState) {
	char *roomStats; //pointer at roomStats
	if (roomState == 0){
		roomStats = "clean";
	} else if (roomState == 1) {
		roomStats = "half-dirty";
	} else {
		roomStats = "dirty";
	}
	return roomStats;
}

//for showing the neighbors
typedef enum {
	North, South, East, West
} neighbors;

// shows the neighbors at what direction
void neighborsDir(neighbors n){
	if (n == North){
		printf("%d to the North, ", PC->r_Location->roomNorth->roomID);
	}if (n == South){
		printf("%d to the South, ", PC->r_Location->roomSouth->roomID);
	}if (n == East){
		printf("%d to the East, ", PC->r_Location->roomEast->roomID);
	}if (n == West){
		printf("%d to the West, ", PC->r_Location->roomWest->roomID);
	}
}

//commands look function
void look(void) {
	//shows the room ID and state
	printf("Room %d, %s, neighbor ",PC->r_Location->roomID, roomInformation(PC->r_Location->roomState));

	//shows the neighbors in the look command
	if (PC->r_Location->roomNorth != NULL){ // != NULL means there's a neighbor
		neighborsDir(North);
	}
	if (PC->r_Location->roomSouth != NULL) { // != NULL means there's a neighbor
		neighborsDir(South);
	}
	if (PC->r_Location->roomEast != NULL) { // != NULL means there's a neighbor
		neighborsDir(East);
	}
	if (PC->r_Location->roomWest != NULL) { // != NULL means there's a neighbor
		neighborsDir(West);
	}

	//shows creatures in the rooms
	printf(" contains: \n");
	for (int i = 0; i < PC->r_Location->creatureLocation; i++){	//game loops through the creature array
		Creature *cre = PC->r_Location->creature[i];
		if (cre->creatureType == 0) {
			printf("PC %d\n", PC->creatureID);
		}else if (cre->creatureType == 1){
			printf("Animal %d\n", cre->creatureID);
		} else if (cre->creatureType == 2){
			printf("NPC %d\n", cre->creatureID);
		}
	}

	//Respect points that's left
	printf("Respect points: %d\n", respect);
}

//clean command
void clean(Creature *cleanCre) {
 	int state = cleanCre->r_Location->roomState;
	if(state == 0) {
		printf("The room is already clean!\n"); //if user types clean to an already clean room
	} else { //if the state of the room is half-dirty(1) or dirty(2)
		state -=1; //state would reduce by 1 after the action
		cleanCre->r_Location->roomState = state; //determines state of room one more time
		reaction(cleanCre, 0);
	}
	cleanRemains(cleanCre, cleanCre->r_Location); //cleans remaining creatures after subject creatures left
}

//dirty command
void dirty(Creature *dirtyCre){
	int state = dirtyCre->r_Location->roomState;
	if(state == 2) {
		printf("The room is already dirty!\n"); //if user types dirty to an already clean room
	} else { //if the state of the room is half-dirty(1) or clean(0)
		state +=1; //state would increase by 1 after the action
		dirtyCre->r_Location->roomState = state; //determines state of room one more time
		reaction(dirtyCre, 1);
	}
	cleanRemains(dirtyCre, dirtyCre->r_Location); //cleans remaining creatures after subject creatures left
}

void forceClean(Creature *forceCleanCre, Room *r){
	if(r->roomState == 0){
		printf("The room is already clean!\n");
	} else {
		if(forceCleanCre->creatureType == 1){
			respect += 3;
			printf("%d licks a lot. Respect is now %d\n", forceCleanCre->creatureID, respect);
		} else if(forceCleanCre->creatureType == 2){
			respect -= 3;
			printf("%d grumbles a lot. Respect is now %d\n", forceCleanCre->creatureID, respect);
		}
		r->roomState -= 1;
		reaction(forceCleanCre, 0);
		cleanRemains(forceCleanCre, PC->r_Location); //cleans remaining creatures after subject creatures left
	}
}

void forceDirty(Creature *forceDirtyCre, Room *r){
	if(r->roomState == 2){
		printf("The room is already dirty!\n");
	} else {
		if(forceDirtyCre->creatureType == 1){
			respect -= 3;
			printf("%d growls a lot. Respect is now %d\n", forceDirtyCre->creatureID, respect);
		} else if(forceDirtyCre->creatureType == 2){
			respect += 3;
			printf("%d smiles a lot. Respect is now %d\n", forceDirtyCre->creatureID, respect);
		}
		r->roomState += 1;
		reaction(forceDirtyCre, 1);
		cleanRemains(forceDirtyCre, PC->r_Location); //cleans remaining creatures after subject creatures left
	}
}

//clears the remaining creatures who are still there
void cleanRemains(Creature *remainCre, Room *r){
	int j = 0;
	for (int i = 0; i < r->creatureLocation; i++){
		if(r->creature[i] != NULL){
			r->creature[j] = r->creature[i];
			j++;
		}
	}
	r->creatureLocation = j;
}

void creatureMove(Creature *moveTheCre);

//creature's reations
void reaction(Creature *reactCre, int cleanDirty){
	if (reactCre->creatureType == 1){
		if(reactCre->r_Location->roomState == 2){
			creatureMove(reactCre);
			cleanRemains(reactCre, PC->r_Location);
		}

	} else if (reactCre->creatureType == 2){
		if (reactCre->r_Location->roomState == 0){
			creatureMove(reactCre);
			cleanRemains(reactCre, PC->r_Location);
		}
	}

	for (int i = 0; i < PC->r_Location->creatureLocation; i++){
		Creature *c = PC->r_Location->creature[i];

		if (cleanDirty == 0 && c->creatureType == 1) {
			if(c->creatureID == reactCre->creatureID){
				continue;
			} else {
				respect++; //keep updating respect
				printf("Animal %d licks. Respect is now %d\n", c->creatureID, respect); //Animal reactions are based on the actions
			}
		} else if (cleanDirty == 0 && c->creatureType == 2) {
			if(c->creatureID == reactCre->creatureID){
				continue;
			} else {
				respect--;
				printf("NPC %d grumbles. Respect is now %d\n", c->creatureID, respect); //NPC reactions are based on the actions
				if (c->r_Location->roomState == 0) {
					creatureMove(c);
				}
			}
		} else if (cleanDirty == 1 && c->creatureType == 2) {
			if(c->creatureID == reactCre->creatureID){
				continue;
			} else {
				respect++;
				printf("NPC %d smiles. Respect is now %d\n", c->creatureID, respect); //NPC reactions are based on the actions
			}

		} else if (cleanDirty == 1 && c->creatureType == 1) {
			if(c->creatureID == reactCre->creatureID){
				continue;
			} else {
				respect--;
				printf("Animal %d growls. respect is now %d\n", c->creatureID, respect); //Animal reactions are based on the actions
				if (c->r_Location->roomState == 2) {
					creatureMove(c);
				}
			}
		}
	}
}

void creatureRemoved(Creature *removedCre, Room* r);
void leavesThruRoof(Creature *roofCre);

//moving the creature function
void creatureMove(Creature *moveTheCre){
	int random = rand()%4;
	switch (random){
		case 0:
			if (move(moveTheCre, moveTheCre->r_Location->roomNorth)){
				printf("%d left toward north\n", moveTheCre->creatureID);
			} else if (move(moveTheCre, moveTheCre->r_Location->roomSouth)){
				printf("%d left toward south\n", moveTheCre->creatureID);
			} else if (move(moveTheCre, moveTheCre->r_Location->roomEast)){
				printf("%d left toward east\n", moveTheCre->creatureID);
			} else if (move(moveTheCre, moveTheCre->r_Location->roomWest)){
				printf("%d left toward west\n", moveTheCre->creatureID);
			} else {
				leavesThruRoof(moveTheCre); //creature leaves through the roof
			}
			break;
		case 1:
			if (move(moveTheCre, moveTheCre->r_Location->roomSouth)){
				printf("%d left toward south\n", moveTheCre->creatureID);
			} else if (move(moveTheCre, moveTheCre->r_Location->roomEast)){
				printf("%d left toward east\n", moveTheCre->creatureID);
			} else if (move(moveTheCre, moveTheCre->r_Location->roomWest)){
				printf("%d left toward west\n", moveTheCre->creatureID);
			} else if (move(moveTheCre, moveTheCre->r_Location->roomNorth)){
				printf("%d left toward north\n", moveTheCre->creatureID);
			} else {
				leavesThruRoof(moveTheCre); //creature leaves through the roof
			}
			break;

		case 2:
			if (move(moveTheCre, moveTheCre->r_Location->roomEast)){
				printf("%d left toward east\n", moveTheCre->creatureID);
			} else if (move(moveTheCre, moveTheCre->r_Location->roomWest)){
				printf("%d left toward west\n", moveTheCre->creatureID);
			} else if (move(moveTheCre, moveTheCre->r_Location->roomNorth)){
				printf("%d left toward north\n", moveTheCre->creatureID);
			} else if (move(moveTheCre, moveTheCre->r_Location->roomSouth)){
				printf("%d left toward south\n", moveTheCre->creatureID);
			} else {
				leavesThruRoof(moveTheCre); //creature leaves through the roof
			}
			break;
		case 3:
			if (move(moveTheCre, moveTheCre->r_Location->roomWest)){
				printf("%d left toward west\n", moveTheCre->creatureID);
			} else if (move(moveTheCre, moveTheCre->r_Location->roomNorth)){
				printf("%d left toward north\n", moveTheCre->creatureID);
			} else if (move(moveTheCre, moveTheCre->r_Location->roomSouth)){
				printf("%d left toward south\n", moveTheCre->creatureID);
			} else if (move(moveTheCre, moveTheCre->r_Location->roomEast)){
				printf("%d left toward east \n", moveTheCre->creatureID);
			} else {
				leavesThruRoof(moveTheCre); //creature leaves through the roof
			}
			break;

	}

}

void creatureSelfAction(Creature *selfCre);
void addToRoom(Creature *addingCre, Room *r);

//checks if creature can move to the next room or not function
int move(Creature *moveCre, Room *nextRoom){
	if (nextRoom == NULL || !checkRoomEmpty(nextRoom)){
		return 0;
	} else {
		creatureRemoved(moveCre, moveCre->r_Location); //removes the creature from the previous
		addToRoom(moveCre, nextRoom); //add the creature to the new room
		creatureSelfAction(moveCre); //change the room's state to satisfy the creature
		cleanRemains(moveCre, moveCre->r_Location); //clean remaining creatures after the subject creatures left
		return 1;
	}
}

//removes creature from previous room function
void creatureRemoved(Creature *removedCre, Room* r){
	for (int i = 0; i < r->creatureLocation; i++){
		if(r->creature[i] != NULL){
			if (removedCre->creatureID == r->creature[i]->creatureID){
				r->creature[i] = NULL;
				removedCre->r_Location = NULL;
				break;
			}
		}
	}
}

//add creatures to the new room function
void addToRoom(Creature *addingCre, Room *r){
	addingCre->r_Location = r; //assign roomLocation of the creature to a new room
	r->creature[r->creatureLocation] = addingCre; //add creature cre to the last element of the creature array
	r->creatureLocation = r->creatureLocation + 1; //keep creatureLocation of the room updated
}

//makes creature clean or dirty the room themselves so they can remain in the room
void creatureSelfAction(Creature *selfCre){
	if (selfCre->creatureType == 1 && selfCre->r_Location->roomState == 2){
		selfCre->r_Location->roomState -= 1;
	} else if (selfCre->creatureType == 2 && selfCre->r_Location->roomState == 0){
		selfCre->r_Location->roomState += 1;
	}
}

//the leavesThruRoof function
void leavesThruRoof(Creature *roofCre){
	printf("Uh oh, %d left through the roof! \n", roofCre->creatureID);
	printf("And because you let %d leave through the roof, \n", roofCre->creatureID);

	creatureRemoved(roofCre, PC->r_Location);
	for (int i = 0; i < PC->r_Location->creatureLocation; i++){
		if(PC->r_Location->creature[i] == NULL){
			continue;
		} else {
			if (PC->r_Location->creature[i]->creatureType == 1){
				respect -=1;
				printf("Animal %d growls. Respect is now %d\n", PC->r_Location->creature[i]->creatureID, respect);
			} else if (PC->r_Location->creature[i]->creatureType == 2){
				respect -=1;
				printf("NPC %d grumbles. Respect is now %d\n", PC->r_Location->creature[i]->creatureID, respect);
			}
		}
	}
}

//check if a Room is full or not
int checkRoomEmpty(Room *room){
	if (room->creatureLocation == 10) {
		return 0; //returns true if room is full
	}
		return 1; //returns false if room is not full
}

int main(){
	linkStart(); //Starts the game
	commands();

	//frees the allocated elements
	free(room);
	free(a_creature);

	return 0;
}