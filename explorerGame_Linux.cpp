/// Dadezana
#include <cstdio>
#include <exception>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <termios.h>
#include "var.h"
using namespace std;



struct rooms{
    char description[200];
    int north, south, east, west;  //numero della stanza nelle varie direzioni
};

int getch(){
   struct termios oldattr, newattr;
   int ch;
   tcgetattr(0, &oldattr);
   newattr=oldattr;
   newattr.c_lflag &= ~( ICANON | ECHO );
   tcsetattr( 0, TCSANOW, &newattr);
   ch=getchar();
   tcsetattr(0, TCSANOW, &oldattr);
   return(ch);
}

int interface(){
    int pos = 0, ascii = 0, pointer[OPTIONS] = {arrowPointer, 32, 32};

    do{
        system("clear");
        cout << char(pointer[0]) << " End program" << endl;
        cout << char(pointer[1]) << " Continue a game" << endl;
        cout << char(pointer[2]) << " New game" << endl;

        ascii = getch();

        switch(ascii){
            case keyS:
                if(pos < OPTIONS-1){
                    pointer[pos] = 32;
                    pos++;
                    pointer[pos] = arrowPointer;
                }
                break;

            case keyW:
                if(pos > 0){
                    pointer[pos] = 32;
                    pos--;
                    pointer[pos] = arrowPointer;
                }
        }
    }while(ascii != keyENTER);

    return pos;     // option indexed by the arrow
}

void resetColor(bool isTorchOn){
    if (isTorchOn) cout << BOLDYELLOW;
    else cout << RESET;
}


///FUNZIONI***********************
void init(struct rooms room[]); // init all the rooms

void homeMap(int current_room);

void undergroundMap(int current_room);

int exploreFirstBedroom(bool isTorchOn, bool mapFound); // return mapFound = true

int exploreSecondBedroom(bool isTorchOn, bool bathroomKeyFound);   //return bathroomKeyFound = true

int exploreBathroom(bool isTorchOn, bool undergroundKeyFound); //return undergroundKeyFound = true

int exploreKitchen(bool torchTakenKitchen);   //return torchs

int exploreBalcony(bool prisonKeyFound);   //return prigione = true

void exploreDiningRoom(bool isTorchOn);

void exploreLunchroom(bool isTorchOn);

int exploreArmory(bool torchTakenArmory);

int exploreFoodStorage(bool *armoryKeyFound, bool isTorchOn);

int explorePrison(bool isTorchOn, int health, int food , char weapon[]);

void wolfStory();
///FUNZIONI***********************

