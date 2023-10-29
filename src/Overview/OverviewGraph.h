#ifndef OverviewGraphH
#define OverviewGraphH

#include <QGraphicsView>

#include <QGraphicsScene>

#include "Tools/Central.h"

namespace Overview
{
   class Graph : public QGraphicsView,
                 private FunctionHub
   {
      Q_OBJECT
   public:
      Graph(QWidget* parent, Central* central);

   private:
      struct Box
      {
         QGraphicsRectItem* rectItem = nullptr;
         QGraphicsSimpleTextItem* textItem = nullptr;
         int inletCount = 0;
         int outletCount = 0;
      };

      using IdMap = QMap<QString, Box>;

   private:
      void patchSelected(QString patchPath, QString key) override;

      IdMap makeObjects(const QJsonObject patcherObject);
      void moveItems(const IdMap& idMap);
      void makeLines(const QJsonObject patcherObject, const IdMap& idMap);

   private:
      Central* central;
      QGraphicsScene* scene;

      QPen blackPen;
      QBrush whiteBrush;
      QBrush grayBrush;
      QFont font;
   };
} // namespace Overview

#endif // NOT OverviewGraphH
