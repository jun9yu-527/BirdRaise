#include "../include/Time.h"
#include <iostream>
using namespace std;

GameEngine::GameEngine(GameEntity* bird_ptr) : current_week(1), week_count(ACTIONS_PER_WEEK), last_grown_week(0), game_state(GameState::Running), bird(bird_ptr) {}

GameEngine::~GameEngine() {}

 // 주 종료 시 패시브 수치 변화 (자연 감소/증가)
void GameEngine::apply_weekly_passive() {
    // 매주 자연적으로 포만감 -10, 청결도 -5, 스트레스 +5
    bird->update_stats(-10, -5, 0, +5);
}

// 성장 단계 자동 체크 및 진화
// getStage() 없이 last_grown_week 으로 중복 성장을 방지하고 setStage() 만 사용
void GameEngine::check_growth() {
    // 이미 이번 주차에 성장 처리를 했으면 스킵
    if (last_grown_week == current_week) return;

    GrowthStage next;
    bool should_grow = true;

    // 주차별 성장 단계 기준
    // 1주차: 알, 2~3주차: 유아기, 4~5주차: 유년기, 6~7주차: 청소년기, 8주차: 성체
    switch (current_week) {
    case 2:  next = GrowthStage::INFANT; break;
    case 4:  next = GrowthStage::CHILD;  break;
    case 6:  next = GrowthStage::TEEN;   break;
    case 8:  next = GrowthStage::ADULT;  break;
    default: should_grow = false;        break;
    }

    if (should_grow) {
        bird->setStage(next);
        last_grown_week = current_week; // 이번 주차 성장 완료 기록
        cout << endl;
        cout << "  ★ [성장] 새가 다음 단계로 성장했습니다!" << endl;
        cout << endl;
    }
}

// 주차 종료 → 다음 주로 이동
void GameEngine::end_week() {
    // 패시브 수치 변화 적용
    apply_weekly_passive();

    // 상태 체크 (사망/도주 조건) — check_status() 내부에서 is_alive 처리
    // 알 단계(1주차)에서는 사망 판정을 하지 않음
    // processTurn()과 동일한 정책: stageInt >= 1(유아기 이상)일 때만 체크
    if (current_week > 1) {
        bird->check_status();
    }

    current_week++;

    if (current_week <= MAX_WEEKS) {
        cout << "  📅 " << (current_week - 1) << "주차가 끝났습니다. "
            << current_week << "주차 시작!" << endl;
        check_growth(); // 주차 변경 시 성장 체크
    }

    // 하루 행동 횟수 초기화
    week_count = ACTIONS_PER_WEEK;
}

// 게임 종료 조건 평가
GameState GameEngine::evaluate_game_state() const {
    if (!bird->getIsAlive()) {
        if (bird->getFullness() <= 0)  return GameState::StarvDead;  // 아사 엔딩
		if (bird->getCleanliness() <= 0) return GameState::SickDead; // 병사 엔딩
        return GameState::OverRan;                                   // 도주 엔딩
    }
    if (current_week > MAX_WEEKS)  return GameState::Clear;
    return GameState::Running;
}

 // 전체적인 게임의 시작부터 엔딩까지의 루프 관리
 // (실제 루프는 GameManager::run() 이 담당)
void GameEngine::run_game_loop() {
    while (game_state == GameState::Running) {
        next_turn();
        game_state = evaluate_game_state();
    }
}

// 플레이어의 선택에 따른 결과값 출력 및 스탯 반영
// 실제 행동 선택과 스탯 반영은 GameManager::processTurn() 이 담당하고,
// 이 메서드는 행동 후 게임 상태를 재평가하는 역할을 수행
void GameEngine::process_action(int choice) {
    (void)choice;
    game_state = evaluate_game_state();
}

// 행동 횟수 차감 및 시간 / 주차 변경 로직
bool GameEngine::next_turn() {
    week_count--;

    if (week_count <= 0) {
        cout << endl;
        cout << "  🌙 한 주가 끝났습니다." << endl;
        end_week();
        game_state = evaluate_game_state();
        return true;
    }

    // 게임 종료 조건 재평가
    game_state = evaluate_game_state();
    return false;
}
