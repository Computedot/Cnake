#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <ncurses.h>

struct Snake{
    int directionX, directionY;
    int positionX, positionY;
};

struct Apple{
    int positionX, positionY;
};

//Changes the position of an object.
void changeApplePosition(struct Apple *pApple, struct Snake body[], int length){
    int randomX, randomY;
    bool freePosition;
    srand(clock());
    do{
        freePosition = true;
        randomX = rand() % 21 + 1;
        randomY = rand() % 21 + 1;
        for(int counter=0; counter<length; counter++){
            if((body[counter].positionX==randomX) & (body[counter].positionY==randomY)){
                freePosition = false;
                break;
            }
        }
    } while(freePosition == false);
    pApple->positionX = randomX;
    pApple->positionY = randomY;
}

//Making sure the snake is inside the 20x20 area
bool checkSnakePosition(struct Snake body[], int length){
    if ((body[length-1].positionX<1) | (body[length-1].positionY<1) | (body[length-1].positionX>21) | (body[length-1].positionY>21)) {return true;}
    else{
        for (int counter=0; counter<length-1; counter++){
            if ((body[length-1].positionX==body[counter].positionX) && (body[length-1].positionY==body[counter].positionY)){
                return true;
            }
        }
    }
    return false;
}

void initializeBody(struct Snake body[]){
    for(int counter=0; counter<100; counter++)
        body[counter].positionX = body[counter].positionY = -1;
}

void generateBorder(){
    int counter;
    for(counter=0; counter<22; counter++){
        mvaddch(0, counter, '#');
        mvaddch(22, counter, '#');
        mvaddch(counter, 0, '#');
        mvaddch(counter, 22, '#');
    }
    mvaddch(22,22,'#');
}

int main(){
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    srand(clock());

    int input = 0;
    int length = 3;     //Length of the snake's body.
    bool moveSnake;     //Checking whether the snake can move
    bool exitProgram;
    struct Snake body[100];
    struct Snake head;
    struct Apple apple, *pApple = &apple;
    initializeBody(body);
    head.positionX = head.positionY = rand() % 21 + 1;
    changeApplePosition(pApple, body, length);
    mvaddch(apple.positionY, apple.positionX, 'a');
    mvaddch(head.positionY, head.positionX, 'O');
    generateBorder();
    refresh();

    do{
        input = getch();
        nodelay(stdscr, true);
        moveSnake = true;

        switch (input) {
            case KEY_HOME:
            case KEY_END: exitProgram = true;   break;
            case KEY_UP:
                if (head.directionY==-1) {moveSnake = false; continue;}
                head.directionX = 0;
                head.directionY = 1;
                head.positionY--;
                break;
            case KEY_DOWN:
                if (head.directionY==1) {moveSnake = false; continue;}
                    head.directionX = 0;
                    head.directionY = -1;
                    head.positionY++;
                    break;
            case KEY_RIGHT:
                if (head.directionX==-1) {moveSnake = false; continue;}
                head.directionX = 1;
                head.directionY = 0;
                head.positionX++;
                break;
            case KEY_LEFT:
                if (head.directionX==1) {moveSnake = false; continue;}
                head.directionX = -1;
                head.directionY = 0;
                head.positionX--;
                break;
            case ERR:
            	if (head.directionY==1) head.positionY--;
                else if (head.directionY==-1) head.positionY++;
                else if (head.directionX==1) head.positionX++;
                else if (head.directionX==-1) head.positionX--;
                break;
            default: continue;
        }
        clear();


        if (moveSnake==true){
            if ((head.positionX==apple.positionX) && (head.positionY==apple.positionY)){
                changeApplePosition(pApple, body, length);
                length++;
            }

            for (int counter=0; counter<length-1; counter++){
                body[counter] = body[counter+1];
            }
            body[length-1] = head;

            generateBorder();
            mvaddch(apple.positionY, apple.positionX, 'a');
            mvaddch(head.positionY, head.positionX, 'O' );
            mvprintw(10,25,"%i",length);
            for(int counter=0; counter<length-1; counter++){
                mvaddch(body[counter].positionY, body[counter].positionX, 'o');
            }
            mvaddch(head.positionY, head.positionX, '0');
            refresh();

            exitProgram = checkSnakePosition(body, length);
        }
    usleep(pow(10, 5));
    }
    while(exitProgram==false);
    mvprintw(10,10,"GAME OVER");
    refresh();
    usleep(pow(10, 6));
    getchar();
    endwin();
    return 0;
}

