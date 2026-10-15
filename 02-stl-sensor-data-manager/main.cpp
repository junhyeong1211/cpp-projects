#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

int countWarnings(
    const string& name,
    const vector<double>& values
) {
    int warningCount = 0;

    if (name == "Temperature") {
        warningCount = count_if(
            values.begin(), values.end(),
            [](double value) {
                return value >= 70;
            }
        );
    }
    else if (name == "Distance") {
        warningCount = count_if(
            values.begin(), values.end(),
            [](double value) {
                return value <= 30;
            }
        );
    }
    else if (name == "Battery") {
        warningCount = count_if(
            values.begin(), values.end(),
            [](double value) {
                return value <= 20;
            }
        );
    }

    return warningCount;
}

void printSensorReport(
    const string& name,
    const vector<double>& values
) {
    cout << "[" << name << "]" << endl;

    if (values.empty()) {
        cout << "No Data\n\n";
        return;
    }

    cout << "Original: ";
    for (double value : values) {
        cout << value << " ";
    }
    cout << endl;

    vector<double> sortedValues = values;
    sort(sortedValues.begin(), sortedValues.end());

    double minValue = sortedValues.front();
    double maxValue = sortedValues.back();

    double sum = 0.0;

    for (double value : values) {
        sum += value;
    }

    double average = sum / values.size();

    int warningCount = countWarnings(name, values);

    cout << "Sorted: ";
    for (double value : sortedValues) {
        cout << value << " ";
    }

    cout << "\nCount: " << values.size();
    cout << "\nMinimum: " << minValue;
    cout << "\nMaximum: " << maxValue;
    cout << "\nAverage: " << average;
    cout << "\nWarning Count: " << warningCount;
    cout << "\n\n";
}

int main() {

    map<string, vector<double>> data;

    data["Temperature"] = {25.5, 27.1, 30.0};
    data["Distance"] = {10.0, 25.0, 15.5};
    data["Battery"] = {95.0, 90.0, 85.0};

    data["Temperature"].push_back(28.4);
    data["Temperature"].push_back(75.0);

    data["Distance"].push_back(30.0);
    data["Distance"].push_back(45.0);

    data["Battery"].push_back(15.0);

    cout << "===== SENSOR DATA MANAGER =====\n\n";

    for (const auto& entry : data) {
        printSensorReport(entry.first, entry.second);
    }

    return 0;
}

