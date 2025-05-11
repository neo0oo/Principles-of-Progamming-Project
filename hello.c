#include <stdio.h>

int starting_room()
{
    char option_1;
    
    printf("After Days of journeying through the forest you stumble upon a cave\n");
    printf("Will you enter?");
    scanf("%c", &option_1);
}

int lose_health(int health, int loss)
{
    health -= loss;
    char option_restart;
    if (health <= 0){
        printf("You have died. Would you like to restart? ");
        scanf("%c",&option_restart);
    }
    else{
        printf("You have %c health", &health);
    }
    return health;
} 

int main(){
    starting_room();
}
