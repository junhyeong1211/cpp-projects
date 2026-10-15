# Sensor Monitoring System

C++의 객체지향 프로그래밍 개념을 활용하여 온도, 거리, 배터리 센서를 하나의 공통 인터페이스로 관리하는 콘솔 프로그램이다.

『C++ 완전정복』 1~8장 학습 내용을 실제 코드에 적용하기 위해 제작했다.

추상 클래스와 상속을 통해 센서별 기능을 구현하고, 다형성과 어그리게이션을 활용해 서로 다른 센서를 동일한 방식으로 관리하도록 설계했다.

## 1. 프로젝트 목표

실제 로봇 시스템에는 온도 센서, 거리 센서, 배터리 센서처럼 서로 다른 종류의 장치가 존재한다.

각 센서는 측정값과 상태 판단 기준이 다르지만, 상위 시스템에서는 공통된 방식으로 접근할 필요가 있다.

이번 프로젝트에서는 이러한 구조를 객체지향 프로그래밍으로 구현하는 것을 목표로 했다.

주요 구현 목표는 다음과 같다.

- 추상 클래스를 이용한 센서 공통 인터페이스 정의
- 상속과 오버라이딩을 통한 센서별 기능 구현
- 부모 포인터와 동적 바인딩을 이용한 센서 통합 관리
- 어그리게이션을 이용한 센서와 모니터의 역할 분리
- 정적 멤버를 이용한 센서 객체 개수 관리
- 동적 할당 및 가상 소멸자를 이용한 객체 생명주기 관리

## 2. 개발 환경

| 항목 | 내용 |
|---|---|
| Language | C++17 |
| Library | C++ Standard Library |
| 주요 개념 | OOP, Inheritance, Polymorphism |
| 메모리 관리 | `new`, `delete`, Virtual Destructor |
| 프로그램 형태 | Console Application |

별도의 외부 라이브러리 없이 C++ 표준 라이브러리만 사용했다.

## 3. 클래스 구조

프로그램은 공통 센서 인터페이스인 `Sensor`, 이를 상속한 세 종류의 센서 클래스, 그리고 센서 정보를 출력하는 `SensorMonitor`로 구성했다.

```text
Sensor (Abstract Class)
├── TemperatureSensor
├── DistanceSensor
└── BatterySensor

SensorMonitor
└── Sensor& sensor
    └── 외부 Sensor 객체 참조
```

각 클래스의 역할은 다음과 같다.

| 클래스 | 역할 |
|---|---|
| `Sensor` | 모든 센서의 공통 인터페이스 정의 |
| `TemperatureSensor` | 온도 측정값 출력 및 과열 판정 |
| `DistanceSensor` | 거리 측정값 출력 및 장애물 접근 판정 |
| `BatterySensor` | 배터리 잔량 출력 및 충전 필요 판정 |
| `SensorMonitor` | 전달받은 센서의 측정값과 상태 출력 |

## 4. 핵심 구현

### 4-1. 추상 클래스와 순수 가상함수

모든 센서가 공통으로 제공해야 하는 기능을 `Sensor` 추상 클래스에 정의했다.

```cpp
class Sensor {
private:
    string name;
    static int sensorCount;

public:
    Sensor(string name);
    virtual ~Sensor();

    string getName();
    static int getSensorCount();

    virtual void read() = 0;
    virtual void checkStatus() = 0;
};
```

`read()`와 `checkStatus()`는 순수 가상함수다.

따라서 `Sensor` 객체는 직접 생성할 수 없으며, 이를 상속하는 구체적인 센서 클래스에서 해당 함수를 구현해야 한다.

이를 통해 서로 다른 센서가 동일한 인터페이스를 사용하도록 구성했다.

### 4-2. 상속과 오버라이딩

각 센서는 `Sensor`를 상속하고, 자신의 측정값과 경고 기준에 맞게 함수를 재정의했다.

예를 들어 온도 센서는 다음과 같이 구현했다.

```cpp
void TemperatureSensor::checkStatus() {
    if (temperature >= 70) {
        cout << "경고: 과열 상태" << endl;
    }
    else {
        cout << "상태: 정상" << endl;
    }
}
```

센서별 경고 조건은 다음과 같다.

| 센서 | 경고 조건 | 경고 메시지 |
|---|---|---|
| Temperature | 70°C 이상 | 과열 상태 |
| Distance | 30cm 이하 | 장애물 접근 |
| Battery | 20% 이하 | 충전 필요 |

모든 센서는 동일한 `checkStatus()` 함수를 제공하지만, 실제 판단 로직은 각 클래스에 따라 다르게 동작한다.

### 4-3. 부모 포인터와 다형성

서로 다른 센서 객체를 하나의 부모 포인터 배열로 관리했다.

```cpp
Sensor* sensors[3];

sensors[0] = new TemperatureSensor(25.5);
sensors[1] = new DistanceSensor(18.0);
sensors[2] = new BatterySensor(15);
```

배열에 저장된 포인터의 자료형은 모두 `Sensor*`이지만, 실제로 가리키는 객체의 종류는 서로 다르다.

```cpp
sensors[i]->read();
sensors[i]->checkStatus();
```

부모 포인터를 통해 가상함수를 호출하면, 실행 시점에 실제 객체의 자료형에 맞는 함수가 선택된다.

이 과정에서 업캐스팅, 오버라이딩, 가상함수, 동적 바인딩이 함께 사용된다.

### 4-4. SensorMonitor와 어그리게이션

`SensorMonitor`는 센서 객체를 직접 생성하거나 소유하지 않고, 외부에 존재하는 센서 객체를 참조하도록 구현했다.

