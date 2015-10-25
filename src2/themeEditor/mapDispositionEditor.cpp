/****************************************************************
**
** Attal : Lords of Doom
**
** mapDispositionEditor.cpp
** Edit map disposition
**
** Version : $Id: mapDispositionEditor.cpp,v 1.17 2008/04/10 20:24:39 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 22/07/2004
**
** Licence :
**	This program is free software; you can redistribute it and/or modify
**   	it under the terms of the GNU General Public License as published by
**     	the Free Software Foundation; either version 2, or (at your option)
**      any later version.
**
**	This program is distributed in the hope that it will be useful,
** 	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
****************************************************************/

#include "mapDispositionEditor.h"

// generic include files
// include files for QT
#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QMouseEvent>
#include <QPoint>
#include <QPixmap>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/genericMapDisposition.h"
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"

#include "libClient/gui.h"
#include "libClient/imageTheme.h"

extern QString IMAGE_PATH;

//
// ----- MapDispositionEditor -----
//

MapDispositionEditor::MapDispositionEditor( QWidget * parent, const char * name )
: QWidget( parent )
{
	setWindowTitle( QString ( name ));

	_mapDisposition = NULL;

	QButtonGroup * group = new QButtonGroup( this );
	QGroupBox * box = new QGroupBox( this );

	QPushButton * pbFree = new QPushButton();
	pbFree->setText( tr( "Free" ) );
	pbFree->setCheckable( true );
	pbFree->setChecked( true );
	FIXEDSIZE( pbFree );

	QPushButton * pbOccupied = new QPushButton();
	pbOccupied->setText( tr( "Occupied" ) );
	pbOccupied->setCheckable( true );
	FIXEDSIZE( pbOccupied );

	QPushButton * pbDoor = new QPushButton();
	pbDoor->setText( tr( "Door" ) );
	pbDoor->setCheckable( true );
	FIXEDSIZE( pbDoor );

	group->setExclusive( true );
	group->addButton( pbFree, 0 );
	group->addButton( pbOccupied, 1 );
	group->addButton( pbDoor, 2 );

	QVBoxLayout * layV1 = new QVBoxLayout();
	layV1->setMargin( 5 );
	layV1->addStretch( 1 );
	layV1->addWidget( pbFree );
	layV1->addStretch( 1 );
	layV1->addWidget( pbOccupied );
	layV1->addStretch( 1 );
	layV1->addWidget( pbDoor );
	layV1->addStretch( 1 );
	layV1->activate();
	box->setLayout( layV1 );
	
	QLabel * labCol = new QLabel( this );
	labCol->setText( tr( "Columns:" ) );
	FIXEDSIZE( labCol );
	
	QLabel * labRow = new QLabel( this );
	labRow->setText( tr( "Rows:" ) );
	FIXEDSIZE( labRow );
	
	QVBoxLayout * layV2 = new QVBoxLayout();
	layV2->setMargin( 5 );
	layV2->addStretch( 1 );
	layV2->addWidget( labCol );
	layV2->addStretch( 1 );
	layV2->addWidget( labRow );
	layV2->addStretch( 1 );
	
	_spinCol = new QSpinBox( this );
	_spinCol->setMinimum( 1 );
	_spinCol->setValue( 5 );
	FIXEDSIZE( _spinCol );
	
	_spinRow = new QSpinBox( this );
	_spinRow->setMinimum( 1 );
	_spinRow->setValue( 5 );
	FIXEDSIZE( _spinRow );
	
	
	QVBoxLayout * layV3 = new QVBoxLayout();
	layV3->setMargin( 5 );
	layV3->addStretch( 1 );
	layV3->addWidget( _spinCol );
	layV3->addStretch( 1 );
	layV3->addWidget( _spinRow );
	layV3->addStretch( 1 );

	_dispositionCanvas = new MapDispositionCanvas( this );
	_dispositionCanvasView = new MapDispositionCanvasView( _dispositionCanvas, this );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( box );
	layout->addStretch( 1 );
	layout->addWidget( _dispositionCanvasView );
	layout->addLayout( layV2 );
	layout->addLayout( layV3 );
	layout->addStretch( 1 );
	layout->activate();

	connect( group, SIGNAL( buttonClicked( int ) ), SLOT( slot_clicked( int ) ) );
	connect( _spinCol, SIGNAL( valueChanged( int ) ), SLOT( slot_resize_col( int ) ) );
	connect( _spinRow, SIGNAL( valueChanged( int ) ), SLOT( slot_resize_row( int ) ) );
}

