#include "MainWindow.h"
#include "commandList.h"

#include <QTabWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QToolBar>
#include <QLabel>
#include <QStatusBar>
#include <QAction>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QRegExp>
#include <QToolTip>
#include <QListWidget>
#include <QDialog>

MainWindow::MainWindow()
{
    tabs = new QTabWidget;

	answer = new QLabel( " Status bar. Will help yu work with Editor..." );
	statusBar()->addWidget( answer );
    toolbar = addToolBar(tr("Actions"));

    toolbar -> setMovable( false );
    newGraph_Action = new QAction(tr(" &New Graph "),this);
	newGraph_Action->setShortcut(tr("Ctrl+N"));
    connect( newGraph_Action , SIGNAL( triggered() ) , this , SLOT( newGraph() ));
    delGraph_Action = new QAction(tr(" &Delete Graph "),this);
	delGraph_Action->setShortcut(tr("Ctrl+D"));
    connect( delGraph_Action , SIGNAL( triggered() ) , this , SLOT( delGraph() ));

    delGraph_Action->setDisabled( true );

    toolbar->addAction( newGraph_Action );
    toolbar->addAction( delGraph_Action );
    
    setCentralWidget( tabs );
    resize( 800 , 600 );
}

void MainWindow::newGraph()
{
	MainWidget *Temp = new MainWidget;
	tabs->addTab( Temp , "untitled" );
	if ( tabs->count() > 0 )
	{
		delGraph_Action->setDisabled( false );
	}
	tabs->setCurrentWidget( Temp );
}

void MainWindow::delGraph()
{
    if ( tabs->count() > 0 )
    {
		tabs->removeTab( tabs->currentIndex() );
    }
    if ( tabs->count() == 0) 
    {
		delGraph_Action->setDisabled( true );
    }
}

void MainWindow::setGraphName( QString Name)
{
	tabs->setTabText( tabs->currentIndex(), Name );
}
void MainWindow::setAnswer( QString message)
{
	answer->setText( message );
}
//------------------------------------------------------------------------

MainWidget::MainWidget( QWidget *parent ) : QWidget( parent )
{
    command_line = new MyLineEdit;
    graphWidget = new Graph(this);

    layout = new QVBoxLayout;

    layout->addWidget( graphWidget );
    layout->addWidget( command_line );
    setLayout ( layout );
    show();

	connect( command_line , SIGNAL( enterPressed() ) , this , SLOT( commandSended()) );

}
void MainWidget::commandSended()
{
	QString Temp = command_line->text();
	QRegExp rx("([a-zA-Z0-9]+)|,|;|:");
	QStringList words = Temp.split(" ").filter(rx);
	if ( words.size() < 1 )
	{
		answer("Sorry but it was wrong command.");
	}
	else
	{
		if ( words[0] == ";setGraphName") 
		{
			int namePlace = words.indexOf(":name");
			if ( namePlace != -1 && (namePlace + 1 < words.size()) )
			{
				((MainWindow*)(parentWidget()->parentWidget()->parentWidget()))->setGraphName( words[2] );
				graphWidget->setName( words[ namePlace+1 ] );
				answer( " You set " +words[ namePlace+1 ]+ " like graph name." );
			}
			else
				answer ( " Add to command parameter :name <string value> " );
		}
		else
		{
			Temp = graphWidget->getCommand( Temp );
			answer( Temp );
		}
	}
	command_line->setText( "" );
}
void MainWidget::answer( QString message )
{
	((MainWindow*)(parentWidget()->parentWidget()->parentWidget()))->setAnswer(message );
}
void MainWidget::messageToLine ( QString message )
{
	if ( command_line->text() != "" )
	{
		message = command_line->text() + message;
		command_line->setText( message );
	}
}
//-----------------------------------------------------------------

MyLineEdit::MyLineEdit()
{
	command_number = 0;
	this->installEventFilter( this ); 

	command_dict 
//	Graph Commands
	<< ";setGraphName" << ";setGraphType" << ";saveGraph" << ";loadGraph" <<
	";exportGraph" << ";importGraph" << 
	";repaintGraph" << ";selectVertex" << ";delSelection" <<
	";copy" << ";paste" <<
	";tofullGraph" << ";toconnectedGraph" << ";totree"
//	Vertex Commands
	<< ";addVertex" << ";delVertex" << ";setVertexName" << ";setVertexColor"
	<< ";setVertexLink" << ";moveVertexTo" << ";findVertex"
//	Rib Commands
	<< ";addRib" << ";delRib" << ";setRibColor"
//	Show Commands
	<< ";showVertexRibCount" << ";showVertexDegree" << ";showMatrix" <<
	";doesGraphConnected" << ";doesGraphTree" << ";doesGraphFull" << 
	";doesGraphEiler" << 
// Other Information
	";findWays" << ";findMinWay" << ";findLength" << 
	";findDiametr" << ";findRadius" << ";findCenter" << ";findEilerCycle" <<
// Parametres
	":Direct" << ":NoDirect"
	<< ":x" << ":y" << ":vertexNumber" << ":color" << ":name"
	<< ":Coherent" << ":Overlapping"
// Colors
	<< ",Blue" << ",White" << ",Yellow" << ",Red" << ",Black" << ",Green"
;
	command_dict.sort();

}

void MyLineEdit::keyPressEvent( QKeyEvent* event)
{
	if ( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
	{
		QString* Temp = new QString(text());
		log.append( Temp );
		command_number = log.size();
		emit enterPressed();
	}
	if ( event->key() == Qt::Key_Up )
	{
		if ( command_number > 0 )
		{
			command_number-- ;
			setText( *log[command_number] );
			selectAll();
		}
	}
	if ( event->key() == Qt::Key_Down )
	{
		if ( command_number < log.size() )
		{
			command_number++;
			if ( log.size() == command_number )
				setText("");
			else
				setText( *log[command_number] );
			selectAll();
		}
	}
	QLineEdit::keyPressEvent( event );
}
bool MyLineEdit::eventFilter( QObject *obj , QEvent *event )
{
	if ( (obj == this) && (event->type() == QEvent::KeyPress) )
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		if ( keyEvent->key() == Qt::Key_Tab)
		{
			QStringList Temp(text().split(" "));
			QString regExpString = "^("+Temp.last()+")([a-zA-Z]|_)+";
			QRegExp rx(regExpString);

			QStringList filtered = command_dict.filter( rx );
			if ( filtered.size() == 0 )
			{
				
			}
			else
			if ( filtered.size() == 1 )
			{
				QString newCommand;
				if ( Temp.size() > 1 )
					newCommand = Temp[0];
				for (int i = 1 ; i < Temp.size() - 1 ; i++)
					newCommand += " "+Temp[i];
				if ( Temp.size() > 1 )
					newCommand+=" ";
				newCommand += filtered[0]+" ";
				setText( newCommand );
			}
			else
			{
				commandListDialog *commandList = new commandListDialog( filtered ,this);
				if ( commandList->exec() )
				{
					QString newCommand;
					if ( Temp.size() > 1 )
						newCommand = Temp[0];
					for (int i = 1 ; i < Temp.size() - 1 ; i++)
						newCommand += " "+Temp[i];
					if ( Temp.size() > 1 )
						newCommand+=" ";
					if ( commandList->selectedItem() != NULL )
						newCommand += commandList->selectedItem()+" ";
					setText( newCommand );
				}
			}
			return true;
		}
	}
	return QLineEdit::eventFilter(obj, event);
}