```cpp
class SensorMonitor {
private:
    Sensor& sensor;

public:
    SensorMonitor(Sensor& sensor);
    void display();
};
```

생성자에서 외부 센서 객체를 참조 멤버에 연결한다.

```cpp
SensorMonitor::SensorMonitor(Sensor& sensor)
    : sensor(sensor)
{
}
```

`display()`에서는 해당 센서의 이름, 측정값, 상태를 출력한다.

```cpp
void SensorMonitor::display() {
    cout << "\n[" << sensor.getName() << "]" << endl;

    sensor.read();
    sensor.checkStatus();
}
```

메인 함수에서는 다음과 같이 사용했다.

```cpp
SensorMonitor monitor(*sensors[i]);
monitor.display();
```

`SensorMonitor`는 기존 센서 객체를 참조할 뿐이므로, 모니터 객체가 소멸하더라도 센서 객체는 삭제되지 않는다.

이러한 비소유 참조 관계를 통해 어그리게이션을 구현했다.

단, `SensorMonitor`가 참조하는 센서 객체는 모니터 객체가 사용하는 동안 유효하게 존재해야 한다.

### 4-5. 정적 멤버와 객체 개수 관리

`Sensor` 클래스의 정적 멤버 `sensorCount`를 이용해 현재 존재하는 센서 객체 수를 관리했다.

```cpp
int Sensor::sensorCount = 0;

Sensor::Sensor(string name)
    : name(name)
{
    sensorCount++;
}

Sensor::~Sensor() {
    sensorCount--;
}
```

정적 멤버는 개별 객체에 속하지 않고 클래스의 모든 인스턴스가 공유한다.

센서 객체가 생성되면 개수가 증가하고, 삭제되면 감소하도록 구현했다.

### 4-6. 동적 할당과 가상 소멸자

센서 객체는 `new`를 통해 동적으로 생성하고, 프로그램 종료 전에 `delete`로 해제했다.

```cpp
for (int i = 0; i < 3; ++i) {
    delete sensors[i];
}
```

부모 클래스의 소멸자를 `virtual`로 선언하여 부모 포인터를 통해 자식 객체를 삭제할 때 올바른 소멸 과정이 실행되도록 했다.

이번 프로젝트에서는 객체 생성과 삭제를 직접 관리했으며, 이후 Modern C++ 학습을 통해 스마트 포인터를 이용하는 방식으로 개선할 예정이다.

## 5. 실행 결과

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

세 종류의 센서에 대해 각각 다른 상태 판정이 수행되는 것을 확인했다.

또한 세 개의 센서 객체를 생성한 후 개수가 3으로 증가하고, 모든 객체를 삭제한 후 0으로 감소하는 것을 확인했다.

## 6. 실행 방법

C++17 이상을 지원하는 컴파일러에서 `main.cpp`를 컴파일한다.

```bash
g++ -std=c++17 main.cpp -o sensor_monitor
```

Linux/macOS에서는 다음 명령으로 실행한다.

```bash
./sensor_monitor
```

Windows에서는 생성된 `sensor_monitor.exe`를 실행하면 된다.

## 7. 배운 점

이번 프로젝트를 통해 개별적으로 학습했던 객체지향 개념을 하나의 프로그램에 적용했다.

- **추상 클래스:** 센서들이 따라야 할 공통 인터페이스 정의
- **상속과 오버라이딩:** 센서별 측정 및 상태 판정 기능 구현
- **다형성:** 부모 포인터로 서로 다른 센서 객체 관리
- **동적 바인딩:** 실제 객체에 맞는 가상함수 실행
- **가상 소멸자:** 부모 포인터를 통한 안전한 객체 삭제
- **정적 멤버:** 현재 생성된 센서 객체 개수 관리
- **어그리게이션:** 외부 센서 객체를 참조하는 모니터 구현

특히 다음 코드에서 포인터와 참조의 관계를 이해하는 데 시간이 필요했다.

```cpp
SensorMonitor monitor(*sensors[i]);
```

`sensors[i]`는 센서 객체의 주소를 저장한 포인터이고, `*sensors[i]`는 해당 포인터가 가리키는 객체를 나타낸다.

이 객체를 `SensorMonitor`의 참조 매개변수로 전달함으로써 새로운 객체를 생성하거나 복사하지 않고 기존 센서를 사용할 수 있었다.

또한 부모 포인터를 통해 동일한 함수를 호출해도 실제 센서의 종류에 따라 서로 다른 동작이 수행되는 것을 확인했다.

## 8. 한계 및 향후 개선

현재 프로그램은 실제 하드웨어 센서와 연결되지 않은 객체지향 구조 학습용 프로그램이다.

- 센서 측정값이 생성자에서 전달한 값으로 고정되어 있음
- 실제 센서 데이터 수집 및 갱신 기능이 없음
- 고정 크기 배열로 세 종류의 센서를 관리함
- `new`와 `delete`를 이용해 메모리를 직접 관리함
- 클래스 선언과 구현이 하나의 파일에 포함되어 있음

향후 STL과 Modern C++을 활용해 다음과 같이 개선할 수 있다.

```text
고정 크기 배열 → std::vector
Raw Pointer → std::unique_ptr
단일 파일 → Header / Source 파일 분리
고정 측정값 → 실제 센서 데이터 연동
```

이번 프로젝트는 복잡한 로봇 제어 프로그램은 아니지만, 서로 다른 센서를 공통 인터페이스로 관리하는 객체지향 구조를 직접 구현했다는 점에서 의미가 있었다.

---

**Source Code:** [main.cpp](./main.cpp)
