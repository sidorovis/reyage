#include "Graph.h"
#include "MainWindow.h"

#include <QString>
#include <QPainter>
#include <QDebug>
#include <QBrush>
#include <QQueue>
#include <QStack>
#include <QFileDialog>
#include <QDataStream>
#include <QTextStream>
#include <QMouseEvent>
#include <QMessageBox>
#include <QRegExp>
#include <cmath>
#include <QPair>

Vertex::Vertex( QPoint pos , QString _Name, QWidget *parent ) : QWidget( parent )
{
	setGeometry( pos.x()- Vertex::Rad , pos.y()-Vertex::Rad ,
				 Vertex::Rad * 2 + 1 , Vertex::Rad * 2 + 1 );
	Name = _Name;
	fileLink = "";
	setTrueToolTip();
	show();
	color = ",White";
	brush = new QBrush(Qt::color0, Qt::SolidPattern);
	setAttribute( Qt::WA_DeleteOnClose );	
}
void Vertex::setColor( QString _color )
{
	color = _color;
	if (color == ",Blue")
		brush = new QBrush( Qt::blue , Qt::SolidPattern );
	if (color == ",Black")
		brush = new QBrush( Qt::black , Qt::SolidPattern );
	if (color == ",Red")
		brush = new QBrush( Qt::red , Qt::SolidPattern );
	if (color == ",Green")
		brush = new QBrush( Qt::green , Qt::SolidPattern );
	if (color == ",Yellow")
		brush = new QBrush( Qt::yellow , Qt::SolidPattern );
	if (color == ",White")
		brush = new QBrush( Qt::white , Qt::SolidPattern );
	repaint();
}
void Vertex::setTrueToolTip()
{
	setToolTip( "Name : <b> "+Name+"</b><br> Link: <b>"+fileLink);
}
void Vertex::draw( QPainter *painter )
{
	painter->setBrush( *brush );
    painter->drawEllipse( 0 , 0 , Vertex::Rad * 2 , Vertex::Rad * 2 );
}
void Vertex::paintEvent( QPaintEvent * )
{
    QPainter painter( this );
    draw( &painter );
}
void Vertex::sendMessage( QString message )
{
	((Graph*)parentWidget())->sendMessage( message );
}
void Vertex::mousePressEvent( QMouseEvent * )
{ 
	QString Temp=QString(" :vertexNumber %1 ").
	arg(((Graph*)parentWidget())->findVertex( this ));
	sendMessage( Temp );
}
QDataStream& operator<<( QDataStream &stream , Vertex* vertex)
{
	stream << vertex->Name << quint16(vertex->pos().x()) << quint16(vertex->pos().y()) << (vertex->color) << vertex->fileLink;
	return stream;
}
//---------------------------------------------------------------------------------------------

Rib::Rib( Vertex *_from , Vertex *_to , QWidget *parent ) : QWidget( parent )
{
	setAttribute( Qt::WA_DeleteOnClose );
    from = _from;
	to = _to;
	color = ",White";
	brush = new QBrush(Qt::white, Qt::SolidPattern);
}
void Rib::setColor( QString _color )
{
	color = _color;
	if (color == ",Blue")
		brush = new QBrush( Qt::blue , Qt::SolidPattern );
	if (color == ",Black")
		brush = new QBrush( Qt::black , Qt::SolidPattern );
	if (color == ",Red")
		brush = new QBrush( Qt::red , Qt::SolidPattern );
	if (color == ",Green")
		brush = new QBrush( Qt::green , Qt::SolidPattern );
	if (color == ",Yellow")
		brush = new QBrush( Qt::yellow , Qt::SolidPattern );
	if (color == ",White")
		brush = new QBrush( Qt::white , Qt::SolidPattern );
}
//---------------------------------------------------------------------------------------------

Graph::Graph( QWidget *parent ) : QWidget( parent) 
{
	type = Graph::NoDirect;
	Name = "untitled";
	selectedVertexes = new QVector<int>;
	sel_vect_point = new QVector<QPoint>();
	clip_from = new QVector<int>;
	clip_to = new QVector<int>;
	setAttribute( Qt::WA_DeleteOnClose );
}
void Graph::mousePressEvent( QMouseEvent * event )
{ 
	QString Temp=QString(" :x %1 :y %2 "). arg(event->pos().x()).arg(event->pos().y());
	sendMessage( Temp );
}
QString Graph::getName()
{
	return Name;
}
void Graph::setName( QString _Name )
{
	Name = _Name;
	((MainWindow*)(parentWidget()->parentWidget()->parentWidget()->parentWidget()))->setGraphName( Name );
}