FILE *ptr;
int main(){
    int current_room = 0;   // room the player is in
    int choice;             // direction chosen by the player
    char ascii[1];
    bool exitGameLoop = false;

// if the key has been found
    bool bathroomKeyFound = false, 
        undergroundKeyFound = false, 
        armoryKeyFound = false, 
        prisonKeyFound = false;
    bool mapFound = false;
    
    bool torchTakenKitchen = false, torchTakenArmory = false; // if kitchen and armory torchs have been taken
    bool isTorchOn = false;
    int torchs = 0, torchMovement = 2;                          // the torch is on for 2 movements
    int food = 0;
    int health = 100;
    char weapon[10];
    bool won = false;

    int pointer[2] = {arrowPointer, 32}, selected = 0, asci = 0, pos = 0;    // for menu
    bool endProgram = false;

    strcpy(weapon, PUNCH);

    ///-----INIZIALIZZAZIONE STANZE--------///
    struct rooms room[MaxRoom];
    init(room);

    ///--------MENU---------///
    system("clear");
    while(!endProgram){
            menu:
            cout << RESET;
            pos = interface();
            cout << "pos: " << pos << endl; 

            switch(pos){
                case 0:
                    do{
                        system("clear");
                        cout << "Are you sure to exit? Unsaved data will be lost\n";
                        cout << char(pointer[0]) << " Yes" << endl;
                        cout << char(pointer[1]) << " No" << endl;
                        asci = getch();

                        switch(asci){

                            case keyS:
                                pointer[0] = 32;
                                selected = 1;
                                pointer[1] = arrowPointer;
                                break;

                            case keyW:
                                pointer[1] = 32;
                                selected = 0;
                                pointer[0] = arrowPointer;
                                break;

                        }

                    }while(asci != keyENTER);

                    if(!selected){
                        exit(0);
                    }else{
                        goto menu;  // i had to
                    }
                    break;

                case 1:
                    if(fopen(myFILE, "rb") == NULL){
                        cout << "\n No games saved\n\n";
                        cin.get();
                        cin.get();
                        goto menu;
                    }else{
                        ptr = fopen(myFILE, "rb");
                        fread(&current_room, sizeof(current_room), 1, ptr);
                        fread(&torchs, sizeof(torchs), 1, ptr);
                        fread(&torchTakenArmory, sizeof(torchTakenArmory), 1, ptr);
                        fread(&torchTakenKitchen, sizeof(torchTakenKitchen), 1, ptr);
                        fread(&isTorchOn, sizeof(isTorchOn), 1, ptr);
                        fread(&torchMovement, sizeof(torchMovement), 1, ptr);
                        fread(&health, sizeof(health), 1, ptr);
                        fread(&bathroomKeyFound, sizeof(bathroomKeyFound), 1, ptr);
                        fread(&undergroundKeyFound, sizeof(undergroundKeyFound), 1, ptr);
                        fread(&armoryKeyFound, sizeof(armoryKeyFound), 1, ptr);
                        fread(&prisonKeyFound, sizeof(prisonKeyFound), 1, ptr);
                        fread(&mapFound, sizeof(mapFound), 1, ptr);
                        fclose(ptr);
                        if(isTorchOn) cout << BOLDYELLOW;
                    }
                    system("clear");
                    break;

                case 2:
                    system("clear");
                    cout << GREEN;
                    cout << " Welcome in this Adventure Game!" << endl;
                    cout << " You are in a seemingly abandoned house, with only a backpack, and it is night\n";
                    cout << " Whatever happens, try to survive as long as possible! Good luck! ;)" << endl;
                    cout << " Press enter to continue...\n ";
                    cin.get();
                    cin.get();
                    cout << RESET;

                    exitGameLoop = false;
                    cout << " Press 'h' to see commands\n";
                ///------------LOOP GIOCO----------///
                    while(!exitGameLoop){
                            printf("\n%s", room[current_room].description);
                            choice = getch();

                            if(choice < 97){
                                choice += 32;   // upper case to lowercase (in case of block maiusc on)
                            }

                            switch(choice){

                                //NORD
                                case n:
                                    if(room[current_room].north != -1){
                                        current_room = room[current_room].north; //aggiorno la posizione del mio personaggio
                                        cout << GREY << "You went north\n";
                                        resetColor(isTorchOn);
                                    if(isTorchOn){
                                                torchMovement--;
                                                if(torchMovement == 0){
                                                    isTorchOn = false;
                                                    cout << STATS_UPDATE << " Torch went out\n";
                                                    cout << RESET;
                                                }
                                            }

                                    }else{
                                        cout << GREY << "\n There is no passage to the north\n";
                                        resetColor(isTorchOn);
                                    }

                                    break;

                                //SUD
                                case s:
                                    if(room[current_room].south == 9 && !armoryKeyFound){
                                        cout << SOMETHING_WRONG << "The door is locked\n";
                                        resetColor(isTorchOn);
                                    }else{
                                        if(room[current_room].south != -1){
                                            current_room = room[current_room].south; //aggiorno la posizione del mio personaggio
                                            cout << GREY << "You went south\n";
                                            resetColor(isTorchOn);
                                            if(isTorchOn){
                                                torchMovement--;
                                                if(torchMovement == 0){
                                                    isTorchOn = false;
                                                    cout << STATS_UPDATE << " Torch went out\n" << RESET;
                                                }
                                            }
                                        }else{
                                            cout << GREY << "\n There is no passage to the south\n";resetColor(isTorchOn);
                                        }


                                    }
                                    break;

                                //EST
                                case e:
                                    if(room[current_room].east == 4 && !bathroomKeyFound || room[current_room].east == 8 && !undergroundKeyFound || room[current_room].east == 11 && !prisonKeyFound){     //controllo se � possibile accedere alla stanza
                                        cout << SOMETHING_WRONG << "The door is locked\n";resetColor(isTorchOn);
                                    }else{
                                        if(room[current_room].east != -1){
                                            current_room = room[current_room].east; //aggiorno la posizione del mio personaggio
                                            cout << GREY << "You went east\n";resetColor(isTorchOn);
                                            if(isTorchOn){
                                                torchMovement--;
                                                if(torchMovement == 0){
                                                    isTorchOn = false;
                                                    cout << STATS_UPDATE << " Torch went out\n" << RESET;
                                                }
                                            }
                                        }else{
                                            cout << GREY << "\n There is no passage to the east\n";resetColor(isTorchOn);
                                        }
                                    }

                                    break;

                                //OVEST
                                case w:
                                    if(room[current_room].west != -1){
                                        current_room = room[current_room].west; //aggiorno la posizione del mio personaggio
                                        cout << GREY << "You went west\n";resetColor(isTorchOn);
                                        if(isTorchOn){
                                                torchMovement--;
                                                if(torchMovement == 0){
                                                    isTorchOn = false;
                                                    cout << STATS_UPDATE << " Torch went out\n" << RESET;
                                                }
                                            }
                                    }else{
                                        cout << GREY << "\n There is no passage to the west\n";resetColor(isTorchOn);
                                    }
                                    break;

                                //QUIT
                                case q:
                                    cout << AQUA << "\n Are you sure you want to exit?(y/n):";
                                    cin >> ascii;

                                    if(strcmp(ascii, "y") == 0){
                                        cout << "\n Want to save?(y/n): ";
                                        cin >> ascii;

                                        exitGameLoop = true;            //vuole uscire quindi esco dal ciclo
                                        if(strcmp(ascii, "y") == 0){
                                            cout << "\n Saving...\n";
                                            ptr = fopen(myFILE, "wb");

                                            fwrite(&current_room, sizeof(current_room), 1, ptr);
                                            fwrite(&torchs, sizeof(torchs), 1, ptr);
                                            fwrite(&torchTakenArmory, sizeof(torchTakenArmory), 1, ptr);
                                            fwrite(&torchTakenKitchen, sizeof(torchTakenKitchen), 1, ptr);
                                            fwrite(&isTorchOn, sizeof(isTorchOn), 1, ptr);
                                            // // fwrite(&torchs, sizeof(torchs), 1, ptr);                  //torcie
                                            fwrite(&torchMovement, sizeof(torchMovement), 1, ptr);
                                            fwrite(&health, sizeof(health), 1, ptr);
                                            fwrite(&bathroomKeyFound, sizeof(bathroomKeyFound), 1, ptr);
                                            fwrite(&undergroundKeyFound, sizeof(undergroundKeyFound), 1, ptr);
                                            fwrite(&armoryKeyFound, sizeof(armoryKeyFound), 1, ptr);
                                            fwrite(&prisonKeyFound, sizeof(prisonKeyFound), 1, ptr);
                                            fwrite(&mapFound, sizeof(mapFound), 1, ptr);
                                            fclose(ptr);
                                            cout << " Saved completed\n";
                                        }
                                        cout << RESET;
                                        exit(0);
                                    }
                                    resetColor(isTorchOn);
                                    break;

                                //HELP
                                case h:
                                    cout << STATS << "\n n = north\n s = south\n e = east\n w = west\n q = quit\n m = map\n x = explore\n i = inventory\n t = torch\n";
                                    resetColor(isTorchOn);
                                    break;

                                //MAPPA
                                case m:
                                    if(mapFound){
                                        if(current_room >= 8 && current_room <= 11){
                                            undergroundMap(current_room);
                                        }else{
                                            homeMap(current_room);
                                        }
                                    }else{
                                        cout << STATS_UPDATE << "No map found in inventory. It might be in some room...\n";resetColor(isTorchOn);
                                    }
                                    break;

                                //ESPLORA
                                case x:
                                    if(current_room > 11 || current_room == 2 || current_room == 8){
                                        cout << SOMETHING_WRONG << "Impossible to explore when in a corridor\n";resetColor(isTorchOn);
                                    }else{
                                        switch(current_room){
                                            case 0:
                                                mapFound = exploreFirstBedroom(isTorchOn, mapFound);
                                                break;

                                            case 1:
                                                bathroomKeyFound = exploreSecondBedroom(isTorchOn, bathroomKeyFound);  //diventa = true
                                                break;

                                            case 3:
                                                if(!torchTakenKitchen) food += 5;
                                                torchs += exploreKitchen(torchTakenKitchen);resetColor(isTorchOn);
                                                torchTakenKitchen = true;
                                                break;

                                            case 4:
                                                undergroundKeyFound = exploreBathroom(isTorchOn, undergroundKeyFound);
                                                break;

                                            case 5:
                                                exploreLunchroom(isTorchOn);
                                                break;

                                            case 6:
                                                exploreDiningRoom(isTorchOn);
                                                break;

                                            case 7:
                                                prisonKeyFound = exploreBalcony(prisonKeyFound);resetColor(isTorchOn);
                                                break;

                                            case 9:
                                                torchs += exploreArmory(torchTakenArmory);resetColor(isTorchOn);
                                                strcpy(weapon, KNIFE);
                                                torchTakenArmory = true;
                                                break;

                                            case 10:
                                                food += exploreFoodStorage(&armoryKeyFound, isTorchOn);
                                                break;

                                            case 11:
                                                won = explorePrison(isTorchOn, health, food, weapon);
                                                exitGameLoop = true;
                                                remove(myFILE);
                                                cin.get();
                                                cin.get();
                                                break;
                                        }
                                    }

                                    break;

                                //TORCIA
                                case t:
                                    if (isTorchOn){cout << "Torch is already on"; break;}
                                    if(torchs > 0){
                                        isTorchOn = true;
                                        torchMovement = 2;
                                        torchs--;
                                        cout << BOLDYELLOW;
                                        cout << " Torch on\n";
                                    }else{
                                        cout << SOMETHING_WRONG;
                                        cout << "No torchs available\n";
                                        system("sleep 0.1");
                                        cout << RESET;
                                    }
                                    break;

                                //INVENTARIO
                                case i:
                                    cout << STATS;
                                    cout << "\n Health: " << health << " / 100";
                                    cout << "\n Food: " << food;
                                    cout << "\n Torchs: " << torchs;
                                    cout << "\n Weapon: " << weapon << endl;
                                    resetColor(isTorchOn);
                                    break;

                                //OPZIONE INESISTENTE
                                default:
                                    cout << SOMETHING_WRONG;
                                    cout << "Command not found" << endl;
                                    system("sleep 0.08");
                                    resetColor(isTorchOn);

                                    break;
                            }   //end switch(choice)
                    }       //end game loop
        break;
        } // end switch
    }
    return 0;
}


