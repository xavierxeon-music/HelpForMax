#ifndef SelectViewH
#define SelectViewH

#include "Tools/Central.h"
#include "Tools/AbstractItemTreeView.h"

class SelectModel;
class Persona;

class SelectView : public Abstract::ItemTreeView,
                   private FunctionHub
{
   Q_OBJECT
public:
   SelectView(MainWindow* mainWindow, SelectModel* model);

private:
   void clicked(ModelItem* item) override;
   void doubleClicked(ModelItem* item) override;

private:
   MainWindow* mainWindow;
};

#endif // NOT SelectViewH
