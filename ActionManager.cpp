#include "ActionManager.h"
#include <cstdlib>

using namespace std;

ActionManager::ActionManager() {
    // 데이터 초기화
    // 스테이지 정의: 알, 유아기, 유년기, 청소년기, 성체
    stage_actions["알"] = { "품어주기", "온도조절", "클래식 들려주기" };
    stage_actions["유아기"] = { "이유식 먹이기", "낮잠 자기", "깃털 고르기" };
    stage_actions["유년기"] = { "먹이 주기", "간단한 훈련", "놀아주기" };
    stage_actions["청소년기"] = { "고급 먹이", "비행 기초", "목욕하기" };
    stage_actions["성체"] = { "사냥 연습", "비행 훈련", "혼자 두기" };

    // { 포만감, 청결도, 훈련도, 스트레스 }
    stat_table["품어주기"] = { 0, 0, 0, -10 };
    stat_table["온도조절"] = { 0, -5, 0, -5 };
    stat_table["클래식 들려주기"] = { 0, 0, 5, -15 };

    stat_table["이유식 먹이기"] = { 20, -5, 0, -5 };
    stat_table["낮잠 자기"] = { -5, 0, 0, -10 };
    stat_table["깃털 고르기"] = { 0, 15, 0, -5 };

    stat_table["먹이 주기"] = { 25, -10, 0, -5 };
    stat_table["간단한 훈련"] = { -10, -5, 10, 10 };
    stat_table["놀아주기"] = { -5, -5, 5, -15 };

    stat_table["고급 먹이"] = { 30, -5, 0, -10 };
    stat_table["비행 기초"] = { -15, -10, 15, 15 };
    stat_table["목욕하기"] = { 0, 30, 0, -5 };

    stat_table["사냥 연습"] = { -20, -15, 20, 20 };
    stat_table["비행 훈련"] = { -15, -10, 25, 15 };
    stat_table["혼자 두기"] = { -5, -5, -5, -20 };
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