void init(struct rooms room[]){
    room[0].north = -1; // main bedroom
    room[0].south = 1;
    room[0].east = 12;
    room[0].west = -1;
    strcpy(room[0].description, "\n You are in the main bedroom. The floor makes very strange noises ... \n The passages lead to south and east: ");

    room[1].north = 0; // secondary bedroom
    room[1].south = -1;
    room[1].east = 13;
    room[1].west = -1;
    strcpy(room[1].description, "\n You are in the secondary bedroom. There is an open window ... and it's cold ... brr \n The passages lead to north and east: ");

    room[2].north = 5; // corridor to the stairs, lunchroom and living room
    room[2].south = 6;
    room[2].east = 8;
    room[2].west = 14;
    strcpy(room[2].description, "\n You are in a corridor. There are rooms in the south, north, west and east: ");

    room[3].north = 14; // Kitchen
    room[3].south = -1;
    room[3].east = 6;
    room[3].west = -1;
    strcpy(room[3].description, "\n You are in the kitchen. Oh! .. It smells good! ^^ \n The steps lead to north and east: ");

    room[4].north = -1; // Bathroom
    room[4].south = -1;
    room[4].east = -1;
    room[4].west = 12;
    strcpy(room[4].description, "\n You are in the bathroom. And what a smell. Apparently someone forgot to flush the toilet -.- \n There is only one passage to the west: ");

    room[5].north = -1; // Dining room
    room[5].south = 2;
    room[5].east = -1;
    room[5].west = -1;
    strcpy(room[5].description, "\n You are in the dining room.\n There is only one passage leading to the south: ");

    room[6].north = 2; // Living room
    room[6].south = -1;
    room[6].east = -1;
    room[6].west = 3;
    strcpy(room[6].description, "\n You are in the living room. Unlike other places it looks empty. \n Passages lead north and west: ");

    room[7].north = -1; // Balcony
    room[7].south = 12;
    room[7].east = -1;
    room[7].west = -1;
    strcpy(room[7].description, "\n You are on the balcony. The full moon lights up the landscape. \n There is only one passage leading to the south: ");

    room[8].north = 10; // Underground corridory
    room[8].south = 9;
    room[8].east = 11;
    room[8].west = 2;
    strcpy(room[8].description, "\n You are in the underground corridor. There is ivy everywhere ... \n There are rooms in north, south, east and west: ");

    room[9].north = 8; // Armory
    room[9].south = -1;
    room[9].east = -1;
    room[9].west = -1;
    strcpy(room[9].description, "\n You are in the armory. It is a very bright place. \n There is only one passage leading to the north: ");

    room[10].north = -1; // Food storage
    room[10].south = 8;
    room[10].east = -1;
    room[10].west = -1;
    strcpy(room[10].description, "\n You are in the food storage. There is a certain musty smell ... \n There is only one passage leading to south: ");

    room[11].north = -1; // Prison
    room[11].south = -1;
    room[11].east = -1;
    room[11].west = 8;
    strcpy(room[11].description, "\n You are in the prison room. It is the darkest room in the house... \n There is only a passage to the west: ");

    room[12].north = 7; // Balcony's corridor
    room[12].south = 13;
    room[12].east = 4;
    room[12].west = 0;
    strcpy(room[12].description, "\n You are in a corridor. There are passages in north, south, east and west: ");

    room[13].north = 12; // Secondary bedroom's corridor
    room[13].south = -1;
    room[13].east = 14;
    room[13].west = 1;
    strcpy(room[13].description, "\n You are in a corridor. There are rooms in the north, east and west: ");

    room[14].north = -1; // Bathroom and kitchen's corridor
    room[14].south = 3;
    room[14].east = 2;
    room[14].west = 13;
    strcpy(room[14].description, "\n You are in a corridor. There are rooms in the south, east and west: ");
}

