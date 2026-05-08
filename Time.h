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

// 게임 상태
enum class GameState {
    Running,
    StarvDead,  // 아사 엔딩
	SickDead,  // 병사 엔딩
    OverRan,   // 도주 엔딩
    Clear      // 클리어
};

class GameEngine {
private:
    /* ── 속성 ── */
    int         current_week;    // 현재 주차 (1 ~ MAX_WEEKS)
    int         week_count;     // 이번 주차 남은 행동 횟수 (최대 ACTIONS_PER_WEEK)
    int         last_grown_week; // 마지막으로 성장 처리한 주차 (중복 성장 방지)
    GameState   game_state;      // 현재 게임 상태
    GameEntity* bird;            // 관리 중인 새 객체 포인터 (소유권 외부)

    /* ── private 헬퍼 ── */
    void      end_week();               // 주 종료 → 다음 주로 이동
    void      check_growth();              // 성장 단계 자동 체크 및 진화
    void      apply_weekly_passive();       // 주 종료 시 패시브 수치 변화
    GameState evaluate_game_state() const; // 사망/도주/클리어 조건 평가

public:
    /* ── 생성자 / 소멸자 ── */
    explicit GameEngine(GameEntity* bird_ptr);
    ~GameEngine();

    // 복사 금지 (포인터 소유권 문제 방지)
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    // 전체적인 게임의 시작부터 엔딩까지의 루프 관리
    void run_game_loop();

    // 플레이어의 선택에 따른 결과값 출력 및 스탯 반영
    void process_action(int choice);

    // 행동 횟수 차감 및 시간 / 주차 변경 로직
    bool next_turn();

    /* ── Getter ── */
    int       get_current_week() const { return current_week; }
    int       get_week_count()    const { return week_count; }  
    GameState get_game_state()   const { return game_state; }
};