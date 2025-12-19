# History List #

* Customer : Samsung
* Machine  : Tray Multi Packing
* Model    : 
* PGM Main : 88-24-0042-SEC-2-MULTIPACKING-SEQ
* LINK : https://bitbucket.org/koses_sw/88-24-0042-sec-2-multipacking-seq/src/main/

## 중요!
* Lader Sensor Password : Koses001!
* CCTV Password : Koses001!

---
## [공통]
* 프로그램 전체적으로 추가 디버깅 예정
* Normal Foam Pad (Bottom) 7장이상 (7~11)
* Partial Foma Pad (Bottom) 6장 이하 (3~6)

---
## [2025/11/06]
* FDC 항목 중 Vacuum 자동 전송 부분 수정. 
** 구조체 use추가 및 Init시 설정 필요. ex) _01_Init.cpp -> InitVACUUM -> VC[vcMainAir1].m_fdcData.use = false; [사용 안할 경우]
* Secs/Gem Event 검증. (AMR 요청, LOT IN, OUT 제외)
 
---
## [2025/10/31]
* MBB Sealing Part Cylinder ADD. (Modify)
* MBB QC Part Motor ADD. (Modify)
* Folding Picker Modify
* Vacuum FDC 전송 추가.

 ---
## [2025/10/22]
* Autorun 조건 변경 (In Picker 기구물 변경)
* Left Cobot Vision -> InPicker Vision으로 옮김.

 ---
## [2025/10/10]
* Dry Running
* 화재감지기 Alarm 추가
* Fan Alarm 추가
* Cobot동작 관련 프로그램 변경.

---
## [2025/08/08]
* I/O 디버깅 및 All Home이후 업데이트

---
## [2025/07/23]
* Log 표준화 Material ID 추가
* Simulation Autorun 디버깅

---
## [2025/07/23]
* Cobot 로그표준화 추가
* AMR활용하는 시뮬레이션 검증 완료.

---
## [2025/07/21]
* Torque Move 포지션 인덱스 추가 (Log 표준화)
* Conveyor 로그표준화 추가 수정 (Log 안남는 현상)
* 빌드 시간 최적화 (ini Read & Wirte 위치 이동)

---
## [2025/07/10]
* Cobot 프로그램 적용
* Cobot Motion 동작 Cycle에 추가
* Log 표준화 정리 중.

---
## [2025/07/07]
* Autorun Simulation 1차 디버깅 완료 (cobot 제외)

---
## [2025/07/04]
* Autorun Simulation으로 디버깅 중.
* Cobot 프로그램 추가. (Cycle내 Motion Move 함수 미적용)

---
## [2025/06/24]
* ~ 6/24 Test 설비 디버깅.
* _15_MBBStack Cycle 구현.
* _14_LabelShuttle Cycle 구현.

* ---
## [2025/06/13]
* Test 설비 프로그램 시작.

* ---
## [2025/06/12]
* Sealing EtherCAT통신 추가 중.

---
## [2025/06/10]
* MMI 구조체 개별 Read-> 일괄 Read로 변경.
* _14_LabelShuttle Cycle 구현중. (추가 보완 필요)
* _13_LabelLaser Cycle 구현. (추가 보완 필요)
* _12_LabelFeeder Cycle 구현.

---
## [2025/06/09]
* _12_LabelFeeder Cycle 구현중.

---
## [2025/06/04]
* _11_FoldPk Cycle 구현.
* _10_BtmFoldConv Cycle 구현.
* _09_MBBQC Cycle 구현.

---
## [2025/06/02]
* _08_MBBShuttle Cycle 구현 중.
* _07_TrayShuttle Cycle 구현.

---
## [2025/05/30]
* _06_FoamBox Cycle 구현.

---
## [2025/05/29]
* _05_FoamPk Cycle 구현.
* PkgData.h 정리 (스네이크 표기법 -> 소문자 카멜표기법)

---
## [2025/05/28]
* _05_FoamPk Cycle 구현중.
* _04_FoamConv Cycle 구현.
* _03_FoamElev Cycle 구현.

---
## [2025/05/27]
* _03_FoamElev Cycle 구현 중.
* _02_InRail Cycle 구현.

---
## [2025/05/26]
* _02_InRail Cycle 구현중.
* _01_InPk Cycle 구현

---
## [2025/05/21]
* 구조체 작성

---
## [2025/05/20]
* I/O Define
* ErrnameDef 재정리.
 ** Define만 변경하여 Paser 빌드 후 카멜표기법으로 재변경.

---
## [2025/05/19]
* 시작

---
## [2025/05/12]
* 설계 I/O List 전달 받음 (1차)