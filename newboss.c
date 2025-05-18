#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> // Added for isprint()

#if defined(_WIN32) || defined(_WIN64)
  #include <curses.h>   // PDCurses
#else
  #include <ncurses.h>  // ncurses
#endif

#define MAX_INPUT 100
#define REACT_TIME 5      // seconds
#define INPUT_SLEEP 10    // ms between key checks

// Structure to define a boss attack
typedef struct {
    const char *description;
    const char *correct_response;
    int damage;
    const char *type;  // "magic" or "physical"
} BossAttack;

// Read a line of input with a timeout (in seconds).
// Returns 1 if user pressed Enter with a non-empty buffer before timeout, else 0.
int timed_input(char *buffer, int size, int timeout_s) {
    int idx = 0;
    buffer[0] = '\0'; // Ensure buffer is initially empty
    clock_t start = clock();
    int timeout_ticks = timeout_s * CLOCKS_PER_SEC;

    nodelay(stdscr, TRUE);   // getch() won’t block
    echo();                  // Show typed characters
    curs_set(1);             // Show cursor

    while ((clock() - start) < timeout_ticks) {
        int c = getch();
        if (c == ERR) {
            // No key pressed right now, just wait
        } else if (c == '\n' || c == '\r') { // Enter key pressed
            buffer[idx] = '\0'; // Null-terminate the string
            break;
        } else if (c == KEY_BACKSPACE || c == 127) { // Handle backspace
            if (idx > 0) {
                idx--;
                // Move cursor back, print space, move cursor back again
                mvaddch(getcury(stdscr), getcurx(stdscr)-1, ' ');
                move(getcury(stdscr), getcurx(stdscr)-1);
            }
        } else if (idx < size - 1 && isprint(c)) { // Add printable character to buffer
            buffer[idx++] = (char)c;
            buffer[idx] = '\0'; // Keep string null-terminated
        }
        // Short sleep to avoid busy-looping and hogging CPU
        napms(INPUT_SLEEP);
    }

    noecho();                // Don't show typed characters anymore
    curs_set(0);             // Hide cursor
    nodelay(stdscr, FALSE);  // getch() will block again

    // Return 1 if input was received (idx > 0), 0 otherwise
    // Simplified return condition: if idx > 0, input was received.
    return (idx > 0);
}

int main() {
    srand((unsigned)time(NULL)); // Seed random number generator

    // Initialize curses
    if (initscr() == NULL) { // Added error check for initscr()
        fprintf(stderr, "Error: Could not initialize curses.\n");
        fprintf(stderr, "Please ensure your terminal is compatible or configured correctly.\n");
        return 1; // Exit if curses fails to initialize
    }
    cbreak();                // Disable line buffering
    noecho();                // Don't echo characters typed by the user
    curs_set(0);             // Hide the cursor
    keypad(stdscr, TRUE);    // Enable reading of function keys (like F1, arrow keys)

    int player_hp = 100, boss_hp = 100;
    char input[MAX_INPUT];
    int dodged = 0; // Flag to check if player successfully dodged

    // Define the boss's attacks
    BossAttack attacks[] = {
        {"swings a sword", "duck", 10, "physical"},
        {"throws a fireball", "jump", 15, "magic"},
        {"charges at you", "block", 20, "physical"},
        {"stabs low", "jump", 10, "physical"},
        {"casts lightning", "block", 15, "magic"}
    };
    int num_attacks = sizeof(attacks) / sizeof(attacks[0]); // Calculate number of attacks
    BossAttack current_attack; // To store the boss's chosen attack for the round

    // Main game loop
    while (player_hp > 0 && boss_hp > 0) {
        current_attack = attacks[rand() % num_attacks]; // Boss randomly selects an attack
        dodged = 0; // Reset dodge status for the new turn

        // --- Boss's Turn ---
        clear(); // Clear the screen
        mvprintw(0, 0, "⚔️  Final Boss Battle ⚔️");
        mvprintw(2, 0, "Boss HP: %3d   You: %3d", boss_hp, player_hp);
        mvprintw(4, 0, "Boss %s! (Options: duck | jump | block)", current_attack.description);
        mvprintw(6, 0, "React in %d seconds: ", REACT_TIME);
        refresh(); // Update the screen

        // Get player's timed input
        int got_input = timed_input(input, MAX_INPUT, REACT_TIME);

        // Process player's reaction
        if (!got_input || strcmp(input, current_attack.correct_response) != 0) {
            player_hp -= current_attack.damage;
            mvprintw(8, 0, "❌ You failed to %s! Lost %d HP.", current_attack.correct_response, current_attack.damage);
        } else {
            dodged = 1;
            mvprintw(8, 0, "✅ Dodged it!");
        }
        refresh();
        napms(1500); // Pause to let player read the outcome

        if (player_hp <= 0) break; // Check if player died

        // --- Player's Turn (if dodged) ---
        clear();
        mvprintw(0, 0, "⚔️  Final Boss Battle ⚔️");
        mvprintw(2, 0, "Boss HP: %3d   You: %3d", boss_hp, player_hp);

        if (dodged) {
            mvprintw(4, 0, "Your turn! Attack with: slash | kick");
            mvprintw(6, 0, "You have %d seconds: ", REACT_TIME);
            refresh();

            got_input = timed_input(input, MAX_INPUT, REACT_TIME);
            if (!got_input) {
                mvprintw(8, 0, "⏱️  Too slow! You missed your chance to attack.");
            } else if (strcmp(input, "slash") == 0 || strcmp(input, "kick") == 0) {
                int dmg = 10 + rand() % 6; // Base damage: 10-15
                // Check for "super effective" moves
                if ((strcmp(input, "slash") == 0 && strcmp(current_attack.type, "physical") == 0) ||
                    (strcmp(input, "kick") == 0 && strcmp(current_attack.type, "magic") == 0)) {
                    dmg += 5; // Bonus damage for super effective
                    mvprintw(8, 0, "💥 Super effective! +5 bonus damage.");
                } else {
                    mvprintw(8, 0, "You hit the boss!");
                }
                mvprintw(9, 0, "You dealt %d damage.", dmg);
                boss_hp -= dmg;
            } else {
                mvprintw(8, 0, "❓ Invalid move. You stumbled and missed.");
            }
        } else {
            mvprintw(4, 0, "You're still recovering from the hit and couldn't attack!");
        }
        refresh();
        napms(2000); // Pause to let player read the outcome
    }

    // --- Game Over ---
    clear();
    if (player_hp <= 0) {
        mvprintw(2, 0, "💀 You died. Game Over. 💀");
    } else {
        mvprintw(2, 0, "🏆 You defeated the boss! Victory! 🏆");
        mvprintw(4, 0, "Your final HP: %d", player_hp);
        // Determine grade based on final HP
        const char *grade = "D"; // Default grade
        if (player_hp == 100) grade = "S (Perfect!)";
        else if (player_hp >= 90) grade = "A+";
        else if (player_hp >= 80) grade = "A";
        else if (player_hp >= 70) grade = "B";
        else if (player_hp >= 60) grade = "C";
        mvprintw(6, 0, "Score: %s", grade);
    }
    mvprintw(8, 0, "Press any key to exit...");
    refresh();
    nodelay(stdscr, FALSE); // Ensure getch waits for a key
    getch();                // Wait for user to press a key

    endwin(); // Clean up curses
    return 0;
}
