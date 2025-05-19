#include "Astar.h"


Astar::Astar(int width, int height, heuristicType heuristic, std::pair<int, int> start, std::pair<int, int> end) :
	width(width), height(height), heuristicT(heuristic), start(start), end(end){
	if (start == end) {
		throw std::invalid_argument("Start and end points are the same");
	}
	pathLength = 0;
	grid.resize(width, std::vector<bool>(height, false));
    paused = false;
    thread = new QThread();
    moveToThread(thread);
    connect(thread, &QThread::started, this, &Astar::findPath);
}

Astar::Astar(QString filename, heuristicType heuristic)
    : heuristicT(heuristic), delay_value(initial_delay){
    std::pair<int, int> dimensions = readFileDimensions(filename);
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nie mozna otworzyc pliku:" << filename;
        return;
    }
    paused = false;
    
    
    QTextStream in(&file);

    qDebug() << "Zawartosc pliku:" << filename;
    QVector<QString> lines;

    while (!in.atEnd()) {
        QString line = in.readLine();
        //qDebug() << line;
        lines.append(line);
    }

    file.close();

    width = dimensions.first;
    height = dimensions.second;
    grid.resize(height, std::vector<bool>(width, false));

    if (lines.size() != height) {
        throw std::runtime_error("Nieprawidłowa liczba wierszy w pliku.");
    }

    for (int j = 0; j < height; ++j) {  // Iteracja po wierszach (Y)
        if (lines[j].size() != width) {
            throw std::runtime_error("Nieprawidłowa liczba znaków w wierszu.");
        }
        for (int i = 0; i < width; ++i) {  // Iteracja po kolumnach (X)
            char value = lines[j][i].toLatin1();  // Konwersja QString -> char - ważne!
            if (value != '1' && value != '0') {
                throw std::invalid_argument("Invalid character in file: " + QString(value).toStdString());
            }
            if (value == '1') {
                grid[j][i] = true;
            }
 
        }
    }

    start = std::make_pair<int, int>(0, 0);
    while (start.first < dimensions.second && start.second < dimensions.first && grid[start.first][start.second] == true) {
        start.first++;
        if (start.first >= dimensions.second) {
            start.first = 0;
            start.second++;
        }
        if (start.second >= dimensions.first) {
            throw std::runtime_error("No valid start point found in the grid.");
        }
    }

    end = std::make_pair(height-1, width-1);
    while (end.first >= 0 && end.second >= 0 && grid[end.first][end.second] == true) {
        end.first--;
        if (end.first < 0) {
            end.first = height - 1;
            end.second--;
        }
        if (end.second < 0) {
            throw std::runtime_error("No valid end point found in the grid.");
        }
    }
    thread = new QThread();
    moveToThread(thread);
    connect(thread, &QThread::started, this, &Astar::findPath);
}

void Astar::startFindPath() {
	thread->start();
}


int Astar::get_pathLength() {
	return pathLength;
}

void Astar::clearObstacles() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			grid[i][j] = false;
		}
	}
}

void Astar::setObstacle(int x, int y) {
	grid[x][y] = true;
}

void Astar::removeObstacle(int x, int y) {
	grid[x][y] = false;
}

