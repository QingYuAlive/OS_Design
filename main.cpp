#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <queue>

using namespace std;

// 用于显示菜单的函数
void menu();

// 进程控制块PCB类
class PCB {
private:
    string processName; // 进程名（标识符）
    int arriveTime; // 到达时间
    int totalTime; // 需要运行时间
    int usedTime; // 已用CPU时间
    char status; // 进程状态
public:
    PCB() {
        processName = "";
        arriveTime = 0;
        totalTime = 0;
        usedTime = 0;
        status = 'W';
    }

    PCB(string processName, int arriveTime, int totalTime) {
        this->processName = processName; // 进程名
        this->arriveTime = arriveTime; // 到达时间
        this->totalTime = totalTime; // 需要运行时间
        this->usedTime = 0; // 已用CPU时间初始化为0
        this->status = 'W'; // 进程状态初始化为就绪态
    }

    const string &getProcessName() const {
        return processName;
    }

    void setProcessName(const string &processName) {
        this->processName = processName;
    }

    int getArriveTime() const {
        return arriveTime;
    }

    void setArriveTime(int arriveTime) {
        this->arriveTime = arriveTime;
    }

    int getTotalTime() const {
        return totalTime;
    }

    void setTotalTime(int totalTime) {
        this->totalTime = totalTime;
    }

    int getUsedTime() const {
        return usedTime;
    }

    void setUsedTime(int usedTime) {
        this->usedTime = usedTime;
    }

    char getStatus() const {
        return status;
    }

    void setStatus(char status) {
        this->status = status;
    }

    // 重载运算符==
    bool operator==(const PCB &pcb) const {
        return processName == pcb.processName;
    }

    // 重载运算符!=
    bool operator!=(const PCB &pcb) const {
        return !(pcb == *this);
    }
};

// 进程类
class Process {
private:
    PCB *pcb; // 进程对应的PCB
    int continuation; // 进程连续占用CPU的时间
public:
    Process(PCB *pcb) {
        this->pcb = pcb;
        continuation = 0;
    }

    void setContinuation(int continuation) {
        this->continuation = continuation;
    }

    int getContinuation() {
        return continuation;
    }

    PCB *getPCB() {
        return pcb;
    }

    void setPCB(PCB *pcb) {
        this->pcb = pcb;
    }

    // 重载运算符==
    bool operator==(const Process &process) const {
        return pcb == process.pcb;
    }

    // 重载运算符!=
    bool operator!=(const Process &process) const {
        return !(process == *this);
    }
};

// 系统进程表类
class ProcessTable {
private:
    vector<Process> processes; // 用于存储当前未完成进程的容器
public:
    ProcessTable() {
        processes.clear(); // 初始化清空系统进程表
    }

    // 获取进程容器的引用（关键），方便从外部获取容器用于直接操作而不是生成副本
    vector<Process> &getProcesses() {
        return processes;
    }

    // 添加进程
    void insertProcess(Process process);

    // 移除进程
    void removeProcess(Process process);

    // 显示当前系统进程表
    void display();

};

// 添加进程
void ProcessTable::insertProcess(Process process) {
    processes.push_back(process);
}

// 移除进程
void ProcessTable::removeProcess(Process process) {
    for (vector<Process>::iterator i = processes.begin(); i != processes.end(); i++) {
        // 这也是为什么要重载process和PCB的==运算符的原因
        if (*i == process) {
            processes.erase(i);
            break;
        }
    }
}

int nowTime = 0; // 当前时间
ProcessTable processTable; // 创建一个全局的系统进程表对象
queue<Process> firstQueue, secondQueue, thirdQueue;

