#include "solverga.h"
#include <QVector>
#include <QDebug>
#include <QDateTime>
class SolverGA::Individual{

    public:
    QVector<int> pocket;
    int n;
    SolverGA *solver;
        Individual(SolverGA *solver)
        {
            this->solver = solver;
            this->n = solver->numOfInd;
            pocket.resize(n);
            for(int i=0;i<n;i++){
                pocket[i] = qrand() % 2;
            }
        }
        int fitnessScore()
        {
            int sum_c = 0;
            for(int i=0;i<this->n;i++) {
                if(pocket[i] == 1)
                    sum_c += solver->costs[i];
            }
            return sum_c;
        }
        int weight(){
            int sum_w=0;
            for(int i=0;i<this->n;i++) {
                if(pocket[i] == 1)
                    sum_w += solver->weights[i];
            }
            return sum_w;
        }
        Individual *crossover(Individual *other){
            Individual *son = new Individual(solver);
            for(int i=0;i<this->n;i++){
                if(this->pocket[i] == other->pocket[i])
                    son->pocket[i] = this->pocket[i];
                else
                    son->pocket[i] = qrand()%2;
            }
            return son;
        }
        bool operator < (Individual *other){
            return this->fitnessScore() < other->fitnessScore();
        }
};
SolverGA::SolverGA(int n, int v, QVector<int> w, QVector<int> c)
{
    //
    this->numOfInd = 5;
    this->n = n;
    this->limit = v;
    this->weights = w;
    this->costs = c;
    population.resize(numOfInd);
    //initiate population

    //*todo: better random
    qsrand(QDateTime::currentMSecsSinceEpoch());
    for(int i=0;i<this->numOfInd;i++)
        population[i] = new Individual(this);

}
void SolverGA::sortPopulation(){
    std::sort(population.begin(), population.end(),
              [](Individual *a, Individual *b){return a->fitnessScore() < b->fitnessScore();});
}