void MapDispositionEditor::slot_clicked( int id )
{
	_dispositionCanvas->setCurrentType( ( GenericMapDisposition::DispositionType )id );
}

void MapDispositionEditor::slot_resize_row(int value)
{
	dispoResize( true, value );

}

void MapDispositionEditor::slot_resize_col(int value)
{
	dispoResize( false, value );
}

void MapDispositionEditor::dispoResize( bool isrow , int value )
{
		int row, col;
		if( isrow ) {
			row = value;
			col = _mapDisposition->getWidth();
		} else {
			col = value;
			row = _mapDisposition->getHeight();
		}

		_dispositionCanvas->clear();
		_mapDisposition->resize( row, col );
		_dispositionCanvas->init( _mapDisposition, _pixmapPath );
		_dispositionCanvas->setSceneRect(0,0, _mapDisposition->getWidth() * DataTheme.tiles.getWidth() , _mapDisposition->getHeight() * DataTheme.tiles.getHeight() );
		_dispositionCanvasView->setMinimumSize( _mapDisposition->getWidth() * DataTheme.tiles.getWidth() , _mapDisposition->getHeight() * DataTheme.tiles.getHeight() );
		//_dispositionCanvas->reinit();
		update();

}

void MapDispositionEditor::init( GenericMapDisposition * mapDisposition, const QString & pixmapPath )
{
	int width, height;
		
	_mapDisposition = mapDisposition;
	_pixmapPath = pixmapPath;
	_dispositionCanvas->init( mapDisposition, pixmapPath );
	if( mapDisposition ) {
		_dispositionCanvas->setSceneRect( 0,0, mapDisposition->getWidth() * DataTheme.tiles.getWidth(), mapDisposition->getHeight() * DataTheme.tiles.getHeight() );
		width = mapDisposition->getWidth();
		height = mapDisposition->getHeight();
		/* we need to use width and height on this point cause using directly mapDisposition->getWidth() 
		*  and mapDisposition->getHeigth() is not possible because _spinCol and _spinRow 
		*  call (by a signal) a function that change the value of mapDisposition 
		* in this case _spinCol change the value of mapDisposition->getHeigth() before _spinRow
		*/
		_spinCol->setValue( width );
		_spinRow->setValue( height );
		_dispositionCanvasView->resize( mapDisposition->getWidth() * DataTheme.tiles.getWidth() + 10, mapDisposition->getHeight() * DataTheme.tiles.getHeight()  + 10 );
		setMinimumHeight( height * DataTheme.tiles.getHeight() + 10 );
		_dispositionCanvas->reinit();
	}
	update();
}

void MapDispositionEditor::clear()
{
	logEE( "Not yet implemented." );
}

//
// ----- MapDispositionCanvas -----
//
MapDispositionCanvas::MapDispositionCanvas( QObject * parent, const char *  /* name */ )
: QGraphicsScene( parent  )
{
	setSceneRect(0,0, 150, 150 );
	setBackgroundBrush(QBrush(Qt::white) );

	//_listPixmaps.setAutoDelete( true );

	_listPixmaps.append( QPixmap( *ImageTheme.getWidgetPixmap( EDIT_FREE ) ) );
	_listPixmaps.append( QPixmap( *ImageTheme.getWidgetPixmap( EDIT_OCC ) ) );
	_listPixmaps.append( QPixmap( *ImageTheme.getWidgetPixmap( EDIT_DOOR ) ) );

	_pixmaps = new QList<QPixmap>( _listPixmaps );
	_dispositionSprites = 0;
	_disposition = 0;
	_background = 0;
	
	_currentType = GenericMapDisposition::FREE;
}

MapDispositionCanvas::~MapDispositionCanvas()
{
	clear();

	if( _pixmaps ) {
		delete _pixmaps;
	}
	
	/*
	while( ! _listPixmaps.isEmpty() ) {
            delete _listPixmaps.takeFirst();
	}

	}*/
}

