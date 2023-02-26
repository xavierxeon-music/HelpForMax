#ifndef ComponentsModelH
#define ComponentsModelH

#include <Central.h>

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

private:
   void setModified(bool enabled) override;
   void rebuild();
   void update();

private:
   MainWindow* mainWindow;
};

#endif // NOT ComponentsModelH
