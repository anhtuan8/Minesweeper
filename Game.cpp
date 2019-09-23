
#include"Game.hpp"
#include<SDL2/SDL_image.h>
#include"Tile.hpp"
#include<ctime>
#include<cstdlib>
#include "utils.hpp"
#include <iostream>
#include "text.hpp"
#include <fstream>

using namespace std;

//SDL_Texture* textures;
//const SDL_Rect srcR_grass = {96,0,16,16},srcR_brick = {80,0,16,16},srcR_bomb = {2,2,12,23};
//SDL_Rect desR_tiles[25][25];
Tile tiles[25][25];
int Game::mapsize,Game::mineNum,Game::tilesize, Game::moveNum, Game::mineLeftNum, Game::level;
SDL_Renderer* Game::g_renderer = nullptr;
SDL_Event Game::event;
TTF_Font* Game::g_font;
Text mineLeft, gameTime, moveLeft, loseNoti, winNoti, startOver, easy, medium, hard, exitGame, bestTime;
Text buttons[4];
bool holdControl;
int timeStart, highscore, easyScore, mediumScore, hardScore;
fstream score;


Game::Game(){
    win = false;
    lose = false;
}

Game::~Game(){
}

int Game::showMenu(string path){
    int dif;
    SDL_Window* menuWindow;
    SDL_Renderer* menuRenderer;
    cout << "Initializing subsystems..." << endl;
    if( SDL_Init(SDL_INIT_EVERYTHING) == 0 ){
    //createWindow
        menuWindow = SDL_CreateWindow("Minesweeper",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600,0);
        if(menuWindow == NULL){
            cout << "Couldn't create window." << endl;
            return -1;
        }
        else{
            SDL_SetWindowResizable(menuWindow,SDL_FALSE);
            cout << "Window created." << endl;
        }
    //create Renderer
        menuRenderer = SDL_CreateRenderer(menuWindow,-1,SDL_RENDERER_ACCELERATED);
        if(menuRenderer == NULL){
            cout << "Couldn't create renderer." << endl;
            return -1;
        }
        else{
            SDL_SetRenderDrawColor(g_renderer,255,255,255,255);
            cout << "Renderer created." << endl;
        }
    }
    if(TTF_Init() == -1){
        cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
    }
    TTF_Font* menuFont = TTF_OpenFont("FreeMonoBold.ttf",45);
    if(menuFont == NULL){
        cout << "Could not open Font" << endl;
        isRunning = false;
    }

    SDL_Surface* tmpSur = IMG_Load(path.c_str());
    SDL_Texture* tex = SDL_CreateTextureFromSurface(menuRenderer,tmpSur);
    SDL_FreeSurface(tmpSur);

    SDL_Event menuEvent;
    SDL_Color black = {0,0,0},white = {255,255,255};

    Text chooseEasy;
    chooseEasy.setText("Easy(8x8)");
    chooseEasy.createText(menuFont,menuRenderer,50,250,black);

    Text chooseMedium;
    chooseMedium.setText("Medium(15x15)");
    chooseMedium.createText(menuFont,menuRenderer,50,325,black);

    Text chooseHard;
    chooseHard.setText("Hard(25x25)");
    chooseHard.createText(menuFont,menuRenderer,50,400,black);

    Text exit;
    exit.setText("Exit");
    exit.createText(menuFont,menuRenderer,50,475,black);

    SDL_RenderClear(menuRenderer);
    SDL_RenderCopy(menuRenderer,tex,NULL,NULL);
    chooseEasy.RenderText(menuRenderer);
    chooseMedium.RenderText(menuRenderer);
    chooseHard.RenderText(menuRenderer);
    exit.RenderText(menuRenderer);
    SDL_RenderPresent(menuRenderer);

    bool showing = true;
    while(showing){
        SDL_PollEvent(&menuEvent);
        switch(menuEvent.type){
            case SDL_QUIT:
                showing = false;
                return -1;
            case SDL_MOUSEBUTTONDOWN:
            {
                int x = menuEvent.button.x;
                int y = menuEvent.button.y;
                if(myUtils::checkFocus(x,y,chooseEasy.getRect())){
                    showing = false;
                    return 0;
                }
                if(myUtils::checkFocus(x,y,chooseMedium.getRect())){
                    showing = false;
                    return 1;
                }
                if(myUtils::checkFocus(x,y,chooseHard.getRect())){
                    showing = false;
                    return 2;
                }
                if(myUtils::checkFocus(x,y,exit.getRect())){
                    showing = false;
                    return -1;
                }

            }
            default:
                break;
        }

    }
    return -1;
}

