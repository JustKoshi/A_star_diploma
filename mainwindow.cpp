#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("A* Visualization");
    ui.setupUi(this);
    disconnect(ui.comboBox, &QComboBox::currentTextChanged, this, &MainWindow::on_comboBox_currentTextChanged);
    ui.comboBox->addItem("Manhattan");
    ui.comboBox->addItem("Euclidean");
    ui.comboBox->addItem("Chebyshev");
    ui.comboBox->addItem("Dijkstra");
    connect(ui.comboBox, &QComboBox::currentTextChanged, this, &MainWindow::on_comboBox_currentTextChanged);

    scene = new QGraphicsScene(this);
    ui.graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{}

void MainWindow::on_fileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Wybierz plik", QDir::homePath(), "Wszystkie pliki (*.*);;Pliki tekstowe (*.txt)");

    if (!fileName.isEmpty()) {
        //check if file is txt
        if (!fileName.endsWith(".txt")) {
            QMessageBox msgBox;
            msgBox.setText("Wybrano plik o nieprawidlowym formacie");
            msgBox.exec();
            return;
        }
        else {
            ui.lineEdit->setText(fileName);
            
            
            
        }
         
    }
}

void MainWindow::on_createButton_clicked() {

    heuristicType heuristic;
    heuristic = ui.comboBox->currentIndex() == 0 ? heuristicType::manhattan :
        ui.comboBox->currentIndex() == 1 ? heuristicType::euclidean :
        ui.comboBox->currentIndex() == 2 ? heuristicType::chebyshev :
        heuristicType::dijkstra;    
    QString filename = ui.lineEdit->text();
    try {
        if (astar != nullptr)
            delete astar;
        astar = new Astar(filename, heuristic);

        connect(astar, &Astar::updateGridCell, this, &MainWindow::updateGridCell);
        connect(astar, &Astar::updateCosts, this, &MainWindow::updateCosts);
        connect(astar, &Astar::clearCostLabels, this, &MainWindow::clearCostLabels);

        createGrid();
        updateGrid();
        ui.gridEditButton->setEnabled(true);
        ui.pathFindButton->setEnabled(true);
        ui.clearButton->setEnabled(true);   
        ui.StartButton->setEnabled(true);
        ui.EndButton->setEnabled(true);
        ui.doubleSpinBox->setEnabled(true);
        ui.resumeButton->setEnabled(true);
        ui.resetButton->setEnabled(true);
    }
    catch (std::exception& e) {
		qDebug() << "exception:" << e.what();
	}
}

void MainWindow::createGrid() {
    int width, height;
    std::tie(width, height) = astar->getDimensions();
    scene->clear();
    gridItems.clear();
    gridItems.resize(height, std::vector<QGraphicsRectItem*>(width, nullptr));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            ClickableRectItem* rect = new ClickableRectItem(j, i);
            rect->setRect(j * 24, i * 24, 24, 24);
            rect->setPen(QPen(Qt::black));
            rect->setBrush(QBrush(Qt::white));
            scene->addItem(rect);
            gridItems[i][j] = rect;
            connect(rect, &ClickableRectItem::clicked, this, &MainWindow::handleSquareClick);
        }
    }
}

void MainWindow::updateGrid() {
    int width, height;
    std::tie(width, height) = astar->getDimensions();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (astar->isObstacle(i, j)) {
                gridItems[i][j]->setBrush(Qt::black);
            }
            else {
                gridItems[i][j]->setBrush(Qt::white);
            }
        }
    }
    std::pair<int, int> start = astar->getStart();
    std::pair<int, int> end = astar->getEnd();
    if (start != std::make_pair(-1, -1)) {
        gridItems[start.first][start.second]->setBrush(Qt::green);
        static_cast<ClickableRectItem*>(gridItems[start.first][start.second])->setLabel("S");

    }
    if (end != std::make_pair(-1, -1)) {
        gridItems[end.first][end.second]->setBrush(Qt::red);
        static_cast<ClickableRectItem*>(gridItems[end.first][end.second])->setLabel("E");
    }
}

