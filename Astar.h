#pragma once

#include "Node.h"
#include "Extras.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <QDebug>
#include <QString>
#include <QFile>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <QCoreApplication>
#include <QColor>
#include <QThread>
#include <Chrono>
#include "stats_str.h"

constexpr int initial_delay = 200;

class Astar : public QObject {
		Q_OBJECT
private:
	int width, height;
	int pathLength;
	std::vector<std::vector<bool>> grid;
	heuristicType heuristicT;
	std::pair<int, int> start, end;
	std::vector<std::vector<bool>> openListState;
	std::vector<std::vector<bool>> closedListState;
	bool paused;
	int delay_value;
	QThread* thread;

signals:
	void updateGridCell(int x, int y, QColor color);
	void updateCosts(int x, int y, float gCost, float fCost);
	void clearCostLabels();
	void pathFound(std::vector<std::pair<int, int>> path);
public:
	Astar(int width, int height, heuristicType heuristic, std::pair<int, int> start, std::pair<int, int> end);
	Astar(QString filename, heuristicType heuristic);
	void clearObstacles();
	void setObstacle(int x, int y);
	void removeObstacle(int x, int y);
	
	void startFindPath();
	void setDelay(double value);
	//void printGrid();
	//void printGrid(std::vector<std::pair<int, int>> path);
	bool pathExists(std::pair<int, int> start, std::pair<int, int> end);
	void saveGrid(QString filename);
	int get_pathLength();
	void setStart(int x, int y);
	void setEnd(int x, int y);
	std::pair<int, int> getDimensions();
	std::pair<int, int> getStart();
	std::pair<int, int> getEnd();
	std::vector<std::pair<int, int>> findPath();
	void pause_pathFinding();
	void resume_pathFinding();
	bool isObstacle(int x, int y) const;
	void setHeuristic(heuristicType heuristic);
	void visualizeGrid(std::vector<std::vector<bool>> openListState, std::vector < std::vector<bool>> closedListState);
	void delay(int milliseconds);
	heuristicType getHeuristic();

	stats_str statistics;
};