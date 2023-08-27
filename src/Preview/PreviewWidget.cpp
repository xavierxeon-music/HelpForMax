#include "PreviewWidget.h"

#include <QApplication>
#include <QDesktopServices>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QJsonArray>
#include <QJsonValue>
#include <QToolBar>
#include <QUrl>
#include <QVBoxLayout>

#include "Tools/JSONModel.h"

const QPen Preview::Widget::blackPen(QColor(0, 0, 0));
const QBrush Preview::Widget::whiteBrush(QColor(255, 255, 255));
const QFont Preview::Widget::font = []()
{
   QFont font = QApplication::font();
   font.setPixelSize(10);
   return font;
}();

Preview::Widget::Widget(QWidget* parent, Central* central)
   : QWidget(parent)
   , central(central)
   , scene(nullptr)
{
   QToolBar* toolBar = new QToolBar(this);
   toolBar->setMovable(false);

   toolBar->addAction(QIcon(":/OpenPackage.svg"), "Open In Max", this, &Widget::slotOpenInMax);

   scene = new QGraphicsScene(this);
   scene->setBackgroundBrush(Qt::lightGray);
   QGraphicsView* graphicsView = new QGraphicsView(scene, this);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);

   masterLayout->addWidget(toolBar);
   masterLayout->addWidget(graphicsView);
}

void Preview::Widget::slotOpenInMax()
{
   if (central->getCurrentKey().isEmpty())
      return;

   const Block::Item block = central->block();
   const QString patchPath = block.getPatchPath();

   QDesktopServices::openUrl(QUrl::fromLocalFile(patchPath));
}

void Preview::Widget::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(key)
   scene->clear();

   QJsonObject object = JSON::fromFile(patchPath);
   if (object.empty())
      return;

   const QJsonObject patcherObject = object["patcher"].toObject();

   IdMap idMap = makeObjects(patcherObject);
   moveItems(idMap);
   makeLines(patcherObject, idMap);
}

Preview::Widget::IdMap Preview::Widget::makeObjects(const QJsonObject patcherObject)
{
   static const QStringList skipList = {"comment", "panel"};
   const QJsonArray boxArray = patcherObject["boxes"].toArray();

   IdMap idMap;

   for (int index = 0; index < boxArray.size(); index++)
   {
      QJsonObject boxObject = boxArray.at(index).toObject();
      boxObject = boxObject["box"].toObject();

      if (!boxObject.contains("maxclass"))
         continue;

      const QString className = boxObject["maxclass"].toString();
      if (skipList.contains(className))
         continue;

      QString text = boxObject["text"].toString();
      const QString id = boxObject["id"].toString();

      QJsonArray patchRectData = boxObject["patching_rect"].toArray();
      QRectF patchRect;
      patchRect.setX(patchRectData[0].toDouble());
      patchRect.setY(patchRectData[1].toDouble());
      patchRect.setWidth(patchRectData[2].toDouble());
      patchRect.setHeight(patchRectData[3].toDouble());

      QGraphicsRectItem* rectItem = scene->addRect(QRectF(0, 0, patchRect.width(), patchRect.height()), blackPen, whiteBrush);
      rectItem->setPos(patchRect.x(), patchRect.y());

      if ("inlet" == className)
         text = "IN";
      else if ("outlet" == className)
         text = "OUT";

      QGraphicsSimpleTextItem* textItem = scene->addSimpleText(text);
      textItem->setPos(patchRect.x() + 5, patchRect.y() + 5);
      textItem->setFont(font);

      const int inletCount = boxObject["numinlets"].toInt();
      const int outletCount = boxObject["numoutlets"].toInt();

      idMap[id] = {rectItem, textItem, inletCount, outletCount};
   }

   return idMap;
}

void Preview::Widget::makeLines(const QJsonObject patcherObject, const IdMap& idMap)
{
   const QJsonArray lineArray = patcherObject["lines"].toArray();

   for (int index = 0; index < lineArray.size(); index++)
   {
      QJsonObject lineObject = lineArray.at(index).toObject();
      lineObject = lineObject["patchline"].toObject();

      const QJsonArray sourceArray = lineObject["source"].toArray();
      const QString sourceId = sourceArray.at(0).toString();
      if (!idMap.contains(sourceId))
         continue;

      const Box sourceBox = idMap[sourceId];
      const int sourceOffset = sourceArray.at(1).toInt();
      const QRectF sourceRect = QRectF(sourceBox.rectItem->pos().x(), sourceBox.rectItem->pos().y(), sourceBox.rectItem->rect().width(), sourceBox.rectItem->rect().height());

      const int outletDist = sourceRect.width() / sourceBox.outletCount;
      const int sourceX = 10 + sourceRect.x() + (outletDist * sourceOffset);
      const int sourceY = sourceRect.y() + sourceRect.height();

      const QJsonArray destArray = lineObject["destination"].toArray();
      const QString destId = destArray.at(0).toString();
      if (!idMap.contains(destId))
         continue;

      const Box destBox = idMap[destId];
      const int destOffset = destArray.at(1).toInt();
      const QRectF destRect = QRectF(destBox.rectItem->pos().x(), destBox.rectItem->pos().y(), destBox.rectItem->rect().width(), destBox.rectItem->rect().height());

      const int inletDist = destRect.width() / destBox.inletCount;
      const int destX = 10 + destRect.x() + (inletDist * destOffset);
      const int destY = destRect.y();

      scene->addLine(sourceX, sourceY, destX, destY, blackPen);
   }
}

void Preview::Widget::moveItems(const IdMap& idMap)
{
   static const int margin = 5;

   QPointF minPoint;

   for (Box box : idMap.values())
   {
      QPointF ipos = box.rectItem->pos();
      if (ipos.isNull())
         continue;

      if (minPoint.isNull())
      {
         minPoint = ipos;
         continue;
      }

      if (minPoint.x() > ipos.x())
         minPoint.setX(ipos.x());

      if (minPoint.y() > ipos.y())
         minPoint.setY(ipos.y());
   }

   if (minPoint.x() < margin)
      minPoint.setX(margin);

   if (minPoint.y() < margin)
      minPoint.setY(margin);

   for (Box box : idMap.values())
   {
      QPointF newPos = box.rectItem->pos() - minPoint;
      box.rectItem->setPos(newPos);
      box.textItem->setPos(newPos + QPointF(5, 5));
   }
}
