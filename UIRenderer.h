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
    string eggArt;
    string infantArt;
    string childAltArt;
    string childArt;
    string teenArt;
    string adultArt;
    string birdArt;
public:
    UIRenderer();
    string drawBar(int value, int twidth);
    void SlowPrint(const string& text, bool newLine = true, int charDelayMs = 100, int lineDelayMs = 500);
    void StatsRender(string name, int mainSat, int stress, int clean, int ful, int trn);
    void ArtRender(int growthStage, int Action);
    void ChioceRender(int week, int actions);
    void ActionResultRender(const string& actionName, const StatChange& statChange, const string& item, bool isRebellious, const string& birdName);

    //void UI_Manage(const ActionManager& actionManger)const;
};

#endif // !UI_H