void MapDispositionCanvas::clear()
{
	uint i, j;

	if( _background ) {
		delete _background;
		_background = 0;
	}
	
	if( _disposition && _dispositionSprites ) {
		for( i = 0; i < _height; i++ ) {
			for( j = 0; j < _width; j++ ) {
				delete _dispositionSprites[ i ][ j ];
				_dispositionSprites[ i ][ j ] = 0;
			}
			delete [] _dispositionSprites[ i ];
		}
		delete [] _dispositionSprites;
		_dispositionSprites = 0;
	}
	
	update();
}


void MapDispositionCanvas::init( GenericMapDisposition * mapDisposition, const QString & pixmapPath )
{
	uint i, j;
	
	clear();
	
	_disposition = mapDisposition;
	_height = _disposition->getHeight();
	_width = _disposition->getWidth();
	_background = new DispositionSprite( this );
	_background->setBackground( pixmapPath );
	_background->setPos( 0, _height * DataTheme.tiles.getHeight() - _background->boundingRect().height() );


	_dispositionSprites = new DispositionSprite ** [ _height ];
	for( i = 0; i < _height; i++ ) {
		_dispositionSprites[ i ] = new DispositionSprite * [ _width ];
		for( j = 0; j < _width; j++ ) {
			_dispositionSprites[ i ][ j ] = new DispositionSprite( this );
			_dispositionSprites[ i ][ j ]->setPos( j * DataTheme.tiles.getWidth(), i * DataTheme.tiles.getHeight() );
			_dispositionSprites[ i ][ j ]->setSpriteType( _disposition->getDisposition( i, j ) );
		}
	}
	update();
}

void MapDispositionCanvas::reinit()
{
	for( uint i = 0; i < _height; i++ ) {
		for( uint j = 0; j < _width; j++ ) {
			_dispositionSprites[ i ][ j ]->setSpriteType( _disposition->getDisposition( i, j ) );
		}
	}
	
	update();
}

void MapDispositionCanvas::changeCell( uint row, uint col )
{
	_disposition->setDisposition( row, col, _currentType );
	_dispositionSprites[ row ][ col ]->setSpriteType( _currentType );
	reinit();
}


//
// ----- MapDispositionCanvasView -----
//

MapDispositionCanvasView::MapDispositionCanvasView( MapDispositionCanvas * canvas , QWidget * parent, const char * /*name */, Qt::WFlags/* f*/ )
: QGraphicsView( (QGraphicsScene*)canvas, parent )
{
	setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	viewport()->setMouseTracking( true );
	_canvas = canvas;
}

void MapDispositionCanvasView::mouseReleaseEvent( QMouseEvent * e )
{
	/* view coordinate could be different from scene coordinate */
	QPointF pos = mapToScene( e->pos());

	if (scene()->sceneRect().contains( pos )) { 
		if( e->button() == Qt::LeftButton ) {
			uint col = (uint)pos.x() / DataTheme.tiles.getWidth();
			uint row = (uint)pos.y() / DataTheme.tiles.getHeight();
			_canvas->changeCell( row, col );
		}
	}
}

//
// ----- DispositionSprite -----
//

const int DispositionSprite::RTTI = Type;

DispositionSprite::DispositionSprite( MapDispositionCanvas * canvas )
: AttalSprite( canvas->getPixmaps(), (QGraphicsScene *)canvas )
{
	_tryi = NULL;
	setEnabled( true );
	setFrame( 0 );
	setZValue( 10 );
}

DispositionSprite::~DispositionSprite()
{
	clearData();
}

void DispositionSprite::clearData()
{
	_listPixmap.clear();
	delete _tryi;
	/*
	while( ! _listPixmap.isEmpty() ) {
		delete _listPixmap.takeFirst();
	}

	}
	*/
}

void DispositionSprite::setSpriteType( GenericMapDisposition::DispositionType type )
{
	setFrame( (uint)type );
}

void DispositionSprite::setBackground( const QString & pixmapPath )
{
	clearData();
	setZValue( 5 );
		
	_listPixmap.append( QPixmap( pixmapPath ) );

	if( _tryi ) {
		delete _tryi;
	}
	_tryi	= new QList<QPixmap>( _listPixmap );
	//todo only in this case
	setSequence( _tryi );
	setFrame(0);
}

int DispositionSprite::type() const
{
	// Enable the use of qgraphicsitem_cast with this item.
	return Type;
}



