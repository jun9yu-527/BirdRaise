#include <iostream>
#include <string>
#include "UIRenderer.h"
using namespace std;

UIRenderer::UIRenderer() {
    //아스키 아트 예시 추후 선택지에 따라서 애니매이션을 넣은다면 간결화 및 더 많은 아트 필요
    eggArt = R"(



















               _____   
              /      .  
             |       | 
             |       | 
              ._____/ 




    )";
    birdArt = R"(
             aaaaaaaaaa
          aaaa         a:.
        aa                :
       a        {o}\` , =;;:
      a      //(       =     \
     a      //(         |    |
    a  '''' //          |    |
   a   \\      //==..a\/
  a //          Y a\\
 a//8888       YY   aaaaaaa
a88\\\``````   YY  aa///2aaaa
a8888888\\\888  a\\5\\\5\aaa
a(....88888     a//////3//////aa
a(..8888 '''''' 'aa\\\5\\\\\\2\aa
 a ((         YY aa////7///6//aa
  aa\\\\      YYY aaa\\\2\\\\aaa
    a        bbbb    aaa///3/7/aa
     aa      ''''''''''aa\4\\\\\a
      a\\   \\       ....aa\\\\\\
       a  \\  \\    ` . . . a\\\\\\
        aa' ' ' YYY\\\\\  \\\\a\\\\\
           (aa ....|    |  bbbb\\\\\
            aaaaaaa( (  //bbbbbbb/"""""""\
              //    ""// aaaaa// /\ ;\\ ;\\
             //       //         \\\ ;\\ ;\\
            //       //          \\ ;\\ ;\\ ;\\
           //       //           \\\ ;\\\ ;\\ ;\\
   //----//-------//---//----|   \\ ;\\ ;\\ ;\\\ ;\\
  //    //       //    //
    )";
}

string UIRenderer::drawBar(int value, int width) {
    string bar = "[";
    int pos = (value * width) / 100;
    for (int i = 0; i < width; ++i) {
        if (i < pos) bar += "■";
        else bar += " ";
    }
    bar += "] " + to_string(value) + "%";
    return bar;
}
void UIRenderer::StatsRender(string name, int mainSat, int stress, int clean, int ful, int trn) {
    system("cls");

    cout << " [" << name << "의 상태]" << endl;
    cout << " 만족도   " << drawBar(mainSat, 50) << endl << endl;

    cout << " 스트레스 " << drawBar(stress, 20) << "   ";
    cout << " 청결도   " << drawBar(clean, 20) << endl;
    cout << " 포만감   " << drawBar(ful, 20) << "   ";
    cout << " 훈련도   " << drawBar(trn, 20) << endl;
    cout << "\n------------------------------------------------------------\n";
}
void UIRenderer::ArtRender(int growthStage, int Action) {
    //추후 선택한 행동에 따라서 이미지도 바뀔 예정 (Action)
<<<<<<< Updated upstream
    string Art;
=======
    (void)Action;
    string Art = eggArt;
>>>>>>> Stashed changes

    if (growthStage <= 0) {
        Art = eggArt;
    }
    else {
        Art = birdArt;
    }
    cout << Art << endl;
}
<<<<<<< Updated upstream
=======

void UIRenderer::ActionArtRender(const string& actionName, string name, int mainSat, int stress, int clean, int ful, int trn) {
    string actionArt1 = adultArt;
    string actionArt2 = adultArt;

    if (actionName == "품어주기") {
        actionArt1 = eggHug1;
        actionArt2 = eggHug2;
    }
    else if (actionName == "온도조절") {
        actionArt1 = eggTemp1;
        actionArt2 = eggTemp2;
    }
    else if (actionName == "클래식 들려주기") {
        actionArt1 = eggClassic1;
        actionArt2 = eggClassic2;
    }
    else if (actionName == "방치하기") {
        actionArt1 = eggIgnore1;
        actionArt2 = eggIgnore2;
    }
    else if (actionName == "이유식 먹이기") {
        actionArt1 = babyFeed1;
        actionArt2 = babyFeed2;
    }
    else if (actionName == "재우기") {
        actionArt1 = babySleep1;
        actionArt2 = babySleep2;
    }
    else if (actionName == "방 청소하기" || actionName == "털 정리해주기") {
        actionArt1 = babyGroom1;
        actionArt2 = babyGroom2;
    }
    else if (actionName == "일반 모이" || actionName == "견과류" || actionName == "해바라기씨") {
        actionArt1 = childFeed1;
        actionArt2 = childFeed2;
    }
    else if (actionName == "간단한 훈련" || actionName == "학원 보내기") {
        actionArt1 = childTrain1;
        actionArt2 = childTrain2;
    }
    else if (actionName == "가볍게 놀아주기" || actionName == "산책하기") {
        actionArt1 = childPlay1;
        actionArt2 = childPlay2;
    }
    else if (actionName == "고급 모이") {
        actionArt1 = teenPremiumFeed1;
        actionArt2 = teenPremiumFeed2;
    }
    else if (actionName == "비행 기초" || actionName == "비행 훈련") {
        actionArt1 = teenFly1;
        actionArt2 = teenFly2;
    }
    else if (actionName == "목욕시키기") {
        actionArt1 = teenBath1;
        actionArt2 = teenBath2;
    }
    else if (actionName == "먹이 사냥") {
        actionArt1 = adultHunt1;
        actionArt2 = adultHunt2;
    }
    else if (actionName == "비행 훈련") {
        actionArt1 = adultFlyTrain1;
        actionArt2 = adultFlyTrain2;
    }
    else if (actionName == "혼자 두기") {
        actionArt1 = adultAlone1;
        actionArt2 = adultAlone2;
    }
    else if (actionName == "깃털 정리") {
        actionArt1 = adultAlone1;
        actionArt2 = adultAlone2;
    }

    const string statusTexts[6] = {
        "행동중.",
        "행동중..",
        "행동중...",
        "행동중...",
        "행동중.....",
        "행동중....."
    };

    for (int i = 0; i < 6; ++i) {
        StatsRender(name, mainSat, stress, clean, ful, trn);
        cout << (i % 2 == 0 ? actionArt1 : actionArt2) << endl;
        cout << "---------------------------------------------------------------------" << endl;
        cout << "  " << statusTexts[i] << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
    }
}


void UIRenderer::EndingArtRender(const string& endingArtName) {
    string Art = adultArt;

    if (endingArtName == "escapeFly") {
        Art = ENDING_escapeFly;
    }
    else if (endingArtName == "returnNature") {
        Art = ENDING_returnNature;
    }
    else if (endingArtName == "shoulderFriend") {
        Art = ENDING_shoulderFriend;
    }
    else if (endingArtName == "happyFly") {
        Art = ENDING_happyFly;
    }
    else if (endingArtName == "fatBird") {
        Art = ENDING_fatBird;
    }
    else if (endingArtName == "ttStar") {
        Art = ENDING_TTSTAR;
    }
    else if (endingArtName == "modelBird") {
        Art = ENDING_modelBird;
    }
    else if (endingArtName == "fossilEgg") {
        Art = ENDING_fossilEgg;
    }
    else if (endingArtName == "starvation") {
        Art = BadENDING_starvation;
    }
    else if (endingArtName == "sickDead") {
        Art = BadENDING_soldier;
    }

    cout << Art << endl;
}

>>>>>>> Stashed changes
void UIRenderer::ChioceRender(int week, int actions) {
    cout << "------------------------------------------------------------" << endl;
    cout << " 현재: " << week << "주차 | 남은 행동: " << actions << "회" << endl;

    //엑션 메니저 완성이 되면 현재주차에 해당하는 선택지를 받아서 출력할 예정
}

