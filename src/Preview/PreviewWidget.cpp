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

   static const QPen blackPen(QColor(0, 0, 0));
   static const QBrush whiteBrush(QColor(255, 255, 255));

   static const QStringList skipList = {"comment"};
   static const QFont font = [&]()
   {
      QFont font = QApplication::font();
      font.setPixelSize(10);
      return font;
   }();

   const QJsonObject patcherObject = object["patcher"].toObject();
   const QJsonArray boxArray = patcherObject["boxes"].toArray();
   const QJsonArray lineArray = patcherObject["lines"].toArray();

   struct Box
   {
      QRectF rect;
      int inletCount = 0;
      int outletCount = 0;
   };

   using IdMap = QMap<QString, Box>;
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

      const int inletCount = boxObject["numinlets"].toInt();
      const int outletCount = boxObject["numoutlets"].toInt();

      idMap[id] = {patchRect, inletCount, outletCount};

      QGraphicsRectItem* rectItem = scene->addRect(patchRect, blackPen, whiteBrush);
      Q_UNUSED(rectItem)

      if ("inlet" == className)
         text = "IN";
      else if ("outlet" == className)
         text = "OUT";

      QGraphicsSimpleTextItem* textItem = scene->addSimpleText(text);
      textItem->setPos(patchRect.x() + 5, patchRect.y() + 5);
      textItem->setFont(font);
   }

   // find objects connected to inlets
   for (int index = 0; index < lineArray.size(); index++)
   {
      QJsonObject lineObject = lineArray.at(index).toObject();
      lineObject = lineObject["patchline"].toObject();

      const QJsonArray sourceArray = lineObject["source"].toArray();
      const QString sourceId = sourceArray.at(0).toString();
      const Box sourceBox = idMap[sourceId];
      const int sourceOffset = sourceArray.at(1).toInt();

      const int outletDist = sourceBox.rect.width() / sourceBox.outletCount;
      const int sourceX = 10 + sourceBox.rect.x() + (outletDist * sourceOffset);
      const int sourceY = sourceBox.rect.y() + sourceBox.rect.height();

      const QJsonArray destArray = lineObject["destination"].toArray();
      const QString destId = destArray.at(0).toString();
      const Box destBox = idMap[destId];
      const int destOffset = destArray.at(1).toInt();

      const int inletDist = destBox.rect.width() / destBox.inletCount;
      const int destX = 10 + destBox.rect.x() + (inletDist * destOffset);
      const int destY = destBox.rect.y();

      scene->addLine(sourceX, sourceY, destX, destY, blackPen);
   }
}
