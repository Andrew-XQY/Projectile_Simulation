#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <vector>

using namespace std;

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
    vector<int> fireSimulation(int trials, int time, Target target, bool save_angles);
    string getNames();
};

#endif // UTILS_H