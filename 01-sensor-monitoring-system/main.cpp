#include <iostream>
#include <string>

using namespace std;

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

class TemperatureSensor : public Sensor {
private:
    double temperature;

public:
    TemperatureSensor(double temperature);

    void read() override;
    void checkStatus() override;
};

class DistanceSensor : public Sensor {
private:
    double distance;

public:
    DistanceSensor(double distance);

    void read() override;
    void checkStatus() override;
};

class BatterySensor : public Sensor {
private:
    int battery;

public:
    BatterySensor(int battery);

    void read() override;
    void checkStatus() override;
};

class SensorMonitor {
private:
    Sensor& sensor;

public:
    SensorMonitor(Sensor& sensor);

    void display();
};

int Sensor::sensorCount = 0;

Sensor::Sensor(string name)
    : name(name)
{
    sensorCount++;
}

Sensor::~Sensor() {
    sensorCount--;
}

string Sensor::getName() {
    return name;
}

int Sensor::getSensorCount() {
    return sensorCount;
}

TemperatureSensor::TemperatureSensor(double temperature)
    : Sensor("Temperature Sensor"),
      temperature(temperature)
{
}

void TemperatureSensor::read() {
    cout << "측정 온도: "
         << temperature << " C" << endl;
}

void TemperatureSensor::checkStatus() {
    if (temperature >= 70) {
        cout << "경고: 과열 상태" << endl;
    }
    else {
        cout << "상태: 정상" << endl;
    }
}

DistanceSensor::DistanceSensor(double distance)
    : Sensor("Distance Sensor"),
      distance(distance)
{
}

void DistanceSensor::read() {
    cout << "측정 거리: "
         << distance << " cm" << endl;
}

void DistanceSensor::checkStatus() {
    if (distance <= 30) {
        cout << "경고: 장애물 접근" << endl;
    }
    else {
        cout << "상태: 정상" << endl;
    }
}

BatterySensor::BatterySensor(int battery)
    : Sensor("Battery Sensor"),
      battery(battery)
{
}

void BatterySensor::read() {
    cout << "배터리 잔량: "
         << battery << " %" << endl;
}

void BatterySensor::checkStatus() {
    if (battery <= 20) {
        cout << "경고: 충전 필요" << endl;
    }
    else {
        cout << "상태: 정상" << endl;
    }
}

SensorMonitor::SensorMonitor(Sensor& sensor)
    : sensor(sensor)
{
}

void SensorMonitor::display() {
    cout << "\n[" << sensor.getName() << "]" << endl;

    sensor.read();
    sensor.checkStatus();
}

int main() {
    Sensor* sensors[3];

    sensors[0] = new TemperatureSensor(25.5);
    sensors[1] = new DistanceSensor(18.0);
    sensors[2] = new BatterySensor(15);

    cout << "현재 센서 개수: "
         << Sensor::getSensorCount() << endl;

    for (int i = 0; i < 3; ++i) {
        SensorMonitor monitor(*sensors[i]);
        monitor.display();
    }

    for (int i = 0; i < 3; ++i) {
        delete sensors[i];
    }

    cout << "\n삭제 후 센서 개수: "
         << Sensor::getSensorCount() << endl;

    return 0;
}