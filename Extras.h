#pragma once
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <algorithm>

std::pair<int, int> readFileDimensions(const QString& filePath);