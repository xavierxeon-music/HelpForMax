#ifndef ComponentsModelH
#define ComponentsModelH

#include <Central.h>
#include <QStandardItemModel>

class ComponentsModel : public QStandardItemModel,
                        private FunctionHub
{
   Q_OBJECT

public:
   ComponentsModel(MainWindow* mainWindow);

private:
   void patchSelected(QString patchPath, QString key) override;
   void rebuild();

private:
   MainWindow* mainWindow;
};

#endif // NOT ComponentsModelH
