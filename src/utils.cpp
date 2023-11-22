#include <iostream>
#include <random>
#include <cmath>
#include "helper.h"

using namespace std;

// Constants
const double g = 9.81;            // Acceleration due to gravity (m/s^2)
const double air_density = 1.225; // Air density (kg/m^3), might be used for more realistic simulation

/*
Generate a random angle based on normal distribution.
*/
class AngleGenerator
{
private:
    mt19937 generator;
    normal_distribution<double> distribution;

public:
    AngleGenerator(double mean, double stddev) : distribution(mean, stddev), generator(random_device()())
    {
    }

    double getRandAngle()
    {
        return distribution(generator);
    }
};

/*
class for target
*/
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

Target::Target(double y0, double y1, double distance)
{
    this->y0 = y0;
    this->y1 = y1;
    this->distance = distance;
}

double Target::getDistance()
{
    return distance;
}

double Target::getLowerBound()
{
    return y0;
}

double Target::getUpperBound()
{
    return y1;
}

void Target::setDistance(double distance)
{
    this->distance = distance;
}

int Target::targetHit(double y) // y: hit position
{
    if (y == (y0 + y1) / 2)
        return 2; // hit the center
    else if (y >= y0 && y <= y1)
        return 1; // hit the target
    else
        return 0; // miss the target
}

/*
class for projectile simulation
*/
class Projectile
{
private:
    string name;                                      // name of the projectile
    double v;                                         // initial velocity (m/s)
    double h1;                                        // height of shooting (m)
    int fire_rate;                                    // number of shots per minute
    AngleGenerator rand_angle = AngleGenerator(0, 0); // random angle generator

public:
    Projectile(double v, double h1, double accuracy, int fire_rate, string name);
    double getRandAngle();
    double angleToHit(double d, double h2);
    long double optimalAngle(Target target);
    // vector<int> fireSimulation(int trials, Target target);
    vector<int> fireSimulation(int trials, Target target, bool save_angles);
    string getNames();
};

Projectile::Projectile(double v, double h1, double accuracy, int fire_rate, string name = "")
{
    this->name = name;
    this->v = v;
    this->h1 = h1;
    this->fire_rate = fire_rate;
    this->rand_angle = AngleGenerator(0, accuracy);
}

long double Projectile::optimalAngle(Target target)
{
    double h2 = (target.getUpperBound() + target.getLowerBound()) / 2;
    double d = target.getDistance();
    return angleToHit(d, h2);
}

double Projectile::getRandAngle()
{
    return rand_angle.getRandAngle();
}

double Projectile::angleToHit(double d, double h2) // give the hight and distance, calculate the angle of projectile to hit that hight
{
    double v_2 = v * v;
    long double common = (v_2 / (g * d));
    long double under = (2 * g / (v_2)) * (h1 - h2 - (g * d * d) / (2 * (v_2)));
    long double plus = atan(common * (1 + sqrt(1 + under)));
    long double minus = atan(common * (1 - sqrt(1 + under)));
    long double tmp1 = plus * (180 / M_PI);
    long double tmp2 = minus * (180 / M_PI);
    return tmp1 < tmp2 ? tmp1 : tmp2; // always returns the smaller angle, how people normally shoot
}

vector<int> Projectile::fireSimulation(int trials, Target target, bool save_angles = false)
{
    vector<double> angles;
    vector<int> res;
    double minAngle = angleToHit(target.getDistance(), target.getLowerBound());
    double maxAngle = angleToHit(target.getDistance(), target.getUpperBound());
    double angle = optimalAngle(target);
    for (int i = 0; i < trials; i++)
    {
        int hit = 0;
        for (int j = 0; j < fire_rate * 5; j++)
        {
            double actual_angle = angle + getRandAngle();
            if (minAngle <= actual_angle && actual_angle <= maxAngle)
            {
                hit++;
                angles.push_back(actual_angle);  // store the angle for later use (only if its on target)
            }
        }
        res.push_back(hit);
    }
    if (save_angles)
    {
        writeToFile(angles, "/angle/" + name + "_angles_" + to_string(trials));
    }
    return res;
}

string Projectile::getNames()
{
    return name;
}