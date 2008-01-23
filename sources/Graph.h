#ifndef VERTEX 
#define VERTEX

#include <QWidget>

class QString;
class QBrush;
class QVector< class T>;
class QPoint;
class QDataStream;

class Vertex : public QWidget
{
    Q_OBJECT

	public:
		static const int Rad = 8;
		Vertex( QPoint , QString , QWidget* );
		void setTrueToolTip();
		void sendMessage( QString message );
		QString Name;
		QString fileLink;
		QBrush *brush;
		void setColor( QString color );
		friend QDataStream& operator <<(QDataStream& , Vertex*);

	protected:
		void draw( QPainter * );
		void paintEvent( QPaintEvent* );
		void mousePressEvent( QMouseEvent * );
		QString color;
};

#endif

#ifndef RIB
#define RIB
#include <QWidget>
class Rib : public QWidget
{
    Q_OBJECT 

    public:
		Rib( Vertex* , Vertex* , QWidget * );
		Vertex *from;
        Vertex *to;
		QBrush *brush;
		void setColor( QString color );
		QString color;
};

#endif


#ifndef GRAPH
#define GRAPH
#include <QWidget>

class Graph : public QWidget
{
    Q_OBJECT 

    public :
		static const int   Direct = 0;
		static const int NoDirect = 1;
		static const quint16 truetype = 0x0002;
		Graph( QWidget *parent );
		QString getName();
		void setName( QString );
		QString getCommand( QString );
		void sendMessage( QString message );
		QVector< Vertex* > nodes;
		QVector< Rib* > ribs;
		int findRib( Vertex* , Vertex* );
		int findVertex( Vertex* );
		friend QDataStream& operator <<(QDataStream& , Graph*);
		void readGraph( QDataStream& );

	protected:
		void mousePressEvent( QMouseEvent * );
		void draw( QPainter *painter );
		void paintEvent( QPaintEvent* );

	private:
		void deleteVertex( int );
		bool doesGraphConnected();
		bool haveNoCycle();
		void connectGraph();
		void treingGraph();
		int findMinLength( int from , int to , QVector<int> &father);
		int findRadius( int from );
		void importGraph( QString &Text );
		QVector<int> findEilerCycle();
		QString Name;
		int type;
		QVector<int> *selectedVertexes;
		QVector<QPoint> *sel_vect_point;
		QVector<int> *clip_from,*clip_to;


};

#endif
