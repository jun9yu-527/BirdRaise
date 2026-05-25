#ifndef UIRENDERER_H
#define UIRENDERER_H

#include <iostream>
#include <string>
#include "ActionManager.h"
using namespace std;

//화면 출력을 전담하는 클래스
// 
//     상단 게이지 
// 
//      그림 
// 
//     하단 정보

class UIRenderer {
private:
    //기본 상태아트
    string eggArt;
    string infantArt;
    string childArt;
    string teenArt;
    string adultArt;
    string birdArt;

    //행동 아트
    //알단계
    string eggHug1;
    string eggHug2;
    string eggTemp1;
    string eggTemp2;
    string eggClassic1;
    string eggClassic2;
    string eggIgnore1;
    string eggIgnore2;
    //유아기 단계
    string babyFeed1;
    string babyFeed2;
    string babySleep1;
    string babySleep2;
    string babyGroom1;
    string babyGroom2;
    //유년기 단계
    string childFeed1;
    string childFeed2;
    string childTrain1;
    string childTrain2;
    string childPlay1;
    string childPlay2;
    string childBrushing1;
    string childBrushing2;

    //청소년 단계
    string teenPremiumFeed1;
    string teenPremiumFeed2;
    string teenFly1;
    string teenFly2;
    string teenBath1;
    string teenBath2;
    //성체 단계
    string adultHunt1;
    string adultHunt2;
    string adultFlyTrain1;
    string adultFlyTrain2;
    string adultAlone1;
    string adultAlone2;
    string adultBath1;
    string adultBath2;

    
    //엔딩 아트ending art---------------------
    string ENDING_escapeFly;
    string ENDING_returnNature;
    string ENDING_shoulderFriend;
    string ENDING_happyFly;
    string ENDING_fatBird;
    string ENDING_TTSTAR; //혹시 이 주석을 본 사람이 있다면 방주의 아이돌, T.T.STAR 많은 관심부탁드립니다 
    string ENDING_modelBird;
    string ENDING_fossilEgg;
    string BadENDING_starvation;
    string BadENDING_soldier;

public:
    UIRenderer();
    string drawBar(int value, int twidth);
    void SlowPrint(const string& text, bool newLine = true, int charDelayMs = 50, int lineDelayMs = 300);
    void StatsRender(string name, int mainSat, int stress, int clean, int ful, int trn);
    void ArtRender(int growthStage, int Action);
    void ActionArtRender(const string& actionName, string name, int mainSat, int stress, int clean, int ful, int trn);
    void EndingArtRender(const string& endingArtName);
    void ChioceRender(int week, int actions);
    void ActionResultRender(const string& actionName, const StatChange& statChange, const string& item, bool isRebellious, const string& birdName);

    //void UI_Manage(const ActionManager& actionManger)const;
};

#endif // !UI_H
