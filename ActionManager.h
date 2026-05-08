#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>


using namespace std;

struct StatChange {
    int fullness;    // 포만감
    int cleanliness; // 청결도
    int training;    // 훈련도
    int stress;      // 스트레스
};

class ActionManager {
private:
    // 단계별 행동 리스트 
    map<string, vector<string>> stage_actions;
    // 행동별 수치 변화표 
    map<string, StatChange> stat_table;

public:
    ActionManager();

    // 현재 단계에 맞는 메뉴 반환 
    std::vector<std::string> get_available_actions(std::string stage);

    // 선택한 행동의 결과값 반환
    StatChange get_action_effects(std::string action_name);

    // 확률적 아이템 효과 처리 
    std::string get_random_item();
};
