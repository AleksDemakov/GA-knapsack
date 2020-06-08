#ifndef SOLVERGA_H
#define SOLVERGA_H
#include <QVector>

class SolverGA
{
public:
    SolverGA(int n, int v, QVector<int> w, QVector<int> c, int generations, double mutationRate);
    void sortPopulation();
    QVector<int> getAns();
    QVector< QVector<int> > getFitnessScoreHistory();

protected:
    class Individual;
    //problem data
    int n, limit, numOfGenerations;
    double mutationRate;
    QVector<int> weights;
    QVector<int> costs;

    QVector<Individual*> population;

    //cnt of individuals in population
    int numOfInd;

    QVector< QVector<int> > fitnessScoreHistory;

    void solve();
    void crossover();
    void mutation();



};

#endif // SOLVERGA_H
