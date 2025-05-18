#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    char* inventory[4];
    int health;
} stats;

static void flush_stdin(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static const int totalItems = 4;
static int play = 1;

char* getItem(stats *player, int index) {
    if (index >= 0 && index < totalItems) {
        return player->inventory[index];
    } else {
        return "Invalid item";
    }
}

void setItem(stats *player, int index, char *newItem) {
    if (index >= 0 && index < totalItems) {
        player->inventory[index] = newItem;
    } else {
        printf("Invalid item");
    }
}

void lose_health(stats *player, int loss){
    player->health -= loss;
    if (player->health <= 0){
        char option_restart;
        do {
            printf("\n[!] You have died. Would you like to restart? (y/N): ");
            scanf(" %c", &option_restart);
            option_restart = tolower(option_restart);
            flush_stdin();
        } while (option_restart != 'y' && option_restart != 'n');
        if (option_restart == 'y'){
            play = 1;
        } else if (option_restart == 'n'){
            play = 0;
        }
    }
    else{
        printf("\n[!] You have %d health\n", player->health);
    }
} 

void start() {
    char response; 
    printf("After days of journeying through the forest, you stumble upon a cave.\nWill you enter? (y/N): ");
    scanf(" %c", &response);
    flush_stdin();
    while (tolower(response) != 'y') {
        printf("The cave calls to you...\nWill you enter? (y/N): ");
        scanf(" %c", &response);
        flush_stdin();
    }
}

int choose(char *options, int n){
    int choice;
    do {
        printf("%s", options);
        printf("\nWhat would you like to do?: ");
        if (scanf(" %i", &choice) != 1){
            flush_stdin();
            printf("\n[!] INVALID CHOICE\n");
            continue;
        };
        flush_stdin();
    } while (choice < 1 || choice > n);
    return choice;
}

int first_room(stats *player) {
    int choice;
    int next = 0;
    printf(
        "\nThe air is damp and cool. A wide, dark chasm splits the floor. A single, narrow stone beam stretches precariously to the other side. It looks unstable.\n"
        "Hint: 'A flickering flame may reveal a hidden truth, pried open by steel.'\n"
    );

    while (next != 1){
        choice = choose("\n1. Use KNIFE\n2. Use ROPE\n3. Use TORCH\n", 3);

        switch (choice) {
        case 1:
            printf("\nWaving the Knife at the chasm doesn't help. Try again.\n");
            break;

        case 2:
            printf("\nUsed Rope to swing across the chasm. Made it across but hurt yourself in the process.\n[!] Challenge Passed.\n");
            lose_health(player, 5);
            next = 1;
            break;

        case 3:
            printf("\nTorch light reveals a loose wall section.\n");
            int choice2 = choose("\n1. Use KNIFE\n2. Use ROPE\n", 2);
            switch (choice2) {
                case 1:
                    printf("\nYou use your Knife to pry open the wall. Secret passage!\n");
                    printf("\nInside you find a PICKAXE and a GASMASK");
                    int choice3 = choose("\n1. Take PICKAXE\n2. Take GASMASK\n", 2);
                    switch (choice3) {
                        case 1:
                            setItem(player, 3, "PICKAXE");
                            break;
                        case 2:
                            setItem(player, 3, "GASMASK");
                            break;
                    }
                    printf("\nYou slip through the passage into the next room.\n[!] Challenge Passed.\n");
                    next = 1;
                    break;

                case 2:
                    printf("\nUsed Rope to swing across the chasm. Made it across but hurt yourself in the process.\n[!] Challenge Passed.\n");
                    lose_health(player, 5);
                    next = 1;
                    break; 
            }
            break;
        default:
            break;
        }
    }
    return next;
}

int main() {
    static stats player = {{"KNIFE", "ROPE", "TORCH", ""}, 100};
    start();
    while (first_room(&player) != 1){}
    printf("\n[!] Thanks for Playing!\n\n");
    return 0;
}
