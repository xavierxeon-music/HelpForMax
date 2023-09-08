#ifndef OverviewGraphH
#define OverviewGraphH

#include <QWidget>

#include <QGraphicsScene>

#include "Tools/Central.h"

namespace Overview
{
   class Widget : public QWidget,
                  private FunctionHub
   {
      Q_OBJECT
   public:
      Widget(QWidget* parent, Central* central);

   private:
      struct Box
      {
         QGraphicsRectItem* rectItem = nullptr;
         QGraphicsSimpleTextItem* textItem = nullptr;
         int inletCount = 0;
         int outletCount = 0;
      };

      using IdMap = QMap<QString, Box>;

   private slots:
      void slotOpenInMax();

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
      QFont font;
   };
} // namespace Overview

#endif // NOT OverviewGraphH
