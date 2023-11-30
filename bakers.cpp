#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int numProcesses = 5;
const int numResources = 3;

vector<int> available = {10, 5, 7};

vector<vector<int>> maximum = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};

vector<vector<int>> allocation = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
};

vector<vector<int>> need(numProcesses, vector<int>(numResources, 0));
vector<int> safeSequence;

void calculateNeed() {
    for (int i = 0; i < numProcesses; ++i) {
        for (int j = 0; j < numResources; ++j) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

bool isSafe(int process, vector<int>& work, vector<bool>& finish) {
    for (int i = 0; i < numResources; ++i) {
        if (need[process][i] > work[i])
            return false;
    }

    for (int i = 0; i < numResources; ++i) {
        work[i] += allocation[process][i];
    }

    finish[process] = true;
    return true;
}

bool bankerAlgorithm() {
    calculateNeed();

    vector<bool> finish(numProcesses, false);
    vector<int> work = available;

    for (int i = 0; i < numProcesses; ++i) {
        if (!finish[i] && isSafe(i, work, finish)) {
            safeSequence.push_back(i);
        }
    }

    // Check if all processes are in safe state
    for (bool processFinish : finish) {
        if (!processFinish) {
            return false;
        }
    }

    return true;
}

int main() {
    if (bankerAlgorithm()) {
        cout << "System is in a safe state.\n";
        cout << "Safe sequence: ";
        for (int process : safeSequence) {
            cout << "P" << process << " ";
        }
        cout << endl;
    } else {
        cout << "System is not in a safe state.\n";
    }

    return 0;
}
