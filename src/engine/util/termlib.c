#include <ncurses.h>
#include <signal.h>
#include <locale.h>

#include "termlib.h"
#include "logger.h"

void adjustTermSize(){
    refresh();
    getmaxyx(stdscr, termSize.y, termSize.x);
    logger("term size changed: {%d, %d}\n", termSize.x, termSize.y);

}

void termMaxSize(){
    getmaxyx(stdscr, termSize.y, termSize.x);
}

void termInit(){

    setlocale(LC_ALL,"");

    initscr();
    noecho();

    // make the cursor invisible
    curs_set(FALSE);

    // remove the delay on getch();
    nodelay(stdscr, TRUE);

    // get the initial termSize
    termMaxSize();
    logger("term size: {%d, %d}\n", termSize.x, termSize.y);

    // register termRefresh as the signal handler for SIGWINCH this doesn't really do what I think it does
    // (it gets called but the window size doesn't change)

    // signal(SIGWINCH, (void *)adjustTermSize);
}

void termFinish(){
    endwin();
}

void termClear(){
    clear();
}

void termRefresh(){
    refresh();
}

void drawLine(char * line, Vec2i * pos){
    mvprintw(termSize.y - (int) pos->y, (int) pos->x, line); 
}

void drawLineAlpha(char * line, Vec2i * pos){

    Vec2i adjust = vec2i(pos->x, pos->y);

    char * p = line;
    while(*p == ' '){
        p++;
        adjust.x++;
    }

    mvprintw(termSize.y - adjust.y, adjust.x, p); 
}

const Vec2i * getTermSize(){
    return &termSize;
};
