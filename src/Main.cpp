#include <windows.h>
#include "../include/GameManager.h"
#include <iostream>
// 1. 이 헤더를 추가하세요.

int main() {
	// 터미널 출력과 입력을 UTF-8로 설정
	system("chcp 65001");
	system("cls"); // 인코딩 변경 메시지 삭제

	SetConsoleOutputCP(65001);

	GameManager gameManager;
	gameManager.run(); // 게임 실행

	return 0;
}