void homeMap(int current_room){
    printf("\n\n");
    printf("               ____________________\n");
    printf("               |                  |\n");
    system("sleep 0.01");
    printf("               |     Balcony      |\n");
    printf("               |                  |\n");
    system("sleep 0.01");
    printf(" ----------------------   --------------------------------\n");
    printf(" |        Main        |   |              |     Dining    |\n");
    system("sleep 0.01");
    printf(" |      bedroom               Bathroom   |      room     |\n");
    printf(" |                    |   |              |               |\n");
    system("sleep 0.01");
    printf(" |                    |   |              |               |\n");
    printf(" |                    |                  |               |\n");
    system("sleep 0.01");
    printf(" ------------ ---------   |---------------- --------------\n");
    printf(" |      Secondary     |                        Stairs --> \n");
    system("sleep 0.01");
    printf(" |       bedroom           ------------ ------- ----------\n");
    printf(" |                    |    |   Kitchen    |    Living    |\n");
    system("sleep 0.01");
    printf(" |                    |    |                     room    |\n");
    printf(" |                    |    |              |              |\n");
    printf(" ---------------------------------------------------------\n");
}

void undergroundMap(int current_room){
    printf("\n\n");
    printf(" ----------------------------------\n");
    printf(" |         Food        |           |\n");
    system("sleep 0.01");
    printf(" |        Storage      |           |\n");
    printf(" |-------------- -------  Prisons  |\n");
    system("sleep 0.01");
    printf(" | <-- Stairs                      |\n");
    printf(" |-------- -------------           |\n");
    system("sleep 0.01");
    printf(" |        Armory       |           |\n");
    printf(" |                     |           |\n");
    printf("  ---------------------------------\n");
}

