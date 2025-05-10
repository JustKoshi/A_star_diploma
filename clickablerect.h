#pragma once
#include <QGraphicsRectItem>
#include <QObject>
#include <QDebug>
#include <QFont>
class ClickableRectItem : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    ClickableRectItem(int x, int y, QGraphicsItem* parent = nullptr);
    void setLabel(const QString& text);
    void resizeRect(float newSize);
signals:
    void clicked(int x, int y);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    int x, y;
    QGraphicsTextItem* label;
};