#pragma once
#include <string>

using namespace std;

// 성장 단계 정의 
enum class GrowthStage {
    EGG,        // 알 (1주차)
    INFANT,     // 유아기 (2~3주차)
    CHILD,      // 유년기 (4~5주차)
    TEEN,       // 청소년기 (6~7주차)
    ADULT       // 성체 (8주차)
};

class GameEntity {
private:
    string name;               // 새의 이름

    int fullness;              // 포만감 (0~100)
    int cleanliness;           // 청결도 (0~100)
    int training;              // 훈련도 (0~100)
    int stress;                // 스트레스
    
    GrowthStage stage;         // 현재 성장 단계 
    bool is_alive;             // 생존 여부
    bool is_sick;              // 질병 여부

    bool abandonedEgg;         // 알 단계에서 방치 여부 (화석 엔딩 조건)
    int  eggActionCount;       // 알 단계 총 행동 횟수
    int  eggIgnoreCount;       // 알 단계 방치하기 횟수

public:
    // 생성자: 알 상태로 시작, 초기 스트레스 100 
    GameEntity(string birdName);

    // 수치 변화 적용 메서드
    void update_stats(int f, int c, int t, int s);

    // 상태 체크 메서드 (사망, 질병, 반항, 도주)
    void check_status();

    // 실시간 평균 만족도 계산
    float get_satisfaction() const;

    // 반항 확률 체크
    bool is_rebellious() const;

    // Getter/Setter (UI 및 로직용)
    string getName() const { return name; }
    int getFullness() const { return fullness; }
    int getCleanliness() const { return cleanliness; }
    int getTraining() const { return training; }
    int getStress() const { return stress; }
    bool getIsAlive() const { return is_alive; }
    void setStage(GrowthStage s) { stage = s; }

    // 알 단계 방치 기록 — isIgnore: 방치하기 선택 시 true
    void recordEggAction(bool isIgnore) {
        eggActionCount++;
        if (isIgnore) eggIgnoreCount++;
        // 5번 행동 중 모두 방치하기면 abandonedEgg = true
        if (eggActionCount >= 5 && eggIgnoreCount >= 5) abandonedEgg = true;
    }

    bool getAbandonedEgg() const { return abandonedEgg; }
};