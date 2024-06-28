#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <cmath>
#include <QDebug>
#include <QObject>
#include "customitem.h"

// 自定义 Item
CustomItem::CustomItem(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    // 画笔 - 边框色

    QPen p = pen();
    p.setWidth(2);
    p.setColor(QColor(0, 160,230));

    setPen(p);
    // 画刷 - 背景色
    setBrush(QColor(247, 160, 57));

    // 可选择、可移动
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void CustomItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() == Qt::ShiftModifier) {
            qDebug() << "Custom item left clicked with shift key.";
            // 选中 item
            setSelected(true);
        }
        else if (event->modifiers() == Qt::AltModifier) {
            qDebug() << "Custom item left clicked with alt key.";
            // 重置 item 大小

            //double radius = boundingRect().width() / 2.0;

            double halfwidth = boundingRect().width() / 2.0;
            double halfheight= boundingRect().height() / 2.0;
            QPointF topLeft = boundingRect().topLeft();
            qDebug() << "WWW" << pos() << boundingRect();

            //m_centerPointF = QPointF(topLeft.x() + pos().x() + radius, topLeft.y() + pos().y() + radius);

            m_centerPointF = QPointF(topLeft.x() + pos().x() + halfwidth, topLeft.y() + pos().y() + halfheight);
            QPointF pos = event->scenePos();
            //qDebug() << boundingRect() << radius << this->pos() << pos << event->pos();

            /*double dist = sqrt(pow(m_centerPointF.x() - pos.x(), 2) + pow(m_centerPointF.y() - pos.y(), 2));
            if (dist / radius > 0.8) { //判断 鼠标拖拽item的边界 是否为边界
                qDebug() << dist << radius << dist / radius;
                m_bResizing = true;
            }
            else {
                m_bResizing = false;
            }*/

            double dist = fmin(abs(m_centerPointF.x()+ halfwidth - pos.x()),abs(m_centerPointF.y()+halfheight - pos.y()));
            if(dist < 20)
            {
                //qDebug() << dist << radius << dist / radius;
                m_bResizing = true;
            }
            else {
                m_bResizing = false;
            }
        }
        else {
            //qDebug() << "Custom item left clicked.";
            QGraphicsItem::mousePressEvent(event);
            event->accept();
        }
    }
    else if (event->button() == Qt::RightButton) {
        //qDebug() << "Custom item right clicked.";
        event->ignore();
    }
}

void CustomItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ((event->modifiers() == Qt::AltModifier) && m_bResizing) {
        QPointF pos = event->scenePos();
        QPointF topLeft = boundingRect().topLeft();
        //double dist = sqrt(pow(m_centerPointF.x() - pos.x(), 2) + pow(m_centerPointF.y() - pos.y(), 2));
        double chang = pos.x()-topLeft.x();
        double kuan = pos.y()-topLeft.y();
        setRect(m_centerPointF.x() - this->pos().x() -chang/2  ,
                m_centerPointF.y() - this->pos().y() -kuan/2  , chang , kuan );
        /*setRect(pos.x()-chang,
                pos.y()-kuan, chang , kuan );*/

    }
    else if (event->modifiers() != Qt::AltModifier) {
        qDebug() << "Custom item moved.";
        QGraphicsItem::mouseMoveEvent(event);
        qDebug() << "moved" << pos();
    }
}

void CustomItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if ((event->modifiers() == Qt::AltModifier) && m_bResizing) {
        m_bResizing = false;
    }
    else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

int CustomItem::type() const
{
    return UserType + 1;
}

// 自定义 Scene
void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Custom scene clicked.";
    QGraphicsScene::mousePressEvent(event);
    if (!event->isAccepted()&&itemtype!=0) {
        if (event->button() == Qt::LeftButton) {
            // 在 Scene 上添加一个自定义 item
            QPointF point = event->scenePos();
            CustomItem *item = new CustomItem();
            QGraphicsTextItem* text_item=new QGraphicsTextItem("fuckyou");
            if(itemtype==1)
            item->setRect(point.x() - 75, point.y() - 25, 150, 50);
            //text_item->setPos(point.x()-75,point.y());
            //item->addToGroup(text_item);
            addItem(item);
            itemtype=0;
        }
        else if (event->button() == Qt::RightButton) {
            // 检测光标下是否有 item
            QGraphicsItem *itemToRemove = NULL;
            foreach(QGraphicsItem *item, items(event->scenePos())) {
                if (item->type() == QGraphicsItem::UserType + 1) {
                    itemToRemove = item;
                    break;
                }
            }
            // 从 Scene 上移除 item
            if (itemToRemove != NULL)
                removeItem(itemToRemove);
        }
    }
}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Custom scene moved.";
    QGraphicsScene::mouseMoveEvent(event);
}

void CustomScene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Backspace) {
        // 移除所有选中的 items
        qDebug() << "selected items " << selectedItems().size();
        while (!selectedItems().isEmpty()) {
            removeItem(selectedItems().front());
        }
    }
    else {
        QGraphicsScene::keyPressEvent(event);
    }
}

void CustomScene::setrect()
{
    itemtype=1;
    //this->setCursor(Qt::IBeamCursor);
}
