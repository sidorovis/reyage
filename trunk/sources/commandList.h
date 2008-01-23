#ifndef COMMAND_LIST
#define COMMAND_LIST

#include "ui_command_list.h"
#include <QDialog>

class commandListDialog : public QDialog , public Ui::commandList
{
	Q_OBJECT

	public:
		commandListDialog( QStringList ,  QWidget* );
		QString selectedItem();
		QStringList command_list;

	signals:
		void enterPressed();

	protected:
		bool eventFilter( QObject *obj , QEvent *event );
};
#endif
