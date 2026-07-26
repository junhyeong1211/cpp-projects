# Sensor Monitoring System

C++ 1~8장 학습 내용을 활용한 객체지향 미니 프로젝트입니다.

온도, 거리, 배터리 센서를 하나의 `Sensor` 인터페이스로 관리하며 추상 클래스, 다형성, 정적 멤버, 어그리게이션을 구현했습니다.

## 주요 기능

- 온도 측정 및 과열 경고
- 거리 측정 및 장애물 접근 경고
- 배터리 잔량 측정 및 충전 경고
- 부모 포인터 배열로 여러 센서 관리
- 생성된 센서 객체 개수 확인

## 클래스 구조

```text
Sensor
├── TemperatureSensor
├── DistanceSensor
└── BatterySensor

SensorMonitor → Sensor 참조
```

## 적용 개념

- 추상 클래스와 순수 가상함수
- 상속과 오버라이딩
- 업캐스팅과 동적 바인딩
- 가상 소멸자
- 정적 멤버
- 어그리게이션
- `new`와 `delete`

## 실행 방법

```bash
g++ -std=c++17 main.cpp -o sensor_monitor
./sensor_monitor
```

## 실행 결과

```text
현재 센서 개수: 3

[Temperature Sensor]
측정 온도: 25.5 C
상태: 정상

[Distance Sensor]
측정 거리: 18 cm
경고: 장애물 접근

[Battery Sensor]
배터리 잔량: 15 %
경고: 충전 필요

삭제 후 센서 개수: 0
```

## 배운 점

`Sensor*` 부모 포인터로 서로 다른 센서를 관리하고, 같은 함수 호출이 실제 객체에 따라 다르게 실행되는 과정을 확인했습니다.

또한 `SensorMonitor`가 외부 센서를 참조하도록 만들어 어그리게이션 관계를 구현했습니다.