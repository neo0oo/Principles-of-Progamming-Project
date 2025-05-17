#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>

#define MAX_INPUT 100

typedef struct {
    const char *description;
    const char *correct_response;
    int damage;
} BossAttack;

int timed_input(char *buffer, int size, int timeout_seconds) {
    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    timeout.tv_sec = timeout_seconds;
    timeout.tv_usec = 0;

    int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
    if (rv <= 0) return 0;
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    return 1;
}

int main() {
    srand(time(NULL));
    int player_hp = 100, boss_hp = 100;
    char input[MAX_INPUT];
    int dodged = 0;

    BossAttack attacks[] = {
        {"swings a sword", "duck", 10},
        {"throws a fireball", "jump", 15},
        {"charges at you", "block", 20},
        {"stabs low", "jump", 10},
        {"casts lightning", "block", 15}
    };
    int num_attacks = sizeof(attacks) / sizeof(BossAttack);

    printf("⚔️  Final Boss Battle Begins!\n");

    while (player_hp > 0 && boss_hp > 0) {
        // --- Boss attack ---
        BossAttack atk = attacks[rand() % num_attacks];
        dodged = 0;

        printf("\nBoss %s! (Options: duck | jump | block)\n", atk.description);
        printf("React in 5 seconds: "); fflush(stdout);

        int responded = timed_input(input, MAX_INPUT, 5);
        if (!responded || strcmp(input, atk.correct_response) != 0) {
            printf("\nYou failed to react! Lost %d HP.\n", atk.damage);
            player_hp -= atk.damage;
        } else {
            printf("✅ Dodged it!\n");
            dodged = 1;
        }

        if (player_hp <= 0) break;

        // --- Player attack ---
        if (dodged) {
            printf("\nYour turn! Attack with: slash | kick\n");
            printf("You have 5 seconds: "); fflush(stdout);

            responded = timed_input(input, MAX_INPUT, 5);
            if (!responded) {
                printf("\nToo slow! You missed your chance.\n");
            } else if (strcmp(input, "slash") == 0 || strcmp(input, "kick") == 0) {
                int dmg = 15 + rand() % 6;  // 15–20 damage
                printf("💥 You hit the boss for %d damage!\n", dmg);
                boss_hp -= dmg;
            } else {
                printf("Invalid move. You missed.\n");
            }
        } else {
            printf("\nYou couldn't attack — still recovering!\n");
        }

        printf("\n❤️ You: %d HP | 👹 Boss: %d HP\n", player_hp, boss_hp);
    }

    if (player_hp <= 0) printf("\n💀 You died. Game Over.\n");
    else printf("\n🏆 You defeated the boss! Victory!\n");

    return 0;
}