//void Astar::printGrid() {
//    if (grid.empty()) {
//		throw std::invalid_argument("Grid is empty");
//    }
//    
//    for (int i = 0; i < height; i++) {
//        QString rowStr;
//        for (int j = 0; j < width; j++) {
//			if (i == start.first && j == start.second) {
//				rowStr += "S ";
//			}
//			else if (i == end.first && j == end.second) {
//				rowStr += "E ";
//			}
//			else {
//				rowStr += grid[i][j] ? "X " : "O ";
//			}
//        }
//        qDebug().noquote() << rowStr;
//    }
//}
//
//void Astar::printGrid(std::vector<std::pair<int, int>> path) {
//	if (grid.empty()) {
//		throw std::invalid_argument("Grid is empty");
//	}
//	if (path.empty()) {
//		throw std::invalid_argument("Path was not found");
//	}
//	for (int i = 0; i < height; i++) {
//		QString rowStr;
//		for (int j = 0; j < width; j++) {
//			if (i == start.first && j == start.second) {
//				rowStr += "S ";
//			}
//			else if (i == end.first && j == end.second) {
//				rowStr += "E ";
//			}
//			else if (std::find(path.begin(), path.end(), std::make_pair(i, j)) != path.end()) {
//				rowStr += "p ";
//			}
//			else {
//				rowStr += grid[i][j] ? "X " : "O ";
//			}
//		}
//		qDebug().noquote() << rowStr;
//	}
//}
std::vector<std::pair<int, int>> Astar::findPath() {
    if (grid.empty()) {
        throw std::invalid_argument("Grid is empty");
    }
    if (!pathExists(start, end)) {
        throw std::invalid_argument("Path does not exist");
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    int iterations = 0;

    std::priority_queue<Node*, std::vector<Node*>, NodeCompare> openList;
    std::vector<std::vector<bool>> closedList(height, std::vector<bool>(width, false));
    std::unordered_map<int, Node*> allNodes;

    Node* startNode = new Node(start.first, start.second, 0,
        Node::heuristic_def(start.first, start.second, end.first, end.second, heuristicT),
        nullptr, heuristicT);

    openList.push(startNode);
    allNodes[start.first * width + start.second] = startNode;

    std::vector<std::pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    while (!openList.empty()) {
      if (paused) {
            QCoreApplication::processEvents();
            //QThread::msleep(100);
            continue;
      }

        Node* current = openList.top();
        openList.pop();
        iterations++;

        int x = current->x, y = current->y;
        if (x == end.first && y == end.second) {
            std::vector<std::pair<int, int>> path;
            while (current) {
                path.push_back({ current->x, current->y });
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());

            for (auto& n : allNodes) delete n.second;
            pathLength = path.size();

            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = endTime - startTime;
            double ratio_iterations_pathLength = static_cast<double>(iterations) / pathLength;
            double ratio_visited_all = static_cast<double>(iterations) / (width * height);

            statistics = stats_str(heuristicT, pathLength, iterations, elapsed.count(), ratio_iterations_pathLength, ratio_visited_all);
            
            for (const auto& cell : path) {
                emit updateGridCell(cell.first, cell.second, QColor(Qt::darkGreen));
                delay(100);
            }
            emit clearCostLabels();

            emit pathFound(path);
            thread->quit();
            return path;

        
        }

        closedList[x][y] = true;
        emit updateGridCell(x, y, QColor(Qt::blue));
        for (auto& dir : directions) {
            int newX = x + dir.first, newY = y + dir.second;
            if (newX < 0 || newX >= height || newY < 0 || newY >= width || grid[newX][newY] || closedList[newX][newY])
                continue;

            float newG = current->g + 1;
            Node* neighbor = new Node(newX, newY, newG,
                Node::heuristic_def(newX, newY, end.first, end.second, heuristicT),
                current, heuristicT);

            if (!allNodes.count(newX * width + newY) || newG < allNodes[newX * width + newY]->g) {
                openList.push(neighbor);
                allNodes[newX * width + newY] = neighbor;
                if (std::make_pair(newX, newY) != end) 
                    emit updateCosts(newX, newY, neighbor->g, neighbor->h);
            }
        }

         std::vector<std::vector<bool>> openListState(height, std::vector<bool>(width, false));
         std::priority_queue<Node*, std::vector<Node*>, NodeCompare> tempOpenList = openList;
         while (!tempOpenList.empty()) {
             Node* node = tempOpenList.top();
             tempOpenList.pop();
             openListState[node->x][node->y] = true;
         }
         visualizeGrid(openListState, closedList);
         QCoreApplication::processEvents();
        
    }

    for (auto& n : allNodes) delete n.second;
    return {};
}

bool Astar::pathExists(std::pair<int, int> start, std::pair<int, int> end) {
    
    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::vector<std::pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    if (grid[start.first][start.second] || grid[end.first][end.second]) {
        qDebug() << "Start or End are blocked!";
        return false;
    }

   
    q.push(start);
    visited[start.first][start.second] = true;

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        if (current == end) {
            return true;
        }

        for (auto& dir : directions) {
            int newX = current.first + dir.first, newY = current.second + dir.second;

            if (newX < 0 || newX >= height || newY < 0 || newY >= width || grid[newX][newY] || visited[newX][newY]) {
                continue;
            }

            q.push({ newX, newY });
            visited[newX][newY] = true;
        }
    }

    qDebug() << "Path does not exist!";
    return false;
}

std::pair<int, int> Astar::getDimensions() {
	return { width, height };
}

bool Astar::isObstacle(int x, int y) const {
    return grid[x][y];
}

std::pair<int, int> Astar::getStart() {
	return start;
}

std::pair<int, int> Astar::getEnd() {
	return end;
}

void Astar::setHeuristic(heuristicType heuristic) {
	heuristicT = heuristic;
}

void Astar::visualizeGrid(std::vector<std::vector<bool>> openListState, std::vector<std::vector<bool>> closedListState) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == start.first && j == start.second) {
                emit updateGridCell(i, j, QColor(Qt::green));
            }
            else if (i == end.first && j == end.second) {
                emit updateGridCell(i, j, QColor(Qt::red));
            }
            else if (closedListState[i][j]) {
                emit updateGridCell(i, j, QColor(Qt::cyan));
            }
            else if (openListState[i][j] && !closedListState[i][j]) {
                emit updateGridCell(i, j, QColor(Qt::yellow));
            }
            
            else {
                emit updateGridCell(i, j, grid[i][j] ? QColor(Qt::black) : QColor(Qt::white));
            }
        }
    }
    delay(delay_value);
}

void Astar::delay(int milliseconds) {
    QThread::msleep(milliseconds);
    QCoreApplication::processEvents();
}

void Astar::saveGrid(QString filename) {
	QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "Nie mozna otworzyc pliku:" << filename;
		return;
	}

	QTextStream out(&file);

    for (int i = 0; i < height; i++) {
		QString rowStr;
        for (int j = 0; j < width; j++) {
            rowStr += grid[i][j] ? "1" : "0";
		}
		out << rowStr << "\n";
	}

	file.close();
}

void Astar::pause_pathFinding() {
    paused = true;
}

void Astar::resume_pathFinding() {
	paused = false;
}

void Astar::setStart(int x, int y) {
	start = std::make_pair(x, y);
}

void Astar::setEnd(int x, int y) {
	end = std::make_pair(x, y);
}

void Astar::setDelay(double value) {
    delay_value = initial_delay * value;
}

heuristicType Astar::getHeuristic() {
	return heuristicT;
}
