#include "ActionManager.h"
#include <cstdlib>

ActionManager::ActionManager() {
<<<<<<< Updated upstream
    // 데이터 초기화
    // 스테이지 정의: 알, 유아기, 유년기, 청소년기, 성체
    stage_actions["알"] = { "품어주기", "온도조절", "클래식 들려주기", "방치하기"};
    stage_actions["유아기"] = { "이유식 먹이기", "낮잠 자기", "깃털 고르기" };
    stage_actions["유년기"] = { "먹이 주기", "간단한 훈련", "놀아주기", "빗어주기"};
    stage_actions["청소년기"] = { "고급 먹이", "비행 기초", "목욕하기" };
    stage_actions["성체"] = { "비행 훈련", "혼자 두기", "먹이 사냥", "목욕하기"};

    // { 포만감, 청결도, 훈련도, 스트레스 }
    stat_table["품어주기"] = { 0, 0, 0, -10 };
    stat_table["온도조절"] = { 0, -5, 0, -5 };
    stat_table["클래식 들려주기"] = { 0, 0, 5, -15 };
    stat_table["방치하기"] = { 0, 0, 0, +10 };

    stat_table["이유식 먹이기"] = { 20, -5, 0, -5 };
    stat_table["낮잠 자기"] = { -5, 0, 0, -10 };
    stat_table["깃털 고르기"] = { 0, 15, 0, -5 };

    stat_table["먹이 주기"] = { 25, -10, 0, -5 };
    stat_table["간단한 훈련"] = { -10, -5, 10, 10 };
    stat_table["놀아주기"] = { -5, -5, 0, -15 };
    stat_table["빗어주기"] = { 0, +20, 0, -5 };

    stat_table["고급 먹이"] = { 30, -5, 0, -10 };
    stat_table["비행 기초"] = { -15, -10, 15, 15 };
    stat_table["목욕하기"] = { 0, 30, 0, -5 };

    stat_table["비행 훈련"] = { -15, -10, 25, 15 };
    stat_table["혼자 두기"] = { -5, -5, -5, -20 };
    stat_table["먹이 사냥"] = { +20, -10, +10, +10 };
    stat_table["깃털 정리"] = { 0, +25, 0, -10 };
=======
    /* ── 단계별 메인 행동 목록 ── */
    // 알 단계는 서브메뉴 없이 바로 실행
    stage_actions["알"]       = { "품어주기", "쓰다듬어주기", "방치하기" };
    stage_actions["유아기"]   = { "이유식 먹이기", "청소하기", "재우기", "놀아주기" };
    stage_actions["유년기"]   = { "먹이 주기", "청소하기", "훈련 시키기", "놀아주기" };
    stage_actions["청소년기"] = { "먹이 주기", "청소하기", "훈련 시키기", "놀아주기" };
    stage_actions["성체"]     = { "먹이 주기", "청소하기", "훈련 시키기", "놀아주기" };
 
    /* ── 메인 행동 → 서브 선택지 ── */
    // 유아기는 이유식/재우기가 단일 행동이라 서브메뉴 없음
    // 청소하기/놀아주기는 유아기부터 공통 서브메뉴 사용
    sub_actions["먹이 주기"]   = { "일반 모이", "견과류", "해바라기씨", "고급 모이" };
    sub_actions["청소하기"]    = { "목욕시키기", "방 청소하기" };
    sub_actions["훈련 시키기"] = { "간단한 훈련", "학원 보내기" };
    sub_actions["놀아주기"]    = { "가볍게 놀아주기", "산책하기", "털 정리해주기" };
 
    /* ── 알 단계 직접 행동 수치 ── */
    // { 포만감, 청결도, 훈련도, 스트레스 }
    stat_table["품어주기"]     = {  0,  0, 0, -15 };
    stat_table["쓰다듬어주기"] = {  0,  0, 0, -15 };
    stat_table["방치하기"]     = {  0,  0, 0, +10 };
 
    /* ── 유아기 단일 행동 수치 ── */
    stat_table["이유식 먹이기"] = { +20, -5, 0, -10 };
    stat_table["재우기"]        = {  -5,  0, 0, -15 };
 
    /* ── 먹이 주기 서브 선택지 ── */
    stat_table["일반 모이"]  = { +15, -5, 0, -10 };
    stat_table["견과류"]     = { +20, -5, 0, -15 };
    stat_table["해바라기씨"] = { +20, -5, 0, -15 };
    stat_table["고급 모이"]  = { +25, -5, 0, -20 };
 
    /* ── 청소하기 서브 선택지 ── */
    stat_table["목욕시키기"]  = {  0, +25, 0, +15 };
    stat_table["방 청소하기"] = {  0, +20, 0, +10 };
 
    /* ── 훈련 시키기 서브 선택지 ── */
    stat_table["간단한 훈련"] = {  -5, 0, +20, +15 };
    stat_table["학원 보내기"] = { -10, 0, +30, +30 };
 
    /* ── 놀아주기 서브 선택지 ── */
    stat_table["가볍게 놀아주기"] = {  -5, 0, 0, -10 };
    stat_table["산책하기"]       = { -10, 0, 0, -15 };
    stat_table["털 정리해주기"]  = { -15, 0, 0, -20 };
 
    /* ── 아이템 보유 필요 여부 ── */
    // false(기본값) = 항상 표시, true = 인벤토리에 있어야 표시
    requires_item["일반 모이"]  = false;
    requires_item["견과류"]     = true;
    requires_item["해바라기씨"] = true;
    requires_item["고급 모이"]  = true;
 
    /* ── 훈련 시 랜덤 아이템 획득 여부 ── */
    gives_item["간단한 훈련"] = true;
    gives_item["학원 보내기"] = true;
 
    /* ── 획득 가능한 아이템 풀 ── */
    item_pool = { "견과류", "해바라기씨", "고급 모이" };
>>>>>>> Stashed changes
}

// 현재 단계에 맞는 메뉴 반환 
std::vector<std::string> ActionManager::get_available_actions(std::string stage) {
    if (stage_actions.find(stage) != stage_actions.end()) {
        return stage_actions[stage];
    }
    return {};
}

// 선택한 행동의 결과값 반환
StatChange ActionManager::get_action_effects(std::string action_name) {
    if (stat_table.find(action_name) != stat_table.end()) {
        return stat_table[action_name];
    }
    return {0, 0, 0, 0};
}

// 확률적 아이템 효과 처리 
std::string ActionManager::get_random_item() {
    int chance = rand() % 100;
    if (chance < 20) return "아이템 발견: 반짝이는 깃털! (매력 상승)";
    if (chance > 90) return "아이템 발견: 영양 만점 벌레! (포만감 보너스)";
    return "";
}
