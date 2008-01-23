#include "commandList.h"

#include <QKeyEvent>
#include <QDebug>
commandListDialog::commandListDialog( QStringList list , QWidget *parent ) : QDialog( parent )
{
	command_list = list;
	setupUi( this );
	connect( this , SIGNAL(enterPressed()) , buttonBox, SIGNAL( accepted()) );
	for (int i = 0 ; i < list.size() ; i++)
		listWidget->addItem( list[i] );
	listWidget->installEventFilter( this ); 
}

QString commandListDialog::selectedItem()
{
	if (listWidget->selectedItems().size()==0)
		return "";
	listWidget->setCurrentItem( listWidget->selectedItems()[0] );
	return command_list [ listWidget->currentRow() ];
}

bool commandListDialog::eventFilter( QObject *obj , QEvent *event )
{
	if ( (obj == listWidget ) && (event->type() == QEvent::KeyPress) )
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		if ( keyEvent->key() == Qt::Key_Return && !(listWidget->selectedItems().empty()))
		{
			emit enterPressed();
		}
	}
	return QDialog::eventFilter( obj , event );
}
