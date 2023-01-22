#include <ncurses.h>

int main(int ac, char **av) {
    (void)ac;
    (void)av;
    initscr();
    refresh();
    getch();
    endwin();
    return 0;
}
