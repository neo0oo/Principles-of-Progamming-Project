#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 100

typedef struct {
    const char *description;
    const char *correct_response;
    int damage;
    const char *type;  // "magic" or "physical"
} BossAttack;

// Simple input helper
void get_input(char *buffer, int size) {
    printf("> ");
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';  // Strip newline
}

int main() {
    srand((unsigned)time(NULL));
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
        // --- Boss Turn ---
        last_attack = attacks[rand() % num_attacks];
        dodged = 0;

        printf("\n👹 Boss %s!\n", last_attack.description);
        printf("How do you react? (duck / jump / block)\n");
        get_input(input, MAX_INPUT);

        if (strcmp(input, last_attack.correct_response) == 0) {
            printf("✅ You dodged the attack!\n");
            dodged = 1;
        } else {
            printf("❌ Wrong move! You took %d damage.\n", last_attack.damage);
            player_hp -= last_attack.damage;
        }

        if (player_hp <= 0) break;

        // --- Player Turn ---
        if (dodged) {
            printf("\nYour turn to attack! (slash / kick)\n");
            get_input(input, MAX_INPUT);

            if (strcmp(input, "slash") == 0 || strcmp(input, "kick") == 0) {
                int dmg = 10 + rand() % 6;  // Base 10–15
                if ((strcmp(input, "slash") == 0 && strcmp(last_attack.type, "physical") == 0) ||
                    (strcmp(input, "kick") == 0 && strcmp(last_attack.type, "magic") == 0)) {
                    dmg += 5;
                    printf("💥 Super effective attack! Bonus damage!\n");
                } else {
                    printf("You strike the boss!\n");
                }
                printf("You dealt %d damage.\n", dmg);
                boss_hp -= dmg;
            } else {
                printf("❓ Invalid move! You missed your attack.\n");
            }
        } else {
            printf("\n😵 You're still recovering and couldn't attack.\n");
        }

        printf("\n❤️ You: %d HP | 👹 Boss: %d HP\n", player_hp, boss_hp);
    }

    // --- Game Over ---
    printf("\n==============================\n");
    if (player_hp <= 0) {
        printf("💀 You were defeated. Game Over.\n");
    } else {
        printf("🏆 You defeated the boss! Victory!\n");
        printf("Your final HP: %d\n", player_hp);

        printf("Score: ");
        if (player_hp == 100)      printf("S (Perfect!)\n");
        else if (player_hp >= 81) printf("A+\n");
        else if (player_hp >= 71) printf("B\n");
        else if (player_hp >= 61) printf("C\n");
        else                      printf("D\n");
    }

    return 0;
}
