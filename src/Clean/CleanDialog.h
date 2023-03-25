#ifndef CleanDialogH
#define CleanDialogH

#include <QDialog>

namespace Clean
{
   class Model;

   class Dialog : public QDialog
   {
      Q_OBJECT

   public:
      Dialog(QWidget* parent, Model* model);
   };
} // namespace Clean

#endif // NOT CleanDialogH
