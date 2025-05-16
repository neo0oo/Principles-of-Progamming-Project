#include <stdio.h>
#include <string.h>

char* getItem(int index) {
    static char* inventory[] = {"Knife", "Rope", "Torch"};
    int totalItems = 3;

    if (index >= 0 && index < totalItems) {
        return inventory[index];
    } else {
        return "Invalid item";
    }
}

int lose_health(int health, int loss)
{
    health -= loss;
    char option_restart;
    if (health <= 0){
        printf("You have died. Would you like to restart? ");
        scanf("%c",&option_restart);
        if (option_restart == "yes"){
            starting_room();
        }
    }
    else{
        printf("You have %d health", health);
    }
    return health;
} 

int initialise_player()
{
    int health = 100;
    //inventory;
}

int starting_room() {
    char response[10]; 
    initialise_player();

    printf("After days of journeying through the forest, you stumble upon a cave.\n");
    printf("Will you enter? (yes/no): ");
    scanf("%s", response);

    if (strcmp(response, "yes") == 0) {
        return 1;  
    } else {
        return 0; 
    }
}

int first_room() {
    char* chosen_item;
    int player_health = 100;

    while (1) {
        printf("\nDimly lit room. Large chasm, beam above. Pick an item or try to cross.\nInventory:\n");
        int total_items = 0;
        while(strcmp(getItem(total_items), "Invalid item") != 0){
            printf("%d: %s\n", total_items + 1, getItem(total_items));
            total_items++;
        }
        printf("%d: Try to cross without an item\nChoose (1-%d): ", total_items + 1, total_items + 1);

        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Enter a number.\n");
            continue;
        }
        getchar(); 

        if (choice >= 1 && choice <= total_items) {
            chosen_item = getItem(choice - 1);
            printf("You use: %s\n", chosen_item);

            if (strcmp(chosen_item, "Rope") == 0) {
                printf("Used Rope to swing across the chasm. Made it safely!\nChallenge Passed.\n");
                return 0;
            } else if (strcmp(chosen_item, "Knife") == 0) {
                printf("Waving the Knife at the chasm doesn't help. Try again.\n");
            } else if (strcmp(chosen_item, "Torch") == 0) {
                printf("Torch light reveals a loose wall section. You use your Knife to pry it open. Secret passage!\n");
                char secret_item_choice[20];
                printf("Items found: 'gasmask', 'pickaxe'. Pick one: ");
                if (scanf("%19s", secret_item_choice) == 1) {
                    getchar(); 
                    if (strcmp(secret_item_choice, "gasmask") == 0 || strcmp(secret_item_choice, "pickaxe") == 0) {
                        printf("Picked %s.\n", secret_item_choice);
                    } else {
                        printf("Didn't pick a valid item from passage.\n");
                    }
                } else {
                    while (getchar() != '\n');
                    printf("No item picked from passage.\n");
                }
                printf("New path found!\n");
                return 0;
            }
        } else if (choice == total_items + 1) {
            printf("Attempting to cross without items... You slip and fall! Ouch!\n");
            lose_health(player_health, 25);
            printf("Climbed back, weakened.\n");
        } else {
            printf("Invalid option.\n");
        }
    } 
}

int main() {
    if (starting_room()) {
        first_room();
    } else {
        printf("You chose not to enter the cave.\n");
    }
    return 0;
}
