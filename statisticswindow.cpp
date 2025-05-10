#include "statisticswindow.h"

std::ostream& operator<<(std::ostream& os, const stats_str& stats) {
    os << "Heuristic: " << static_cast<int>(stats.heuristic) << "\n";
    os << "Path Length: " << stats.pathLength << "\n";
    os << "Iterations: " << stats.iterations << "\n";
    os << "Time Elapsed: " << stats.timeElapsed << " seconds\n";
    os << "Ratio of Iterations to Path Length: " << stats.ratio_iterations_pathLength << "\n";
    os << "Ratio of Visited Nodes to All Nodes: " << stats.ratio_visted_all << "\n";
    return os;
}

statisticswindow::statisticswindow(QWidget* parent, std::unordered_map<heuristicType, stats_str> stats)
	: QMainWindow(parent), statistics(stats)
{   
    ui.setupUi(this);

    setMinimumSize(1500, 1000);


    for (auto& entry : statistics) {
        if (entry.second.pathLength == 0) continue;
        std::cout << entry.second;
    }

    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(centralWidget);

    // Add separate bar charts for each statistic
    layout->addWidget(createBarChart("Path Length", [](const stats_str& stats) {
        return stats.pathLength;
        }));

    layout->addWidget(createBarChart("Iterations", [](const stats_str& stats) {
        return stats.iterations;
        }));

    layout->addWidget(createBarChart("Time Elapsed", [](const stats_str& stats) {
        return stats.timeElapsed;
        }));

    layout->addWidget(createBarChart("Ratio Iterations/Path Length", [](const stats_str& stats) {
        return stats.ratio_iterations_pathLength;
        }));

    layout->addWidget(createBarChart("Ratio Visited/All Nodes", [](const stats_str& stats) {
        return stats.ratio_visted_all;
        }));

    // Set the central widget
    setCentralWidget(centralWidget);
}

QChartView* statisticswindow::createBarChart(const QString& title, std::function<double(const stats_str&)> valueExtractor)
{

    QChart* chart = new QChart();
    chart->setTitle(title);

    QBarSet* manhattanSet = new QBarSet("Manhattan");
    QBarSet* euclideanSet = new QBarSet("Euclidean");
    QBarSet* dijkstraSet = new QBarSet("Dijkstra");
    QBarSet* chebyshevSet = new QBarSet("Chebyshev");

    manhattanSet->setColor(Qt::darkYellow);
    euclideanSet->setColor(Qt::red);
    dijkstraSet->setColor(Qt::blue);
    chebyshevSet->setColor(Qt::darkGreen);

    double maxValue = 0.0;
    for (const auto& entry : statistics) {
        heuristicType heuristic = entry.first;
        stats_str stats = entry.second;

        double value = valueExtractor(stats);
        maxValue = std::max(maxValue, value);

        switch (heuristic) {
        case heuristicType::manhattan:
            *manhattanSet << value;
            break;
        case heuristicType::euclidean:
            *euclideanSet << value;
            break;
        case heuristicType::dijkstra:
            *dijkstraSet << value;
            break;
        case heuristicType::chebyshev:
            *chebyshevSet << value;
            break;
        }
    }

    QBarSeries* series = new QBarSeries();
    series->append(manhattanSet);
    series->append(euclideanSet);
    series->append(dijkstraSet);
    series->append(chebyshevSet);

    series->setLabelsVisible(true);
    series->setLabelsFormat("@value");

    series->setBarWidth(0.8); 
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->addSeries(series);

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append("Heuristics");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText(title);
    axisY->setLabelFormat("%.2f");
    axisY->setRange(0, maxValue * 1.1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);


    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);


    return chartView;
}

statisticswindow::~statisticswindow()
{}