void Game::init(const char* title, int xpos, int ypos, int width, int height){
    clean();
    cout << "Initializing subsystems..." << endl;
    if( SDL_Init(SDL_INIT_EVERYTHING) == 0 ){ //Init successfully
        cout << "Successfully initialized subsystems." << endl;
        //createWindow
        window = SDL_CreateWindow(title,xpos,ypos,width,height,0);
        if(window == NULL){
            cout << "Couldn't create window." << endl;
        }
        else{
            SDL_SetWindowResizable(window,SDL_FALSE);
            cout << "Window created." << endl;
        }

        //create Renderer
        g_renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
        if(g_renderer == NULL){
            cout << "Couldn't create renderer." << endl;
        }
        else{
            SDL_SetRenderDrawColor(g_renderer,255,255,255,255);
            cout << "Renderer created." << endl;
        }
        isRunning = true;
    } else{
        isRunning = false;
    }
    //init font
    if(TTF_Init() == -1){
        cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
    }
    g_font = TTF_OpenFont("Ubuntu-L.ttf",30);
    if(g_font == NULL){
        cout << "Could not open Font" << endl;
        isRunning = false;
    }
    //open highscore
    score.open("highscores.txt", ios::out|ios::in);
    if(score.is_open()){
        cout << "Open highscore" << endl;
        isRunning = true;
    }
    else{
        cout << "Cannot open highscore" << endl;
        isRunning = false;
    }
    isRunning = true;
}

void Game::initAttributes(int difficulty){
    //value for game attributes
     resetAttributes(difficulty);
    //initialize tile map

    myUtils::initMap(mineNum,mapsize,tiles,g_renderer,tilesize);

    //init game button
    SDL_Color red = {255,0,0},black = {0,0,0};
    string val6 = "Start over";
    startOver.setText(val6);
    startOver.createText(g_font,g_renderer,650,450,black);
    string val7 = "Easy (8x8)";
    easy.setText(val7);
    easy.createText(g_font,g_renderer,650,250,black);

    medium.setText("Medium (15x15)");
    medium.createText(g_font,g_renderer,650,300,black);

    hard.setText("Hard (25x25)");
    hard.createText(g_font,g_renderer,650,350,black);

    string val8 = "Exit Game";
    exitGame.setText(val8);
    exitGame.createText(g_font,g_renderer,650,500,black);

    buttons[0] = startOver;
    buttons[1] = easy;
    buttons[2] = exitGame;
}

