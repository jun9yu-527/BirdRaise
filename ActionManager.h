#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

struct StatChange {
    int fullness;    // 포만감
    int cleanliness; // 청결도
    int training;    // 훈련도
    int stress;      // 스트레스
};

class ActionManager {
private:
    // 단계별 메인 행동 리스트 
    map<string, vector<string>> stage_actions;
    
    // 메인 행동 → 서브 선택지 목록
    map<string, vector<string>> sub_actions;

    // 서브 선택지 → 수치 변화
    map<string, StatChange> stat_table;

    // 서브 선택지 → 아이템 보유 필요 여부 (true면 인벤토리에 있어야 표시)
    map<string, bool> requires_item;

    // 서브 선택지 → 훈련 시 랜덤 아이템 획득 여부
    map<string, bool> gives_item;

    // 플레이어 아이템 인벤토리
    set<string> inventory;

    // 획득 가능한 아이템 목록
    vector<string> item_pool;

public:
    ActionManager();

    // 현재 단계에 맞는 메인 행동 목록 반환
    vector<string> get_available_actions(string stage);
 
    // 메인 행동에 딸린 서브 선택지 반환 (아이템 보유 여부 필터링 포함)
    // 서브메뉴가 없는 행동(알 단계 등)은 빈 벡터 반환
    vector<string> get_sub_actions(string main_action);
 
    // 선택지의 수치 변화 반환 (메인 또는 서브 모두 가능)
    StatChange get_action_effects(string action_name);
 
    // 훈련 시 랜덤 아이템 획득 처리 — 획득한 아이템 이름 반환 (없으면 "")
    string try_get_item(string action_name);
 
    // 인벤토리 출력용 문자열 반환
    string get_inventory_string() const;
};
