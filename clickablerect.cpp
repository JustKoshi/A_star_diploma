#include "clickablerect.h"
#include <QGraphicsSceneMouseEvent>

ClickableRectItem::ClickableRectItem(int x, int y , QGraphicsItem* parent)
    : QGraphicsRectItem(parent), x(x), y(y) {
    label = new QGraphicsTextItem(this);
    QFont font = label->font();
    font.setPointSize(7);
    font.setBold(true);
    label->setFont(font);
    label->setDefaultTextColor(Qt::black);
    
    label->setPos(x * 22 + 5, y * 22 + 5);
}

void ClickableRectItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    emit clicked(x, y);
    QGraphicsRectItem::mousePressEvent(event);
}

void ClickableRectItem::setLabel(const QString& text) {
    label->setPlainText(text);
    QRectF rect = this->rect();
    QRectF textRect = label->boundingRect();
    label->setPos(rect.x() + (rect.width() - textRect.width()) / 2,
        rect.y() + (rect.height() - textRect.height()) / 2);
}

void ClickableRectItem::resizeRect(float size) {
    float newSize = size * 24;
    setRect(x * newSize, y * newSize, newSize, newSize);

    QFont font = label->font();
    if (size < 0.75) {
        label->setVisible(false);
    }
    else {
        label->setVisible(true);
        font.setPointSize(6);
        font.setBold(true);
    }
    label->setFont(font);

    QRectF rect = this->rect();
    QRectF textRect = label->boundingRect();
    label->setPos(rect.x() + (rect.width() - textRect.width()) / 2,
        rect.y() + (rect.height() - textRect.height()) / 2);
    
	
	
}
