#include "Extras.h"

std::pair<int, int> readFileDimensions(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nie mozna otworzyc pliku!";
        return {};
    }

    QTextStream in(&file);
    int height = 0;
    int maxWidth = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        height++; 
        maxWidth = qMax(maxWidth, line.length()); 
    }
    qDebug() << "Wysokosc:" << height << "Najwieksza szerokosc:" << maxWidth;

    file.close();
    return { maxWidth, height };
}