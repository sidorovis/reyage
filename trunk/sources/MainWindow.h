#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <QMainWindow>
#include "Graph.h"
class MainWidget;
class QTabWidget;
class QToolBar;
class QAction;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public :
		MainWindow();
		void setAnswer( QString );
		void setGraphName( QString );
		QObject* clipboard;
	
    public slots:
		void newGraph();
		void delGraph();
	
    private :
		QTabWidget *tabs;
		QToolBar *toolbar;
		QAction *newGraph_Action;
		QAction *delGraph_Action;
		QLabel *answer;
};

#endif

#ifndef MAIN_WIDGET
#define MAIN_WIDGET

#include <QWidget>
class MyLineEdit;
class QVBoxLayout;
class GraphFrame;
class QVector< class T >;
class QStringList;

class MainWidget : public QWidget
{
    Q_OBJECT

    public :
		MainWidget( QWidget *parent = 0);
		void messageToLine( QString ); 

    private slots:
		void commandSended();

    private :
		void answer( QString );
		MyLineEdit *command_line;
		QVBoxLayout *layout;
		Graph *graphWidget;

};

#endif


#ifndef COMMAND_EDIT
#define COMMAND_EDIT

#include <QLineEdit>

class MyLineEdit : public QLineEdit
{
	Q_OBJECT

	public:
		 MyLineEdit();

	signals:
		void enterPressed();
	
	protected:
		void keyPressEvent( QKeyEvent* );
		bool eventFilter( QObject *obj , QEvent *event );

	private:
		int command_number;
		QVector< QString *> log;
		QStringList command_dict;

};
#endif
