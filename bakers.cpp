#include <iostream>
#include <vector>

using namespace std;

const int NUM_PROCESSES = 5;
const int NUM_RESOURCES = 3;

bool isSafeState(const vector<vector<int>>& max, const vector<vector<int>>& alloc, const vector<int>& avail, vector<int>& safeSeq) {
    vector<int> work = avail;
    vector<bool> finish(NUM_PROCESSES, false);

    for (int k = 0; k < NUM_PROCESSES; ++k) {
        bool found = false;
        for (int i = 0; i < NUM_PROCESSES; ++i) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < NUM_RESOURCES; ++j) {
                    if (max[i][j] - alloc[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate) {
                    for (int j = 0; j < NUM_RESOURCES; ++j) {
                        work[j] += alloc[i][j];
                    }
                    safeSeq.push_back(i);
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            // If no process can be allocated, the system is not in a safe state
            return false;
        }
    }

    // If all processes are finished, the system is in a safe state
    return true;
}

int main() {
    // Given snapshot at time t0
    vector<vector<int>> max = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    vector<vector<int>> alloc = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    vector<int> avail = {3, 3, 2};
    vector<int> safeSeq;

    if (isSafeState(max, alloc, avail, safeSeq)) {
        cout << "The system is in a safe state.\nSafe sequence: ";
        for (int i = 0; i < safeSeq.size(); ++i) {
            cout << "P" << safeSeq[i];
            if (i != safeSeq.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    } else {
        cout << "The system is not in a safe state.\n";
    }

    return 0;
}
