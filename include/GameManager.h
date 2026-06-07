#pragma once
#include "GameEntity.h"
#include "Time.h"
#include "ActionManager.h"
#include "UIRenderer.h"
#include <string>
using namespace std;

class GameManager {
private:
    GameEntity* bird;           // 새 객체 (동적 생성)
    GameEngine* engine;         // 게임 흐름 제어 객체 (동적 생성)
    ActionManager  actionManager;  // 행동 및 선택지 관리
    UIRenderer     ui;             // 화면 출력 담당

    // 현재 주차로 성장 단계(int) 계산 — getStage() 없이 주차만으로 판별
    // ArtRender(int growthStage, ...) 및 get_available_actions(string) 에 사용
    int         calcStageInt(int week) const;
    string      calcStageName(int week) const;

    void initializeGame();   // 시작 스토리 출력 및 새 이름 설정
    void processTurn();      // 한 턴 처리: UI 출력 → 행동 선택 → 결과 반영 → 턴 진행
    void handleEnding();     // 게임 종료 조건에 따라 엔딩 출력

public:
    GameManager();
    ~GameManager();

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    void run();  // 게임 전체 루프 실행 및 종료 처리
};