int exploreFirstBedroom(bool isTorchOn, bool mapFound){
    if(!isTorchOn){
      cout << "\n In the absence of light you struggle to find something useful .... Have you already tried to check if there are any torchs somewhere?";
      return false;
    }else{
        cout << "\n There are old books on the bedside table. ";
        getch();
        cout << "The headlines have faded, but they seem to be about curiosities about the animal world.\n";
        getch();
        cout << " They look interesting. I wonder if there are others in that drawer.";
        getch();
        if(!mapFound){
            cout << " There are some sheets in the tray. One of them appears to be a map\n";
            getch();
            cout << OBJECT_TAKEN << " You found a map!(m)\n\n";
            resetColor(isTorchOn);
        }else{
            cout << " There are only old papers in the drawer\n";
        }

        return true;
    }


}

int exploreSecondBedroom(bool isTorchOn, bool bathroomKeyFound){
    if(!isTorchOn){
      cout << "\n In the absence of light you struggle to find something useful .... Have you already tried to check if there are any torchs somewhere?";
      return false;
    }else{
        cout << "\n There are some beds. ";
        getch();
        cout << "Many of them are scratched. ";
        getch();
        cout << "They look like claws of an animal...";
        getch();
        cout << " and what's more ... a large animal\n";
        if(!bathroomKeyFound){
            getch();
            cout << "You continue exploring and find a key. Who knows what it will open.\n";
            cout << OBJECT_TAKEN << " + 1 key" << endl;
            resetColor(isTorchOn);
        }

        return true; // bathroomKeyFound = true
    }


}

