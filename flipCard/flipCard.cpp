//
//  main.cpp
//
//  Created by 김지우
//

#include <bangtal>
#include <string>
#include <iostream>
using namespace bangtal;
using namespace std;
#include <ctime>
#include <cstdlib>

auto main_scene = Scene::create("카드 뒤집기", "Images/back_ground.png");
auto scorePage = Scene::create("", "Images/back_ground.png");
auto final = Scene::create("", "Images/back_ground.png");


ObjectPtr cards[28];
int emo[28] = { 0, }; // 카드 초기화
int unhappy = rand() % 4 + 1; // 불행카드 갯수 (1~4)

int hscore[3] = {10,20,30}; // 스마일카드 점수 (셋 중 하나)
int hap = hscore[rand() % 3];
int uscore[3] = { 250, 500, 750 }; // 불행카드 점수 (셋 중 하나)
int unh = uscore[rand() % 3];

int totalScore = 0; // 전체 점수
int roundScore = 0; // 라운드 점수

int r = 1;
int rounds[10]; //라운드별 점수 배열

auto playbutton = Object::create("Images/play.png", main_scene, 520, 255);
auto nextbutton = Object::create("Images/arrow.png", scorePage, 520, 255);

string indexToImage(int index) {
    return "Images/emot" + to_string(emo[index]) + ".png";
}

string hscImage(int hap) {
    if (hap==10) return "Images/hscore0.png";
    else if (hap==20) return "Images/hscore1.png";
    else return "Images/hscore2.png";
}

string uscImage(int unh) {
    if (unh == 250) return "Images/uscore0.png";
    else if (unh == 500) return "Images/uscore1.png";
    else return "Images/uscore2.png";
}

string unhImage(int unhappy) {
    return "Images/unh" + to_string(unhappy) + ".png";
}

string scoreToMes(int r, int s) {
    return to_string(r) +"라운드 점수는 " + to_string(s) + "점 입니다.";
}

void shuffle() { // 불행카드 위치 셔플 함수
    srand((unsigned int)time(NULL));

    for (int i = 0; i < 28; i++) {
        emo[i] = 0;
    }

    for (int j = 0; j < unhappy; j++) {
        if (emo[rand() % 28] == 1) --j;
        else emo[rand() % 28] = 1;
    }
}

void game_init() { // 본 게임 화면
    auto game = Scene::create("", "Images/back_ground.png");
    game->enter();

    srand((unsigned int)time(NULL));

    auto hsc = Object::create(hscImage(hap), game, 950, 650);
    auto usc = Object::create(uscImage(unh), game, 950, 585);
    auto unhap = Object::create(unhImage(unhappy), game, 950, 530);
    auto next = Object::create("Images/next.png", game, 950, 130);


    for (int ind = 0; ind < 28; ++ind) {
        cards[ind] = Object::create("Images/cardback.png", game, 100 + (ind % 7) * (100 + 20), 40 + (ind / 7) * (150 + 20));

        cards[ind]->setOnMouseCallback([&, ind](auto, auto, auto, auto)->bool {
            cards[ind]->setImage(indexToImage(ind));

            if (emo[ind] == 0) {
                roundScore += hap;

            }
            else {
                roundScore -= unh;
                showMessage("라운드 종료");
                rounds[r - 1] = roundScore;
                r++;
                totalScore += roundScore;
                
                if (r < 11) {
                    score_init();
                }
                else {
                    final_init();
                }
            }

            return true;
            });
    }

    next->setOnMouseCallback([&](auto, auto, auto, auto)->bool {

        rounds[r - 1] = roundScore;
        totalScore += roundScore;
        r++;

        if (r < 11) {
            score_init();
        }
        else {
            final_init();
        }

        return true;
        });

    shuffle();

}

void score_init() { // 라운드별 점수 안내 및 다음 라운드로 넘어가기 전 화면
    scorePage->enter();

    nextbutton->show();

    showMessage(scoreToMes(r-1, rounds[r-2]));

    roundScore = 0;
    hap = hscore[rand() % 3];
    unh = uscore[rand() % 3];
    unhappy = rand() % 4 + 1;

    nextbutton->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        game_init();
        nextbutton->hide();

        return true;
        });
}

string finalMes(int Total) {
    return "최종 점수는 " + to_string(Total) + "점 입니다.";
}

void final_init() {  // 최종 점수 화면
    final->enter();

    showMessage(finalMes(totalScore));
}

int main()
{
    srand((unsigned int)time(NULL));

    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);

    playbutton->setOnMouseCallback([&](auto, auto, auto, auto)->bool {
        game_init();
        playbutton->hide();

        return true;
        });

    startGame(main_scene);

}