void MainWindow::handleSquareClick(int x, int y) {
    std::pair<int, int> start = astar->getStart();
    std::pair<int, int> end = astar->getEnd();
    if (!astar) return;
    if (grid_edit) {
        if (astar->isObstacle(y, x)) {
            astar->removeObstacle(y, x);
            gridItems[y][x]->setBrush(Qt::white);
            return;
        }

        else {
            astar->setObstacle(y, x);
            gridItems[y][x]->setBrush(Qt::black);
        }

    }

    if (start_edit&&!astar->isObstacle(y,x)&&std::make_pair(y,x)!=astar->getEnd())
    {
        if (start != std::make_pair(-1, -1)) {
            gridItems[start.first][start.second]->setBrush(Qt::white);
            static_cast<ClickableRectItem*>(gridItems[start.first][start.second])->setLabel("");
        }
		astar->setStart(y, x);
		gridItems[y][x]->setBrush(Qt::green);
        clearGrid_without_obstacles();
		static_cast<ClickableRectItem*>(gridItems[y][x])->setLabel("S");
		start_edit = false;
        
	}

   
    if (end_edit&&!astar->isObstacle(y, x)&& std::make_pair(y, x) != astar->getStart()) 
    {
        if (end != std::make_pair(-1, -1)) {
            gridItems[end.first][end.second]->setBrush(Qt::white);
            //static_cast<ClickableRectItem*>(gridItems[end.first][end.second])->setLabel("");
		}
		astar->setEnd(y, x);
		gridItems[y][x]->setBrush(Qt::red);
        clearGrid_without_obstacles();
		//static_cast<ClickableRectItem*>(gridItems[y][x])->setLabel("E");
		end_edit = false;
        
	}

    ui.info_label->setText("Press button below to change start / end point");
}

void MainWindow::on_gridEditButton_clicked() {
	grid_edit = !grid_edit;
    clearGrid_without_obstacles();
}

void MainWindow::on_pathFindButton_clicked() {
    try {
        ui.pauseButton->setEnabled(true);
        ui.fastforwardButton->setEnabled(true);
        ui.fastforward2Button->setEnabled(true);

        disconnect(astar, &Astar::pathFound, this, &MainWindow::markPath);
        connect(astar, &Astar::pathFound, this, &MainWindow::markPath);
        toggleGridEditables();
        astar->findPath();
        if (grid_edit)
            grid_edit = !grid_edit;
        
        if (astar->statistics.pathLength != 0) {
            stats[astar->statistics.heuristic] = astar->statistics;
        }
    }
    catch (std::exception& e) {
        qDebug() << "exception:" << e.what();
      }
}

void MainWindow::markPath(std::vector<std::pair<int, int>> path) {
    clearGrid_without_obstacles();
    std::pair<int, int> start = astar->getStart();
    std::pair<int, int> end = astar->getEnd();
    for (auto& p : path) {
		if (p == start || p == end) continue;
		gridItems[p.first][p.second]->setBrush(Qt::darkGreen);
        
	}
    ui.pauseButton->setEnabled(false);
	ui.resumeButton->setEnabled(false);
    ui.fastforwardButton->setEnabled(false);
    ui.fastforward2Button->setEnabled(false);
	QMessageBox::information(this, "Path Found", "Path Found!");
    toggleGridEditables();
}

void MainWindow::clearGrid() {
    int width, height;
	std::tie(width, height) = astar->getDimensions();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
			gridItems[i][j]->setBrush(Qt::white);
		}
	}
	std::pair<int, int> start = astar->getStart();
	std::pair<int, int> end = astar->getEnd();
    if (start != std::make_pair(-1, -1)) {
		gridItems[start.first][start.second]->setBrush(Qt::green);
	}
    if (end != std::make_pair(-1, -1)) {
		gridItems[end.first][end.second]->setBrush(Qt::red);
	}
}

void MainWindow::clearGrid_without_obstacles() {
    int width, height;
	std::tie(width, height) = astar->getDimensions();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (astar->isObstacle(i, j)) {
                continue;
            }
			gridItems[i][j]->setBrush(Qt::white);
            static_cast<ClickableRectItem*>(gridItems[i][j])->setLabel("");
            
		}
	}
	std::pair<int, int> start = astar->getStart();
	std::pair<int, int> end = astar->getEnd();
    if (start != std::make_pair(-1, -1)) {
		gridItems[start.first][start.second]->setBrush(Qt::green);
        static_cast<ClickableRectItem*>(gridItems[start.first][start.second])->setLabel("S");
	}
    if (end != std::make_pair(-1, -1)) {
		gridItems[end.first][end.second]->setBrush(Qt::red);
        static_cast<ClickableRectItem*>(gridItems[end.first][end.second])->setLabel("E");
	}
}

