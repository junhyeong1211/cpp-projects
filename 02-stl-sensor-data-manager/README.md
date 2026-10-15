# STL Sensor Data Manager

C++ STL의 컨테이너와 알고리즘을 활용하여 여러 센서의 측정값을 저장하고, 통계 분석 및 이상 상태 판정을 수행하는 콘솔 프로그램이다.

『C++ 완전정복』 12장 STL 학습 내용을 실제 코드에 적용하기 위해 제작했다.

## 1. 프로젝트 목표

기존 Sensor Monitoring System에서는 각 센서가 하나의 측정값을 관리했다.

이번 프로젝트에서는 센서별로 여러 측정 기록을 저장하고, STL을 이용해 데이터를 분석하는 기능을 구현했다.

주요 목표는 다음과 같다.

- `map`과 `vector`를 이용한 센서별 데이터 관리
- 측정값 추가 및 순회
- 원본 데이터를 보존하면서 정렬
- 최솟값, 최댓값, 평균 계산
- 센서별 이상 상태 감지
- 함수 분리 및 빈 데이터 처리

## 2. 개발 환경

| 항목 | 내용 |
|---|---|
| Language | C++17 |
| Library | STL |
| Container | `map`, `vector` |
| Algorithm | `sort`, `count_if` |
| 주요 문법 | Iterator, Lambda, Template, const Reference |

별도의 외부 라이브러리 없이 C++ 표준 라이브러리만 사용했다.

## 3. 프로그램 구조

프로그램은 세 개의 함수로 구성했다.

**main()**

센서별 측정값을 저장하고, 각 센서에 대한 분석 함수를 호출한다.

**printSensorReport()**

센서 측정값 출력, 정렬, 통계 계산 및 결과 출력을 담당한다.

**countWarnings()**

센서별 경고 조건을 검사하고, 이상 상태에 해당하는 측정값의 개수를 반환한다.

```text
main()
 └── printSensorReport()
       ├── Original Data
       ├── Sorting
       ├── Minimum / Maximum
       ├── Average
       └── countWarnings()
             └── Warning Count
```

## 4. 핵심 구현

### 4-1. STL 컨테이너를 이용한 데이터 관리

```cpp
map<string, vector<double>> data;

data["Temperature"] = {25.5, 27.1, 30.0};
data["Distance"] = {10.0, 25.0, 15.5};
data["Battery"] = {95.0, 90.0, 85.0};
```

`map`은 센서 이름을 키로 사용하고, `vector`는 각 센서의 여러 측정값을 저장한다.

새로운 데이터는 `push_back()`으로 추가할 수 있다.

### 4-2. 원본 데이터를 유지하면서 정렬

```cpp
vector<double> sortedValues = values;
sort(sortedValues.begin(), sortedValues.end());
```

원본 측정 순서가 변경되지 않도록 복사본을 생성한 뒤 정렬한다.

### 4-3. 통계 분석

정렬된 데이터의 첫 번째와 마지막 원소를 이용해 최솟값과 최댓값을 구한다.

평균은 모든 원소의 합계를 측정값 개수로 나누어 계산한다.

### 4-4. STL 알고리즘을 활용한 이상 상태 감지

`count_if()`와 람다식을 이용해 경고 조건을 만족하는 측정값의 개수를 계산한다.

| 센서 | 경고 조건 |
|---|---|
| Temperature | 70°C 이상 |
| Distance | 30cm 이하 |
| Battery | 20% 이하 |

```cpp
warningCount = count_if(
    values.begin(),
    values.end(),
    [](double value) {
        return value >= 70;
    }
);
```

위 코드는 온도 측정값 중 70°C 이상인 원소의 개수를 반환한다.

### 4-5. 빈 데이터 처리

```cpp
if (values.empty()) {
    cout << "No Data\n\n";
    return;
}
```

측정값이 없는 경우 잘못된 원소 접근이나 평균 계산을 수행하지 않도록 처리했다.

## 5. 실행 결과

```text
===== SENSOR DATA MANAGER =====

[Battery]
Original: 95 90 85 15
Sorted: 15 85 90 95
Count: 4
Minimum: 15
Maximum: 95
Average: 71.25
Warning Count: 1

[Distance]
Original: 10 25 15.5 30 45
Sorted: 10 15.5 25 30 45
Count: 5
Minimum: 10
Maximum: 45
Average: 25.1
Warning Count: 4

[Temperature]
Original: 25.5 27.1 30 28.4 75
Sorted: 25.5 27.1 28.4 30 75
Count: 5
Minimum: 25.5
Maximum: 75
Average: 37.2
Warning Count: 1
```

## 6. 실행 방법

C++17 이상을 지원하는 컴파일러에서 `main.cpp`를 컴파일해 실행한다.

```bash
g++ -std=c++17 main.cpp -o sensor_manager
```

Linux/macOS에서는 다음 명령으로 실행한다.

```bash
./sensor_manager
```

Windows에서는 생성된 실행 파일 `sensor_manager.exe`를 실행하면 된다.

## 7. 배운 점

이번 프로젝트를 통해 STL에서 배운 개념을 하나의 데이터 처리 프로그램으로 연결했다.

- **map / vector:** 센서별 측정 데이터 저장
- **Iterator:** 컨테이너의 원소 접근 및 알고리즘 범위 지정
- **sort:** 측정값 정렬
- **count_if / Lambda:** 조건을 만족하는 측정값 개수 계산
- **const Reference:** 불필요한 복사 없이 데이터 전달
- **함수 분리:** 데이터 준비, 분석, 경고 판정의 역할 구분

특히 `map`과 `vector`를 함께 사용하면서 중첩 컨테이너의 구조를 이해할 수 있었다.

또한 `main()`에 모여 있던 코드를 기능별 함수로 분리하면서 함수의 매개변수, 반환값, 변수의 유효 범위를 복습했다.

## 8. 한계 및 향후 개선

현재 프로그램은 실제 하드웨어와 연결되지 않은 학습용 데이터 분석 프로그램이다.

- 측정값이 코드 내부에 고정되어 있음
- 센서별 경고 임계값이 함수 내부에 정의되어 있음
- 측정 시간이나 실시간 데이터 수집 기능은 없음

향후 Modern C++과 ROS 2를 학습하면서 실제 센서 데이터를 처리하는 구조로 확장할 수 있다.

---

**Source Code:** [main.cpp](./main.cpp)