int exploreBathroom(bool isTorchOn, bool undergroundKeyFound){
    if(!isTorchOn){
      cout << "\nIn the absence of light you struggle to find something useful .... First turn on a torch (t)";
      return false;
    }else{
        cout << "\n There is a sink. ";
        getch();
        cout << "But it's broken.";
        getch();
        cout << " the walls are all crumbled and stained with blood\n";
        getch();
        cout << "It looks like a murder scene.\n";
        
        if(!undergroundKeyFound){
            getch();
            cout << " There is also a bathtub. ";
            cout << " Something inside appears to be shimmering.\n";
            getch();
            cout << " it's a key.\n";
            cout << OBJECT_TAKEN << " + 1 key. ";resetColor(isTorchOn);
            getch();
            cout << "Who knows what it will open...\n";
        }

        return true;
    }
}

int exploreKitchen(bool torchTakenKitchen){


    cout << "\n As you enter the kitchen, you notice a glow.";
    getch();
    cout << " There are stoves on and there seems to be a scent of meat ...";
    getch();
    cout << "Could it be that I'm not alone in this dilapidated house?\n";
    if(!torchTakenKitchen){
        
        getch();
        cout << " Objects can be glimpsed through the light of the stove. ";
        cout << "They seems sticks...";
        getch();
        cout << OBJECT_TAKEN << "\n+8 torchs\n";
        cout << "+5 food\n";
        return 8;   // num of torch found
    }else{
        return 0;
    }



}

void exploreLunchroom(bool isTorchOn){
    if(isTorchOn){
        cout << "\n The table is set...";
        getch();
        cout << " and on the plate there are some leftovers of food.";
        getch();
        cout << " Apparently someone ate some hours ago...";
        getch();
        cout << " And that can only mean one thing...\n";
    }else{
        cout << "In the absence of light you struggle to find something useful .... First turn on a torch (t)\n";
    }

}

void exploreDiningRoom(bool isTorchOn){
    if(!isTorchOn){
      cout << "\n In the absence of light you struggle to find something useful... First turn on a torch (t)";
    }else{
        cout << "\n Apart from a battered sofa and a TV there seems to be nothing.";
        getch();
        cout << "Wait a moment... ";
        getch();
        cout << "There appears to be dark hair on the sofa. Whose will they ever be? Maybe a girl?\n";
    }
}

int exploreBalcony(bool prisonKeyFound){
    cout << "\n There are trees all around the house.";
    getch();
    cout << "You see a river flowing, further ahead after the trees. It is just beautiful... :D";
    if(!prisonKeyFound){
        getch();
        cout << "\n Looking closer at the balcony you notice a crack, quite long; ";
        getch();
        cout << " You follow it with your eyes and notice something that glitters.\n";
        getch();
        cout << " It's a key!\n";
        cout << OBJECT_TAKEN << "+1 key\n";
    }
    return true;
}

int exploreArmory(bool torchTakenArmory){

    cout << "This warehouse is really big... ";
    getch();
    cout << "There are some ammunition and some gunpowder on the floor..\n";
    getch();
    cout << "It was once supposed to be a military depot...\n";
    cout << " There are weapons of all kinds, from firearms to simple knives\n ";
    getch();
    cout << " I wonder if they still work.\n";
    getch();
    cout << " *You take some ammo and load it onto a silenced pistol*\n";
    getch();
    cout << " You try to fire a shot at a target, located at the back of the room, but nothing happens\n";

    if(!torchTakenArmory){
        getch();
        cout << " Then you decide to take a simple knife\n";
        getch();
        cout << " Explore the last corner of the warehouse and find some torchs\n";
        cout << OBJECT_TAKEN << " + 1 knife, + 12 torchs\n";
        return 12;
    }else{
        return 0;
    }
}

