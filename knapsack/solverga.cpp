#include "solverga.h"
#include <QVector>
#include <QDebug>
#include <QDateTime>
#include <QRandomGenerator>

class SolverGA::Individual{

    public:
    QVector<int> pocket;
    int n;
    SolverGA *solver;
        Individual(SolverGA *solver)
        {
            this->solver = solver;
            this->n = solver->n;
            pocket.resize(n);
            for(int i = 0; i < n; i++){
                pocket[i] = qrand() % 2;
            }
        }
        Individual(Individual * ind)
        {
            this->solver = ind->solver;
            this->n = solver->n;
            this->pocket = ind->pocket;
        }
        int weight()
        {
            int sum_w = 0;

            for(int i = 0; i < this->n; i++) {
                if(pocket[i] == 1)
                    sum_w += solver->weights[i];
            }

            return sum_w;
        }
        int fitnessScore()
        {
            int sum_c = 0;

            for(int i = 0; i < this->n; i++) {
                if(pocket[i] == 1)
                    sum_c += solver->costs[i];
            }

            if (this->weight() > solver->limit) return solver->limit - this->weight();
            return sum_c;
        }
        Individual *crossover(Individual *other)
        {
            Individual *son = new Individual(solver);
            for(int i = 0; i < this->n; i++){
                if(this->pocket[i] == other->pocket[i])
                    son->pocket[i] = this->pocket[i];
                else
                    son->pocket[i] = qrand()%2;
            }
            return son;
        }
        void crossover(Individual * p2, QVector<Individual *> & sons)
        {

            Individual * son1 = new Individual( solver );
            Individual * son2 = new Individual( solver );

            for (int i = 0; i < (n + 1) / 2; i++) {
                son1->pocket[i] = pocket[i];
                son2->pocket[i] = p2->pocket[i];
            }
            for (int i = (n + 1) / 2; i < n; i++) {
                son2->pocket[i] = pocket[i];
                son1->pocket[i] = p2->pocket[i];
            }

            sons.push_back(son1);
            sons.push_back(son2);

        }
        void mutate()
        {
            int pos = QRandomGenerator::global()->bounded( this->n );
            pocket[ pos ] ^= 1;
        }
        bool operator < (Individual *other)
        {
            return this->fitnessScore() < other->fitnessScore();
        }

};
SolverGA::SolverGA(int n, int v, QVector<int> w, QVector<int> c, int generations, double mutationRate, int numOfInd)
{
    //
    this->numOfInd = numOfInd;
    this->n = n;
    this->limit = v;
    this->weights = w;
    this->costs = c;
    this->numOfGenerations = generations;
    this->mutationRate = mutationRate;

}
SolverGA::~SolverGA() {

}
void SolverGA::sortPopulation()
{
    std::sort(population.begin(), population.end(),
              [](Individual *a, Individual *b){return a->fitnessScore() > b->fitnessScore();});
}

void SolverGA::crossover()
{
    int id1 = 1, id2 = 1;
    double crossoverRate = 0.8;
    double chance;
    QVector< Individual * > sons;
    while (sons.size() < numOfInd / 2) {
        chance = QRandomGenerator::global()->generateDouble();

        if (chance > crossoverRate) continue;

        id1 = id1 % (numOfInd + 1) / 2;
        id2 = (id1 + 1) % (numOfInd + 1) / 2;

        sons.push_back( population[0]->crossover( population[id1] ) );

        id1++;

    }

    id1 = 0;
    for (int i = (numOfInd + 1) / 2; i < numOfInd; i++ ) {
        population[i] = sons[id1];
        id1++;
    }

}

void SolverGA::mutation()
{
    double chance;

    for (Individual * ind : population) {
        chance = QRandomGenerator::global()->generateDouble();

        if (chance <= this->mutationRate) ind->mutate();
    }



}

void SolverGA::solve()
{


    makePopulation();

    res_fitness = INT_MIN;
    res_generation = numOfGenerations;
    res_generation_ans = INT_MIN;
    res = {};

    fitnessScoreHistory.clear();

    for (int generation = 0; generation < this->numOfGenerations; generation++) {
        sortPopulation();

        if (population[0]->fitnessScore() >= res_fitness) {
            res_fitness = population[0]->fitnessScore();

            if (res != nullptr) delete res;

            res = new Individual( population[0] );

            //qDebug() << res->fitnessScore() << " fs res";

            if ( check() && population[0]->fitnessScore() > res_generation_ans ) {
                res_generation = generation - qMax( 10, numOfGenerations * 5 / 100 );
                res_generation_ans = population[0]->fitnessScore();
            }
        }

        fitnessScore.clear();

        for (Individual * ind : population) {
            fitnessScore.push_back( ind->fitnessScore() );
        }

        fitnessScoreHistory.push_back( fitnessScore );

        crossover();

        mutation();

    }

}


QVector<int> SolverGA::getAns() {
    qDebug() << res->fitnessScore() << " fitness";
    return res->pocket;
}

QVector< QVector<int> > const & SolverGA::getFitnessScoreHistory()
{
    return fitnessScoreHistory;
}

void SolverGA::makePopulation() {
    population.resize(numOfInd);

    qsrand(QDateTime::currentMSecsSinceEpoch());
    for(int i = 0; i < this->numOfInd; i++)
        population[i] = new Individual(this);
}

int SolverGA::getAnsGeneration() {
    return  res_generation;
}

void SolverGA::setMutationRate(double rate) {
    this->mutationRate = rate;
}

bool SolverGA::check() {

    int cnt = qMax( 10, numOfGenerations * 5 / 100 );

    if ( fitnessScoreHistory.size() - cnt < 0 ) return false;

    for (int i = fitnessScoreHistory.size() - 1; i >= fitnessScoreHistory.size() - cnt; i--) {
        if (fitnessScoreHistory[i][0] != fitnessScoreHistory.back()[0]) return false;
    }

    return  true;

}

void SolverGA::setNumOfInd(int numOfInd) {
    this->numOfInd = numOfInd;
}
