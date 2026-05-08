#include "GameManager.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

GameManager::GameManager() : bird(nullptr), engine(nullptr) {}

GameManager::~GameManager() {
    delete engine;
    delete bird;
}

 // ArtRender 에 넘길 int 값 반환 (0 = 알, 1 이상 = 새)
int GameManager::calcStageInt(int week) const {
    if (week <= 1) return 0;  // 알
    if (week <= 3) return 1;  // 유아기
    if (week <= 5) return 2;  // 유년기
    if (week <= 7) return 3;  // 청소년기
    return 4;                 // 성체
}

// ActionManager::get_available_actions 에 넘길 한글 단계명 반환
string GameManager::calcStageName(int week) const {
    if (week <= 1) return "알";
    if (week <= 3) return "유아기";
    if (week <= 5) return "유년기";
    if (week <= 7) return "청소년기";
    return "성체";
}

 // 게임 초기화: 시작 스토리 출력 및 새 이름 설정
void GameManager::initializeGame() {
    system("cls");

    cout << "============================================================" << endl;
    cout << "                    새  키  우  기                         " << endl;
    cout << "============================================================" << endl;
    cout << endl;
    cout << "  오늘도 평범한 하루가 될 것 같았다." << endl;
    cout << "  그런데 길을 걷다가 수풀 사이에서" << endl;
    cout << "  작고 따뜻한 무언가를 발견했다." << endl;
    cout << endl;

    // UIRenderer 의 eggArt 를 활용 — ArtRender(0, 0) 이 알 아트 출력
    ui.ArtRender(0, 0);

    cout << "  ......알이었다." << endl;
    cout << "  버려진 것인지, 떨어진 것인지 알 수 없지만" << endl;
    cout << "  이 작은 생명을 그냥 지나칠 수는 없었다." << endl;
    cout << endl;
    cout << "  앞으로 8주, 최선을 다해 이 새를 돌봐주세요." << endl;
    cout << "============================================================" << endl;
    cout << endl;

    string birdName;
    cout << "  새에게 이름을 붙여주세요: ";
    cin >> birdName;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << endl;
    cout << "  [" << birdName << "] (이)라는 이름을 붙여주었다." << endl;
    cout << "  새로운 인연이 시작된다..." << endl;
    cout << endl;
    cout << "  아무 키나 눌러 시작" << endl;
    cin.get();

    bird = new GameEntity(birdName);
    engine = new GameEngine(bird);
}

// 한 턴 처리: UI 출력 → 행동 선택 → 결과 반영 → 턴 진행
void GameManager::processTurn() {
    int week = engine->get_current_week();
    int stageInt = calcStageInt(week);
    string stageName = calcStageName(week);

    // 1) 스탯 UI 출력
    ui.StatsRender(
        bird->getName(),
        static_cast<int>(bird->get_satisfaction()),
        bird->getStress(),
        bird->getCleanliness(),
        bird->getFullness(),
        bird->getTraining()
    );

    // 2) 성장 단계에 맞는 아스키 아트 출력 (0=알 아트, 1이상=새 아트)
    ui.ArtRender(stageInt, 0);

    // 3) 현재 단계에 맞는 행동 목록 가져오기
    vector<string> actions = actionManager.get_available_actions(stageName);

    // 4) 주차/남은 행동 횟수 + 선택지 출력
    ui.ChioceRender(week, engine->get_week_count());
    cout << endl;
    for (int i = 0; i < (int)actions.size(); i++) {
        cout << "  [" << (i + 1) << "] " << actions[i] << endl;
    }
    cout << endl;

    // 5) 플레이어 입력
    int choice = 0;
    while (true) {
        cout << "  행동을 선택하세요 (1~" << actions.size() << "): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice >= 1 && choice <= (int)actions.size()) break;

        cout << "  잘못된 선택입니다. 다시 입력해주세요." << endl;
    }

    // 6) 선택한 행동의 수치 변화 가져오기
    string selectedAction = actions[choice - 1];
    StatChange sc = actionManager.get_action_effects(selectedAction);

    // 7) 스탯 반영
    bird->update_stats(sc.fullness, sc.cleanliness, sc.training, sc.stress);

    // 8) 결과 메시지 출력
    cout << endl;
    cout << "  > " << selectedAction << " 을(를) 했습니다." << endl;
    cout << "    포만감 " << (sc.fullness >= 0 ? "+" : "") << sc.fullness
        << "  청결도 " << (sc.cleanliness >= 0 ? "+" : "") << sc.cleanliness
        << "  훈련도 " << (sc.training >= 0 ? "+" : "") << sc.training
        << "  스트레스 " << (sc.stress >= 0 ? "+" : "") << sc.stress
        << endl;

    // 9) 확률적 아이템 효과 처리
    string item = actionManager.get_random_item();
    if (!item.empty()) {
        cout << "  ★ " << item << endl;
    }

    // 10) 반항 체크 — 유아기(stageInt >= 1) 이상일 때만 체크
    if (stageInt >= 1 && bird->is_rebellious()) {
        cout << "  !! " << bird->getName()
            << "(이)가 반항합니다! 스트레스가 추가 상승합니다." << endl;
        bird->update_stats(0, 0, 0, 10);
    }

    // 11) 상태 체크 (사망 / 도주 조건) — 알(stageInt == 0) 이 아닐 때만
    if (stageInt >= 1) {
        bird->check_status();
    }

    // 12) 턴 진행 — 주차 종료 시 문구가 보이도록 키 입력 대기
    cout << endl;
    cout << "  아무 키나 눌러 계속" << endl;
    cin.get();

    bool week_ended = engine->next_turn();

    // 주차가 종료됐을 때 성장/주차 문구가 cls에 지워지지 않도록 추가 대기
    if (week_ended && engine->get_game_state() == GameState::Running) {
        cout << endl;
        cout << "  아무 키나 누르면 다음 주차로 넘어갑니다." << endl;
        cin.get();
    }
}

