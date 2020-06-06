#include "solverga.h"
#include <QVector>
#include <QDebug>
#include <QDateTime>
class Individual
{
    QVector<int> pocket;
    int n;
    public:
        Individual(int n)
        {
            qsrand(QDateTime::currentMSecsSinceEpoch());
            this->n = n;
            pocket.resize(n);
            for(int i=0;i<n;i++){
                pocket[i] = qrand() % 2;
            }
            qDebug()<<pocket;
        }
        int fitnessScore(QVector<int> &c)
        {
            int sum_c = 0;
            for(int i=0;i<this->n;i++) {
                if(pocket[i] == 1)
                    sum_c += c[i];
            }
            return sum_c;
        }
        int weight(QVector<int> &w){
            int sum_w=0;
            for(int i=0;i<this->n;i++) {
                if(pocket[i] == 1)
                    sum_w += w[i];
            }
            return sum_w;
        }

};
SolverGA::SolverGA(int n, int v, QVector<int> w, QVector<int> c)
{
    this->n = n;
    this->limit = v;
    this->weights = w;
    this->costs = c;
    Individual *test = new Individual(n);

    QVector<Individual> population;
    for(int i=0;i<this->numOfInd;i++)
        population.push_back(*test);
}

Individual findFittest(){

}
