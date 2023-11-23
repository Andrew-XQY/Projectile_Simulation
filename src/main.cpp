#include <iostream>
#include "utils.h"
#include "helper.h"

using namespace std;

int main()
{
    array<int, 3> trials = {1e3, 1e4, 1e5};
    Target t(0.3, 2.3, 100); // lower bound, upper bound, distance
    Projectile p1(450, 0.7, 3, 3, "musket"); // velocity, shoot hight, accuracy, fire_rate, name
    Projectile p2(600, 0.3, 1, 2, "rifle"); 
    array<Projectile, 2> p = {p1, p2};
    string save_to = "./output/";
    int duration = 5; // simulation time for single trial
    double min_distance = 1, max_distance = 1000;

    for (Projectile p_tmp : p)
    {
        cout << "Optimal angle for " << p_tmp.getNames() << ": " << p_tmp.optimalAngle(t) << " (degrees)" << endl;
        for (int i = 0; i < trials.size(); i++)  // Task2: Simulations. Each trial for five minutes
        {
            vector<int> res = p_tmp.fireSimulation(trials[i], duration, t, true); 
            writeToFile(res, p_tmp.getNames() + "_" + to_string(trials[i]));
        }
        vector<string> relation;
        relation.push_back("distance,mean,std_dev"); // header of csv file
        for (int i = min_distance; i < max_distance; i++)
        {
            t.setDistance(i);
            vector<int> res = p_tmp.fireSimulation(1000, duration, t, false);
            array<double, 2> stats = calculateMeanAndStdDev(res);
            string temp = to_string(i) + "," + to_string(stats[0]) + "," + to_string(stats[1]);
            relation.push_back(temp);
        }
        writeToFile(relation, p_tmp.getNames() + "_relation", "csv");
        t.setDistance(100); // reset distance
    }

    cout << "All tasks completed!" << endl;
    return 0;
}
