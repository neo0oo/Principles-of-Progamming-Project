#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32) || defined(_WIN64)
  #include <curses.h>    // PDCurses
#else
  #include <ncurses.h>   // ncurses
#endif

#define MAX_INPUT 100
#define REACT_TIME 5      // seconds
#define INPUT_SLEEP 10    // ms between key checks

typedef struct {
    const char *description;
    const char *correct_response;
    int damage;
    const char *type;  // "magic" or "physical"
} BossAttack;

// Read a line of input with a timeout (in seconds). Returns 1 if user pressed Enter
// with a non-empty buffer before timeout, else 0.
int timed_input(char *buffer, int size, int timeout_s) {
    int idx = 0;
    buffer[0] = '\0';
    clock_t start = clock();
    int timeout_ticks = timeout_s * CLOCKS_PER_SEC;

    nodelay(stdscr, TRUE);    // getch() won’t block
    echo();                   // show typed characters
    curs_set(1);              // show cursor

    while ((clock() - start) < timeout_ticks) {
        int c = getch();
        if (c == ERR) {
            // no key pressed right now
        } else if (c == '\n' || c == '\r') {
            buffer[idx] = '\0';
            break;
        } else if (c == KEY_BACKSPACE || c == 127) {
            if (idx > 0) {
                idx--;
                mvaddch(getcury(stdscr), getcurx(stdscr)-1, ' ');
                move(getcury(stdscr), getcurx(stdscr)-1);
            }
        } else if (idx < size-1 && isprint(c)) {
            buffer[idx++] = (char)c;
            buffer[idx] = '\0';
        }
        // avoid busy-looping
        napms(INPUT_SLEEP);
    }

    noecho();
    curs_set(0);
    nodelay(stdscr, FALSE);

    return (idx > 0 && (buffer[0] != '\0'));
}

int main() {
    srand((unsigned)time(NULL));

    // initialize curses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

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
    int num_attacks = sizeof(attacks)/sizeof(*attacks);
    BossAttack last_attack;

    while (player_hp > 0 && boss_hp > 0) {
        last_attack = attacks[rand() % num_attacks];
        dodged = 0;

        clear();
        mvprintw(0, 0, "⚔️  Final Boss Battle");
        mvprintw(2, 0, "Boss HP: %3d   You: %3d", boss_hp, player_hp);
        mvprintw(4, 0, "Boss %s! (Options: duck | jump | block)", last_attack.description);
        mvprintw(6, 0, "React in %d seconds: ", REACT_TIME);
        refresh();

        int got = timed_input(input, MAX_INPUT, REACT_TIME);
        if (!got || strcmp(input, last_attack.correct_response) != 0) {
            player_hp -= last_attack.damage;
            mvprintw(8, 0, "❌ You failed! Lost %d HP.", last_attack.damage);
        } else {
            dodged = 1;
            mvprintw(8, 0, "✅ Dodged it!");
        }
        refresh();
        napms(1000);
        if (player_hp <= 0) break;

        clear();
        mvprintw(0, 0, "⚔️  Final Boss Battle");
        mvprintw(2, 0, "Boss HP: %3d   You: %3d", boss_hp, player_hp);

        if (dodged) {
            mvprintw(4, 0, "Your turn! Attack with: slash | kick");
            mvprintw(6, 0, "You have %d seconds: ", REACT_TIME);
            refresh();

            got = timed_input(input, MAX_INPUT, REACT_TIME);
            if (!got) {
                mvprintw(8, 0, "⏱️  Too slow! You missed.");
            } else if (strcmp(input,"slash")==0 || strcmp(input,"kick")==0) {
                int dmg = 10 + rand()%6;
                if ((strcmp(input,"slash")==0 && strcmp(last_attack.type,"physical")==0) ||
                    (strcmp(input,"kick" )==0 && strcmp(last_attack.type,"magic"   )==0)) {
                    dmg += 5;
                    mvprintw(8, 0, "💥 Super effective! +5 bonus.");
                } else {
                    mvprintw(8, 0, "You hit!");
                }
                mvprintw(9, 0, "You dealt %d damage.", dmg);
                boss_hp -= dmg;
            } else {
                mvprintw(8, 0, "Invalid move. You missed.");
            }
        } else {
            mvprintw(4, 0, "You couldn't attack—still recovering!");
        }
        refresh();
        napms(1000);
    }

    clear();
    if (player_hp <= 0) {
        mvprintw(2, 0, "💀 You died. Game Over.");
    } else {
        mvprintw(2, 0, "🏆 You defeated the boss! Victory!");
        mvprintw(4, 0, "Your final HP: %d", player_hp);
        const char *grade = (player_hp==100?"S":player_hp>=81?"A+":player_hp>=71?"B":player_hp>=61?"C":"D");
        mvprintw(6, 0, "Score: %s", grade);
    }
    mvprintw(8, 0, "Press any key to exit...");
    refresh();
    getch();

    endwin();
    return 0;
}