void MainWindow::on_clearButton_clicked() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Clear Grid", "Are you sure you want to clear the grid?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        clearGrid();
    }

    
}

void MainWindow::on_resetButton_clicked() {
    if (!astar) {
        return;
    }
    astar->setHeuristic(ui.comboBox->currentIndex() == 0 ? heuristicType::manhattan :
        ui.comboBox->currentIndex() == 1 ? heuristicType::euclidean :
        ui.comboBox->currentIndex() == 2 ? heuristicType::chebyshev :
        heuristicType::dijkstra);
    clearGrid_without_obstacles();
}

void MainWindow::on_comboBox_currentTextChanged() {
    if (!astar) {
		return;
	}
    astar->setHeuristic(ui.comboBox->currentIndex() == 0 ? heuristicType::manhattan :
        ui.comboBox->currentIndex() == 1 ? heuristicType::euclidean :
        ui.comboBox->currentIndex() == 2 ? heuristicType::chebyshev :
        heuristicType::dijkstra);
    clearGrid_without_obstacles();
}

void MainWindow::updateGridCell(int x, int y, QColor color) {
    gridItems[x][y]->setBrush(color);
}

void MainWindow::updateCosts(int x, int y, float gCost, float hCost) {
    QString label = QString("%1 %2").arg(gCost).arg(hCost);
    static_cast<ClickableRectItem*>(gridItems[x][y])->setLabel(label);
}

void MainWindow::clearCostLabels() {
    int width, height;
    std::tie(width, height) = astar->getDimensions();
    std::pair<int, int> start = astar->getStart();
    std::pair<int, int> end = astar->getEnd();

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (std::make_pair(i, j) != start && std::make_pair(i, j) != end) {
                static_cast<ClickableRectItem*>(gridItems[i][j])->setLabel("");
            }
        }
    }
}


void MainWindow::on_pauseButton_clicked() {
	astar->pause_pathFinding();
	ui.pauseButton->setEnabled(false);
	ui.resumeButton->setEnabled(true);
}

void MainWindow::on_resumeButton_clicked() {
    astar->setDelay(1.0);
    astar->resume_pathFinding();
	ui.pauseButton->setEnabled(true);
	ui.resumeButton->setEnabled(false);

}

void MainWindow::on_saveButton_clicked() {
	QString fileName = QFileDialog::getSaveFileName(this, "Zapisz plik", QDir::homePath(), "Pliki tekstowe (*.txt)");
    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(".txt")) {
            QMessageBox msgBox;
            msgBox.setText("Wybrano plik o nieprawidlowym formacie");
            msgBox.exec();
            return;
        }
        else {
            ui.lineEdit->setText(fileName);
            astar->saveGrid(fileName);


        }
        
		
	}
}

void MainWindow::on_StartButton_clicked() {
	start_edit = !start_edit;
	end_edit = false;
	ui.info_label->setText("Click on the grid to set start point");
}

void MainWindow::on_EndButton_clicked() {
	end_edit = !end_edit;
	start_edit = false;
	ui.info_label->setText("Click on the grid to set end point");
}

void MainWindow::resizeGridItems(float arg) {
    for(auto& row : gridItems) {
		for(auto& item : row) {
            auto clickableItem = dynamic_cast<ClickableRectItem*>(item);
            if (clickableItem) {
                clickableItem->resizeRect(arg);
            }
		}
	}

}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1) {
    resizeGridItems(static_cast<float>(arg1));
}

void MainWindow::on_fastforwardButton_clicked() {
    astar->setDelay(0.45);
}

void MainWindow::on_fastforward2Button_clicked() {
    astar->setDelay(0.20);
}

void MainWindow::on_statsButton_clicked() {
    if (astar) {
		statisticswindow* statsWindow = new statisticswindow(this, stats);
		statsWindow->setWindowTitle("Statistics");
		//statsWindow->setStats(astar->get_pathLength(), astar->get_visitedNodes(), astar->get_timeElapsed());
		statsWindow->show();
	}
}

void MainWindow::toggleGridEditables(){
    ui.StartButton->setEnabled(!ui.StartButton->isEnabled());
    ui.EndButton->setEnabled(!ui.EndButton->isEnabled());
    ui.gridEditButton->setEnabled(!ui.gridEditButton->isEnabled());
    ui.pathFindButton->setEnabled(!ui.pathFindButton->isEnabled());
    ui.clearButton->setEnabled(!ui.clearButton->isEnabled());
}
