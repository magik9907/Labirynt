#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MENUOPTION 5

//STRUCT
//define game setting
typedef struct {
    int levels;
    int width;
    int height;
    char *file;
    int startx;
    int starty;
    int renderXmin;
    int renderXmax;
    int renderYmin;
    int renderYmax;
} Game;

//define map
typedef struct {
    char mark;
    short visible;
    short visited;
} Map;

//player struct - x,y is position
typedef struct {
    char xyMark;
    int posx;
    int posy;
} Player;

//COMPONENTS
//menu component
int menu ();
//creating game component
void startGame();

//TOOLS
//read map from file tool
void readFromFile(Game *game, Map **map, FILE *f);

//map generate component
Map** generate(Game *game, FILE *f);

//set mark visible and put on screen map
void setVisible (char opt, Map **map, Game *game, Player *player);

//do move up, down, left, right
int doStep(char move, Map **map, Game *game, Player *player);

//========================================================

int main()
{

    int option = 1;

    while(option != MENUOPTION) {
        option = menu();

        switch(option){
            case 1:
                startGame();
                break;
            case MENUOPTION:
                break;
            default:
                system("cls");
                printf("zly numer opcji\n \n");
        }

    }


    return 0;
}

//COMPONENTS
int menu (){
    int i;

    printf("1. Nowa gra\n");
    printf("2. Stworz gre\n");
    printf("3. Wyniki\n");
    printf("4. Sterowanie\n");
    printf("5. Koniec gry\n");


    printf("Podaj numer: ");
    fflush(stdin);
    scanf("%d",&i);
    return i;
}


void startGame(){
    char move;
    char str[50] = "mapy/";
    int i,status;
    Game *game = malloc (sizeof(Game));

    game->file = "test.txt";
    strcat(str,game->file);

    FILE *f = fopen(str,"r");
    if(f == NULL){
        perror("blad odczytu pliku");
    }

    Player *player = malloc(sizeof(Player));


    fscanf(f, "%d", &game->levels);

    do{

        Map **map = generate(game, f);

        player->posx=game->startx;
        player->posy=game->starty;
        player->xyMark = '.';
        setVisible('f', map, game, NULL);
        system("cls");
        while( map[player->posy][player->posx].mark!='@' && move != '0'){
            setVisible('v', map, game,NULL);
            move = getc(stdin);
            fflush(stdin);
            status = doStep(move,map,game,player);
            system("cls");

            switch(status){
            case 0:
                printf("YOU WIN!!!\n\n");
                break;
            case 1:
                printf("juz ta sciane prawie minalem. Ale to bylo prawie\n\n");
                break;
            case 2:
                printf("Arghhhh!!! Kto postawil ta sciane\n\n");
                break;
            case 3:
                printf("magiczne sciany same nie bija\n\n");
                break;
            case 4:
                printf("uderzyles w niewidzialna sciane\n\n");
                break;
            case 5:
                printf("uderzyles w sciane\n\n");
                break;
            }

        }

        for(i = 0; i<game->height;i++)
            free(map[i]);
        free(map);

    }while(game->levels > 0 && move !='0');

    free(game);
    free(player);
    free(game);
    fclose(f);

    system("PAUSE");
}


//========================================================


//TOOLS

Map** generate(Game *game, FILE *f){
    Map **map;
    int i;

    fscanf(f, "%d %d", &game->width, &game->height);

    map = (Map **) malloc (game->height * game->width * sizeof(Map *));
    for (i = 0; i < game->height; i++){
        map[i] =  malloc(game->width * sizeof(Map ));
    }

    readFromFile(game, map, f);

    return map;
}

void readFromFile(Game *game, Map **map, FILE *f) {
        char pom;
        int i, l;
        for(i = 0; i<game->height;i++){
        for(l = 0; l<game->width;){
            pom = fgetc(f);
            //printf("%c",pom);
            if(pom == '\n')
                continue;
            map[i][l].mark = pom;
            map[i][l].visited = 0;
            map[i][l].visible = 0;
            if(map[i][l].mark == '+'){
                game->startx = l;
                game->starty = i;
            }
            l++;

        }
    }
}

