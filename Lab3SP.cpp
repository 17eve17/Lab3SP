#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int priority;
    int startTime = -1;
    int finishTime = 0;
    int waitingTime = 0;
    int turnaroundTime = 0;
};

void printProcesses(const std::vector<Process>& processes) {
    std::cout << "PID\tArrival\tBurst\tPriority\tStart\tFinish\tWaiting\tTurnaround\n";
    for (const auto& p : processes) {
        std::cout << p.pid << "\t"
            << p.arrivalTime << "\t"
            << p.burstTime << "\t"
            << p.priority << "\t\t"
            << p.startTime << "\t"
            << p.finishTime << "\t"
            << p.waitingTime << "\t"
            << p.turnaroundTime << "\n";
    }
}

void printAverages(const std::vector<Process>& processes) {
    double totalWaiting = 0, totalTurnaround = 0;
    for (const auto& p : processes) {
        totalWaiting += p.waitingTime;
        totalTurnaround += p.turnaroundTime;
    }
    int n = processes.size();
    std::cout << "Average Waiting Time: " << totalWaiting / n << "\n";
    std::cout << "Average Turnaround Time: " << totalTurnaround / n << "\n";
}

void fcfs(std::vector<Process> processes) {
    int time = 0;
    for (auto& p : processes) {
        if (time < p.arrivalTime) time = p.arrivalTime;
        p.startTime = time;
        p.finishTime = p.startTime + p.burstTime;
        p.turnaroundTime = p.finishTime - p.arrivalTime;
        p.waitingTime = p.startTime - p.arrivalTime;
        time = p.finishTime;
    }
    std::cout << "\nFCFS Scheduling:\n";
    printProcesses(processes);
    printAverages(processes);
}

void sjf(std::vector<Process> processes) {
    int time = 0;
    std::vector<Process> readyQueue;
    std::vector<Process> finished;
    std::sort(processes.begin(), processes.end(), [](Process a, Process b) { return a.arrivalTime < b.arrivalTime; });

    while (!processes.empty() || !readyQueue.empty()) {
        for (auto it = processes.begin(); it != processes.end();) {
            if (it->arrivalTime <= time) {
                readyQueue.push_back(*it);
                it = processes.erase(it);
            }
            else ++it;
        }

        if (!readyQueue.empty()) {
            auto shortest = std::min_element(readyQueue.begin(), readyQueue.end(),
                [](Process a, Process b) { return a.burstTime < b.burstTime; });
            Process p = *shortest;
            p.startTime = time;
            p.finishTime = p.startTime + p.burstTime;
            p.turnaroundTime = p.finishTime - p.arrivalTime;
            p.waitingTime = p.startTime - p.arrivalTime;
            time = p.finishTime;
            finished.push_back(p);
            readyQueue.erase(shortest);
        }
        else time++;
    }

    std::cout << "\nSJF Scheduling:\n";
    printProcesses(finished);
    printAverages(finished);
}

void priorityAging(std::vector<Process> processes, int agingInterval = 2) {
    int time = 0;
    std::vector<Process> readyQueue;
    std::vector<Process> finished;
    std::sort(processes.begin(), processes.end(), [](Process a, Process b) { return a.arrivalTime < b.arrivalTime; });

    while (!processes.empty() || !readyQueue.empty()) {
        for (auto it = processes.begin(); it != processes.end();) {
            if (it->arrivalTime <= time) {
                readyQueue.push_back(*it);
                it = processes.erase(it);
            }
            else ++it;
        }

        if (!readyQueue.empty()) {
            for (auto& p : readyQueue) {
                if ((time - p.arrivalTime) % agingInterval == 0 && (time - p.arrivalTime) > 0) p.priority--;
            }
            auto highest = std::min_element(readyQueue.begin(), readyQueue.end(),
                [](Process a, Process b) { return a.priority < b.priority; });
            Process p = *highest;
            p.startTime = time;
            p.finishTime = p.startTime + p.burstTime;
            p.turnaroundTime = p.finishTime - p.arrivalTime;
            p.waitingTime = p.startTime - p.arrivalTime;
            time = p.finishTime;
            finished.push_back(p);
            readyQueue.erase(highest);
        }
        else time++;
    }

    std::cout << "\nPriority Scheduling with Aging:\n";
    printProcesses(finished);
    printAverages(finished);
}

int main() {
    std::vector<Process> processes = {
        {1, 0, 8, 3},
        {2, 1, 4, 1},
        {3, 2, 9, 4},
        {4, 3, 5, 2}
    };

    fcfs(processes);
    sjf(processes);
    priorityAging(processes);

    return 0;
}
