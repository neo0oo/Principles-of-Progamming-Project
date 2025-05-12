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

int first_room()
{
    int chasm_option = 0;
    
    while (1) {
        printf("\nYou enter a dimly lit room. You see a large chasm with a beam above it. What would you like to do?\n");
        printf("Option 1: Rope Swing across the beam\n");
        printf("Option 2: Use your knife\n");
        printf("Option 3: Use your torch and knife\n");
        printf("Choose an option (1-3): ");
        scanf("%d", &chasm_option);
        getchar(); 

        if (chasm_option == 1){
            printf("You have lost 10 health. ");
            lose_health(100, 10);
            break;
        }
        else if (chasm_option == 2){
            printf("You swing at nothing... That didn't work. Try again.\n");
        }
        else if (chasm_option == 3){
            char item_Option1[20];
            printf("You use your torch and see a loose bit on the wall. You use your knife to pry it open. A secret passage opens.\n");
            printf("2 items are on the floor: a gas mask and a pickaxe. Which one do you pick? Choose wisely: ");
            scanf("%s", item_Option1);
            printf("You picked the %s.\n", item_Option1);
            break;
        }
        else{
            printf("That is not a valid option. Please choose 1, 2, or 3.\n");
        }
    }

    return 0;
}

int main() {
    if (starting_room()) {
        first_room();
    } else {
        printf("You chose not to enter the cave.\n");
    }
    return 0;
}