void Graph::draw( QPainter *painter )
{	
	if ( type == Graph::NoDirect )
		for ( int i = 0 ; i < ribs.size() ; i++ )
		{
			QPoint start = ribs[i]->from->pos()+QPoint(Vertex::Rad,Vertex::Rad);
			QPoint finish = ribs[i]->to->pos()+QPoint(Vertex::Rad,Vertex::Rad);
			if (start == finish)
			{
				painter->drawEllipse( 
					start.x() - Vertex::Rad - 2, start.y() - Vertex::Rad - 2,
					Vertex::Rad * 2 + 4, Vertex::Rad * 2 + 4);
			}
			else
			{
			 int  a = finish.y() - start.y();
			 int  b = - finish.x() + start.x();
			double alpha = atan2( a , b );
			double xsin = Vertex::Rad * sin( alpha ) / 5;
			double ycos = Vertex::Rad * cos( alpha ) / 5;
			QPointF points[4] = 
			{
				QPointF( (int)(start.x() + xsin ),(int)(start.y() +ycos)),
				QPointF( (int)(start.x() - xsin ),(int)(start.y() -ycos)),
				QPointF( (int)(finish.x()- xsin), (int)(finish.y()-ycos)),
				QPointF( (int)(finish.x()+ xsin), (int)(finish.y()+ycos)) 
			};
			painter->setBrush( *(ribs[i]->brush) );
			painter->drawPolygon( points , 4 );
			}
		}
	else
		for ( int i = 0 ; i < ribs.size() ; i++ )
		{
			QPoint start = ribs[i]->from->pos()+QPoint(Vertex::Rad,Vertex::Rad);
			QPoint finish = ribs[i]->to->pos()+QPoint(Vertex::Rad,Vertex::Rad);
			if (start == finish)
			{
				painter->drawEllipse( 
					start.x() - Vertex::Rad - 2, start.y() - Vertex::Rad - 2,
					Vertex::Rad * 2 + 4, Vertex::Rad * 2 + 4);
			}
			else
			{
			 int  a = finish.y() - start.y() ;
			 int  b = - finish.x() + start.x();
			double alpha = atan2( a , b );
			double xsin = Vertex::Rad * sin( alpha ) / 5;
			double ycos = Vertex::Rad * cos( alpha ) / 5;
			QPointF points[3] = 
			{
				QPointF( (int)(start.x() + xsin ),(int)(start.y() +ycos)),
				QPointF( (int)(start.x() - xsin ),(int)(start.y() -ycos)),
				QPointF( (int)(finish.x()), (int)(finish.y()))
			};
			painter->setBrush( *(ribs[i]->brush) );
			painter->drawPolygon( points , 3 );
			}
		}
}
QDataStream& operator<<( QDataStream &stream , Graph *graph )
{
	stream << quint16( Graph::truetype ) << quint16( graph->type ) << graph->Name << quint16(graph->nodes.size());
	for (int i = 0 ; i < graph->nodes.size() ; i++)
		stream << graph->nodes[i];
	stream << quint16(graph->ribs.size());
	for (int i = 0 ; i < graph->ribs.size() ; i++)
		stream 	<< quint16(graph->findVertex(graph->ribs[i]->from)) 
				<< quint16(graph->findVertex(graph->ribs[i]->to )) 
				<< (graph->ribs[i]->color);
	return stream;
}
void Graph::readGraph( QDataStream &stream )
{
	quint16 id , _type , sizeN;
	stream >> id >> _type >> Name >> sizeN;
/*	if ( id != truetype)
	{
		return ;
	}
*/	setName( Name );
	type = _type ;
	for (int i = 0 ; i < (int)sizeN ; i++)
	{
		QString name , color , link;
		qint16 x,y;
		stream >> name >> x >> y >> color >> link;
		x+=0;y+=2;
		nodes.append( new Vertex( *(new QPoint(x,y)),name,this));
		nodes.last()->setColor( color );
		nodes.last()->fileLink = link;
		nodes.last()->setTrueToolTip();
	}
	stream >> sizeN;
	for (int i = 0 ; i < (int)sizeN ; i++)
	{
		QString color , name , link;
		quint16 from , to;
		stream >> from >> to >> color;
		ribs.append( new Rib(nodes[from] ,  nodes[ to ] , this));
		ribs.last()->setColor( color);
	}
	repaint();
}

int Graph::findRib( Vertex* from , Vertex* to)
{
	if (type == Graph::Direct)
	{
		for (int i = 0 ; i < ribs.size() ; i++)
			if ( ribs[i]->from == from && ribs[i]->to == to )
			{
				return i;
			}
	}
	else
	{
		for (int i = 0 ; i < ribs.size() ; i++)
			if ( (from == ribs[i]->from && ribs[i]->to == to) || 
					(from == ribs[i]->to && ribs[i]->from == to))
			{
				return i;
			}
	}
	return -1;
}
int Graph::findVertex( Vertex* f )
{
	for (int i = 0 ; i < nodes.size() ; i ++)
		if ( f == nodes[i] ) 
			return i;
	return -1;
}
void Graph::paintEvent( QPaintEvent * )
{
	QPainter painter( this );
	draw( &painter );
}

void Graph::treingGraph()
{
	bool v[ nodes.size() ];
	int c = 1;
	int father[ nodes.size() ];
	for (int i = 0 ; i < nodes.size() ; i++)
	{
		v[i] = false;
		father[i] = -1;
	}
	v[0] = true;
	QQueue< int > queue;
	queue.push_back(0);
	while (c < nodes.size())
	{
		for (int i = 0 ; i < nodes.size() ; i++)
			if ( !v[i] && findRib(nodes[queue.head()],nodes[i]) != -1 )
			{
				v[i] = true;
				queue.push_back(i);
				father[i] = queue.head();
				c++;
			}
		queue.pop_front();
	}
	for (int i = 0 ; i < nodes.size() ; i++)
		for (int u = 0 ; u < nodes.size() ; u++)
			if (findRib (nodes[i],nodes[u])!=-1)
			{
				if (father[i] == u || father[u] == i)
				{
					// Do nothing
				}
				else
				{
					int ribNum = findRib (nodes[i],nodes[u]);
					ribs.remove( ribNum );
				}
			}
}
void Graph::connectGraph()
{
	QQueue< int > queue;
	for (int y = 0 ; y < nodes.size() ; y++)
	{
		queue.clear();
		bool met[ nodes.size() ];
		for (int i = 0 ; i < nodes.size() ; i++)
			met[ i ] = false;
		met[ y ] = true;
		queue.push_back( y );
		while (!queue.isEmpty())
		{
			for (int i = 0 ; i < ribs.size(); i ++)
			{
				if ( findVertex(ribs[i]->from) == queue.head() &&
					!met[findVertex(ribs[i]->to)] )
				{
					queue.push_back( findVertex(ribs[i]->to) );
					met [findVertex(ribs[i]->to) ] = true;
				}
				if ( type != Direct && 
					findVertex(ribs[i]->to) == queue.head() &&
					!met[findVertex(ribs[i]->from)] )
				{
					queue.push_back( findVertex(ribs[i]->from) );
					met [findVertex(ribs[i]->from) ] = true;
				}
			}
			queue.pop_front();
		}
		for (int i = 0 ; i < nodes.size() ; i++ )
			if ( !met[i] )
			{	
				ribs.append( new Rib(nodes[y] ,  nodes[ i ] , this));
				i = nodes.size();y--;
			}
	}
}
bool Graph::doesGraphConnected()
{
	QQueue< int > queue;
	bool met[ nodes.size() ];
	for (int i = 0 ; i < nodes.size() ; i++)
		met[ i ] = false;
	met[ 0 ] = true;
	queue.push_back( 0 );
	while (!queue.isEmpty())
	{
		for (int i = 0 ; i < ribs.size(); i ++)
		{
			if ( findVertex(ribs[i]->from) == queue.head() &&
				!met[findVertex(ribs[i]->to)] )
			{
				queue.push_back( findVertex(ribs[i]->to) );
				met [findVertex(ribs[i]->to) ] = true;
			}
			if ( type != Direct && 
				findVertex(ribs[i]->to) == queue.head() &&
				!met[findVertex(ribs[i]->from)] )
			{
				queue.push_back( findVertex(ribs[i]->from) );
				met [findVertex(ribs[i]->from) ] = true;
			}
		}
		queue.pop_front();
	}
	int count = 0;
	for (int i = 0 ; i < nodes.size() ; i++ )
		if (met[i]) 
			count++;
	if (count == nodes.size())
		return true;
	else
		return false;
}