void setVisible (char opt, Map **map, Game *game, Player *player){
    int i, l;
    switch(opt){
        case 'a':
            if(player->posy>0)
                map[player->posy-1][player->posx].visible=1;
            if(player->posy<game->height-1)
                map[player->posy+1][player->posx].visible=1;
            if(player->posx-1 >= 0){
                map[player->posy][player->posx-1].visible=1;
                if(player -> posx > 0 && player->posx == game ->renderXmin)
                    game->renderXmin--;
            }
            break;

        case 'd':
            if(player->posy>0)
                map[player->posy-1][player->posx].visible=1;
            if(player->posy<game->height-1)
                map[player->posy+1][player->posx].visible=1;
            if(player->posx+1 < game->width){
                map[player->posy][player->posx+1].visible=1;

                if(player -> posx < game -> width - 1 && player -> posx == game ->renderXmax)
                    game->renderXmax++;
            }
            break;
        case 's':
            if(player->posx > 0)
                map[player->posy][player->posx-1].visible=1;
            if(player->posx < game->width - 1)
                map[player->posy][player->posx+1].visible=1;
            if(player->posy+1 < game->height){
                map[player->posy+1][player->posx].visible=1;

                if(player -> posy < game -> height-1 && player->posy == game ->renderYmax)
                    game->renderYmax++;
            }
            break;
        case 'w':
            if(player->posx > 0)
                map[player->posy][player->posx-1].visible=1;
            if(player->posx < game->width - 1)
                map[player->posy][player->posx+1].visible=1;
            if(player->posy-1 >= 0){
                map[player->posy-1][player->posx].visible=1;
                if(player -> posy > 0 && player->posy == game ->renderYmin)
                    game->renderYmin--;
            }
            break;
        case 'f':
            map[game->starty][game->startx].visible = 1;

            if(game->starty == 0) {
                game->renderYmin = 0;
                game->renderYmax = 1;
                map[1][game->startx].visible = 1;
            }else if(game->starty == game->height-1) {
                map[game->starty-1][game->startx].visible = 1;
                game->renderYmin = game->starty-1;
                game->renderYmax = game->starty;
            }
            else{
                map[game->starty-1][game->startx].visible = 1;
                map[game->starty+1][game->startx].visible = 1;
                game->renderYmin = game->starty-1;
                game->renderYmax = game->starty+1;
            }

            if(game->startx == 0) {
                map[game->starty][1].visible = 1;
                game->renderXmin = 0;
                game->renderXmax = 1;
            }else if(game->startx == game->width-1) {
                map[game->starty][game->startx-1].visible = 1;
                game->renderXmin = game->startx-1;
                game->renderXmax = game->startx;
            }
            else{
                map[game->starty][game->startx-1].visible = 1;
                map[game->starty][game->startx+1].visible = 1;
                game->renderXmin = game->startx-1;
                game->renderXmax = game->startx+1;
            }
               map[game->starty][game->startx].visited = 1;

            break;
        case 'v':
            printf("s-krok w dol\nw-krok w gore\na-krok w lewo\nd-krok w prawo\n\n");
            printf("\n\nPodaj znak\n");
            for(i = game->renderYmin; i<=game->renderYmax;i++)
            {
                for(l = game->renderXmin;l<=game->renderXmax;l++){
                    if(map[i][l].visible == 1)
                        printf("%c",map[i][l].mark);
                    else
                        printf("%c",' ');
                }
                printf("\n");
            }
            break;
    }
}

int doStep(char move, Map **map, Game *game, Player *player){

    switch(move){
    case 'a':
    case 'A':
        if(player->posx == 0)
            return 3;
        if(map[player->posy][player->posx-1].mark == 'X')
            return 5;
        map[player->posy][player->posx].mark = player->xyMark;
        player->posx--;
        player->xyMark = map[player->posy][player->posx].mark;
        if(map[player->posy][player->posx].visited == 0){
            map[player->posy][player->posx].visited = 1;
            setVisible('a', map, game, player);
        }
        break;
    case 'w':
    case 'W':
        if(player -> posy == 0)
            return 1;
        if(map[player -> posy - 1][player -> posx].mark == 'X')
            return 5;
        map[player -> posy][player -> posx].mark = player->xyMark;
        player -> posy--;
        player->xyMark = map[player->posy][player->posx].mark;
        if(map[player -> posy][player -> posx].visited == 0){
            map[player -> posy][player -> posx].visited = 1;
            setVisible('w', map, game, player);
        }
        break;
    case 'd':
    case 'D':
        if(player->posx+1==game->width)
            return 4;
        if(map[player->posy][player->posx+1].mark == 'X')
            return 5;
        map[player->posy][player->posx].mark = player->xyMark;
        player->posx++;
        player->xyMark = map[player->posy][player->posx].mark;
        if(map[player->posy][player->posx].visited ==0){
            map[player->posy][player->posx].visited = 1;
            setVisible('d', map, game, player);
        }
        break;
    case 's':
    case 'S':
        if(player->posy+1==game->height)
            return 2;
        if(map[player->posy+1][player->posx].mark == 'X')
            return 5;
        map[player->posy][player->posx].mark = player->xyMark;
        player->xyMark = map[player->posy][player->posx].mark;
        player->posy++;
        player->xyMark = map[player->posy][player->posx].mark;
        if(map[player->posy][player->posx].visited == 0){
            map[player->posy][player->posx].visited = 1;
            setVisible('s', map, game, player);
        }
        break;
    }
    if( map[player->posy][player->posx].mark == '@')
        return 0;
     map[player->posy][player->posx].mark = '+';
    return 9;
}
