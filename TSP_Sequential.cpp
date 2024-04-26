#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int MAX_N = 15; // Maximum number of cities, adjust as needed

int n;                  // Number of cities
int dist[MAX_N][MAX_N]; // Distance matrix

vector<int> generateRandomSolution()
{
    vector<int> solution;
    for (int i = 0; i < n; ++i)
        solution.push_back(i);
    random_shuffle(solution.begin() + 1, solution.end()); // Shuffle cities, excluding the starting city
    return solution;
}

int calculateDistance(const vector<int> &solution)
{
    int totalDistance = 0;
    for (int i = 0; i < n - 1; ++i)
        totalDistance += dist[solution[i]][solution[i + 1]];
    totalDistance += dist[solution[n - 1]][solution[0]]; // Return to the starting city
    return totalDistance;
}

vector<int> levyFlight(const vector<int> &solution)
{
    // Perform a simple 2-opt local search for now
    // You can replace this with a more sophisticated search if needed
    vector<int> newSolution = solution;
    int i = rand() % (n - 1) + 1; // Randomly select a city to swap
    int j = rand() % (n - 1) + 1;
    if (i > j)
        swap(i, j);
    reverse(newSolution.begin() + i, newSolution.begin() + j);
    return newSolution;
}

int main()
{
    // Input
    srand(time(nullptr)); // Seed the random number generator
    cout << "Enter the number of cities: ";
    cin >> n;
    cout << "Enter the distance matrix: \n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> dist[i][j];

    // Parameters
    const int populationSize = 15;
    const int iterations = 1000;

    // Initialize
    vector<vector<int>> population(populationSize);
    for (int i = 0; i < populationSize; ++i)
        population[i] = generateRandomSolution();

    // Start the timer
    auto start = high_resolution_clock::now();

    // Main loop
    for (int iter = 0; iter < iterations; ++iter)
    {
        // Selection
        sort(population.begin(), population.end(), [](const vector<int> &a, const vector<int> &b)
             { return calculateDistance(a) < calculateDistance(b); });

        // Levy Flight
        for (int i = 0; i < populationSize / 2; ++i)
        {
            population[i + populationSize / 2] = levyFlight(population[i]);
        }

        // Replacement
        sort(population.begin(), population.end(), [](const vector<int> &a, const vector<int> &b)
             { return calculateDistance(a) < calculateDistance(b); });

        // Abandonment (optional)

        // Output best solution so far
        cout << "Iteration " << iter + 1 << ", Best distance: " << calculateDistance(population[0]) << endl;
    }

    // Stop the timer
    auto stop = high_resolution_clock::now();

    cout << "Best solution: ";
    for (int i = 0; i < n; ++i)
        cout << population[0][i] << " ";
    cout << endl;

    // Calculate and output the execution time
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << "Execution time: " << duration.count() << " nanoseconds" << endl;

    return 0;
}
