#ifndef SOLVERGA_H
#define SOLVERGA_H
#include <QVector>

class SolverGA
{
public:
    SolverGA(int n, int v, QVector<int> w, QVector<int> c, int generations, double mutationRate,int numOfInd);
    ~SolverGA();
    void sortPopulation();
    QVector<int> getAns();
    int getAnsGeneration();
    QVector< QVector<int> > const & getFitnessScoreHistory();
    void solve();
    void setMutationRate(double rate);
    void setNumOfInd(int numOfInd);


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
    int res_generation_ans;

    QVector<int> fitnessScore;

    bool check();



};

#endif // SOLVERGA_H
