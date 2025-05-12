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
        printf("You have %d health", &health);
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

int first_room()
{
    int chasm_option;
    printf("You enter a dimly lit room, you see a large chasm with a beam above it, what would you like to do?\n");
    printf("Option 1: Rope Swing across the beam\n");
    printf("Option 2: Use your knife\n");
    printf("Option 3: Use your torch and knife");
    scanf("%d", &chasm_option);

    if (chasm_option == 1){
        printf("You have lost 10 health");
        lose_health(100, 10);
    }
    else if (chasm_option == 2){
        printf("You swing at nothing, pick another option");
    }
    else if (chasm_option == 3){
        char item_Option1[20];
        printf("You use your torch and see a loose bit on the wall. You then use your knife to ply the loose bit and a secret passage opens.\n");
        printf("2 items are on the floor. There is a gas mask and a pickaxe. Which one do you pick? Choose Wisely: ");
        scanf("%c", &item_Option1);
    }
    else{
        printf("That is not an option, please pick again");
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
