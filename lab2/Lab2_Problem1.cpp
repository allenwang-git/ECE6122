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
#include <sstream>

using namespace std;

std::atomic<long> steps(0);
int currentTimes = 0, equalTimes = 0, coverageTimes;
double currentAVG, prevAVG = 0, coverageSteps = 0;

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
 * This function is the multi-threads function to run the task many times
 * @param n is the number of times each thread need to run
 * */
void antRunNRounds(int n) {
//  use Random Class in c++11 to generate random number, using tid as unique seed
    unsigned seed = hash<thread::id>()(this_thread::get_id());
    default_random_engine e(seed);
    uniform_int_distribution<int> u(1, 4);
    int count(0);
    while (count < n) {
        ++count;
        ++currentTimes;
        int ant_x = 2, ant_y = 2, counter = 0;
        bool seedFlag = false;
        bool lowSeedFlag[] = {true, true, true, true, true};
        bool highSeedFlag[] = {false, false, false, false, false};
//        Do one round task
        do {
            counter++;
            int next_x, next_y;
            do {// generate next grid
                next_y = next_x = 0;
                switch (u(e)) {
                    case 1:
                        next_y++;
                        break;
                    case 2:
                        next_x++;
                        break;
                    case 3:
                        next_y--;
                        break;
                    case 4:
                        next_x--;
                        break;
                    default:
                        break;
                }
                //    Regenerate a new grid if the grid is beyond the 5x5 range
            } while (ant_x + next_x > 4 || ant_x + next_x < 0 || ant_y + next_y > 4 || ant_y + next_y < 0);
            ant_x += next_x;
            ant_y += next_y;

//      Condition that ant will take a seed
            if (ant_y == 0 && !seedFlag && lowSeedFlag[ant_x]) {
                seedFlag = true;
                lowSeedFlag[ant_x] = false;
            }
//      Condition that ant will drop a seed
            if (ant_y == 4 && seedFlag && !highSeedFlag[ant_x]) {
                seedFlag = false;
                highSeedFlag[ant_x] = true;
            }
        } while (!antFinish(highSeedFlag));
        steps += counter;
//        check if converge
        double currentAVG = steps / (double) currentTimes;
        currentAVG = round(currentAVG * 1000000) / 1000000;
//        cout<<currentAVG<<endl;
        if (currentAVG == prevAVG) equalTimes++;
        else equalTimes = 0;
        prevAVG = currentAVG;
        if (equalTimes == 10) {
            coverageTimes = equalTimes;
            coverageSteps = currentAVG;
        }
    }
}

/*
 * This is the Main function of this program to create multi-thread task and
 * output final result to the ProblemOne.txt
 *
 * */
int main(int argc, char *argv[]) {
    //   Create a file for output result
    fstream outputFile("ProblemOne.txt", ios::out | ios::trunc);
    outputFile.precision(6);
    fixed(outputFile);

    double average;
    int runTimes = 10000008;
    if (argv[1] > 0)
        stringstream(argv[1]) >> runTimes;

    //  create multi-threads
    vector<std::thread> threads;
    auto nthreads = thread::hardware_concurrency();
    for (int i = 1; i <= nthreads; ++i)
        threads.push_back(std::thread(antRunNRounds, runTimes / nthreads));
    for (auto &th: threads) th.join();

    //    compute the average steps ants need to move
    average = steps / (double) runTimes;
    cout << "Average Steps: " << average << endl;
    //    Output prime factors to file
    if (outputFile.good()) {
        outputFile << "Number of threads created: " << nthreads << endl << endl;
        outputFile << "Expected number of steps: " << average << endl << endl;
        if (coverageSteps != 0)
            outputFile << "Total number of runs needed for solution convergence: " << coverageTimes
                       << " " << coverageSteps << endl;
        else
            outputFile << "Total number of runs needed for solution convergence: " << runTimes << endl;
    } else {
        cout << "[ECE6122-Lab2-1] Can not open output file.\n";
    }
    return 0;
}