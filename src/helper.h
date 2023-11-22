#include <vector>
#include <fstream>

using namespace std;

// Input a vector of any type, return the mean and standard deviation of the vector values
template <typename T>
inline array<double, 2> calculateMeanAndStdDev(const vector<T> &v)
{
    double mean = 0.0;
    double stdDev = 0.0;
    mean = accumulate(v.begin(), v.end(), 0.0) / v.size();
    for (auto &num : v)
    {
        stdDev += (num - mean) * (num - mean);
    }
    stdDev = sqrt(stdDev / v.size());
    return {mean, stdDev};
}

// Input a vector of any type, the filename to be saved, and the type of file.
template <typename T>
inline void writeToFile(const vector<T> &vec, const string &filename = "", const string &type = "txt")
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
