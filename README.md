# HangeulClock_DIY_v2
한글시계 v2 - v1을 바탕으로 완전히 새롭게 만드는 v2

## 제작동기: 한글시계 v1을 만들면서 든 한 가지 생각...

'이야 남의 것 배껴서 똑같이 만들었는데 내가 응용해서 만든 것도 하나 있어야지 않겠나...' \
그래서 만들게 된 한글시계 v2 이다. \
-사실 말만 v2이지 상당히 다른 구조를 가지고 있다.

## 이번 프로그래밍에 중요한 사항

특별히 중요한 것은 모든 것을 "함수화"한다는 것이다.

모든 함수는 각자의 기능에 충실하게 작성되어야 하며, 함수 용도 외의 타용도의 코드가 작성되면 안 된다. 반드시 해당 기능만 있어야 한다.

## V2 개선사항(고쳐야 할 점)

PCB에서 flicker 제어선으로 빠지는 곳에 있는 10k옴저항 제거

PCB에 RESET button 추가 및 케이스에 이를 적용

케이스 무게 어떻게든 줄이기(디자인 조정), 전선 꼽히는 부분 조절

외부밝기측정부 재설계(부품이 안 맞으니 조립이 안 됨)

시계의 가장자리 부분으로 빛이 샌다. 이것을 해결할 방법을 찾아야 함.

회로설계에서 버튼핀 재배열 좀 하자(어째서 모드버튼만 2번인가)

아두이노 내부의 전체 순서를 결정하는 time 변수는 이름을 internal_millis 내지는 '내부의 시간기준'이란 뜻을 가질 수 있는 이름으로 변경하기

한글시계 온습도 표시 시에 온도 및 습도(단위)는 네오픽셀 색깔을 다르게 하자.\
색깔이 같으면 숫자 읽을 때 햇갈릴 수도 있을 듯

스위치 디바운싱을 소프트웨어로 처리하지말고 하드웨어로 처리하자. 그럼 얼마나 편하겠니...

## 구현해야하는 기능

--무엇보다 버튼제어로 특정 기능이 작동되게 짜는것이 급선무.

플리커 기능

LED색상모드 및 밝기모드 제어 추가점 

온습도 센서 처음부터 코드 짜기

알람 기능 추가

알람의 기능이 바뀌었다. 초기에는 시간 및 알람 버튼을 누를 때, 알람 활성화 및 비활성화를 하고 알람버튼만 짧게 누를 시에는 알람 활성화 여부를 보여줬는데, 신규 방법은, 알람 버튼을 짧게 누르면 알람의 상태가 바뀜과 동시에 바뀐 상태로 보여준다.

알람을 구동하는 코드의 수정이 필요합니다. 

* 수정 필요한 코드

0시가 될 때에 플리커가 작동하지 않는다. 
소프트웨어가 꼬인 것으로 보임

길게 누르면 1이 나옴...(삼항 연산자 뒤의 값)

시간 변경 모드 진입 시 처음에 0시가 나오는 오류가 있음.

모든 3항연산자 잘못 작성함. +1을 변수+=1로 고쳐야 한다.

## 제작시 참고사항

v1과 달라진 점:

1. 중앙 flicker 추가 - 3번 핀 아날로그 출력 연결

2. 온습도 센서 추가    - 8번 핀 디지털 연결

3. 조도센서 추가     - A1 핀 아날로그 입력 연결

4. 부저 추가 (알람)  - 7번 핀 디지털 연결

5. 각종 버튼 추가    - 각 기능 구현

## 기능별 설명

네오픽셀은 기존의 무지개 색상 및 WarmLED를 추가하였고 이걸 따로 조합해야함

이번 버전에서는 Warm White 기본, 쿨화이트, 무지개 순으로 색깔 진행

~~랜덤의 경우 더 많은(30색)의 색상에서 랜덤으로 추출 가능하도록 설정~~

---

플리커(중앙에 깜빡이는 놈)는 1초마다 켜젔다 꺼졌다를 반복함

타임라인 상으로 보자면

0.0 --- 1.0 --- 2.0 --- 3.0 --- 4.0 s\
O N --- OFF --- O N --- OFF --- O N ... \
이런 형태를 취함

그리고, 이렇게 ON OFF 가 바뀌는 과졍에서 
조금 부드럽게, 그러나 탄력있게 바뀌도록 설계 \
마치 모션 그래픽처럼 \
+탄력있는 건 매우 어려움 \
+플리커는 모든 기능에서 깜박인다, 2초 간격

---

조도센서는\
밝기 조정 모드에서 밝기를 외부 조도에 맞게 
제어하는 모드를 넣음으로써 구현

--------------------------------

온습도 센서는

