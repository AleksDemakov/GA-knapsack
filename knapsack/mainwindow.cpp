#include "mainwindow.h"
#include "solverga.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QChartView>
#include <QSplineSeries>
#include <QValueAxis>

using namespace QtCharts;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget( ui->gridLayoutWidget );

    //const QString fileName = QFileDialog::getOpenFileName(this);
    QFile file("in.txt");
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    QList<QString> dataList = data.split('\n');
    int limit = dataList[0].split(' ')[0].toInt();
    int num = dataList[0].split(' ')[1].toInt();

    QVector<int> a, b;

    for(QString i:dataList[1].split(' '))
        a.push_back(i.toInt());

    for(QString i:dataList[2].split(' '))
        b.push_back(i.toInt());

    int gen = dataList[3].split(' ')[0].toInt();
    double rate = dataList[3].split(' ')[1].toDouble();
    int numOfInd = dataList[3].split(' ')[2].toInt();

//    QVector<int> a = {1,2,3,4,5,6,7,8,9};
//    QVector<int> b = {10,9,8,7,6,5,4,3,2};
    //SolverGA *solver = new SolverGA(num, limit, a, b, gen, rate, numOfInd);


    QChartView * chartView;

    chartView = new QChartView( createGenerationsChart( num, limit, a, b, gen, rate, numOfInd ) );
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->gridLayout->addWidget( chartView, 0, 0, 1, 2 );

    chartView = new QChartView( createMutationRateChart( num, limit, a, b, gen, numOfInd ) );
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->gridLayout->addWidget( chartView, 1, 0 );

    chartView = new QChartView( createNumOfIndChart( num, limit, a, b, gen, rate ) );
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->gridLayout->addWidget( chartView, 1, 1 );





    //exit(0);


}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::getMean(QVector<int> const &v) {
    int sum = 0;
    for (int it : v) {
        sum += it;
    }

    return 1.0 * sum / v.size();
}

QChart * MainWindow::createGenerationsChart( int num, int limit, QVector<int> &a, QVector<int> &b, int gen, double rate, int numOfInd ) {

    SolverGA * solver = new SolverGA(num, limit, a, b, gen, rate, numOfInd);

    solver->solve();

    QVector< QVector<int> > fitness = solver->getFitnessScoreHistory();

    delete solver;

    QLineSeries* series_max = new QLineSeries();
    series_max->setName("Max fitness");

    QSplineSeries* series_mean = new QSplineSeries();
    series_mean->setName("Mean fitness per population");


    double min_value = INT_MAX;
    int max_value = INT_MIN;

    for (int i = 0; i < fitness.size(); i++) {
        series_max->append( i + 1, fitness[i][0] );
        series_mean->append( i + 1, getMean( fitness[i] ) );

        min_value = qMin(min_value, getMean( fitness[i] ) );
        max_value = qMax(max_value, fitness[i][0]);

        qDebug() << fitness[i][0];
    }



    qDebug() << min_value << " " << max_value;

    QPen pen(QRgb(0xfdb157));
    pen.setWidth(3);
    series_max->setPen(pen);

    QChart * chart = new QChart();
    QFont sansFont("Helvetica [Cronyx]", 10);

    chart->setTitle("Fitness chart");

    chart->setTitleFont( QFont("Helvetica [Cronyx]", 11, QFont::Bold) );

    QValueAxis * axisX = new QValueAxis();
    QValueAxis * axisY = new QValueAxis();
    axisY->setRange( (int)(min_value) - 1, max_value + 1 );

    axisX->setTitleText("Generation");
    axisY->setTitleText("Fitness");

    axisX->setLabelFormat("%d");
    axisY->setLabelFormat("%d");

    //axisX->setTickCount(10);
    //axisY->setTickAnchor(10);

    axisX->applyNiceNumbers();
    axisY->applyNiceNumbers();



    chart->addSeries( series_max );
    chart->addSeries( series_mean );

    chart->legend()->setMarkerShape( QLegend::MarkerShapeFromSeries );
    chart->legend()->setFont( sansFont );

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);


    axisY->setLabelsEditable();

    series_max->attachAxis( axisX );
    series_max->attachAxis( axisY );
    series_mean->attachAxis( axisX );
    series_mean->attachAxis( axisY );

    return chart;

}

QChart * MainWindow::createMutationRateChart( int num, int limit, QVector<int> &a, QVector<int> &b, int gen, int numOfInd )
{
    QLineSeries * series = new QLineSeries();
    SolverGA * solver = new SolverGA(num, limit, a, b, gen, 0.7, numOfInd);



    for (double rate = 0.0; rate <= 1.0; rate += 0.1) {

        solver->setMutationRate(rate);

        series->append( rate, getFastAnswer(solver, 100) );
    }

    delete solver;

    series->setName("Mean generation (100 experiments)");
    series->setPen( QColor("red") );


    QChart * chart = new QChart();

    chart->setTitleFont( QFont("Helvetica [Cronyx]", 10, QFont::Bold) );
    chart->setTitle("Generation that stops algorithm");

    QValueAxis * axisX = new QValueAxis();
    QValueAxis * axisY = new QValueAxis();

    axisX->setTitleText("Mutation rate");
    axisY->setTitleText("Generation");

    axisX->applyNiceNumbers();
    axisY->applyNiceNumbers();

    chart->addSeries( series );

    chart->legend()->setFont( QFont("Helvetica [Cronyx]", 9) );
    chart->legend()->setMarkerShape( QLegend::MarkerShapeFromSeries );

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    //chart->createDefaultAxes();

    return  chart;
}

double MainWindow::getFastAnswer(SolverGA *solver, int iterations) {

    QVector<int> answers;

    for (int i = 0; i < iterations; i++) {

        solver->solve();

        answers.push_back( solver->getAnsGeneration() );

        //qDebug() << solver->getAnsGeneration();

    }

    return  getMean( answers );

}


QChart * MainWindow::createNumOfIndChart( int num, int limit, QVector<int> &a, QVector<int> &b, int gen, double rate ) {
    QLineSeries * series = new QLineSeries();
    SolverGA * solver = new SolverGA(num, limit, a, b, gen, rate, 1);

    for (int numOfInd = 8; numOfInd <= 22; numOfInd += 2) {

        solver->setNumOfInd( numOfInd );

        series->append( numOfInd, getFastAnswer(solver, 100) );
    }

    delete solver;

    series->setName("Mean generation (100 experiments)");
    series->setPen( QColor("red") );

    QChart * chart = new QChart();

    chart->setTitleFont( QFont("Helvetica [Cronyx]", 10, QFont::Bold) );
    chart->setTitle("Generation that stops algorithm");

    QValueAxis * axisX = new QValueAxis();
    QValueAxis * axisY = new QValueAxis();

    axisX->setTitleText("Population size");
    axisY->setTitleText("Generation");

    axisX->applyNiceNumbers();
    axisY->applyNiceNumbers();

    axisX->setLabelFormat("%d");

    chart->addSeries( series );

    chart->legend()->setFont( QFont("Helvetica [Cronyx]", 9) );
    chart->legend()->setMarkerShape( QLegend::MarkerShapeFromSeries );

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    //chart->createDefaultAxes();

    return  chart;
}



