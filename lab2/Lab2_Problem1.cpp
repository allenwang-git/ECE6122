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
#include "ctime"
#include "mutex"
#include <sys/time.h>

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
void generateNextGrid(int &x, int &y, int r) {
//    srand((int)time(NULL));
//    int nextGrid = rand() % 4 + 1;

////    struct timeval time;
//    time_t seed = time(nullptr);
//    static default_random_engine rd( seed);
////  use Random Class in c++11 to generate random number
////    random_device rd;
//    static uniform_int_distribution<short> u(1, 4);
//    int nextGrid =;u(rd)
    x = y = 0;
    switch (r) {
        case 1:
            y++;
            break;
        case 2:
            x++;
            break;
        case 3:
            y--;
            break;
        case 4:
            x--;
            break;
        default:
//            cout<<"wrong";
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
//    int ant_x = 2, ant_y = 2, counter = 0, antFinish=0;
//    bool seedFlag = false;
//    bool lowSeedFlag[] = {true, true, true, true, true},
//        highSeedFlag[] = {false, false, false, false, false};
//
//    do {
//        counter++;
//        int next_x, next_y;
////        clock_t s,e;
////        s=clock();
//        do {
//            generateNextGrid(next_x, next_y);
//            //    Regenerate a new grid if the grid is beyond the 5x5 range
//        } while (ant_x + next_x > 4 || ant_x + next_x < 0 || ant_y + next_y > 4 || ant_y + next_y < 0);
//        ant_x += next_x;
//        ant_y += next_y;
////        e=clock();
////        cout<<this_thread::get_id()<<" "<<(double)(e-s)/ CLOCKS_PER_SEC<<endl;
////      Condition that ant will take a seed
//        if (ant_y == 0 && !seedFlag &&  lowSeedFlag[ant_x]) {
//            seedFlag = true;
//            lowSeedFlag[ant_x] = false;
//        }
////      Condition that ant will drop a seed
//        if (ant_y == 4 && seedFlag && !highSeedFlag[ant_x]) {
//            seedFlag = false;
//            highSeedFlag[ant_x] = true;
//            ++antFinish;
//        }
//    } while (antFinish<=4);

//    return counter;
}

std::atomic<long> steps(0);

/*
 * This function is the multi-threads function to run the task many times
 * @param n is the number of times each thread need to run
 * */
void antRunNRounds(int n) {
    int count(0);
    std::mutex mtx;

    struct timeval time;
//    time_t seed = time(nullptr);
    static default_random_engine rd;//( time.tv_sec+time.tv_usec);
//  use Random Class in c++11 to generate random number
//    random_device rd;
    static uniform_int_distribution<int> u(1, 4);
    while (count < n) {
        ++count;
        int ant_x = 2, ant_y = 2, counter = 0;
//        generateNextGrid(a,b,u(rd));
        bool seedFlag = false;
        bool lowSeedFlag[] = {true, true, true, true, true},
                highSeedFlag[] = {false, false, false, false, false};

        do {
            counter++;
            int next_x, next_y;
//        clock_t s,e;
//        s=clock();
            do {
                mtx.lock();
                generateNextGrid(next_x, next_y, rd() % 4 + 1);
                mtx.unlock();
                //    Regenerate a new grid if the grid is beyond the 5x5 range
            } while (ant_x + next_x > 4 || ant_x + next_x < 0 || ant_y + next_y > 4 || ant_y + next_y < 0);
            ant_x += next_x;
            ant_y += next_y;
//        e=clock();
//        cout<<this_thread::get_id()<<" "<<(double)(e-s)/ CLOCKS_PER_SEC<<endl;
//      Condition that ant will take a seed
            if (ant_y == 0 && !seedFlag && lowSeedFlag[ant_x]) {
                seedFlag = true;
                lowSeedFlag[ant_x] = false;
            }
//      Condition that ant will drop a seed
            if (ant_y == 4 && seedFlag && !highSeedFlag[ant_x]) {
                seedFlag = false;
                highSeedFlag[ant_x] = true;
//                ++antFinish;
            }
        } while (!antFinish(highSeedFlag));
        steps += counter;
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
    const int runTimes = 1000000;
    double average;

    vector<std::thread> threads;
    auto nthreads = thread::hardware_concurrency();
    for (int i = 1; i <= nthreads; ++i)
        threads.push_back(std::thread(antRunNRounds, runTimes / nthreads));
    for (auto &th: threads) th.join();
    cout << steps << endl;
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