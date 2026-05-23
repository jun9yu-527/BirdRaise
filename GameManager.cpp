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
    ui.SlowPrint("  오늘도 평범한 하루가 될 것 같았다.");
    ui.SlowPrint("  그런데 길을 걷다가 수풀 사이에서");
    ui.SlowPrint("  작고 따뜻한 무언가를 발견했다.");
    cout << endl;

    // UIRenderer 의 eggArt 를 활용 — ArtRender(0, 0) 이 알 아트 출력
    ui.ArtRender(0, 0);

    ui.SlowPrint("  ......알이었다.");
    ui.SlowPrint("  버려진 것인지, 떨어진 것인지 알 수 없지만");
    ui.SlowPrint("  이 작은 생명을 그냥 지나칠 수는 없었다.");
    cout << endl;
    ui.SlowPrint("  앞으로 8주, 최선을 다해 이 새를 돌봐주세요.");
    cout << "============================================================" << endl;
    cout << endl;

    string birdName;
    ui.SlowPrint("  새에게 이름을 붙여주세요: ", false);
    cin >> birdName;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << endl;
    ui.SlowPrint("  [" + birdName + "] (이)라는 이름을 붙여주었다.");
    ui.SlowPrint("  새로운 인연이 시작된다...");
    cout << endl;
    ui.SlowPrint("  아무 키나 눌러 시작");
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
    if (engine->get_current_week() == 1)
    {
        bird->recordEggAction(selectedAction == "방치하기");
    }
    StatChange sc = actionManager.get_action_effects(selectedAction);

    // 7) 스탯 반영
    bird->update_stats(sc.fullness, sc.cleanliness, sc.training, sc.stress);

    // 8) 확률적 아이템 효과 처리
    string item = actionManager.get_random_item();

    // 9) 반항 체크 — 유아기(stageInt >= 1) 이상일 때만 체크
    bool isRebellious = false;
    if (stageInt >= 1 && bird->is_rebellious()) {
        isRebellious = true;
        bird->update_stats(0, 0, 0, 10);
    }

    // 10) 선택지 목록을 지우고, 변화가 반영된 스탯과 행동 아트를 출력
    ui.ActionArtRender(
        selectedAction,
        bird->getName(),
        static_cast<int>(bird->get_satisfaction()),
        bird->getStress(),
        bird->getCleanliness(),
        bird->getFullness(),
        bird->getTraining()
    );
    ui.ActionResultRender(selectedAction, sc, item, isRebellious, bird->getName());

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

    // 최종 만족도
    float sat = bird->get_satisfaction();
    // 포만감
    int full = bird->getFullness();
    // 훈련도
    int train = bird->getTraining();
    // 청결도
    int clean = bird->getCleanliness();

    cout << "============================================================" << endl;
    cout << "                      게  임  종  료                       " << endl;
    cout << "============================================================" << endl;
    cout << endl;
    cout << "  새의 이름  : " << bird->getName() << endl;
    cout << "  최종 만족도: " << sat << " / 100" << endl;
    cout << endl;


    // 1. 알을 1주차 내내 방치한 경우는 다른 상태보다 화석 엔딩을 우선한다.
    if (bird->getAbandonedEgg())
    {
        ui.EndingArtRender("fossilEgg");
        cout << "  " << bird->getName() << "(이)가 갑자기 생각이 났다." << endl;
        cout << "  '오랜만에 상태 확인을 해볼까'" << endl;
        cout << "  '...어라? 이게 왜...'" << endl;
        cout << "  '화석이 되어버린거야..?'" << endl;
        cout << "  " << bird->getName() << "(이)는 부화하지 못하고 화석이 되어버렸다." << endl;
        cout << endl;
        cout << "         [ 히든 엔딩 - 화석이 되어버린 알 ]" << endl;
        return;
    }

    // 2. 게임 상태 이상 엔딩 (아사 / 병사 / 도주)
    if (state == GameState::StarvDead)
    {
        ui.EndingArtRender("starvation");
        cout << "  " << bird->getName() << "(이)가 굶주림으로 인해 아사 했습니다..." << endl;
        cout << "  내가 너무 무관심 했나.." << endl;
        cout << "  다음 생엔 더 좋은 집사 만나서 행복하게 살아!" << endl;
        cout << endl;
        cout << "         [ 아사 엔딩 - 잘 챙겨주지 못해서 미안해 ]" << endl;
        return;
    }

    if (state == GameState::SickDead)
    {
        ui.EndingArtRender("sickDead");
        cout << "  " << bird->getName() << "(이)가 질병으로 인해 병사했습니다..." << endl;
        cout << "  '병원에 데려가지 못해서 미안해.'" << endl;
        cout << endl;
        cout << "         [ 병사 엔딩 - 집이 언제 이렇게 더러워졌지? ]" << endl;
        return;
    }

    if (state == GameState::OverRan)
    {
        ui.EndingArtRender("escapeFly");
        cout << "  " << bird->getName() << "(이)가 스트레스를 견디지 못하고 자유를 찾아 떠났습니다!" << endl;
        cout << "  '어? 어디갔지..??'" << endl;
        cout << "  '창문을 열고 날아갔구나..'" << endl;
        cout << "  '행복하고 자유로운 삶을 살 길 바랄게!'" << endl;
        cout << endl;
        cout << "         [ 도주 엔딩 - 막상 떠나니 아쉽네.. ]" << endl;
        return;
    }

    // 3. 특수 조건 엔딩 (히든 / 스타 / 모델)
    if (full >= 100 && sat >= 100)
    {
        ui.EndingArtRender("fatBird");
        cout << "  " << bird->getName() << "(은)는 너무 많이 먹고 움직이지 않아\n" << endl;
        cout << "  날지 못하게 되었다.\n" << endl;
        cout << "  '다이어트 좀 해야되는거 아니야?'" << endl;
        cout << "  " << bird->getName() << "(은)는 당신과 함께하는게 행복한 듯 하다.\n" << endl;
        cout << endl;
        cout << "         [ 히든 엔딩 - 새이기를 포기한거야? ]" << endl;
        return;
    }

    if (train >= 100 && sat >= 100)
    {
        ui.EndingArtRender("ttStar");
        cout << "  " << bird->getName() << "의 묘기를 찍은 영상이 조회수가 1억뷰가 넘었다.\n" << endl;
        cout << "  '이대로 인기스타 되는거 아니야?'\n" << endl;
        cout << "  세계 곳곳에서 방송 출연 제의를 받게 되었다." << endl;
        cout << "  " << bird->getName() << "(은)는 세계적인 인기스타가 되었다.\n" << endl;
        cout << endl;
        cout << "         [ 히든 엔딩 - 너 덕분에 내가 인생 폈다! ]" << endl;
        return;
    }

    if (clean >= 100 && sat >= 80)
    {
        ui.EndingArtRender("modelBird");
        cout << "  " << bird->getName() << "의 털이 아름답게 빛나기 시작했다.\n" << endl;
        cout << "  인별에 사진을 올렸는데 모델 제의를 받게 되었다.\n" << endl;
        cout << "  사진 작가 - '진짜 이렇게 아름다운 새는 처음 봅니다.'" << endl;
        cout << "  " << bird->getName() << "(은)는 세계적인 조류 잡지의 모델이 되었다.\n" << endl;
        cout << endl;
        cout << "         [ 히든 엔딩 - 조류 잡지 모델이 되었다. ]" << endl;
        return;
    }


    // 4. 만족도(sat) 기준 일반 엔딩
    if (sat <= 25)
    {
        ui.EndingArtRender("escapeFly");
        cout << "  " << bird->getName() << "(이)가 날아갔다." << endl;
        cout << "  저 멀리, 한 번도 돌아보지 않고." << endl;
        cout << "  ...더 잘 해줄 수 있었는데." << endl;
        cout << endl;
        cout << "         [ 배드 엔딩 - 아쉬운 이별 ]" << endl;
    }
    else if (sat <= 50)
    {
        ui.EndingArtRender("returnNature");
        cout << "  " << bird->getName() << "(이)가 힘차게 날갯짓을 하며" << endl;
        cout << "  드넓은 하늘 속으로 사라졌다." << endl;
        cout << "  잘 가, 잘 살아!" << endl;
        cout << endl;
        cout << "         [ 노말 엔딩 - 아쉬운 이별 ]" << endl;
    }
    else if (sat <= 75)
    {
        srand(time(NULL));

        if (rand() % 2)
        {
            ui.EndingArtRender("shoulderFriend");
            cout << "  " << bird->getName() << "(이)가 하늘로 날아오르려다..." << endl;
            cout << "  이내 다시 당신의 어깨 위로 내려앉는다." << endl;
            cout << "  '...어라? 이게 아닌데.'" << endl;
            cout << "  '뭐, 잘 됐나...?'" << endl;
            cout << endl;
            cout << "         [ 굿 엔딩? - 예상치 못한 동거 ]" << endl;
        }
        else
        {
            ui.EndingArtRender("happyFly");
            cout << "  " << bird->getName() << "(이)가 힘차게 날갯짓을 하며" << endl;
            cout << "  드넓은 하늘 속으로 사라졌다." << endl;
            cout << "  잘 가, 잘 살아!" << endl;
            cout << endl;
            cout << "         [ 굿 엔딩 - 방생 성공 ]" << endl;
        }
    }
    else
    {
        ui.EndingArtRender("shoulderFriend");
        cout << "  " << bird->getName() << "(이)가 날아가지 않고 옆에 머물렀다." << endl;
        cout << "  나랑 떨어지기 싫다는 것일까?" << endl;
        cout << "  나도 정이 많이 들긴 했어." << endl;
        cout << "  기왕 이렇게 된거 계속 같이 살자!" << endl;
        cout << endl;
        cout << "         [ 굿 엔딩 - 앞으로도 잘 부탁해 ]" << endl;
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
