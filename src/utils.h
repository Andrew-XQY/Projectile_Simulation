#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <tuple>
#include <cmath>
#include <numeric>
using namespace std;

array<double, 2> calculateMeanAndStdDev(const vector<int> &v);

template <typename T>
void writeToFile(const vector<T> &vec, const string &filename = "", const string &type = "txt")
{
    ofstream myfile("./output/" + filename + "." + type);
    if (!myfile.is_open())
    {
        cerr << "Error opening file for writing." << endl;
        return;
    }
    for (const auto &element : vec)
    {
        myfile << element << endl;
    }
}

class AngleGenerator
{
private:
    mt19937 generator;
    normal_distribution<double> distribution;

public:
    AngleGenerator(double mean, double stddev);
    double getRandAngle();
};

class Target
{
private:
    double y0;       // lower bound
    double y1;       // upper bound
    double distance; // distance from the shooter
public:
    Target(double y0, double y1, double distance);
    int targetHit(double y);
    double getDistance();
    double getLowerBound();
    double getUpperBound();
    void setDistance(double distance);
};

class Projectile
{
private:
    string name;               // name of the projectile
    double v;                  // initial velocity (m/s)
    double h1;                 // height of shooting (m)
    int fire_rate;             // number of shots per minute
    AngleGenerator rand_angle; // random angle generator

public:
    Projectile(double v, double h1, double accuracy, int fire_rate, string name);
    double getRandAngle();
    long double optimalAngle(Target target);
    double angleToHit(double d, double h2);
    vector<int> fireSimulation(int trials, Target target);
    string getNames();
};

#endif // UTILS_H