/****************************************************************
**
** Attal : Lords of Doom
**
** insideBase.cpp
** display the base and its buildings
**
** Version : $Id: insideBase.cpp,v 1.27 2009/10/28 12:09:26 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 28/10/2001
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

#include "insideBase.h"
 
// generic include files
// include files for QT
#include <QMouseEvent>
#include <QTimer>

// application specific include files
#include "libCommon/genericInsideBuilding.h"
#include "libCommon/genericBase.h"
#include "libCommon/log.h"

#include "libClient/insideBuilding.h"
#include "libClient/imageTheme.h"

//
// ----- InsideBase -----
//
InsideBase::InsideBase( QObject * parent, GenericBase * base )
    : QGraphicsScene( parent  )
{
	setSceneRect( 0,0, 960, 720 );
	_background = 0;
	if( base ) 
		setBase( base );
}

InsideBase::~InsideBase()
{
	TRACE("~InsideBase");	
	clear();
}

void InsideBase::clear()
{

	TRACE("InsideBase::clear");	

	while( ! _list.isEmpty() ) {
		delete _list.takeFirst();
	}
	_base = 0;
	_newBase = true;
}

void InsideBase::reinit()
{
	TRACE("InsideBase::reinit");	

	if( _base ) {
		_background = ImageTheme.getInsideBase( _base->getRace() );
		setBackgroundBrush(QBrush ( * _background));

		for( uint i = 0; i < _base->getBuildingCount(); i++ ) {
			addBuilding( _base->getBuilding( i ) );
		}
	}
}

void InsideBase::setBase( GenericBase * base )
{
	TRACE("InsideBase::setBase( base %p )", base );

	if( _base != base ) {
		clear();
		_base = base;
		_background = ImageTheme.getInsideBase( _base->getRace() );
		setBackgroundBrush( QBrush (* _background ) );

		for( uint i = 0; i < base->getBuildingCount(); i++ ) {
			addBuilding( base->getBuilding( i ) );
		}
	}
	update(sceneRect());
}

void InsideBase::addBuilding( GenericInsideBuilding * building )
{
	// check if building is already added (remove a bug )
	for( int i = 0; i < _list.count(); i++ ) {
		if( _list.at( i )->getBuilding()->getLevel() == building->getLevel() ) {
			return;
		}
	}

	InsideBuilding * inside = new InsideBuilding( _base->getRace(), building, this );
	inside->move( building->getX(), building->getY() );
	inside->show();
	_list.append( inside );
}

void InsideBase::delBuilding( uint level )
{
	InsideBuilding * inside = 0;

	for( int i = 0; i < _list.count(); i++ ) {
		if( _list.at( i )->getBuilding()->getLevel() == level ) {
			inside = _list.at( i );
			break;
		}
	}
	if( inside ) {
		_list.removeAll( inside );
		delete inside;
	}
}

void InsideBase::delBuilding( GenericInsideBuilding * building )
{
	InsideBuilding * inside = 0;

	for( int i = 0; i < _list.count(); i++ ) {
		if( _list.at( i )->getBuilding() == building ) {
			inside = _list.at( i );
			break;
		}
	}
	if( inside ) {
		_list.removeAll( inside );
		delete inside;
	}
}

void InsideBase::drawBackground( QPainter * painter, const QRectF& rect )
{
	if( _background ) {
		painter->fillRect( rect, Qt::black );
		QRectF tmp = rect.intersected( sceneRect() );
		painter->drawPixmap( tmp, * _background, tmp );
	} else {
		painter->fillRect( rect, Qt::black );
	}
}

void InsideBase::drawItems( QPainter * painter, int numItems,  QGraphicsItem *items[],const QStyleOptionGraphicsItem options[] , QWidget *widget )
{
	for (int i = 0; i < numItems; ++i) {
		// Draw the item
		painter->save();
		painter->setMatrix(items[i]->sceneMatrix(), true);
		items[i]->paint(painter, options, widget);
		painter->restore();
	}
}

//
// ----- InsideBaseView -----
//

InsideBaseView::InsideBaseView( InsideBase * baseScene, QWidget * parent )
	: QGraphicsView( baseScene , parent)
{
	TRACE("InsideBaseView scene %p", baseScene );

	viewport()->setMouseTracking( true ) ;
	_selected = 0;
	_scene = baseScene;
	update();
	connect( this, SIGNAL( sig_update() ), scene(), SLOT( update() ) );
}

InsideBaseView::~InsideBaseView()
{
	TRACE("~InsideBaseView");
}

/** handles mouse move event */
void InsideBaseView::mouseMoveEvent( QMouseEvent * e )
{
	QPointF pos = mapToScene( e->pos());
	QList<QGraphicsItem *> list;
	QList<QGraphicsItem *> list2 = scene()->items( pos );
	
	uint nbItems2 = (uint) list2.count();
	for( unsigned int i = 0; i < nbItems2; i++ ) {
		if( list2[ i ]->type() == InsideBuilding::RTTI ) {
			if( ( (InsideBuilding*)list2[ i ] )->hit( pos ) ) {
				list.append( list2[ i ] );
			}
		}
	}
	
	if( _scene->getNewBase()==true){
		_selected = 0;
		emit sig_update();
		_scene->setNewBase(false);
	}

	uint nbItems = (uint) list.count();
	for( unsigned int i = 0; i < nbItems; i++ ) {	
		if( _selected ) {
			if( _selected != (InsideBuilding*)list[i] ) {
				_selected->deselect();
				_selected = (InsideBuilding*)list[i];
				_selected->select();
				update();
			} 				
		} else {
			_selected = (InsideBuilding*)list[i];
			_selected->select();
			update();
		} 
	}
	
	if( ( list.count() == 0 ) && _selected ) {
		_selected->deselect();
		_selected = 0;
		update();
	}
}


