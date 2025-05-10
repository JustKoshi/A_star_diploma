#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "statisticswindow.h"
#include "clickablerect.h"
#include <QMessageBox>
#include "Astar.h"
#include <unordered_map>
#include <QFileDialog>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    Ui::MainWindow ui;
    Astar* astar = nullptr;
    QGraphicsScene* scene;
    std::vector<std::vector<QGraphicsRectItem*>> gridItems;
    std::unordered_map<heuristicType, stats_str> stats;
    void createGrid();
    void updateGrid();
    void clearGrid();
    void toggleGridEditables();
    void resizeGridItems(float arg);
    void clearGrid_without_obstacles();
    void markPath(std::vector<std::pair<int, int>> path);
    bool grid_edit = false;
    bool start_edit = false;
    bool end_edit = false;

    
private slots:
    void on_fileButton_clicked();
    void on_createButton_clicked();
    void on_gridEditButton_clicked();
    void on_pathFindButton_clicked();
    void on_clearButton_clicked();
    void on_pauseButton_clicked();
    void on_resumeButton_clicked();
    void on_saveButton_clicked();
    void on_StartButton_clicked();
    void on_EndButton_clicked();
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_statsButton_clicked();
    void on_fastforwardButton_clicked();
    void on_fastforward2Button_clicked(); 
    void on_resetButton_clicked();
    void handleSquareClick(int x, int y);
    void updateGridCell(int x, int y, QColor color);
    void updateCosts(int x, int y, float gCost, float fCost);
    void clearCostLabels();
   
    void on_comboBox_currentTextChanged();
    
};
