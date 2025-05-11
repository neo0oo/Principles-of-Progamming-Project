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
        printf("You have %c health", &health);
    }
    return health;
} 

int initialise_player()
{
    int health = 100
    inventory
}

int starting_room()
{
    char option_1;
    initialise_player();
    
    printf("After Days of journeying through the forest you stumble upon a cave\n");
    printf("Will you enter?");
    scanf("%c", &option_1);
    
    return &option_1
}


int first_room()
{
    printf("You enter a dimly lit room, you see a large chasm with a beam above it, what would you like to do? ");
    
}


int main(){
    if (starting_room()) == "yes"{
        first_room()        
}