// 엔딩 처리
void GameManager::handleEnding() {
    // 알 상태에서는 엔딩 화면 없이 게임 계속 (호출 자체를 막음)
    int stageInt = calcStageInt(engine->get_current_week());
    if (stageInt == 0) return;

    system("cls");

    GameState state = engine->get_game_state();
    float     satisfaction = bird->get_satisfaction();

    // 엔딩에서도 현재 단계 아트 출력
    ui.ArtRender(stageInt, 0);

    cout << "============================================================" << endl;
    cout << "                      게  임  종  료                       " << endl;
    cout << "============================================================" << endl;
    cout << endl;
    cout << "  새의 이름  : " << bird->getName() << endl;
    cout << "  최종 만족도: " << satisfaction << " / 100" << endl;
    cout << endl;

    if (state == GameState::StarvDead) {
        cout << "새가 굶주림으로 인해 아사했습니다..." << endl;
        cout << "         [ 아사 엔딩 ]" << endl;

    }
    else if (state == GameState::SickDead) {
        cout << "새가 질병으로 인해 병사했습니다..." << endl;
        cout << "         [ 병사 엔딩 ]" << endl;

    }
    else if (state == GameState::OverRan) {
        cout << "새가 스트레스를 견디지 못하고 자유를 찾아 떠났습니다!" << endl;
        cout << "         [ 도주 엔딩 ]" << endl;

    }
    else {
        if (satisfaction >= 75) {
            cout << "  " << bird->getName() << "(이)가 하늘로 날아오르려다..." << endl;
            cout << "  이내 다시 당신의 어깨 위로 내려앉는다." << endl;
            cout << "  '...어라? 이게 아닌데.'" << endl;
            cout << "  '뭐, 잘 됐나...?'" << endl;
            cout << endl;
            cout << "         [ 굿 엔딩? - 예상치 못한 동거 ]" << endl;
        }
        else if (satisfaction >= 50) {
            cout << "  " << bird->getName() << "(이)가 힘차게 날갯짓을 하며" << endl;
            cout << "  드넓은 하늘 속으로 사라졌다." << endl;
            cout << "  잘 가, 잘 살아!" << endl;
            cout << endl;
            cout << "         [ 굿 엔딩 - 방생 성공 ]" << endl;
        }
        else {
            cout << "  " << bird->getName() << "(이)가 날아갔다." << endl;
            cout << "  저 멀리, 한 번도 돌아보지 않고." << endl;
            cout << "  ...더 잘 해줄 수 있었는데." << endl;
            cout << endl;
            cout << "         [ 노말 엔딩 - 아쉬운 이별 ]" << endl;
        }
    }

    cout << endl;
}

void GameManager::run() {
    initializeGame();

    while (engine->get_game_state() == GameState::Running) {
        processTurn();
    }

    handleEnding();
}