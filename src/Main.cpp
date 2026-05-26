#include <windows.h>
#include "../include/GameManager.h"
#include <iostream>
#include <cstdlib>  // srand, rand
#include <ctime>    // time

int main() {
	// 터미널 출력과 입력을 UTF-8로 설정
	system("chcp 65001");
	system("cls"); // 인코딩 변경 메시지 삭제

	SetConsoleOutputCP(65001);

	// 난수 시드 초기화 — 게임 시작 시 한 번만 호출해야 rand()가 매번 다른 결과를 냄
	// handleEnding()의 50/50 엔딩 분기 등 rand() 사용처 전체에 영향
	srand(static_cast<unsigned int>(time(NULL)));

	GameManager gameManager;
	gameManager.run(); // 게임 실행

	return 0;
}