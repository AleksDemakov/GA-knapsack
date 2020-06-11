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
    const QString fileName = QFileDialog::getOpenFileName(this);
    QFile file(fileName);
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
    SolverGA *solver = new SolverGA(num, limit, a, b, gen, rate, numOfInd);
    solver->solve();


    QVector<int> res = solver->getAns();

    QChartView * chartView = new QChartView(this);
    chartView->setChart( createChart( solver ) );
    chartView->setRenderHint(QPainter::Antialiasing);



    ui->gridLayout->addWidget( chartView, 0, 0 );


    solver->solve();

    res = solver->getAns();

    chartView = new QChartView(this);
    chartView->setChart( createChart( solver ) );
    chartView->setRenderHint(QPainter::Antialiasing);


    ui->gridLayout->addWidget( chartView, 0, 1 );

    solver->solve();

    res = solver->getAns();

    chartView = new QChartView(this);
    chartView->setChart( createChart( solver ) );
    chartView->setRenderHint(QPainter::Antialiasing);


    ui->gridLayout->addWidget( chartView, 1, 0 );

    solver->solve();

    res = solver->getAns();

    chartView = new QChartView(this);
    chartView->setChart( createChart( solver ) );
    chartView->setRenderHint(QPainter::Antialiasing);


    ui->gridLayout->addWidget( chartView, 1, 1 );


    //chartView->setFixedSize(500, 500);



    qDebug() << res;

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

QChart * MainWindow::createChart(SolverGA *solver) {
    QVector< QVector<int> > fitness = solver->getFitnessScoreHistory();

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
    QFont sansFont("Helvetica [Cronyx]", 12);

    chart->setTitle("Fitness chart");

    chart->setTitleFont( QFont("Helvetica [Cronyx]", 12, QFont::Bold) );

    QValueAxis * axisX = new QValueAxis();
    QValueAxis * axisY = new QValueAxis();
    axisY->setRange( (int)(min_value) - 1, max_value + 1 );

    axisX->setTitleText("Generation");
    axisY->setTitleText("Fitness");

    axisX->setLabelFormat("%d");
    axisY->setLabelFormat("%d");

    //axisX->setTickCount(10);
    axisY->setTickCount( 20 );
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