// 显示当前系统进程表和各级队列
void ProcessTable::display() {
    cout << endl;
    // 空表
    if (processes.empty()) {
        cout << "all processes have finished running" << endl;
    } else {
        // 非空表
        vector<string> completedProcess; // 已完成的进程的名单
        cout << left << setw(20) << "processName" << setw(20) << "arriveTime" << setw(20) << "totalTime" << setw(20)
             << "usedTime" << setw(20) << "status" << endl;
        for (vector<Process>::iterator i = processes.begin(); i != processes.end(); i++) {
            PCB *pcb = (*i).getPCB(); // 获取PCB
            // 完成态
            if (pcb->getStatus() == 'F') {
                completedProcess.push_back(pcb->getProcessName()); // 放入名单
            }
            // 显示进程信息
            cout << left << setw(20) << pcb->getProcessName() << setw(20) << pcb->getArriveTime() << setw(20)
                 << pcb->getTotalTime() << setw(20) << pcb->getUsedTime() << setw(20) << pcb->getStatus() << endl;
        }
        // 提示当前已完成的进程
        for (vector<string>::iterator i = completedProcess.begin(); i != completedProcess.end(); i++) {
            cout << *i << " have finished" << endl;
            for (vector<Process>::iterator j = processes.begin(); j != processes.end(); j++) {
                PCB *pcb = (*j).getPCB();
                if ((*pcb).getProcessName() == *i) {
                    processes.erase(j); // 从系统进程表中移除已完成的进程
                    j--;
                }
            }
        }
        completedProcess.clear(); // 清空名单
    }
    cout  << endl << "Queue1 | ";
    if (!firstQueue.empty()) {
        for (int i = 0; i < firstQueue.size(); i++) {
            cout << firstQueue.front().getPCB()->getProcessName();
            Process *process = &(firstQueue.front());
            firstQueue.pop();
            firstQueue.push(*process);
            if (i != firstQueue.size() - 1) {
                cout << "<---";
            }
        }
    }
    cout << " |" << endl << "Queue2 | ";
    if (!secondQueue.empty()) {
        for (int i = 0; i < secondQueue.size(); i++) {
            cout << secondQueue.front().getPCB()->getProcessName();
            Process *process = &(secondQueue.front());
            secondQueue.pop();
            secondQueue.push(*process);
            if (i != secondQueue.size() - 1) {
                cout << "<---";
            }
        }
    }
    cout << " |" << endl << "Queue3 | ";
    if (!thirdQueue.empty()) {
        for (int i = 0; i < thirdQueue.size(); i++) {
            cout << thirdQueue.front().getPCB()->getProcessName();
            Process *process = &(thirdQueue.front());
            thirdQueue.pop();
            thirdQueue.push(*process);
            if (i != thirdQueue.size() - 1) {
                cout << "<---";
            }
        }
    }
    cout << " |" << endl;
    cout << endl;
}



