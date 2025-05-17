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
    const char *type;  // "magic" or "physical"
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
        {"swings a sword", "duck", 10, "physical"},
        {"throws a fireball", "jump", 15, "magic"},
        {"charges at you", "block", 20, "physical"},
        {"stabs low", "jump", 10, "physical"},
        {"casts lightning", "block", 15, "magic"}
    };
    int num_attacks = sizeof(attacks) / sizeof(BossAttack);
    BossAttack last_attack;

    printf("⚔️  Final Boss Battle Begins!\n");

    while (player_hp > 0 && boss_hp > 0) {
        // --- Boss attack ---
        last_attack = attacks[rand() % num_attacks];
        dodged = 0;

        printf("\nBoss %s! (Options: duck | jump | block)\n", last_attack.description);
        printf("React in 5 seconds: "); fflush(stdout);

        int responded = timed_input(input, MAX_INPUT, 5);
        if (!responded || strcmp(input, last_attack.correct_response) != 0) {
            printf("\nYou failed to react! Lost %d HP.\n", last_attack.damage);
            player_hp -= last_attack.damage;
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
                int dmg = 10 + rand() % 6; // base 10–15
                int bonus = 5;

                if ((strcmp(input, "slash") == 0 && strcmp(last_attack.type, "physical") == 0) ||
                    (strcmp(input, "kick") == 0 && strcmp(last_attack.type, "magic") == 0)) {
                    dmg += bonus;
                    printf("💥 Super effective! Bonus damage.\n");
                }

                printf("You hit the boss for %d damage!\n", dmg);
                boss_hp -= dmg;
            } else {
                printf("Invalid move. You missed.\n");
            }
        } else {
            printf("\nYou couldn't attack — still recovering!\n");
        }

        printf("\n❤️ You: %d HP | 👹 Boss: %d HP\n", player_hp, boss_hp);
    }

    if (player_hp <= 0) {
        printf("\n💀 You died. Game Over.\n");
    } else {
        printf("\n🏆 You defeated the boss! Victory!\n");

        printf("Your final HP: %d\n", player_hp);
        printf("Score: ");
        if (player_hp == 100) {
            printf("S\n");
        } else if (player_hp >= 81) {
            printf("A+\n");
        } else if (player_hp >= 71) {
            printf("B\n");
        } else if (player_hp >= 61) {
            printf("C\n");
        } else {
            printf("D\n");
        }
    }

}
