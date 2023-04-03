#include "../icl/top.h"

int count_chars(const char *file) {
    int count = 0;
    int fd = 0;
    char c;

    fd = open(file, O_RDONLY);
    if (fd == -1)
        return -1;
    while (read(fd, &c, 1) == 1)
        count++; 
    close(fd);
    return count;
}

char *open_read(const char *file) {
    char *buf = NULL;
    int count = 0;
    int fd = 0;
    char c;

    count = count_chars(file);
    if (count == -1 || count == 0)
        return NULL;
    buf = malloc(sizeof(char) * (count + 1));
    if (buf == NULL)
        return NULL;
    fd = open(file, O_RDONLY);
    if (fd == -1)
        return NULL;
    for (int i = 0; read(fd, &c, 1) == 1; i++)
        buf[i] = c;
    buf[count] = '\0';
    close(fd);
    return buf;
}

int nb_proc(void) {
    char *cpuinfo = open_read(cpu_path);
    char **arr = str2arr(cpuinfo, ":\n");
    int rows = count_rows(cpuinfo, ":\n");
    int nb = 0;

    free(cpuinfo);
    for (int y = rows - 1; y > 0; y--) {
        if (my_strstr(arr[y], "processor")) {
            nb = atoi(arr[y+1]) + 1;
            return nb;
        }
    }
    for (int y = 0; arr[y] != NULL; y++)
        free(arr[y]);
    return 0;
}

void fill_stCores(st_cores *core) {
    char *cpustat = open_read(cpu_stat);
    char **arr = str2arr(cpustat, "\n");
    char **tab = NULL;
    int nbProc = nb_proc() + 1;

    for (int y = 1, w = 0; y < nbProc; y++) {
        //printw("\narr[%d] = %s\n", y, arr[y]);
        //refresh();
        tab = str2arr(arr[y], " ");
        if (my_strstr(tab[0], "cpu"))
            core[w++].id = atoi(&tab[0][my_strlen(tab[0]) - 1]);
        //free(tab[0]);
        /*
        for (int i = 1, u = 0; tab[i] != NULL; i++) {
            core->prev_iddle[u++] = atoi(tab[i]);  
            free(tab[i]);
        }
        */
        //free(arr[y]);
    }
}

void print_st_cores(st_cores *core) {
    int maxx = 0;
    int maxy = 0;
    int xhalf = 0;
    int nbProc = nb_proc();
    (void)maxy;
    fill_stCores(core);
    getmaxyx(stdscr, maxy, maxx);
    xhalf = maxx/2;
    for (int i = 0; i < nbProc; i++) {
        if (i % 2 == 0)
            mvprintw(3+i, (xhalf/2), "CPU %d ", core[i].id);
        else 
            mvprintw(3+i-1, (xhalf+xhalf/2), "CPU %d ", core[i].id);
        refresh();
    }
}

void mem_usage(void) {
    char *meminfo = open_read(mem_path);
    char **arr = str2arr(meminfo, ":\n");
    char **mem = NULL;
    
    free(meminfo);
    mem = malloc(sizeof(char *) * 2);
    if (mem == NULL)
        return;
    for (int i = 0; arr[i] != NULL; i++) {
        if (my_strstr(arr[i], "MemTotal"))
            mem[TOTAL] = my_strdup(arr[i+1]);
        if (my_strstr(arr[i], "MemAvailable"))
            mem[AVAILABLE] = my_strdup(arr[i+1]);
        free(arr[i]);
    }
    printw("Mem : %s/%s", mem[AVAILABLE], mem[TOTAL]);
    refresh();
    free(mem[0]);
    free(mem[1]);
}

void window(st_cores *core) {
    int ch = 0;

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    while ((ch = getch()) != 113) {
        clear();
        mem_usage();
        print_st_cores(core);
        sleep(2);
    }
    refresh();
    endwin();
}

int main(int ac, char **av) {
    (void)ac;
    (void)av;
    st_cores core[nb_proc()];

    window(core);
    return 0;
}
