#ifndef SOLVERGA_H
#define SOLVERGA_H
#include <QVector>

class SolverGA
{
public:
    SolverGA(int n, int v, QVector<int> w, QVector<int> c);


protected:
    //problem data
    int n, limit;
    QVector<int> weights;
    QVector<int> costs;

    //cnt of individuals in population
    int numOfInd = 5;

};

#endif // SOLVERGA_H