void Game::handleEvents(){
    SDL_PollEvent(&event);
    switch(event.type){
        case SDL_QUIT://Close game window
            isRunning = false;
            paused = false;
            break;
        //other mouse events
        case SDL_MOUSEBUTTONDOWN:
        {
            if(holdControl == true) break;
            holdControl = true;

            int x = event.button.x;
            int y = event.button.y;
            int i = x/tilesize, j = y /tilesize;
            //open cells
            if(event.button.button == SDL_BUTTON_LEFT ){
                myUtils::clickTile(i,j,tiles,mapsize,this);
            }
            else if(event.button.button == SDL_BUTTON_RIGHT){
                if(!tiles[i][j].isOpen()){
                    if(!tiles[i][j].isFlaged()){
                        tiles[i][j].flag(this);
                        //cout << tiles[i][j].isFlaged() << endl;
                    }
                    else{
                        tiles[i][j].deflag(this);
                        //cout << tiles[i][j].isFlaged() << endl;
                    }
                }
            }

            //click buttons
            if(myUtils::checkFocus(x,y, startOver.getRect())){
                resetAttributes(level);
                myUtils::initMap(mineNum,mapsize,tiles,g_renderer,tilesize);
                paused = false;
                isRunning = true;
            }
            if(myUtils::checkFocus(x,y, exitGame.getRect())){
                isRunning = false;
                paused = false;
            }
            if(myUtils::checkFocus(x,y,easy.getRect())){
                level = 0;
                resetAttributes(level);
                myUtils::initMap(mineNum,mapsize,tiles,g_renderer,tilesize);
                paused = false;
                isRunning = true;
            }
            if(myUtils::checkFocus(x,y,medium.getRect())){
                level = 1;
                resetAttributes(level);
                myUtils::initMap(mineNum,mapsize,tiles,g_renderer,tilesize);
                paused = false;
                isRunning = true;
            }
            if(myUtils::checkFocus(x,y,hard.getRect())){
                level = 2;
                resetAttributes(level);
                myUtils::initMap(mineNum,mapsize,tiles,g_renderer,tilesize);
                paused = false;
                isRunning = true;
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            holdControl = false;
            break;
        }
        case SDL_MOUSEMOTION:
        {
            int x = event.button.x;
            int y = event.button.y;
            SDL_Color red = {255,0,0},blue = {0,0,255},black = {0,0,0};
            if(myUtils::checkFocus(x,y, startOver.getRect())){
                startOver.free();
                startOver.createText(g_font,g_renderer,650,450,blue);
            }
            else{
                startOver.free();
                startOver.createText(g_font,g_renderer,650,450,black);
            }
            if(myUtils::checkFocus(x,y, exitGame.getRect())){
                exitGame.free();
                exitGame.createText(g_font,g_renderer,650,500,blue);
            }
            else{
                exitGame.free();
                exitGame.createText(g_font,g_renderer,650,500,black);
            }
            if(myUtils::checkFocus(x,y,easy.getRect())){
                easy.free();
                easy.createText(g_font,g_renderer,650,250,blue);
            }
            else{
                easy.free();
                easy.createText(g_font,g_renderer,650,250,black);
            }
            if(myUtils::checkFocus(x,y,medium.getRect())){
                medium.free();
                medium.createText(g_font,g_renderer,650,300,blue);
            }
            else{
                medium.free();
                medium.createText(g_font,g_renderer,650,300,black);
            }
            if(myUtils::checkFocus(x,y,hard.getRect())){
                hard.free();
                hard.createText(g_font,g_renderer,650,350,blue);
            }
            else{
                hard.free();
                hard.createText(g_font,g_renderer,650,350,black);
            }
            break;
        }
        default:
            break;
    }
}

void Game::update(){
    //cout << "updating.."<<endl;
    //cout << myUtils::countAdjacentMine(0,0,tiles,mapsize) << " ";
    if(moveNum==0){
        win = true;
        this->finish();
    }
    SDL_Color red = {255,0,0},blue = {0,0,255},black = {0,0,0};
    string val1 = "Mine left:  " + to_string(mineLeftNum);
    mineLeft.setText(val1);
    mineLeft.createText(g_font,g_renderer,650,10,red);

    string val2 = "Time: " + to_string( (SDL_GetTicks() -timeStart)/1000);
    gameTime.setText(val2);
    gameTime.createText(g_font,g_renderer,650,60,blue);

    string val3 = "Move left: " + to_string(moveNum);
    moveLeft.setText(val3);
    moveLeft.createText(g_font,g_renderer,650,110,black);

    string val4 = "Best time:" + to_string(highscore);
    bestTime.setText(val4);
    bestTime.createText(g_font,g_renderer,650,160,black);

    if(lose){
        TTF_Font* font = TTF_OpenFont("FreeMonoBold.ttf",80);
        string val4 = "You lose :<";
        loseNoti.setText(val4);
        loseNoti.createText(font,g_renderer,100,250,red);
        TTF_CloseFont(font);
    }
    if(win){
        TTF_Font* font = TTF_OpenFont("FreeMonoBold.ttf",80);
        string val4 = "You win";
        loseNoti.setText(val4);
        loseNoti.createText(font,g_renderer,100,250,red);
        TTF_CloseFont(font);
    }
}

void Game::render(){
    //cout << "rendering" << endl;
    SDL_RenderClear(g_renderer);
    //Rendering stuffs
    //render tiles
    for(int i = 0; i< mapsize ; i++){
        for(int j = 0 ; j < mapsize ; j++){
            tiles[i][j].renderTile(this);
        }
    }
    //render info
    mineLeft.RenderText(g_renderer);
    gameTime.RenderText(g_renderer);
    moveLeft.RenderText(g_renderer);
    loseNoti.RenderText(g_renderer);
    winNoti.RenderText(g_renderer);
    exitGame.RenderText(g_renderer);
    easy.RenderText(g_renderer);
    medium.RenderText(g_renderer);
    hard.RenderText(g_renderer);
    startOver.RenderText(g_renderer);
    bestTime.RenderText(g_renderer);

    SDL_RenderPresent(g_renderer);
}

void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(g_renderer);
    TTF_CloseFont(g_font);
    for(int i = 0; i< mapsize ; i++){
        for(int j = 0 ; j < mapsize ; j++){
            tiles[i][j].free();
        }
    }
    mineLeft.free();gameTime.free();moveLeft.free();loseNoti.free();winNoti.free();
    startOver.free();easy.free();exitGame.free();
    SDL_Quit();
    cout << "Game cleaned." << endl;
}

