#include "GameEntity.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>

using namespace std;

// 생성자: 알 상태로 시작, 초기 스트레스 100 
GameEntity::GameEntity(string birdName) 
    : name(birdName), fullness(50), cleanliness(100), 
      training(0), stress(100), stage(GrowthStage::EGG), 
      is_alive(true), is_sick(false), abandonedEgg(false) {}

// 수치 변화 적용 메서드
void GameEntity::update_stats(int f, int c, int t, int s) {
    // clamp를 사용하여 0~100 범위를 강제함
    fullness = clamp(fullness + f, 0, 100);
    cleanliness = clamp(cleanliness + c, 0, 100);
    training = clamp(training + t, 0, 100);
    stress = clamp(stress + s, 0, 100);
}

// 상태 체크 메서드 (사망, 질병, 반항, 도주)
void GameEntity::check_status() {
    // 사망 조건: 포만감 0일 때 확정 사망
    if (fullness <= 0) {
        is_alive = false;
        cout << "새가 굶주림으로 인해 아사했습니다..." << endl;
    }
    
    // 사망 조건: 청결도 0일 때 확정 사망
    if (cleanliness <= 0) {
        is_alive = false;
        cout << "새가 질병으로 인해 병사했습니다..." << endl;
    }

    // 질병 발생: 청결도 25 이하
    is_sick = (cleanliness <= 25);

    // 도주 조건: 스트레스 90~100
    if (stress >= 90) {
        is_alive = false;
        cout << "새가 스트레스를 견디지 못하고 자유를 찾아 떠났습니다!" << endl;
    }
}

// 실시간 평균 만족도 계산
// 공식: (포만감 + 청결도 + 훈련도( 100  - 스트레스) / 4
float GameEntity::get_satisfaction() const {
    float total = fullness + cleanliness + training + (100 - stress);
    return total / 4.0f;
}

// 반항 확률 체크 (스트레스 50 이상일 때 20% 확률)
bool GameEntity::is_rebellious() const {
    if (stress >= 50) {
        return (rand() % 100) < 20; 
    }
    return false;
}
