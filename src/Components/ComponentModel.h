#ifndef ComponentsModelH
#define ComponentsModelH

#include "Tools/Central.h"

#include <QStandardItemModel>

class ComponentsModel : public QStandardItemModel,
                        private FunctionHub
{
   Q_OBJECT
public:
   class Filtered;

public:
   ComponentsModel(MainWindow* mainWindow);

public:
   void patchSelected(QString patchPath, QString key) override;
   void update();
   QStandardItem* getItem(const int& row, const int& column = 0) const;

private:
   void setModified(bool enabled, QString key) override;
   void rebuild();

private:
   MainWindow* mainWindow;
};

#endif // NOT ComponentsModelH
