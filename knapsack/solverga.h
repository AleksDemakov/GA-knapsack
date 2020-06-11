#ifndef SOLVERGA_H
#define SOLVERGA_H
#include <QVector>

class SolverGA
{
public:
    SolverGA(int n, int v, QVector<int> w, QVector<int> c, int generations, double mutationRate,int numOfInd);
    void sortPopulation();
    QVector<int> getAns();
    int getAnsGeneration();
    QVector< QVector<int> > getFitnessScoreHistory();
    void solve();


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

    Individual * res;


    void crossover();
    void mutation();
    void makePopulation();


private:
    int res_fitness;
    int res_generation;



};

#endif // SOLVERGA_H