// 采用多级反馈队列调度算法运行若干时间片
void Run(int time) {
    for (int i = 0; i < time;) {
//        vector<Process> processes = processTable.getProcesses(); // 获取当前系统进程表
        // 循环系统进程表
//        for (int j = 0; j < processTable.getProcesses().size(); j++) {
//            PCB *pcb = processTable.getProcesses()[j].getPCB(); // 获取PCB
//            // 如果到达时间等于当前时间，说明该进程刚刚到达，将其放入第1级队列的队尾
//            if (pcb->getArriveTime() == nowTime) {
//                firstQueue.push(processTable.getProcesses()[j]);
//                processTable.getProcesses()[j].getPCB()->setStatus('R'); // 设置状态为运行态
//            }
//        }
        // 只要第1队队列不空，就持续对第1队队列中的进程调度并分配时间片
        if (!firstQueue.empty()) {
            // 此时已经进入第1队队列中，如果第2、3队队列不空，说明上次时间片分配给的是第2、3队队列的进程，此时需要将处于第2、3队队列中的进程的持续运行记录清0
            if (!secondQueue.empty()) {
//                for (int j = 0; j < secondQueue.size(); j++) {
//                    Process *process = &(secondQueue.front());
//                    process->setContinuation(0);
//                    secondQueue.pop();
//                    secondQueue.push(*process); // 这一行和上一行的顺序不能调换，他们操作的是同一个process对象
//                }
                if (secondQueue.front().getContinuation() != 0) {
                    Process *process = &(secondQueue.front());
                    process->setContinuation(0);
                    secondQueue.pop();
                    secondQueue.push(*process); // 这一行和上一行的顺序不能调换，他们操作的是同一个process对象
                }
            }
            if (!thirdQueue.empty()) {
//                for (int j = 0; j < thirdQueue.size(); j++) {
//                    Process *process = &(thirdQueue.front());
//                    process->setContinuation(0);
//                    thirdQueue.pop();
//                    thirdQueue.push(*process); // 这一行和上一行的顺序不能调换，他们操作的是同一个process对象
//                }
                if (thirdQueue.front().getContinuation() != 0) {
                    Process *process = &(thirdQueue.front());
                    process->setContinuation(0);
                    thirdQueue.pop();
                    thirdQueue.push(*process); // 这一行和上一行的顺序不能调换，他们操作的是同一个process对象
                }
            }
            nowTime++; // 当前时间增加
            i++; // 模拟分配了时间片=1的CPU时间
            Process &process = firstQueue.front(); // 获取第1队队列首进程
            PCB *pcb = process.getPCB(); // 获取该进程的PCB
            pcb->setUsedTime(pcb->getUsedTime() + 1); // 已用CPU时间+1
            process.setContinuation(process.getContinuation() + 1);
            // 如果运行完成
            if (pcb->getUsedTime() == pcb->getTotalTime()) {
                firstQueue.pop(); // 弹出第1队队列
                pcb->setStatus('F');
            } else {
                // 由于第一队队列的时间片为1，因此不用考虑已占用CPU时间大于需要运行时间的情况，此时考虑该进程未完成的情况
                process.setContinuation(0); // 发生调度，需要将进程的持续运行记录清0
                secondQueue.push(process); // 放入第2队队列的队尾
//                cout << process.getContinuation() << endl;
                firstQueue.pop(); // 弹出第1队队列
            }
        } else if (!secondQueue.empty()) {
            // 同上，此时已经进入第2队队列中，如果第3队队列不空，此时需要将处于第3队队列中的进程的持续运行记录清0
            if (!thirdQueue.empty()) {
//                for (int j = 0; j < thirdQueue.size(); j++) {
//                    Process *process = &(thirdQueue.front());
//                    process->setContinuation(0);
//                    thirdQueue.pop();
//                    thirdQueue.push(*process);
//                }
                if (thirdQueue.front().getContinuation() != 0) {
                    Process *process = &(thirdQueue.front());
                    process->setContinuation(0);
                    thirdQueue.pop();
                    thirdQueue.push(*process); // 这一行和上一行的顺序不能调换，他们操作的是同一个process对象
                }
            }
//            cout << "firstQueueEmpty!" << endl;
            Process *process = &(secondQueue.front()); // 获取第2队队列的队首进程
//            secondQueue.front().getPCB()->setUsedTime(2);
            PCB *pcb = (*process).getPCB(); // 该进程的PCB
//            pcb->setUsedTime(2);
            // 只要第2队队列不为空且本次分配的CPU时间片未使用完，就不断对处于第2队队列中的进程进行调度和分配时间片
            while (!secondQueue.empty() && i != time) {
                i++; // 模拟分配了时间片=1的CPU时间
                nowTime++; // 当前时间增加
                pcb->setUsedTime(pcb->getUsedTime() + 1); // 当前进程已占用CPU时间增加
                process->setContinuation(process->getContinuation() + 1); // 当前进程连续运行时间的记录增加
                // 若进程完成
                if (pcb->getUsedTime() == pcb->getTotalTime()) {
//                    cout << pcb->getProcessName() << " pop from secondQueue" << endl;
                    pcb->setStatus('F');
                    secondQueue.pop(); // 从第2队队列中弹出该进程
                    process = &(secondQueue.front()); // 切换下一个进程
                    pcb = process->getPCB(); // 获取该进程PCB
                    continue;
                }
                // 若到达第2队队列所能分配的最大时间片
                if (process->getContinuation() == 2) {
//                    cout << pcb->getProcessName() << " remove to thirdQueue from secondQueue" << endl;
                    process->setContinuation(0); // 发生进程调度，进程连续运行时间记录清0
                    thirdQueue.push(*process); // 将该进程放入第3队队列
                    secondQueue.pop(); // 将该进程从第2队队列中弹出
                    process = &(secondQueue.front()); // 切换下一个进程
                    pcb = process->getPCB(); // 获取该进程的PCB
                }
            }

        } else {
//            cout << "secondQueueEmpty" << endl;
            // 当其他队列为空时便对第3队队列中的进程进行调度并分配时间片
            Process *process = &(thirdQueue.front()); // 获取第3队队列队首的进程
            PCB *pcb = (*process).getPCB(); // 获得该进程的PCB
            // 当程序进入到处理第三队队列的进程调度时，这意味着前两个队列都是空的，由于第3队队列是最高级的队列，进程在这将采用时间片轮转的调度算法
            while (!thirdQueue.empty() && i != time) {
                i++;
                nowTime++;
                pcb->setUsedTime(pcb->getUsedTime() + 1);
                process->setContinuation(process->getContinuation() + 1);
                if (pcb->getUsedTime() == pcb->getTotalTime()) {
                    cout << pcb->getProcessName() << " pop from thirdQueue" << endl;
                    pcb->setStatus('F');
                    thirdQueue.pop();
                    process = &(thirdQueue.front());
                    pcb = process->getPCB();
                    continue;
                }
                if (process->getContinuation() == 4) {
                    cout << pcb->getProcessName() << " remove to the end" << endl;
                    process->setContinuation(0);
                    thirdQueue.pop();
                    thirdQueue.push(*process); // 这一行和上一行顺序不能调换，他们操作的是同一个process对象
                    process = &(thirdQueue.front());
                    pcb = process->getPCB();
                }
            }
            if (thirdQueue.empty()) {
                nowTime += time - i;
                i = time;
            }
        }
    }
    processTable.display();
    menu();
}