시계 전면의 버튼을 통하여 시계 - 온습도 표시 모드 변경함\
이 떄, 온도 또는 습도를 2자리 7세그먼트로 보여줌\
ex) *은 LED 점등, .은 LED 소등

    * * *  * * *

    . . *  * . *

    * * *  . . *

    * . .  . . *

    * * *  . . *

    x . .  . . -

    (-는 온습도  LED입니다)

센서는 2초에 1회 업데이트 되며 
평상시에는 업데이트 하지 않다가 
온습도 모드 진입시에 온습도 데이터를 업데이트 하여 표시

--------------------------------

알람 기능은

알람 설정 버튼을 길게 누름으로서 알람 설정 모드로 진입 가능\
알람은 시간 설정하는 방법과 동일하게 설정이 가능하며

알람 설정 버튼을 짧게 누르면 2~5초 동안 알람이 켜져있는지 꺼져있는지가

화면에 O 또는 X 로 나타나게 됨

기능 구현은

가능하다면 DS3231의 내부 알람 기능을 활용하면 좋겠지만

안 된다면 아두이노에서 코드로 작성하는 것으로 함.

--------------------------------

버튼별 기능은

- 시계 전면의 모드 전환 버튼: 모드 전환 역할

   이 버튼은 눌렀다가 땔 때 시간에 관계없이 모드만 바꿔줌.

- 시계 측면 위의 LED 버튼: LED관련 제어 역할

   이 버튼은 짧게 누르면 밝기 변화 길게 누르면 색상이 변화함.

   색상 변화는 네오픽셀 항목 참조.

- 시계 측면 둘째 시간 버튼: 시간을 수정하는 역할

   이 버튼은 처음에 짧게 누르면 반응 없음 길게 누르면 시간 수정 모드로 젼환함.

   시간 수정모드에 진입하게 되면 모든 시간을 표시하는 LED가 깜박임

   이후, 시간 버튼을 짧게 누르면 시단위 1씩 추가 알람 버튼을 짧게 누르면 분단위 1씩 추가된다.

   시간 버튼을 길게 누른다면 다시 한 변 모든 시간을 표시하는 LED가 깜박이며

   시간 수정 모드는 종료됨.

- 시계 측면 셋째 알람 버튼: 알람을 설정하는 버튼

   이 버튼은 처음에 짧게 누르면 알람이 활셩화 되어 있는지 아닌지 표시함 \
   시간 버튼과 알람 버튼을 같이 누르게 된다면(오차 20ms이내) 알람을 활성화&비활성화함 \
   이 때 가능하다면 \
    알람이 활성화 될 때는 X가 사라지며 O가 나타나고 \
    알람이 비활성화 될 때는 O가 사라지며 X가 나타나도록 한다.

   처음에 길게 누르면 알람 설정을 하게 된다. \
   이후, 시간 버튼을 짧게 누르면 알람 시단위 1씩 추가 \
   알람 버튼을 짧게 누르면 알람 분단위 1씩 추가된다.\
   알람 버튼을 길게 누른다면 O가 나타나고, 알람은 활성화 된 상태로 알람 설정 모드는 종료됨.

## 제작일기
사실 깃헙 readme를 작성하면서 뭘 적아야 될지 모르겠어서 그냥 일기나 적으려 한다.

2021-03 어느 날: 한글시계 v2에 대한 소망과 희망을 가지고 대략적인 디자인 및 기능들을 일주일에 걸쳐 구상한다.

2021-05-29: 한글시계 v2 디자인 착수. 앞판 디자인 완료. 

2021-05-30: 한글시계 v2 의 기본적인 디자인 완료. 

2021-06-07: 깃헙 레포지토리 생성.

2021-06-23: 오랜만에 이 프로젝트를 건드렸다.
지난 2주간 무드등 한글시계 작업한다고 어찌나 바빴던지...
여튼 오늘은, 프로젝트의 실제 파일들을 복사하고, 깃 레포지지토리와 연동시켰다.

2021-08-31: 진짜 오랜만에 일지를 쓴다.
그동안 무드등 한글시계 PCB 작업하느랴 이것저것 만지느랴 바빴다.
그러나 지난 2주 동안, 디자인 및 PCB작업을 모두 완료하였으며, 다음주 중으로 모든 부품과
PCB가 온다.
이제 납땜을 하고 프로그램을 올리면 끝이 난다.
추석 전으로 마무리 하려는데 가능할련지...

2021-09-02: 남는 건 기록밖에 없다...
오늘은 DHT센서 라이브러리를 추가하고, 사용하는 핀을 상수로 지정하였으며, 
모든 기능에 대해 상당히 구체적으로 기술하였다.
위에 보는 바와 같이 말이다.
그리고, 변수명은 아직 못 바꿨다. 그리고 새로운 변수도 추가를 못했다.
내일 흐름을 짜면서 변수들도 필요한 것들 추가하고, 코드를 재정비 해야겠다.