bool Graph::haveNoCycle()
{
	QQueue< int > queue;
	int met[ nodes.size() ];
	for (int i = 0 ; i < nodes.size() ; i++)
		met[ i ] = -1;
	met[ 0 ] = 0;

	queue.push_back( 0 );

	while ( !queue.isEmpty() )
	{
		for (int i = 0 ; i < ribs.size(); i ++)
		{
			if ( findVertex(ribs[i]->from) == queue.head() )
			{
				if ( met[findVertex(ribs[i]->to)] == -1 )
				{
					queue.push_back( findVertex(ribs[i]->to) );
 					met [findVertex(ribs[i]->to) ] = queue.head();
				}
				else
				if ( met[findVertex(ribs[i]->from)]!=findVertex(ribs[i]->to))
					return false;
			}
			if ( type != Direct && findVertex(ribs[i]->to) == queue.head() )
			{
				if ( met[findVertex(ribs[i]->from)] == -1 )
				{
					queue.push_back( findVertex(ribs[i]->from) );
 					met [findVertex(ribs[i]->from) ] = queue.head();
				}
				else
				if ( met[findVertex(ribs[i]->to)] != findVertex(ribs[i]->from))
					return false;
			}
		}
		queue.pop_front();
	}
	return true;
}
int Graph::findMinLength( int from , int to , QVector<int> &father)
{
	QQueue<int> queue;
	QVector<int> way2it(nodes.size(),10000000);
	way2it[ from ] = 0;
	father[ from ] = from;
	queue.push_back( from );
	while ( !queue.isEmpty() )
	{
		for (int i = 0 ; i < ribs.size() ; i++)
		{
			if ( findVertex(ribs[i]->from) == queue.head() &&
				way2it[findVertex(ribs[i]->to)] > way2it[findVertex(ribs[i]->from)] + 1 )
			{
				father[ findVertex(ribs[i]->to) ] = queue.head();
				queue.push_back( findVertex(ribs[i]->to));
				way2it[findVertex(ribs[i]->to)] = way2it[findVertex(ribs[i]->from)] + 1 ;
			}
			if ( type == NoDirect && findVertex(ribs[i]->to) == queue.head() &&
				way2it[findVertex(ribs[i]->from)] > way2it[findVertex(ribs[i]->to)] + 1 )
			{
				father[ findVertex(ribs[i]->from) ] = queue.head();
				queue.push_back( findVertex(ribs[i]->from));
				way2it[findVertex(ribs[i]->from)] = way2it[findVertex(ribs[i]->to)] + 1;
			}
		}
		queue.pop_front();
	}
	return way2it[to];
}
int Graph::findRadius( int from )
{
	QVector<int> father(nodes.size(),0);
	int maks_min = 0;
	for (int u = 0 ; u < nodes.size() ; u ++ )
	{
		int temp = findMinLength( from , u , father );
		if ( temp > maks_min )
			maks_min = temp;
	}
	return maks_min;
}

void Graph::deleteVertex( int number)
{
	Vertex* temp = nodes[ number ];
	for (int i = 0 ; i < ribs.size() ; i++)
		if ( ribs[i]->from == temp || ribs[i]->to == temp)
		{
			ribs.remove( i );
			i--;
		}
	nodes.remove( number );
	temp->close();
}
QVector<int> Graph::findEilerCycle()
{
	QStack<int> way;
	QStack<int> cur_rib;
	QVector<bool> metka(ribs.size() , false);
	int kol = 0;
	way.push( 0 );
	cur_rib.push( -1 );
	while ( !way.isEmpty() )
	{
		bool come_in = false;
		if ( kol == ribs.size() && way.first() == way.last() )
		{
			QVector<int> ans;
			for (int i = 0 ; i < way.size() ; i ++ )
				ans << way[i];
			return ans;
		}
		for ( int i = cur_rib.top() + 1 ; i < ribs.size() ; i++ )
		{
			if ( !metka[i] && findVertex(ribs[i]->from) == way.top() )
			{
				come_in = true;
				metka[i] = true;
				kol ++;
				cur_rib.pop();
				cur_rib.push( i );
				way.push( findVertex( ribs[i]->to ) );
				cur_rib.push( -1 );
				break;
			}
			if ( !metka[i] && type == NoDirect && findVertex(ribs[i]->to) == way.top() )
			{
				come_in = true;
				metka[i] = true;
				kol ++;
				cur_rib.pop();
				cur_rib.push( i );
				way.push( findVertex( ribs[i]->from ) );
				cur_rib.push( -1 );
				break;
			}
		}
		if ( !come_in )
		{
			kol --;
			way.pop();
			cur_rib.pop();
			metka[ cur_rib.top() ] = false;
		}
	}
	return QVector<int>(0,0);
}