void Game::finish(){
    //set man hinh ket thuc game
    for(int i = 0 ; i < mapsize ; i++){
        for(int j = 0 ; j < mapsize ; j++){
            int data = tiles[i][j].getData();
            if(data == -1){
                tiles[i][j].setTexture("mine.png" , g_renderer);
            }
            else{
                tiles[i][j].setTexture(to_string(data) + ".png" , g_renderer);
            }
            tiles[i][j].renderTile(this);
        }
    }
    //ghi diem vao highscores.txt
    if(win){
        if(score.is_open()){
            score.close();
        }
        score.open("highscores.txt", ios::trunc|ios::out);
        int btime = (SDL_GetTicks() - timeStart)/1000;
        switch(level){
            case 0:
                if(btime < easyScore){
                    easyScore = btime;
                }
                break;
            case 1:
                if(btime < mediumScore)
                    mediumScore = btime;
                break;
            case 2:
                if(btime < hardScore)
                    hardScore = btime;
                break;
            default:
                break;
        }
        score << to_string(easyScore) + " " + to_string(mediumScore) + " " + to_string(hardScore) ;
    }

    paused = true;
    //cout<< isRunning << endl;
}

void Game::resetAttributes(int dif){
    //set highscore, mapsize and mineNum
    if(score.is_open()){
            score.close();
        }
    score.open("highscores.txt", ios::in|ios::out);
    score >> easyScore >> mediumScore >> hardScore;
    switch(dif){
        case 0:{
            mapsize = 8;
            mineNum = 8;
            highscore = easyScore;
        }
        break;
        case 1:{
            mapsize = 15;
            mineNum = 30;
            highscore = mediumScore;
        }
        break;
        case 2:{
            mapsize = 25;
            mineNum = 99;
            highscore = hardScore;
        }
        break;
        default:{
            isRunning = false;
        }
        break;
    }

    //other constants
    tilesize = SCREEN_SIDE / mapsize;
    moveNum = mapsize*mapsize - mineNum;
    mineLeftNum = mineNum;
    timeStart = SDL_GetTicks();
    level = dif;
    win = false;
    lose = false;
    paused = false;
    holdControl = false;
    loseNoti.free();
    winNoti.free();
}
