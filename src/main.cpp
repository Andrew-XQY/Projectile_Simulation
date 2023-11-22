#include <iostream>
#include "utils.h"
#include "helper.h"

using namespace std;

int main()
{
    array<int, 3> trials = {1e3, 1e4, 1e5};
    Target t(0.3, 2.3, 100); // lower bound, upper bound, distance

    // Task1: Numerically calculate the optimal angle to hit a bullseye
    Projectile p1(450, 0.7, 3, 3, "musket"); // velocity, shoot hight, accuracy, fire_rate, name
    cout << "Optimal angle for " << p1.getNames() << ": " << p1.optimalAngle(t) << " (degrees)" << endl;

    // Task2: Simulations. Each trial for five minutes
    for (int i = 0; i < trials.size(); i++)
    {
        vector<int> res = p1.fireSimulation(trials[i], t, true); 
        writeToFile(res, p1.getNames() + "_" + to_string(trials[i]));
    }

    // Task3: Repeat the previous steps for rifle
    Projectile p2(600, 0.3, 1, 2, "rifle");
    cout << "Optimal angle for " << p2.getNames() << ": " << p2.optimalAngle(t) << " (degrees)" << endl;
    for (int i = 0; i < trials.size(); i++)
    {
        vector<int> res = p2.fireSimulation(trials[i], t, true);
        writeToFile(res, p2.getNames() + "_" + to_string(trials[i]));
    }

    // Task4: Run trials to obtain relation between distance and accuracy
    array<Projectile, 2> p = {p1, p2};
    double min_distance = 1;
    double max_distance = 1000;
    for (size_t i = 0; i < p.size(); i++)
    {
        Projectile p_tmp = p[i];
        vector<string> relation;
        relation.push_back("distance,mean,std_dev"); // header of csv file
        for (int i = min_distance; i < max_distance; i++)
        {
            t.setDistance(i);
            vector<int> res = p_tmp.fireSimulation(1000, t, false);
            array<double, 2> statistics = calculateMeanAndStdDev(res);
            string temp = to_string(i) + "," + to_string(statistics[0]) + "," + to_string(statistics[1]);
            relation.push_back(temp);
        }
        writeToFile(relation, p_tmp.getNames() + "_relation", "csv");
    }

    cout << "All tasks completed!" << endl;
    return 0;
}
