#pragma once
/*
 * GameEngine.h
 * 게임 흐름 제어 클래스
 * - 8주간의 시간 흐름과 하루 5회의 행동 기회를 제어하는 메인 로직
 */

#include "GameEntity.h"
#include "UIRenderer.h"

constexpr int MAX_WEEKS = 8;
constexpr int ACTIONS_PER_WEEK = 5;

enum class GameState {
    Running,

    // 사망 엔딩
    StarvDead,  // 아사 엔딩
    SickDead,   // 병사 엔딩
    OverRan,    // 도주 엔딩

    // 특수 엔딩
    EggAbandon, // 화석 엔딩

    Clear       // 클리어 (만족도 기반 일반/히든 엔딩)
};

class GameEngine {
private:
    int         current_week;       // 현재 주차 (1 ~ MAX_WEEKS)
    int         remaining_actions;  // 이번 주차 남은 행동 횟수 (최대 ACTIONS_PER_WEEK)
    int         last_grown_week;    // 마지막으로 성장 처리한 주차 (중복 성장 방지)
    GameState   game_state;         // 현재 게임 상태
    GameEntity* bird;               // 관리 중인 새 객체 포인터(소유권 없음)

    void      end_week();                  // 주 종료 후 다음 주로 이동
    void      check_growth();              // 성장 단계 자동 체크 및 진화
    void      apply_weekly_passive();      // 주 종료 후 패시브 수치 변화
    GameState evaluate_game_state() const; // 사망/도주/클리어 조건 평가

public:
    explicit GameEngine(GameEntity* bird_ptr);
    ~GameEngine();

    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    bool next_turn();

    int       get_current_week()      const { return current_week; }
    int       get_remaining_actions() const { return remaining_actions; }
    GameState get_game_state()        const { return game_state; }
};