void Graph::importGraph( QString &Text )
{
	int dotdot = Text.indexOf(":");
	int dotcomma_1 = Text.indexOf(";",dotdot+1);
	int dotcomma_2 = Text.indexOf(";",dotcomma_1+1);
	int dot = Text.indexOf(".",dotcomma_2+1);
	if ( !(dotdot < dotcomma_1 && dotcomma_1 < dotcomma_2 && dotcomma_2 < dot) )
	{
		Text = "You have problem with importing file. <b> Try watch ':' ';' ',' symbols.</b>";
		return;
	}
	QRegExp rx("^[a-zA-Z0-9_!]+$");
	QStringList name_list = 
		Text.mid ( 0 , dotdot ).remove(QRegExp("[ \n]")).split(" ").filter(rx);
	if ( name_list.size() != 1 )
	{
		Text = "Problem with getting graph name. <b> I can't recognize ':' </b>";
		return;
	}
	QStringList type_list = 
		Text.mid ( dotdot + 1 , dotcomma_1 - 1 - (dotdot + 1) ).remove("\n").split(" ").filter(rx);

	if ( type_list.size() != 1 || 
		( type_list[0] != "ORIENT" && type_list[0] != "UNORIENT" ) )
	{
		Text = "Problem with getting graph type. Cant find <b>ORIENT</b> or <b>UNORIENT</b>.";
		return;
	}
	QStringList node_list = 
		Text.mid( dotcomma_1 + 1 , dotcomma_2 - (dotcomma_1 + 1) )
			.remove(QRegExp("([a-zA-Z0-9]+[ \n\t\r]+[a-zA-Z0-9]+)"))
			.remove(QRegExp("[ \n\r\t]")).split(",").filter(rx);
	QStringList ribs_list =
		Text.mid( dotcomma_2 + 1 , dot - (dotcomma_2 + 1) )
			.remove(QRegExp("([a-zA-Z0-9]+[ \n\t\r][a-zA-Z0-9]+)"))
			.remove(QRegExp("[ \n\r\t]"))
			.replace("->",",").split(",").filter(rx);
	
	for (int i = 0 ; i < node_list.size() ; i++)
		for( int u = i + 1 ; u < node_list.size() ; u++)
			if (node_list[i] == node_list[u] )
			{
				Text = "Sorry but nodes names are not monosemantic. Try set one name: <b>"+node_list[i]+"</b>";
				return;
			}
	if ( ribs_list.size() % 2 != 0 )
	{
		Text = "Sorry but ribs record is incorrect.";
		return;
	}
	while ( !nodes.isEmpty() )
		deleteVertex( 0 );
	QMap<QString,Vertex*> names;
	int regul = 70;
	int x = regul;
 	int y = regul;
	for (int i = 0 ; i < node_list.size() ; i ++ )
	{
		nodes << (new Vertex(QPoint(x,y),node_list[i],this));
		names.insert( node_list[i] , nodes.last() );
		if ( x < size().width() - regul )
			x += regul;
		else
		{
			x = regul;
			y += regul;
		}
	}
	for (int i = 0 ; i < ribs_list.size() ; i+=2 )
	{
		if ( names.find(ribs_list[i]) == names.end() )
		{
			Text = "There is no such vertex with name : <b>"+ribs_list[i]+"</b>.";
			while ( !nodes.isEmpty() )
				deleteVertex( 0 );
			return;
		}
		if ( names.find(ribs_list[i+1]) == names.end())
		{
			Text = "There is no such vertex with name : <b>"+ribs_list[i+1]+"</b>.";
			while ( !nodes.isEmpty() )
				deleteVertex( 0 );
			return;
		}
	}
	Name = name_list[0];
	if ( type_list[0] == "ORIENT" )
		type = Direct;
	else
		type = NoDirect;
	for (int i = 0 ; i < ribs_list.size() ; i+=2 )
	{
		int from = findVertex( names[ribs_list[ i ]] ) ;
		int to   = findVertex( names[ribs_list[ i + 1 ]]) ;
		ribs.append( new Rib(nodes[from] ,  nodes[ to ] , this));
	}
	repaint();
}

void Graph::sendMessage( QString message )
{
	((MainWidget*)parentWidget())->messageToLine( message );
}
// TODO work with command !!!

