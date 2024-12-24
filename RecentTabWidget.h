#ifndef RecentTabWidgetH
#define RecentTabWidgetH

#include <QTabWidget>

#include <QFileInfo>
#include <QString>
#include <QWidget>

class RecentTabWidget : public QTabWidget
{
   Q_OBJECT

public:
   RecentTabWidget(QWidget* parent, const QString& settingsKey);
   ~RecentTabWidget();

public:
   QMenu* getRecentMenu();

protected:
   struct Entry
   {
      QString name;
      std::function<void()> openFunction;
   };

protected:
   const QStringList& getCurrrentFiles() const;
   void addCurrentFile(const QString& fileName);
   void removeCurrentFile(const QString& fileName);
   void addRecentFile(const QString& fileName);
   virtual Entry creatreEntry(const QFileInfo& fileInfo) = 0;

private:
   void populateRecentMenu();

private:
   QString settingsKey;
   QStringList currentFileList;
   QStringList recentFileList;
   QMenu* recentMenu;
};

#endif // NOT RecentTabWidgetH