//void input() {
//    cout << "please input number of processes:";
//    int n;
//    cin >> n;
//    cout << "please input the properties of the process" << endl;
//    for (int i = 1; i <= n; i++) {
//        string processName;
//        int arriveTime, totalTime;
//        cout << "process" << i << "Name:";
//        cin >> processName;
//        cout << "arriveTime:";
//        cin >> arriveTime;
//        cout << "totalTime:";
//        cin >> totalTime;
//        PCB temp(processName, arriveTime, totalTime);
//        pcbList.push_back(temp);
//    }
//}

// 添加进程菜单
void addProcess() {
    string processName; // 进程名
    int arriveTime = nowTime, totalTime; // 到达时间、所需要总时间
    cout << "processName:";
    cin >> processName;
    cout << "totalTime:";
    cin >> totalTime;
    PCB pcb(processName, arriveTime, totalTime); // 创建PCB对象
    Process process(&pcb); // 创建进程对象
    processTable.insertProcess(process); // 将进程放入系统进程表中
    process.getPCB()->setStatus('R');
    firstQueue.push(process);
    menu();
}

// 系统运行菜单
void systemRun() {
    cout << "run time:";
    int runTime;
    cin >> runTime; // 单次运行时间
    Run(runTime);
}

// 菜单
void menu() {
    cout << "Add a process          (press 1)" << endl;
    cout << "System run             (press 2)" << endl;
    cout << "Show current processes (press 3)" << endl;
    cout << "Exit                   (press 4)" << endl;
    int choice;
    cin >> choice;
    switch (choice) {
        case 1:
            addProcess();
            break;
        case 2:
            systemRun();
            break;
        case 3:
            processTable.display();
            menu();
            break;
        case 4:
            exit(1);
    }
}

int main() {
    menu();
//    PCB pcb1 = PCB("process1", 0, 3);
//    PCB pcb2 = PCB("process2", 0, 3);
//    PCB pcb3 = PCB("process3", 0, 3);
//    Process process1(&pcb1);
//    Process process2(&pcb2);
//    Process process3(&pcb3);
//    ProcessTable processTable;
//    process2.getPCB()->setStatus('F');
//    process2.getPCB()->setArriveTime(3);
//    process3.getPCB()->setStatus('F');
//    processTable.insertProcess(process1);
//    processTable.insertProcess(process2);
//    processTable.insertProcess(process3);
//    processTable.display();
//    processTable.display();
//    cout << process1.getPCB().getPID() << endl;
    return 0;
}