2021-09-08: 간만에 리드미 적는다. 
이번주 토요일에 온라인 발표를 하기에 정신없이 발표 준비중이다.
함꼐 더불어, 코드와 모든 것을 열심히 하고 있다. 
어제 알리에서 SK6812LED를 잘못 보내준 것을 알고...ㅜ.ㅜ 
직구는 언제나 조심해야 하는 것 같다.
오늘은 정신없이 버튼 함수를 뜯어고치는 중이다.

2021-09-09: 이번주는 한글시계로 달리는 주간인가보다. 
오늘은 코드 수정을 정말 열심히 했다. 
v1의 유산을 모두 청산하고, 본격적인 v2 코드의 모습을 잡아가기 시작했다. 
기존에, 나의 기억력으로 숫자로만 지정한 것들도 모두 상수로 전환하고, 
변수 이름의 통일성 없는 것도 모두 새롭게 이름을 바꾸었으며, 
배열을 사용할 수 있는 것은 배열을 사용하였고, 
변수들을 카테고리 별로 정리, 각각 주석을 적었다. 
또한, h파일의 어수선함도 정리하였다.

2021-09-11: 오늘은 영메이커 놀이데이에서 한글시계 관련 발표를 하였고, 
한글시계 PCB의 문제점 중에서 스케치를 바꿔야 하는 작업 빛 푼프린트 수정 작업을 완료하였다..
내일은 PCB아트웤만 끝내면 된ㄷr...

2021-09-15: 그저께부터 3일간 PCB의 문제를 해결하고, 각 부품에 대한 호환성 및 기타 
모든 것을 비교한 결과, 현재로서 완벽하다고 판단되는 PCB를 주문하였다.
이제, 코드에 전력을 기한 다음, 다른 모양의 시계도... ㅎ.ㅎ

2021-10-18: 거의 한 달이 지났다... 
우선, LED는 불량이 와서 문제가 생겼고, 국내에서 구해야 할 것 같다. 
코드의 경우는 지난번의 기억을 더듬어서 마저 작업하던 cpp파일을 이어 수정하였다.
현재, updateHour까지 편집을 완료하였다. 다음부터는 updateMin 함수 편집을 시작하면 된다.

2021-11-15: 또 한 달이 지나서야 리드미를 쓴다. 
그제, 어제, 오늘동안 updateHour, updateMin 수정완료하였다(if->case). 
또한, 변수들을 전부 확인 완료하였다. (더 이상 수정할 변수 없음)
이젠 버튼 함수와 새로운 기능들을 추가해야지...

2021-11-22: 무슨 한글시계 개발을 일주일 간격으로 하는 것 같다...ㅋㅋ 
도대체 이게 무슨 일인지.... 

2022-01-06: 새로운 한 해가 밝았다. 
지난 6일동안 열심히 납땜하고 삽질한 결과, 하드웨어는 성공적으로 완성하였다. 
어제오늘로 나눠 테스트 한 각 부품들의 상태도 최고. 
이제 이 위에서 돌아갈 코드만 짜면 끝난다...

2022-01-09: 벌써 일지 작성한지 3일이 지났네... 
깃헙을 보면 알겠지만, 지난 3일동안 절대 논 것이 아니라 너무 빡세게 코딩 작업을 해서 일지를 안 쓴거다(까먹은 거 안 비밀). 
여튼간에, 오늘까지의 작업을 대략 정리하자면

1. 보기 힘들었던 변수 상태들 싹다 정리
2. 이해 안 갔던 변수 및 함수들 전부 주석 달음
3. 안 쓰는 과거의 코드 다 지움
4. if문들 대거 정리
5. 오늘 새벽 작업 포함 알람 기능까지 작성 완료.

오늘 저녁까지(작성당시 새벽) 프로그래밍 작업 완료하면, 코드 종이에 그려 보면서 오류 있는지 없는지 체크, 논리상 오류나는 부분 없으면 바로 한글시계에 업로딩 해서 실제 테스트를 진행해 본다.

2024-04-27: 이 프로젝트를 마무리한지도 어느덧 1년이 지났다. 대학교 오고 나서 시간이 나기도 하고, 다시 제대로 살려보고 싶어서 코드에 손 대보게 되었다. 진짜로 살려보자. 이제는 나도 능력이 성장했으니...

2024-05-03: 다른 프로젝트로 바쁜 와중에, 이것까지 하려니 정신이 없다. 그래도 내가 하고 싶어서 하는 거다 보니 열정이 난다. 오늘은 코드를 정리하고, 나도 모르게 꼬여 있었던 깃 브랜치를 정리하였다. 
