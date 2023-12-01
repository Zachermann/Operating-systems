#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3

int available[MAX_RESOURCES];
int maximum[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];

void initializeSystem() {
    // Initialize available resources
    printf("Enter the number of instances for each resource:\n");
    for (int j = 0; j < MAX_RESOURCES; ++j) {
        printf("Resource %d: ", j);
        scanf("%d", &available[j]);
    }

    // Initialize maximum claims
    printf("\nEnter the maximum claims for each process and resource:\n");
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        for (int j = 0; j < MAX_RESOURCES; ++j) {
            printf("Process %d, Resource %d: ", i, j);
            scanf("%d", &maximum[i][j]);
            need[i][j] = maximum[i][j];
        }
    }

    // Initialize allocation matrix
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        for (int j = 0; j < MAX_RESOURCES; ++j) {
            allocation[i][j] = 0;
        }
    }
}

void displaySystemState() {
    printf("\nSystem State:\n");
    printf("Available: ");
    for (int j = 0; j < MAX_RESOURCES; ++j) {
        printf("%d ", available[j]);
    }
    printf("\n");

    printf("Maximum:\n");
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        for (int j = 0; j < MAX_RESOURCES; ++j) {
            printf("%d ", maximum[i][j]);
        }
        printf("\n");
    }

    printf("Allocation:\n");
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        for (int j = 0; j < MAX_RESOURCES; ++j) {
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }

    printf("Need:\n");
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        for (int j = 0; j < MAX_RESOURCES; ++j) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
}

bool isSafeState(int requestProcess, int request[]) {
    // Check if request is within need and available
    for (int j = 0; j < MAX_RESOURCES; ++j) {
        if (request[j] > need[requestProcess][j] || request[j] > available[j]) {
            return false;
        }
    }

    // Simulate resource allocation and check for safety
    for (int j = 0; j < MAX_RESOURCES; ++j) {
        available[j] -= request[j];
        allocation[requestProcess][j] += request[j];
        need[requestProcess][j] -= request[j];
    }

    // Check if the system is still in a safe state
    bool finish[MAX_PROCESSES] = {false};
    int work[MAX_RESOURCES];
    for (int j = 0; j < MAX_RESOURCES; ++j) {
        work[j] = available[j];
    }

    bool changed;
    do {
        changed = false;
        for (int i = 0; i < MAX_PROCESSES; ++i) {
            if (!finish[i]) {
                bool canFinish = true;
                for (int j = 0; j < MAX_RESOURCES; ++j) {
                    if (need[i][j] > work[j]) {
                        canFinish = false;
                        break;
                    }
                }

                if (canFinish) {
                    finish[i] = true;
                    for (int j = 0; j < MAX_RESOURCES; ++j) {
                        work[j] += allocation[i][j];
                    }
                    changed = true;
                }
            }
        }
    } while (changed);

    // Restore the state for future checks
    for (int j = 0; j < MAX_RESOURCES; ++j) {
        available[j] += request[j];
        allocation[requestProcess][j] -= request[j];
        need[requestProcess][j] += request[j];
    }

    // Check if all processes finish
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        if (!finish[i]) {
            return false;
        }
    }

    return true;
}

void requestResources(int requestProcess, int request[]) {
    if (isSafeState(requestProcess, request)) {
        for (int j = 0; j < MAX_RESOURCES; ++j) {
            available[j] -= request[j];
            allocation[requestProcess][j] += request[j];
            need[requestProcess][j] -= request[j];
        }
        printf("Request granted.\n");
    } else {
        printf("Request denied. System will be in an unsafe state.\n");
    }
}

void releaseResources(int releaseProcess, int release[]) {
    for (int j = 0; j < MAX_RESOURCES; ++j) {
        available[j] += release[j];
        allocation[releaseProcess][j] -= release[j];
        need[releaseProcess][j] += release[j];
    }
}

int main() {
    initializeSystem();
    displaySystemState();

    int choice, process, request[MAX_RESOURCES], release[MAX_RESOURCES];

    do {
        printf("\nEnter command (1 = Request, 2 = Release, 0 = Exit): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter process number for request: ");
                scanf("%d", &process);

                printf("Enter resource request: ");
                for (int j = 0; j < MAX_RESOURCES; ++j) {
                    scanf("%d", &request[j]);
                }

                requestResources(process, request);
                break;

            case 2:
                printf("Enter process number for release: ");
                scanf("%d", &process);

                printf("Enter resource release: ");
                for (int j = 0; j < MAX_RESOURCES; ++j) {
                    scanf("%d", &release[j]);
                }

                releaseResources(process, release);
                break;

            case 0:
