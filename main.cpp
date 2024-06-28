#include <QApplication>
#include <QGraphicsView>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QObject>
#include <QFont>
#include "customitem.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建 item
    CustomItem *pItem = new CustomItem();
    pItem->setRect(20, 20, 150, 50);

    // 将 item 添加至场景中
    CustomScene scene;
    scene.setSceneRect(0, 0, 400, 300);
    //scene.addItem(pItem);

    QPushButton* draw_rect_button=new QPushButton("Button");
    draw_rect_button->setFont(QFont("仿宋",18));
    draw_rect_button->setText("绘制");
    draw_rect_button->resize(160,80);
    QGraphicsView view;
    view.setFixedSize(1500, 1000);
    view.setGeometry(0,0,1000,1000);
    QPointF viewPoint(view.mapToScene(draw_rect_button->x(),draw_rect_button->y()));
    QGraphicsProxyWidget* proxy = scene.addWidget(draw_rect_button);
    proxy->setPos(-530,-320);
    QObject::connect(draw_rect_button, &QPushButton::pressed, &scene, &CustomScene::setrect);
    //QObject::connect(draw_rect_button, QPushButton::pressed, &scene, CustomScene::setrect);
    // 为视图设置场景

    view.setScene(&scene);
    view.show();
    view.resize(1000, 1000);

    return a.exec();
}