/*
 * handles mouse press event
 */
void InsideBaseView::mouseReleaseEvent( QMouseEvent * e )
{
	QPointF pos = mapToScene( e->pos());
	QList<QGraphicsItem *> list;
	QList<QGraphicsItem *> list2 = scene()->items( pos );
	
	uint nbItems2 = (uint) list2.count();
	for( unsigned int i = 0; i < nbItems2; i++ ) {
		if( list2[ i ]->type() == InsideBuilding::RTTI ) {
			if( ( (InsideBuilding*)list2[ i ] )->hit( pos ) ) {
				list.append( list2[ i ] );
			}
		}
	}
	
	uint nbItems = (uint) list.count();
	for( unsigned int i = 0; i < nbItems; i++ ) {	
		if( e->button() == Qt::LeftButton ) {
			emit sig_building( ((InsideBuilding *)list[i])->getBuilding() );
		} else {
		}
	}
}

/** handles mouse move event */
void InsideBaseView::scrollContentsBy( int dx, int dy )
{
	QGraphicsView::scrollContentsBy( dx, dy );
	emit sig_update();
}

void InsideBaseView::drawForeground( QPainter * painter , const QRectF & rect )
{

	QGraphicsView::drawForeground( painter, rect );
	
	painter->setPen(QColor(0,0,0));
	//painter->drawRect(0,0,120,60);
	//painter->fillRect(QRectF(mapToScene(QPoint(0,0)),QSize(120,60)), QBrush(QColor(0,0,0)));
	painter->setPen(QColor(255,255,255));

	 for ( int i = 0; i < _listM.count(); ++i) {
		painter->drawText(mapToScene(QPoint(10 ,12 + 12 * i)), _listM.at( i ) );
	}
	
	//painter->drawText(rect, Qt::AlignLeft, tr("Qt by\nTrolltech"));
	
}

void InsideBaseView::newMessage( QString string)
{
	TRACE("message %s", qPrintable( string ) );

	if( _listM.count() > 6 ) {
		_listM.removeFirst();
	}

	_listM.append( string );
	QTimer::singleShot( 15000, this, SLOT( slot_removeMessage() ) );
	emit sig_update();
}

void InsideBaseView::slot_removeMessage()
{
	TRACE("count %d", _listM.count());

	if( !_listM.isEmpty() ) {
		_listM.removeFirst();
	}
	emit sig_update();
}