int exploreFoodStorage(bool* armoryKeyFound, bool isTorchOn){
    if(!isTorchOn){
        cout << "\n In the absence of light you struggle to find something useful. Turn on a torch first(t)" << endl;
        return 0;
    }else{
        cout << "\n The food storage is really big!";
        getch();
        cout << "\n I wonder if any food is still good...";
        if(!*armoryKeyFound){   //se non ha trovato le chiavi (di conseguenza non ha mai esplorato)
            getch();
            cout << " The only food in good condition seems to be bread.\n";
            system("sleep 1");
            cout << OBJECT_TAKEN << " +10 food\n";resetColor(isTorchOn);
            getch();
            cout << " Walking around the room you stumble upon something\n ";
            getch();
            cout << "It's a box; let's see what's inside\n";
            getch();
            cout << "It's a key!\n";
            cout << OBJECT_TAKEN << "+1 key";resetColor(isTorchOn);
            *armoryKeyFound = true;
            return 10;
        }else{
            return 0;
        }

    }
}

int explorePrison(bool isTorchOn, int health, int food, char weapon[]){
    const int punchDamage = -6, knifeDamage = -12, throwKnife = -21, wolfDamage = -20, restore = 15;
    int wolfHP = 80;
    int choice = 0;
    int failAttack = 0;  // if the attack will be successfull or not
    bool hasPlayerAttacked = false; // if the player doesn't attack, the wolf will do the same
    string knife = "No";
    int wolfMisses = 0; // how many time the wolf miss the player
    int playerMisses = 0; // how many time the player miss the wolf
    srand(time(NULL));

    if(!isTorchOn){
        return false;
        cout << "In the absence of light you struggle to find something useful. Turn on a torch first(t)\n";
    }else{
        wolfStory();

        if(strcmp(weapon, KNIFE) == 0){
            cout << "\n You grab the knife firmly in your hand, ready for a fight";
            knife = "Yes";
        }else{
            cout << "\n Clench your fists, ready for a fight; ";
            getch();
            cout << "Perhaps it was better to see if there had been a weapon in the armory...";
        }
        

        cout << endl << "\n\n   " << char(175) << "p = punch(-6hp)\nc = knife(-12hp)\ne = eat(+15hp)\nl = throw knife(-21 hp) " << char(174) << "\n";
        cout << SOMETHING_WRONG << "Warning: Once the knife is thrown, it will no longer be possible to retrieve it\n\n";resetColor(isTorchOn);
         
     // fight loop
        while(health > 0 && wolfHP > 0){
            hasPlayerAttacked = false;

            cout << STATS;
            cout << "\n\n\n Health: " << health << " / 100\t\t\t\t\tLupo: " << wolfHP << " / 220\n";
            cout << " Food: " << food << endl;
            cout << " Knife: " << knife << "\n\n"; // if the player has a knife or not
            resetColor(isTorchOn);

            cout << "  What do you want to do?: ";
            choice = getch();
            switch(choice){
                // PUNCH
                case p:
                // 1/3 possibilities of miss the enemy
                    if(playerMisses == 2){
                        failAttack = 1;
                    }else{
                        failAttack = rand()%3;
                    }

                    hasPlayerAttacked = true;
                    if(failAttack < 2){
                        wolfHP += punchDamage;
                        cout << "Hit hit! the enemy loses " << punchDamage << " hp\n";
                        playerMisses = 0;
                    }else{
                        cout << "Hit missed!\n";
                        playerMisses++;
                    }
                    break;

                // KNIFE
                case c:
                    if(knife == "No"){
                        cout << SOMETHING_WRONG;
                        cout << "You have no knife!\n";
                        system("sleep 0.08");
                        resetColor(isTorchOn);
                    }else{
                        hasPlayerAttacked = true;
                        if(playerMisses == 2){
                            failAttack = 1;
                        }else{
                            failAttack = rand()%3;
                        }

                        if(failAttack < 2){
                            wolfHP += knifeDamage;
                            cout << "Hit hit! the enemy loses " << knifeDamage << " hp\n";
                            playerMisses = 0;
                        }else{
                            cout << "Hit missed!\n";
                            playerMisses++;
                        }
                    }
                    break;


                // THROW KNIFE
                case l:
                    if(knife == "Yes"){
                        wolfHP += throwKnife;
                        cout << FIGHT_UPDATE << "The enemy is surprised by the unexpected move! The enemy loses " << throwKnife << " hp\n";resetColor(isTorchOn);
                        knife = "No";
                        playerMisses = 0;
                    }else{
                        cout << SOMETHING_WRONG;
                        cout << "You have no knife!\n";
                        system("sleep 0.08");
                        resetColor(isTorchOn);
                    }

                    break;

                // EAT
                case e:
                    if(food > 0){
                        cout << "This bread taste good! " << OBJECT_TAKEN << "+15hp\n";resetColor(isTorchOn);
                        health += restore;
                        food--;
                        hasPlayerAttacked = true;
                    }else{
                        cout << SOMETHING_WRONG;
                        cout << "No food left!\n";resetColor(isTorchOn);
                        system("sleep 0.08");
                    }

                    break;
            }

            if(choice == l){
                cout << "\n Opponent's turn: The enemy was surprised by your unexpected move, this turn he didn't attack ";
            
            }else if(hasPlayerAttacked && wolfHP > 0){
                cout << "\n Opponent's turn: ";
                if(wolfMisses == 2){
                    failAttack = 1;    //azzero la varibile per registrare nuovi attacchi mancati
                }else{
                    failAttack = rand()%3;
                }

                if(failAttack < 2){
                    health -= 13 + rand()%9;        //danno da 13 a 21
                    cout << "The wolf hit you with its huge claws! arrghh...\n";
                    wolfMisses = 0;
                }else{
                    wolfMisses++;
                    cout << "Your opponent is tired and missed the hit!\n";
                }

            }
        }

        //Esito finale
        if(health <= 0){
            cout << SOMETHING_WRONG << "\n\n You are dead :(\n The wolf got the better of it" << RESET;
            return false;
        }else{
            cout << OBJECT_TAKEN << "\n\n Congratulations! You won! Now that fur ball won't bother you anymore! XD\n\n";
            return true;
        }


    }
    return false;
}

