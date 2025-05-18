#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TOTAL_ITEMS 4

static int play = 1;
static int has_totem = 0;

typedef struct{
    char* inventory[TOTAL_ITEMS];
    int health;
} stats;

static void flush_stdin(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void showStats(stats *player) {
    printf("\n[!] Your current health is: %d", player->health);
    printf("\n[!] Your inventory contains:\n");
    for (size_t i = 0; i < TOTAL_ITEMS; i++) {
        printf("[%zu] %s\n", i+1, player->inventory[i]);
    }
}

void setItem(stats *player, int index, char *newItem) {
    if (index >= 0 && index < TOTAL_ITEMS) {
        player->inventory[index] = newItem;
    } else {
        printf("Invalid item");
    }
}

void lose_health(stats *player, int loss){
    player->health -= loss;
    printf("Your health is now: %d\n", player->health);
}

void start() {
    char response; 
    printf("\nAfter days of journeying through the forest, you stumble upon a cave.\nWill you enter? (y/N): ");
    fflush(stdout);
    scanf(" %c", &response);
    flush_stdin();
    while (tolower(response) != 'y') {
        printf("The cave calls to you...\nWill you enter? (y/N): ");
        fflush(stdout);
        scanf(" %c", &response);
        flush_stdin();
    }
}

int choose(char *options, int n){
    int choice;
    do {
        printf("%s", options);
        printf("\nWhat would you like to do?: ");
        if (scanf(" %i", &choice) != 1 || choice > n){
            flush_stdin();
            printf("\n[!] INVALID CHOICE\n");
            continue;
        };
        flush_stdin();
    } while (choice < 1 || choice > n);
    return choice;
}

int room1(stats *player) {
    int choice;
    int next = 0;
    printf("\n\n==================================================[ LEVEL ONE ]==================================================\n\n");
    printf(
        "\nThe air is damp and cool. A wide, dark chasm splits the floor. A single, narrow stone beam stretches precariously to the other side. It looks unstable.\n"
        "Hint: 'A flickering flame may reveal a hidden truth, pried open by steel.'\n"
    );

    while (next != 1){
        if (player->health <= 0){return 0;}
        choice = choose("\n1. Use KNIFE\n2. Use ROPE\n3. Use TORCH\n", 3);
        switch (choice) {
        case 1:
            printf("\n[~] Waving the KNIFE at the chasm doesn't help. Try again.\n");
            break;

        case 2:
            printf("\n[~] Used ROPE to swing across the chasm. Made it across but hurt yourself in the process.\n[!] Challenge Passed.\n");
            lose_health(player, 5);
            printf("\n\n==================================================[ LEVEL TWO ]==================================================\n\n");
            next = 1;
            break;

        case 3:
            printf("\n[~] TORCH light reveals a loose wall section.\n");
            int choice2 = choose("\n1. Use KNIFE\n2. Use ROPE\n", 2);
            switch (choice2) {
                case 1:
                    printf("\n[~] You use your KNIFE to pry open the wall. Secret passage!\n");
                    printf("\n[~] Inside you find a PICKAXE and a GASMASK");
                    int choice3 = choose("\n1. Take PICKAXE\n2. Take GASMASK\n", 2);
                    switch (choice3) {
                        case 1:
                            setItem(player, 3, "PICKAXE");
                            break;
                        case 2:
                            setItem(player, 3, "GASMASK");
                            break;
                    }
                    printf("\n[~] You take the item and slip through the passage into the next room.\n[!] Challenge Passed.\n");
                    printf("\n\n==================================================[ LEVEL TWO ]==================================================\n\n");
                    next = 1;
                    break;

                case 2:
                    printf("\n[~] Used ROPE to swing across the chasm. Made it across but hurt yourself in the process.\n[!] Challenge Passed.\n");
                    lose_health(player, 5);
                    printf("\n\n==================================================[ LEVEL TWO ]==================================================\n\n");
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

int room2(stats *player){
    int choice;
    int step = 0, next = 0;
    int wrong[3] = {0,0,0};
    printf(
        "\nBefore you lies a great abyss. You can't see the bottom. To cross it, you must choose between the right or left tiles of the bridge.\n"
        "Hint: 'In football there is a term called 1 2 1'\n"
    );
    while (next != 1 || step <= 2) {
        if (player->health <= 0){return 0;}
        printf("\n\nTile Set %d...", step+1);
        choice = choose("\n1. LEFT\n2. RIGHT\n", 2);
        if (step == 0){
            switch (choice) {
                case 1:
                    printf("\n[~] Ouch! Spike in the foot. Try again.\n");
                    lose_health(player, 10);
                    wrong[0] = 1;
                    break;
                case 2:
                    printf("\n[~] Good Choice! Keep moving.\n");
                    step++;
                    break;
            }
            continue;
        }
        if (step == 1){
            switch (choice) {
                case 1:
                    printf("\n[~] Nice! Almost there.\n");
                    step++;
                    break;
                case 2:
                    printf("\n[~] Woah! Poison dart!? Try again.\n");
                    lose_health(player, 10);
                    wrong[1] = 1;
                    break;
            }
            continue;
        }
        if (step == 2){
            switch (choice) {
                case 1:
                    printf("\n[~] Falling Boulder!! Try again.\n");
                    lose_health(player, 10);
                    wrong[2] = 1;
                    break;
                case 2:
                    printf("\n[~] You made it! Well done.\n[!] Challenge Passed.\n");
                    step++;
                    if (wrong[0] && wrong[1] && wrong[2]){
                        printf("\n\n\n*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*\n");
                        printf("\n\nWhat's this?\n\nSo many mistakes...\n\nThe gods of the cave feel bad for you.\n\n[!] You've been gifted a HOLY TOTEM and your wounds have been healed.");
                        printf("\n\n*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*\n\n\n");
                        has_totem = 1;
                        player->health = 100;
                        showStats(player);
                    }
                    printf("\n\n==================================================[ LEVEL THREE ]==================================================\n\n");
                    next = 1;
                    break;
            }
            continue;
        }
    }
    return next;
}

int room3(stats *player){
    int choice;
    int next = 0;
    printf(
        "\nA large heavy stone door slams shut behind you. The air begins to fill with thick green gas pouring from a vent in the wall!\n"
    );
    while (next != 1) {
        if (player->health <= 0){return 0;}
        if (player->inventory[3] == "PICKAXE") {
            int choice = choose("\n1. Use KNIFE\n2. Use ROPE\n3. Use TORCH\n4. Use PICKAXE\n", 4);
            switch (choice) {
                case 1:
                    printf("\n[~] You try to disable the vent with a KNIFE as gas burns your eyes, but it's not enough. You need something bigger...\n");
                    lose_health(player, 10);
                    break;

                case 2:
                    printf("\n[~] What is ROPE going to do!? You're wasting valuable time...\n");
                    lose_health(player, 5);
                    break;

                case 3:
                    printf("\n[~] The TORCH ignites the gas and the room explodes!!\n");
                    lose_health(player, player->health);
                    next = 1;
                    break;
                
                case 4:
                    printf("\n[~] You smash the vent with the PICKAXE, taking some damage in the process. The gas stops, revealing the exit.\n[!] Challenge Passed.\n");
                    lose_health(player, 5);
                    printf("\n\n==================================================[ LEVEL FOUR ]==================================================\n\n");
                    next = 1;
                    break;
            }
        }
        if (player->inventory[3] == "GASMASK") {
            int choice = choose("\n1. Use KNIFE\n2. Use ROPE\n3. Use TORCH\n4. Use GASMASK\n", 4);
            switch (choice) {
                case 1:
                    printf("\n[~] You try to disable the vent with a KNIFE as gas burns your eyes, but it's not enough.\n");
                    lose_health(player, 10);
                    break;

                case 2:
                    printf("\n[~] What is ROPE going to do!? You're wasting valuable time...\n");
                    lose_health(player, 5);
                    break;

                case 3:
                    printf("\n[~] The TORCH ignites the gas and the room explodes!!\n");
                    lose_health(player, player->health);
                    next = 1;
                    break;
                
                case 4:
                    printf("\n[~] You put on the GASMASK. You feel your way through the gas to the exit.\n[!] Challenge Passed.\n");
                    printf("\n\n==================================================[ LEVEL FOUR ]==================================================\n\n");
                    next = 1;
                    break;
            }
        }
    }
    return next;
}

int room4(stats *player){
    int choice;
    int next = 0;
    printf(
        "\nA horde of skeletons descends upon you and you soon find yourself surrounded!\n"
    );

    while (next != 1) {
        if (player->health <= 0){return 0;}
        if (has_totem) {
            if (player->inventory[3] == "PICKAXE") {
                int choice = choose("\n1. Use KNIFE\n2. Use ROPE\n3. Use TORCH\n4. Use PICKAXE\n5. Use HOLY TOTEM", 5);
                switch (choice) {
                    case 1:
                        printf("\n[~] You fight them off with the KNIFE but are injured in the process.\n[!] Challenge Passed.\n");
                        lose_health(player, 10);
                        printf("\n\n==================================================[ LEVEL FOUR ]==================================================\n\n");
                        next = 1;
                        break;

                    case 2:
                        printf("\n[~] You try to fend them off with the ROPE, but it doesn't do much...\n");
                        lose_health(player, 5);
                        break;

                    case 3:
                        printf("\n[~] The TORCH only aggravates them and they attack in a frenzy.\n");
                        lose_health(player, player->health);
                        next = 1;
                        break;
                    
                    case 4:
                        printf("\n[~] Wielding the PICKAXE, you manage to fend off the skeletons.\n[!] Challenge Passed.\n");
                        printf("\n\n==================================================[ LEVEL FOUR ]==================================================\n\n");
                        next = 1;
                        break;

                    case 5:
                        printf("\n[~] The skeletons recoil at the sight of the totem. You pass unharmed.\n[!] Challenge Passed.\n");
                        printf("\n\n==================================================[ LEVEL FOUR ]==================================================\n\n");
                        next = 1;
                        break;
                }
            }
            if (player->inventory[3] == "GASMASK") {
                int choice = choose("\n1. Use KNIFE\n2. Use ROPE\n3. Use TORCH\n4. Use GASMASK\n5. Use HOLY TOTEM", 5);
                switch (choice) {
                    case 1:
                        printf("\n[~] You fight them off with the KNIFE but are injured in the process.\n[!] Challenge Passed.\n");
                        lose_health(player, 10);
                        printf("\n\n==================================================[ LEVEL FOUR ]==================================================\n\n");
                        next = 1;
                        break;

                    case 2:
                        printf("\n[~] You try to fend them off with the ROPE, but it doesn't do much...\n");
                        lose_health(player, 5);
                        break;

                    case 3:
                        printf("\n[~] The TORCH only aggravates them and they attack in a frenzy.\n");
                        lose_health(player, player->health);
                        next = 1;
                        break;
                    
                    case 4:
                        printf("\n[~] The skeletons look at each other confused for a moment before they attack.\n");
                        lose_health(player, player->health);
                        next = 1;
                        break;

                    case 5:
                        printf("\n[~] The skeletons recoil at the sight of the totem. You pass unharmed.\n[!] Challenge Passed.\n");
                        printf("\n\n==================================================[ LEVEL FOUR ]==================================================\n\n");
                        next = 1;
                        break;
                }
            }
        } else {
            if (player->inventory[3] == "PICKAXE") {
                int choice = choose("\n1. Use KNIFE\n2. Use ROPE\n3. Use TORCH\n4. Use PICKAXE", 4);
                switch (choice) {
                    case 1:
                        printf("\n[~] You fight them off with the KNIFE but are injured in the process.\n[!] Challenge Passed.\n");
                        lose_health(player, 10);
                        printf("\n\n==================================================[ LEVEL FOUR ]==================================================\n\n");
                        next = 1;
                        break;

                    case 2:
                        printf("\n[~] You try to fend them off with the ROPE, but it doesn't do much...\n");
                        lose_health(player, 5);
                        break;

                    case 3:
                        printf("\n[~] The TORCH only aggravates them and they attack in a frenzy.\n");
                        lose_health(player, player->health);
                        next = 1;
                        break;
                    
                    case 4:
                        printf("\n[~] Wielding the PICKAXE, you manage to fend off the skeletons.\n[!] Challenge Passed.\n");
                        printf("\n\n==================================================[ LEVEL FOUR ]==================================================\n\n");
                        next = 1;
                        break;
                }
            }
            if (player->inventory[3] == "GASMASK") {
                int choice = choose("\n1. Use KNIFE\n2. Use ROPE\n3. Use TORCH\n4. Use GASMASK\n", 4);
                switch (choice) {
                    case 1:
                        printf("\n[~] You fight them off with the KNIFE but are injured in the process.\n[!] Challenge Passed.\n");
                        lose_health(player, 10);
                        printf("\n\n==================================================[ LEVEL FOUR ]==================================================\n\n");
                        next = 1;
                        break;

                    case 2:
                        printf("\n[~] You try to fend them off with the ROPE, but it doesn't do much...\n");
                        lose_health(player, 5);
                        break;

                    case 3:
                        printf("\n[~] The TORCH only aggravates them and they attack in a frenzy.\n");
                        lose_health(player, player->health);
                        next = 1;
                        break;
                    
                    case 4:
                        printf("\n[~] The skeletons look at each other confused for a moment before they attack.\n");
                        lose_health(player, player->health);
                        next = 1;
                        break;

                }
            }
    }
    return next;
    }
}

int game_loop(stats *player){
    start();
    showStats(player);

    // Level One
    while (player->health > 0 && room1(player) != 1) {
        if (player->health <= 0) return 0;
    }

    // Level Two
    while (player->health > 0 && room2(player) != 1) {
        if (player->health <= 0) return 0;
    }

    // Level Three
    while (player->health > 0 && room3(player) != 1) {
        if (player->health <= 0) return 0;
    }

    // Level Four
    while (player->health > 0 && room4(player) != 1) {
        if (player->health <= 0) return 0;
    }

    // Boss Fight

    // Victory!
    return 1;
}

char grade(int hp){
    if (hp == 100) return 'S';
    else if (hp >= 90) return 'A';
    else if (hp >= 80) return 'B';
    else if (hp >= 70) return 'C';
    else if (hp >= 60) return 'D';
    else return 'F';
}

int main() {
    char restart;
    stats player;

    do {
        player = (stats){{"KNIFE", "ROPE", "TORCH", ""}, 100};

        int result = game_loop(&player);

        if (!result){
            do {
                printf("\n[!] You have died. Would you like to restart? (y/N): ");
                scanf(" %c", &restart);
                restart = tolower(restart);
                flush_stdin();
            } while (restart != 'y' && restart != 'n');
        } 
        else {
            printf("\nCONGRATULATIONS, YOU WON!\nYour final HP was: %d\nGrade: %c\n\nPlay again? (y/N): ", 
                player.health, 
                grade(player.health));
            scanf(" %c", &restart);
            restart = tolower(restart);
            flush_stdin();
        }
    } while (restart == 'y');

    printf("\n[!] Thanks for Playing!\n\n");
    return 0;
}
