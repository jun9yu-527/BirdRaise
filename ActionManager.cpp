#include "ActionManager.h"
#include <cstdlib>
using namespace std;

ActionManager::ActionManager() {
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
    sub_actions["놀아주기"]    = { "쓰다듬어주기", "산책하기", "털 정리해주기" };
 
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
    stat_table["쓰다듬어주기"]   = {  -5, 0, 0, -10 };
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
}
 
// 현재 단계에 맞는 메인 행동 목록 반환
vector<string> ActionManager::get_available_actions(string stage) {
    if (stage_actions.find(stage) != stage_actions.end()) {
        return stage_actions[stage];
    }
    return {};
}
 
// 메인 행동에 딸린 서브 선택지 반환
// requires_item 이 true 인 항목은 인벤토리에 있을 때만 포함
vector<string> ActionManager::get_sub_actions(string main_action) {
    if (sub_actions.find(main_action) == sub_actions.end()) {
        return {};  // 서브메뉴 없는 행동 (알 단계 등)
    }
 
    vector<string> result;
    for (const string& sub : sub_actions[main_action]) {
        // 아이템 불필요 항목은 항상 포함
        if (requires_item.find(sub) == requires_item.end() || !requires_item.at(sub)) {
            result.push_back(sub);
            continue;
        }
        // 아이템 필요 항목은 인벤토리에 있을 때만 포함
        if (inventory.count(sub) > 0) {
            result.push_back(sub);
        }
    }
    return result;
}
 
// 선택지의 수치 변화 반환
StatChange ActionManager::get_action_effects(string action_name) {
    if (stat_table.find(action_name) != stat_table.end()) {
        return stat_table[action_name];
    }
    return { 0, 0, 0, 0 };
}
 
// 훈련 시 랜덤 아이템 획득 처리 (40% 확률)
string ActionManager::try_get_item(string action_name) {
    if (gives_item.find(action_name) == gives_item.end() || !gives_item.at(action_name)) {
        return "";
    }
    if ((rand() % 100) < 40) {
        string acquired = item_pool[rand() % item_pool.size()];
        inventory.insert(acquired);
        return acquired;
    }
    return "";
}
 
// 인벤토리 출력용 문자열 반환
string ActionManager::get_inventory_string() const {
    if (inventory.empty()) return "없음";
    string result;
    for (const string& item : inventory) {
        if (!result.empty()) result += ", ";
        result += item;
    }
    return result;
}