void wolfStory(){
        cout << "You see the moon from a window..wow..\n";
        getch();
        cout << "But looking around better... Maybe this is the most dismal place you've ever seen...";
        getch();
        cout << "There are a lot of skulls...";
        getch();
        cout << " Moving forward a few steps...";
        getch();
        cout << " You notice on the walls, some blood...";
        getch();
        cout << "And it is fresh.\n";
        getch();
        cout << " In a prison further up, you notice a sack.";
        getch();
        cout << " The closer you get ... the more the bag takes on a well-defined shape...";
        getch();
        cout << " It's a man...";
        getch();
        cout << "And he has a knife in his hand; it seems stained with blood too.\n";
        getch();
        cout << "\nA gust of wind closes the door behind you (The classic, right?). ";
        getch();
        cout << "A shadow moves quickly behind you...";
        getch();
        cout << "It looks like a man. Given the situation, I don't think he has good intentions...\n";
        getch();
        cout << "It starts moving towards you...";
        getch();
        cout << "In fear you can't even move...";
        getch();
        cout << "\nHe passes by you but acts as if he hadn't seen you. ";
        getch();
        cout << "Fiu. Maybe he's not evil.";
        getch();
        cout << "He is positioned under the window; now you can see him well.";
        getch();
        cout << "\nHe looks tired, as if he has been running.";
        getch();
        cout << "\n Looking closer you notice that he has scratches on his face and arms, he looks like he just came out of a fight...";
        getch();
        cout << "And the right arm seems to be in worse shape; has a large wound, not excessively deep, but long";
        getch();
        cout << "\nHe collapses to the ground, apparently in convulsions...\n";
        getch();
        cout << "Long hairs begin to grow all over the body, the chest gets bigger, the face gets longer, the teeth become more pointed, the nails become claws and in no time there is a werewolf..";
        getch();
}