QString Graph::getCommand( QString command )
{
	QRegExp rx("([a-zA-Z0-9]+)|_|;|:");
	QStringList commands = command.split(" ").filter(rx);
	QString answer="";

	if ( commands[0] == ";setGraphType")
	{
		int directPlace = commands.indexOf(":Direct");
		int nodirectPlace = commands.indexOf(":NoDirect");

		if ( directPlace != -1 )
		{
			type = Graph::Direct;
			answer = " Graph set to Direct";
			repaint();
		}
		else
		if ( nodirectPlace != -1 )
		{
			type = Graph::NoDirect;
			answer = " Graph set to No Direct";
			repaint();
		}
		else
			answer = "Use :Direct or :NoDirect like parameter.";
	}
	if ( commands[0] == ";saveGraph" )
	{
		QString fileName = QFileDialog::getSaveFileName(this,
									tr("Save Graph"), ".",
									tr("Graph files (*.grp)"));
		if (fileName != "")
		{
			if ( !fileName.endsWith(".grp") )
 				fileName+=".grp";
			QFile file( fileName );
			file.open(QIODevice::WriteOnly);
			QDataStream out ( &file );

			out << this;

			repaint();
			file.close();
			answer = " Graph is now saved to file. ";
		}
		else
			answer = " Graph not saved";
	}
	if ( commands[0] == ";loadGraph")
	{
		QString fileName = QFileDialog::getOpenFileName(this,
									tr("Open Graph"), ".",
									tr("Graph files (*.grp)"));
		if ( fileName != "")
		{
			if ( !fileName.endsWith(".grp") )
 				fileName+=".grp";
			QFile file( fileName );
			file.open(QIODevice::ReadOnly);
			QDataStream in ( &file );
			while ( nodes.size() > 0 )
			{
				deleteVertex( 0 );
			}
			readGraph( in );
			file.close();
			answer = " Graph loaded from file. ";
		}
		else
			answer = " You don't choose any file to load.";
	}
	if ( commands[0] == ";exportGraph" )
	{
		QString fileName = QFileDialog::getSaveFileName(this,
									tr("Save Graph"), ".",
									tr("Txt files (*.txt)"));
		if ( fileName != "")
		{
			if ( !fileName.endsWith(".txt") )
 				fileName+=".txt";
			QFile file( fileName );
			file.open(QIODevice::WriteOnly);
			QTextStream in ( &file );
			in << Name << " : " << endl;
			if ( type == Direct )
				in << "ORIENT ; " << endl;
			else
				in << "UNORIENT ; " << endl;
			for (int i = 0 ; i < nodes.size() - 1 ; i++ )
				in << nodes[i]->Name << " , ";
			if ( nodes.size() >= 1)
				in << nodes[ nodes.size() - 1 ]->Name;
			in << " ; " << endl;

			for (int i = 0 ; i < ribs.size() - 1 ; i++ )
				in <<ribs[i]->from->Name << " -> "<<ribs[i]->to->Name<<" , ";
			if ( ribs.size() >= 1 )
				in << ribs[ ribs.size() - 1 ]->from->Name << " -> " 
				<< ribs[ ribs.size() - 1 ]->to->Name;
			in << " . " << endl;
			file.close();
			answer = "I export graph to "+fileName;
		}
		else
			answer = "You don't choose file to export";
	}
	if ( commands[0] == ";importGraph" )
	{
		QString fileName = QFileDialog::getOpenFileName(this,
									tr("Load Graph"), ".",
									tr("Txt files (*.txt)"));
		if ( fileName != "")
		{
			if ( !fileName.endsWith(".txt") )
 				fileName+=".txt";
			QFile file( fileName );
			file.open(QIODevice::ReadOnly);
			QTextStream in ( &file );
			QString Text = in.readAll();
			file.close();
			
			importGraph( Text );
			QMessageBox::about(this, ("ImportText"), Text);
			
			answer = " Trying to import file.";
			
		}
			answer = "Yu don't chose file to import.";
	}
	if ( commands[0] == ";repaintGraph" )
	{
		repaint();
		answer = " repainted ... ";
	}
	if ( commands[0] == ";addVertex" )
	{
		int xPlace = commands.indexOf(":x");
		int yPlace = commands.indexOf(":y");
		
		bool okX , okY;
		if ( xPlace > -1 && xPlace+1 < commands.size() && 
				yPlace > -1 && yPlace+1 < commands.size())
		{
			int xCoord = commands[ xPlace + 1 ].toInt( &okX , 10 );
			int yCoord = commands[ yPlace + 1 ].toInt( &okY , 10 );
			if ( okX && okY)
			{
				nodes.append( new Vertex( *(new QPoint(xCoord,yCoord)),"unknown_node_name"+QString("%1").
				arg(nodes.size()*100000+ribs.size()),this));
				nodes.last()->repaint();
				answer = " Vertex created. ";
			}
			else
				answer = " Use it with :x <int> :y <int>.";
		}
		else 
			answer ="Bad parametres. Use with :x <coordinate> :y <coordinate>";
	}
	if ( commands[0] == ";delVertex" )
	{
		int numberPlace = commands.indexOf(":vertexNumber");
		bool ok;
		if ( numberPlace > -1 && numberPlace + 1 < commands.size() )
		{
			int number = commands[ numberPlace + 1 ].toInt( &ok , 10 );
			if ( ok && number < nodes.size() )
			{
				deleteVertex( number );
				repaint();
				answer = " Vertex deleted";
			}
			else
				answer = " No such vertex or <int value> not corrected.";
		}
		else
			answer = "No such parametres like :vertexNumber <int value>";
	}
	if ( commands[0] == ";setVertexName" )
	{
		int numberPlace = commands.indexOf(":vertexNumber");
		int namePlace = commands.indexOf(":name");
		bool ok;
		if ( numberPlace > -1 && numberPlace+1 < commands.size() &&
			namePlace > -1 && namePlace+1 < commands.size() )
		{
			int number = commands[ numberPlace + 1 ].toInt( &ok , 10 );
			QString name = commands[ namePlace + 1 ];
			if ( ok && number < nodes.size()  && number > -1 )
			{
				nodes[ number ]->Name = name;
				nodes[ number ]->setTrueToolTip();
				answer = " We set "+name+" like vertex Name.";
			}
			else
				answer = " It is bad <int value>";
		}
		else
			answer = " Use with parametres :vertexNumber <int_value> :name <string value>.";
	}
	if ( commands[0] == ";setVertexLink" )
	{
		int numberPlace = commands.indexOf(":vertexNumber");
		bool ok;
		if ( numberPlace > -1 && numberPlace+1 < commands.size() )
		{
			int number = commands[ numberPlace + 1 ].toInt( &ok , 10 );
			if ( ok && number < nodes.size()  && number > -1 )
			{
				QString fileName = QFileDialog::getOpenFileName(this,
					tr("Load Graph"), ".",
					tr("Txt files (*.txt)"));
				if ( fileName != "")
				{
					nodes[ number ]->fileLink = fileName;
					answer = "Fikle link is set succesfully";
					nodes[ number ]->setTrueToolTip();
				}
				else
					answer = " Yu don't choose file link.";
			}
			else
				answer = " It is bad <int value>";
		}
		else
			answer = " Use with parametr :vertexNumber <int_value>";
	}
	if ( commands[0] == ";setVertexColor" )
	{
		int numberPlace = commands.indexOf(":vertexNumber");
		int colorPlace = commands.indexOf(":color");
		if ( numberPlace > -1 && numberPlace+1 < commands.size() &&
			colorPlace > -1 && colorPlace+1 < commands.size())
		{
			bool okN;
			int number = commands[ numberPlace + 1 ].toInt( &okN , 10 );
			QString color = commands[ colorPlace + 1 ];
			if ( okN && number < nodes.size() && number > -1  )
			{
				nodes[ number ]->setColor( color );
				answer = "Color is installed";
			}
			else
				answer = "Use right <int value> parameter.";
		}
		else
			answer = "Use type parametres :vertexNumber <int value> :color ,<Color Type>";
	}
	if ( commands[0] == ";moveVertexTo" )
	{
		int numberPlace = commands.indexOf(":vertexNumber");
		int xPlace = commands.indexOf(":x");
		int yPlace = commands.indexOf(":y");
		if ( numberPlace > -1 && numberPlace+1 < commands.size() &&
			xPlace > -1 && xPlace+1 < commands.size() &&
			yPlace > -1 && yPlace+1 < commands.size() )
		{
			bool okN, okX, okY;
			int number = commands[ numberPlace + 1 ].toInt( &okN , 10 );
			int x = commands[ xPlace + 1 ].toInt( &okX , 10 );
			int y = commands[ yPlace + 1 ].toInt( &okY , 10 );
			if ( okN && okX && okY  && number < nodes.size() )
			{
				nodes[ number ]->move( x - Vertex::Rad , y - Vertex::Rad );
				repaint();
				answer = " You moved vertex to new place";
			}
			else
				answer = " Sorry but one of <int_value> is wrong.";
		}
		else
			answer = " Use next style :vertexNumber <int value> :x <int> :y <int>";
	}
	if ( commands[0] == ";addRib" )
	{
		int fromPlace = commands.indexOf(":vertexNumber");
		int toPlace = commands.indexOf(":vertexNumber" , fromPlace + 1);
		if ( fromPlace > -1 && fromPlace+1 < commands.size() &&
				toPlace > -1 && toPlace+1 < commands.size())
		{
			bool ok1,ok2;
			int from =  commands[ fromPlace + 1 ].toInt( &ok1 , 10 );
			int to =  commands[ toPlace + 1 ].toInt( &ok2 , 10 );
			if ( ok1 && ok2 && 
				from < nodes.size() && to<nodes.size() && from > -1 && to > -1 )
			{
				ribs.append( new Rib(nodes[from] ,  nodes[ to ] , this));
				repaint();
				answer = "We added new Rib.";
			}
			else
				answer = "Sorry but some of int parametres is wrong.";
		}
		else
			answer = "Use this type like parametres :vertexNumber <start Int> :vertexNumber <finish Int> .";
	}
	if ( commands[0] == ";delRib" )
	{
		int fromPlace = commands.indexOf(":vertexNumber");
		int toPlace = commands.indexOf(":vertexNumber" , fromPlace + 1);
		if ( fromPlace > -1 && fromPlace+1 < commands.size() &&
				toPlace > -1 && toPlace+1 < commands.size() )
		{
			bool ok1,ok2;
			int from =  commands[ fromPlace + 1 ].toInt( &ok1 , 10 );
			int to =  commands[ toPlace + 1 ].toInt( &ok2 , 10 );
			if ( ok1 && ok2 && 
				from < nodes.size() && to<nodes.size() && from > -1 && to > -1 )
			{
				int ribNum = findRib( nodes[from] , nodes[to] );
				if ( ribNum > -1 )
				{
					ribs.remove( ribNum );
					repaint();
					answer = " Yu delete rib";
				}
				else
					answer = "Sorry but I can't find this rib.";
			}
			else
				answer = "Some of you <int value> parametr is wrong.";
		}
		else
			answer = "Yu need to try another parametres :vertexNumber <start Vertex Int> :vertexNumber <finish vertex Int>";
	}
	if ( commands[0] == ";setRibColor" )
	{
		int fromPlace = commands.indexOf(":vertexNumber");
		int toPlace = commands.indexOf(":vertexNumber" , fromPlace + 1);
		int colorPlace = commands.indexOf(":color");
		if ( fromPlace > -1 && fromPlace+1 < commands.size() &&
				toPlace > -1 && toPlace+1 < commands.size() &&
				colorPlace > -1 && colorPlace < commands.size() )
		{
			bool ok1,ok2;
			int from =  commands[ fromPlace + 1 ].toInt( &ok1 , 10 );
			int to =  commands[ toPlace + 1 ].toInt( &ok2 , 10 );
			QString color = commands[ colorPlace + 1 ];
			if ( ok1 && ok2 && 
				from < nodes.size() && to<nodes.size() && from > -1 && to > -1 )
			{
				int ribNum = findRib( nodes[from] , nodes[to] );
				if ( ribNum > -1 )
				{
					ribs[ ribNum ]->setColor( color );
					repaint();
					answer = "Color is changed.";
				}
				else
					answer = "I can't find this rib :\\";
			}
			else
				answer = " One of <int value> is wrong.";
		}
		else
			answer = "Use next scheme :vertexNumber <Int from Vertex> :vertexNumber <Int to Vertex> :color .<Color Param>";
	}
	if ( commands[0] == ";showVertexRibCount" )
	{
		QMessageBox::about(this, ("About Graph"),
			("<h2>"+ Name +"</h2>"
			"<p> Vertex count: </p>"+ QString("%1").arg( nodes.size() ) +
			"<p> Rib count: </p> "+ QString("%1").arg( ribs.size() )));
		answer = " I show you ribs and nodes counts.";
	}
	if ( commands[0] == ";showVertexDegree" )
	{
		int vertexNumberPlace = commands.indexOf(":vertexNumber");
		if ( vertexNumberPlace > -1 && vertexNumberPlace < commands.size() )
		{
			bool ok;
			int index = commands[ vertexNumberPlace + 1].toInt( &ok , 10 );
			if ( !ok )
				answer = " Sorry your <int value> do not llok like correct.";
			else
			{
				int count = 0;
				for (int i = 0 ; i < ribs.size() ; i++)
					if ( ribs[i]->from == nodes[ index ] || ribs[i]->to == nodes[ index ] )
					count ++;
				QMessageBox::about(this, ("About Vertex"),
					("<h2>"+ nodes[ index ]->Name +"</h2>"
					"<b> Degree : </b>"+ QString("%1").arg( count )));
				answer = " I show you information about one Vertex.";
			}
		}
		else
		{
			QVector<int> counts( nodes.size() );
			for (int index = 0 ; index < nodes.size() ; index++)
				for (int i = 0 ; i < ribs.size() ; i++)
					if ( ribs[i]->from == nodes[ index ] || ribs[i]->to == nodes[ index ] )
					counts[ index ] ++;
			QString message = "";
			for (int i = 0 ; i < counts.size() ; i++ )
				message += "<b> " + QString("%1").arg( i ) + ": </b>" +
					QString("%1").arg( counts[i] )+ " <br>";
			QMessageBox::about(this, ("About Vertexes"), message);
			answer = " I show you information about all vertexes.";
		}
	}
	if ( commands[0] == ";showMatrix" )
	{
		int regime = commands.indexOf(":Coherent");
		if ( regime != -1 )
		{
			int a[  nodes.size() ][  nodes.size() ];
			for (int i = 0 ; i < nodes.size() ; i++)
				for (int u = 0 ; u < nodes.size() ; u++)
					a[i][u]=0;
			for (int i = 0 ; i < ribs.size() ; i++)
			{
				int from = findVertex( ribs[i]->from );
				int to = findVertex( ribs[i]->to );
				a[ from ][ to ] = 1;
					if ( type != Direct)
						a[ to ][ from ] ++ ;
			}
			QString message = "";
			for (int i = 0 ; i < nodes.size() ; i++ )
			{
				for (int u = 0 ; u < nodes.size() ; u++)
				{
					message += QString("%1 ").arg(a[i][u]);
				}
				message += "<br>";
			}
			QMessageBox::about(this, ("About Vertexes"), message);
			answer = " Thisis coherent matrix of this graph.";
		}
		else
		if ( commands.indexOf(":Overlapping") != -1 )
		{
			int a[  nodes.size() ][  nodes.size() ];
			for (int i = 0 ; i < nodes.size() ; i++)
				for (int u = 0 ; u < nodes.size() ; u++)
					a[i][u]=0;
			for (int i = 0 ; i < ribs.size() ; i++)
			{
				int from = findVertex( ribs[i]->from );
				int to = findVertex( ribs[i]->to );
				a[ from ][ to ] ++ ;
				if ( type != Direct)
					a[ to ][ from ] ++ ;
			}
			QString message = "";
			for (int i = 0 ; i < nodes.size() ; i++ )
			{
				for (int u = 0 ; u < nodes.size() ; u++)
				{
					message += QString("%1 ").arg(a[i][u]);
				}
				message += "<br>";
			}
			QMessageBox::about(this, ("About Vertexes"), message);
			answer = "This is overlapping matrix of this graph";
		}
		else
		{
			answer = "Try use one of parametres ':Overlapping' ':Coherent'";
		}
	}
	if ( commands[0] == ";doesGraphConnected")
	{
		if (nodes.size() == 0 )
			answer = " In this graph I can't see one vertex. It's connected.";
		else
		if ( type == Direct)
			answer = "This graph is Direct. Testing on connected is not available.";
		else
		if ( doesGraphConnected())
		{
			answer = " This graph is Connected.";
		}
		else
			answer = " This graph consist from several parts.";
	}
	if ( commands[0] == ";doesGraphTree")
	{
		if (nodes.size() == 0 )
			answer = " It's tree.";
		else
		if ( type == Direct)
			answer = "This graph is Direct. It's hard to say about him like about tree or not tree.";
		else
		if ( doesGraphConnected() && haveNoCycle())
		{
			answer = " This graph can be tree.";
		}
		else
			answer = " Not Tree.";
	}
	if ( commands[0] == ";doesGraphFull" )
	{
		bool full = true;
		for (int i = 0 ; i < nodes.size() ; i++)
		{
			bool a[ nodes.size() ];
			for (int u = 0 ; u < nodes.size() ; u++ )
				a[u] = false;
			
			for (int u = 0 ; u < nodes.size() ; u++ )
				if ( findRib( nodes[i] , nodes[u] ) !=  -1 )
					a[u] = true;
			for (int u = 0 ; u < nodes.size(); u++)
				if ( u!=i && !a[u])
					full = false;
		}
		if (full)
			answer = " Graph is full ";
		else
			answer = " Graph don't full";
	}
	if ( commands[0] == ";doesGraphEiler")
	{
		QVector<int> way = findEilerCycle( );
		if ( way.size() == 0 && nodes.size() != 0)
		{
			answer = " There is no eiler cycle.";
		}
		else 
		if ( way.size() != 0 )
		{
			answer = "This graph ascribe to Eiler Graphs.";
		}
		else
		{
			answer = "There is no nodes in graph.";
		}
	}
	if ( commands[0] == ";findWays" )
	{
		int fromPlace = commands.indexOf(":vertexNumber");
		int toPlace = commands.indexOf(":vertexNumber" , fromPlace + 1 );
		if ( fromPlace > -1 && toPlace > -1 && toPlace+1 < commands.size() )
		{
			bool ok1 , ok2 ;
			int from = commands[ fromPlace+1 ].toInt( &ok1 , 10 );
			int to = commands[ toPlace+1 ].toInt( &ok2 , 10 );
			if ( ok1 && ok2 &&
				from > -1 && from < nodes.size() &&
				to > -1 && to < nodes.size() )
			{
				QStack<int> stack;
				QStack<int> search;
				QVector<bool> metka( nodes.size() , false );
				metka[ from ] = true;
				stack.push( from );
				search.push( 0 );
				QStringList Answer;
				while ( !stack.isEmpty() )
				{
					bool goes = false;
					if ( to == findVertex( nodes[ stack.top() ]) )
					{
						QString way = "";
						for (int i = 0 ; i < stack.size() ; i++)
							way = way +" " + QString(
					 nodes[stack[i]]->Name +  "[%1]").arg(stack[i]);
						Answer << way;
					}
					else
					for ( int i = search.top() ; i < ribs.size(); i ++ )
					{
						if ( findVertex(ribs[i]->from) == stack.top() && 
								!metka[findVertex(ribs[i]->to)])
						{
							search.pop();
							search.push( i + 1 );
							stack.push( findVertex( ribs[i]->to ) );
							metka[ findVertex( ribs[i]->to ) ] = true;
							search.push( 0 );
							goes = true;
							i = ribs.size();continue;
						}
						if ( type == NoDirect && 
							findVertex(ribs[i]->to) == stack.top() &&
								!metka[findVertex(ribs[i]->from)])
						{
							search.pop();
							search.push( i + 1 );
							stack.push( findVertex(ribs[i]->from) );
							metka[ findVertex(ribs[i]->from) ] = true;
							search.push( 0 );
							goes = true;
							i = ribs.size();continue;
						}
					}
					if ( !goes )
					{
						search.pop();
						metka[ stack.pop() ] = false;
					}
				}
				QString message = "";
				for (int i = 0 ; i < Answer.size() ; i++ )
					message += Answer[i] + "<br>";
				QMessageBox::about(this, ("Ways"), message);
				answer = "Algorithm of ways finding was working succesfully.";
			}
			else
				answer = "Some of your <int value> is wrong";
		}
		else
			answer = "Yu need to add parametres :vertexNumber <int From> :vertexNumber <int To> .";
	}
	if ( commands[0] == ";findMinWay" )
	{
		int fromPlace = commands.indexOf(":vertexNumber");
		int toPlace = commands.indexOf(":vertexNumber" , fromPlace + 1 );
		if ( fromPlace > -1 && toPlace > -1 && toPlace+1 < commands.size() )
		{
			bool ok1 , ok2 ;
			int from = commands[ fromPlace+1 ].toInt( &ok1 , 10 );
			int to = commands[ toPlace+1 ].toInt( &ok2 , 10 );
			if ( ok1 && ok2 &&
				from > -1 && from < nodes.size() &&
				to > -1 && to < nodes.size() )
			{
				QVector<int> father(nodes.size(),0);
				findMinLength( from , to , father);
				int i = to;
				QStringList Answer;
				while ( father [ i ] != i)
				{
					Answer << QString(nodes[i]->Name + "[%1] ").arg(i);
					i = father[ i ];
				}
				Answer << QString(nodes[from]->Name + "[%1] ").arg(from);
				QString message;
				for (int i = Answer.size() - 1 ; i > -1 ; i -- )
				{
					message += Answer[i];
				}
				QMessageBox::about(this, ("Minimal Way"), message);
				answer = QString("You see Min way from %1 to %2").arg(from).arg(to);
			}
			else
				answer = "Your <int parametres is\\are wrong.";
		}
		else
			answer = " Please enter :vertexNumber <int_from> :vertexNumber <int_to> like parametres.";
	}
	if ( commands[0] == ";findLength" )
	{
		int fromPlace = commands.indexOf(":vertexNumber");
		int toPlace = commands.indexOf(":vertexNumber" , fromPlace + 1 );
		if ( fromPlace > -1 && toPlace > -1 && toPlace+1 < commands.size() )
		{
			bool ok1 , ok2 ;
			int from = commands[ fromPlace+1 ].toInt( &ok1 , 10 );
			int to = commands[ toPlace+1 ].toInt( &ok2 , 10 );
			if ( ok1 && ok2 &&
				from > -1 && from < nodes.size() &&
				to > -1 && to < nodes.size() )
			{
				QVector<int> father(nodes.size(),0);
				answer = QString("From %1 To %2 = %3").arg(from).arg(to).arg(findMinLength(from , to , father));
			}
			else
				answer = "Your <int parametres is\\are wrong.";
		}
		else
			answer = " Please enter :vertexNumber <int_from> :vertexNumber <int_to> like parametres.";
	}
	if ( commands[0] == ";findDiametr" )
	{
		int maks_min = 0;
		for (int i = 0 ; i < nodes.size() ; i ++ )
		{
			int temp = findRadius( i );
			if (temp > maks_min)
				maks_min = temp;
		}
		answer = QString(" Diametr to this graph = %1").arg( maks_min );
	}

	if ( commands[0] == ";findRadius" )
	{
		int fromPlace = commands.indexOf(":vertexNumber");
		if ( fromPlace > -1 && fromPlace + 1 < commands.size() )
		{
			bool ok ;
			int from = commands[ fromPlace+1 ].toInt( &ok , 10 );
			if ( ok && from > -1 && from < nodes.size() )
			{
				int maks_min = findRadius( from );
				answer = QString(" Radius to %1 vertex = %2").arg(from).arg( maks_min );
			}
			else
				answer = " Yu write wrong <int> parametr";
		}
		else
			answer = "Use :vertexNumber <int_value> like parametr.";
	}
	if ( commands[0] == ";findCenter")
	{
		int center = -1;
		int center_value = 10000000;
		for (int i = 0 ; i < nodes.size() ; i++)
		{
			int temp = findRadius( i );
			if ( temp < center_value )
			{
				center_value = temp;
				center = i;
			}
		}
		if ( center == -1 )
			answer = "No Vertex in Graph.";
		else
			answer = QString("Center: %1").arg(center);
	}
	if ( commands[0] == ";findEilerCycle" )
	{
		QVector<int> way = findEilerCycle( );
		if ( way.size() == 0 && nodes.size() != 0)
		{
			answer = " There is no eiler cycle.";
		}
		else 
		if ( way.size() != 0 )
		{
			QString Answer;
			for (int i = 0 ; i < way.size() ; i++ )
				Answer = Answer + nodes[way[i]]->Name+QString("[%1] ").arg(way[i]);
			QMessageBox::about(this, ("Ways"), Answer);
			answer = "I show to yu eiler cycle.";
		}
		else
		{
			answer = "There is no nodes in graph.";
		}
	}
	if ( commands[0] == ";selectVertex")
	{
		int fromPlace = commands.indexOf(":vertexNumber");
		if ( fromPlace > -1 && fromPlace + 1 < commands.size() )
		{
			bool ok ;
			int from = commands[ fromPlace+1 ].toInt( &ok , 10 );
			if ( ok && from > -1 && from < nodes.size() )
			{
				selectedVertexes->push_back( from );
				sel_vect_point->push_back( nodes[from]->pos() );
				answer="Vertex succesfully selected";
			}
			else
				answer = " You write wrong <int> parameter.";
		}
		else
			answer = "You is with :vertexNumber <intValue> parametres.";
	}
	if ( commands[0] == ";delSelection")
	{
		selectedVertexes->clear();
		sel_vect_point->clear();
		answer="Selection deleted;";
	}
	if ( commands[0] == ";copy")
	{
		MainWidget *tempWidget = (MainWidget*)parentWidget();
		MainWindow *tempWindow = ((MainWindow*)(tempWidget->parentWidget()->parentWidget()->parentWidget()));
		clip_from->clear();
		clip_to->clear();
		for (int i = 0 ; i < selectedVertexes->size() ; i++)
			for (int u = i ; u < selectedVertexes->size() ; u++)
				if ( findRib(
								nodes[ selectedVertexes->at(i) ],
								nodes[ selectedVertexes->at(u) ]
							) != -1 )
					{
						clip_from->push_back(i);
						clip_to->push_back(u);
					}
		QVector<QObject*>* clip = new QVector<QObject*>();
		tempWindow->clipboard = (QObject*)clip;
		clip->push_back( (QObject*)selectedVertexes );
		clip->push_back( (QObject*)sel_vect_point );
		clip->push_back( (QObject*)clip_from );
		clip->push_back( (QObject*)clip_to );
		answer="Selection copied in clipboard;";
	}
	if ( commands[0] == ";paste")
	{
		MainWidget *tempWidget = (MainWidget*)parentWidget();
		MainWindow *tempWindow = ((MainWindow*)(tempWidget->parentWidget()->parentWidget()->parentWidget()));
		if ( tempWindow->clipboard != NULL )
		{
			QVector<QObject*>* clip = (QVector<QObject*>*)tempWindow->clipboard;
			QVector<int>*vert = (QVector<int>*)clip->at(0);
			QVector<QPoint>* points = (QVector<QPoint>*)clip->at(1);
			QVector<int>*from = (QVector<int>*)clip->at(2);
			QVector<int>*to = (QVector<int>*)clip->at(3);
			int current_size = nodes.size();
			for (int i = 0 ; i < vert->size() ; i++)
			{
				nodes.append( 
					new Vertex( (points->at(i)),
								"unknown_node_name"+QString("%1").
				arg(nodes.size()*100000+ribs.size()),this));
				nodes.last()->repaint();
			}
			for (int i = 0 ; i < from->size() ; i++)
			{
				ribs.append( new Rib(nodes[current_size+from->at(i)] ,  nodes[ current_size+to->at(i) ] , this));
			}
				repaint();
			answer = "Pasted from clipboard. Beware Clipboard are cleaned";
			tempWindow->clipboard = NULL;
		}
		else
			answer = "Sorry but at first paste something in clipboard";
	}
	if ( commands[0] == ";tofullGraph")
	{
		for (int i = 0 ; i < nodes.size() ; i++ )
			for (int u = 0 ; u < nodes.size() ; u++ )
				if ( findRib( nodes[i] , nodes[u] ) == -1 )
				{
					ribs.append( new Rib(nodes[i] ,  nodes[u] , this));
				}
		repaint();
		answer = "Now graph is Full.";
	}
	if ( commands[0] == ";toconnectedGraph" )
	{
		if (!doesGraphConnected())
		{
			connectGraph();
			repaint();
			answer = "Now Graph Connected";
		}
		else
			answer = "Graph is connected allright";
	}
	if ( commands[0] == ";totree")
	{
		if (type == NoDirect)
		{	if (doesGraphConnected())
			{
				treingGraph();
				repaint();
				answer = "Now Graph is tree";
			}
			else
				answer = "Graph is not connected. Make it connected first (;2connectedGraph)";
		}
		else
			answer = "Sorry but operation onnly for No Direct Graphs";
	}
	if (commands[0] == ";findVertex")
	{
		int fromPlace = commands.indexOf(":name");
		if ( fromPlace > -1 && fromPlace + 1 < commands.size() )
		{
			QString from = commands[ fromPlace+1 ];
			QString Answer = "";
			for (int i = 0 ; i < nodes.size() ; i++)
				if ( nodes[i]->Name.contains(from, Qt::CaseInsensitive))
				{
					Answer += nodes[i]->Name + "<br>";
				}
			QMessageBox::about(this, ("You find: "+from), (Answer));
			answer="Find completed.";
		}
		else
			answer = "Use like ;findVertex :name <string_value of what to search>";
	}
	if (answer == "")
		answer = " Wrong command ... ";
	return answer;
}
