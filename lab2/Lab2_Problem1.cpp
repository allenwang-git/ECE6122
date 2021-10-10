/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 10/11/2021

Description:
This program use std::thread to calculate the average step ant needs
to take all seeds from bottom to top. The final result will be output
into file "ProblemOne.txt".
*/

#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <vector>
#include <atomic>
//#include "ctime"

using namespace std;

//struct Ant {
//    int grid_x = 2;
//    int grid_y = 2;
//    bool seedFlag = false;
//};

/*
 * This function check whether the ant finish one round task or not
 *
 * @return true if finished and false if not
 * */
bool antFinish(const bool high[]) {
    for (int i = 0; i < 5; i++) {
        if (!high[i]) return false;
    }
    return true;
}

/*
 * This function use random generator to decide which is the next grid.
 * @param a is an object of Ant
 * @param x is the grid's x coordinate
 * @param y is the grid's y coordinate
 * */
void generateNextGrid(int &x, int &y) {
//    srand((int)time(NULL));
//    int nextGrid = rand() % 4 + 1;
//    static default_random_engine e(time(0));
//  use Random Class in c++11 to generate random number
    random_device rd;
    static uniform_int_distribution<unsigned> u(1, 4);
//    int nextGrid =;
    x = y = 0;
    switch (u(rd)) {
        case 1:
            y = 1;
            break;
        case 2:
            x = 1;
            break;
        case 3:
            y = -1;
            break;
        case 4:
            x = -1;
            break;
        default:
            break;
    }

}


/*
 * This function realize the ant's one-time task, which is taking all seeds from bottom to the top.
 *
 * @return counter is the number of steps the ant taking.
 *
 * */
int antRunARound() {
    int ant_x = 2, ant_y = 2;
    bool seedFlag = false;
//    clock_t start,end,end2;
    bool lowSeedFlag[] = {true, true, true, true, true};
    bool highSeedFlag[] = {false, false, false, false, false};
    int counter = 0;
    do {
        counter++;
        int next_x, next_y;
//        start=clock();		//程序开始计时


        do {
            generateNextGrid(next_x, next_y);
            //    Regenerate a new grid if the grid is beyond the 5x5 range
        } while (ant_x + next_x > 4 || ant_x + next_x < 0 || ant_y + next_y > 4 || ant_y + next_y < 0);
        ant_x += next_x;
        ant_y += next_y;
//        end=clock();		//程序结束用时
//        cout<<"gbg:"<<(double)(end-start)<<endl;
//      Condition that ant will take a seed
        if (!seedFlag && ant_y == 0 && lowSeedFlag[ant_x]) {
            seedFlag = !seedFlag;
            lowSeedFlag[ant_x] = false;
        }
//      Condition that ant will drop a seed
        if (seedFlag && ant_y == 4 && !highSeedFlag[ant_x]) {
            seedFlag = !seedFlag;
            highSeedFlag[ant_x] = true;
        }
//        end2=clock();		//程序结束用时
//        cout<<(double)(end2-end)<<endl;
    } while (!antFinish(highSeedFlag));

    return counter;
}

std::atomic<long> steps(0);

/*
 * This function is the multi-threads function to run the task many times
 * @param n is the number of times each thread need to run
 * */
void antRunNRounds(int n) {
    int count(0);
    while (count < n) {
        ++count;
        steps += antRunARound();
    }
}

/*
 * This is the Main function of this program to create multitread task and
 * output final result to the ProblemOne.txt
 *
 * */
int main() {
    //   Create a file for output result
    fstream outputFile("ProblemOne.txt", ios::out | ios::trunc);
    const int runTimes = 10000;
    double average;

    vector<std::thread> threads;
    auto nthreads = thread::hardware_concurrency();
    for (int i = 1; i <= nthreads; ++i)
        threads.push_back(std::thread(antRunNRounds, runTimes / nthreads));
    for (auto &th: threads) th.join();
    cout << steps;
    //    compute the average steps ants need to move
    average = steps / (double) runTimes;

    //    Output prime factors to file
    if (outputFile.good()) {
        outputFile << "Number of threads created: " << nthreads << endl << endl;
        outputFile << "Expected number of steps: " << average << endl << endl;
        outputFile << "Total number of runs needed for solution convergence: " << endl << endl;
    } else {
        cout << "[ECE6122-Lab2-1] Can not open output file.\n";
    }
    return 0;
}