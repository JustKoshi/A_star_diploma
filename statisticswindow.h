#pragma once


#include <QMainWindow>
#include "ui_statisticswindow.h"
#include "stats_str.h"
#include <unordered_map>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QDebug>
#include <iostream>
#include <QHBoxLayout>


class statisticswindow : public QMainWindow
{
	Q_OBJECT

public:
	statisticswindow(QWidget* parent = nullptr, std::unordered_map<heuristicType, stats_str> stats = {});
	~statisticswindow();

private:
	Ui::statisticswindowClass ui;
	std::unordered_map<heuristicType, stats_str> statistics;
	QChartView* createBarChart(const QString& title, std::function<double(const stats_str&)> valueExtractor);
};

