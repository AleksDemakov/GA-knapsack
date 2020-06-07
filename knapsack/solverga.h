#ifndef SOLVERGA_H
#define SOLVERGA_H
#include <QVector>

class SolverGA
{
public:
    SolverGA(int n, int v, QVector<int> w, QVector<int> c);
    void sortPopulation();

protected:
    class Individual;
    //problem data
    int n, limit;
    QVector<int> weights;
    QVector<int> costs;

    QVector<Individual*> population;

    //cnt of individuals in population
    int numOfInd;

};

#endif // SOLVERGA_H
