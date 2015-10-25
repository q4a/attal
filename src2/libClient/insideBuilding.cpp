/****************************************************************
**
** Attal : Lords of Doom
**
** insideBuilding.cpp
** display building on the inside view
**
** Version : $Id: insideBuilding.cpp,v 1.15 2008/11/09 20:13:48 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 04/11/2001
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

#include "insideBuilding.h"
 
// generic include files
#include <algorithm>
// include files for QT
#include <QPainter>
#include <QPixmap>
#include <QSizeF>
// application specific include files
#include "conf.h"

#include "libCommon/log.h"
#include "libCommon/genericInsideBuilding.h"

#include "libClient/imageTheme.h"

const int InsideBuilding::RTTI = Type;

InsideBuilding::InsideBuilding( int type, GenericInsideBuilding * build, QGraphicsScene * canvas )
: QGraphicsRectItem( 0, canvas )
{
	_type = type;
	_building = build;
 	//collisions( true );
 	//setFrame( 2 * _building->getLevel() );
	_oldpix = ImageTheme.getInsideBuilding( _type , _building->getLevel());
	_pixmap = _oldpix;
	_image = _pixmap.toImage();
	
	QImage image = 	highlight( _image );
	_newpix = QPixmap();
	_newpix = QPixmap::fromImage(image , Qt::AutoColor);

	setRect(QRectF( 0 , 0, _pixmap.width(), _pixmap.height()));
	setZValue( InsideBuilding::RTTI);
	setEnabled( true );
}

InsideBuilding::~InsideBuilding()
{
}

/*
ImageItem::ImageItem( QImage img, QCanvas *canvas )
    : QCanvasRectangle( canvas ), image(img)
{
    setSize( image.width(), image.height() );

#if !defined(Q_WS_QWS)
    pixmap = QPixmap::fromImage(image, OrderedAlphaDither);
#endif
}
*/
int InsideBuilding::type() const
{
	// Enable the use of qgraphicsitem_cast with this item.
	return Type;
}

void InsideBuilding::paint( QPainter *p , const QStyleOptionGraphicsItem * /* option */, QWidget * /*  widget */)
{
	p->drawPixmap( rect() ,  _pixmap, rect() );
}

bool InsideBuilding::hit( const QPointF & p ) const
{
	bool ret = false;
	int ix = (int)(p.x())-int(x());
	int iy = (int)(p.y())-int(y());
	    
	if ( _image.valid( ix , iy ) ) {
		QRgb pixel = _image.pixel( ix, iy );
		ret = ( qAlpha( pixel ) != 0 );
		
	}
	return ret;
}


void InsideBuilding::move( int x, int y )
{
	setPos( x, y );
	setZValue( int( y / 10 ) );
}

void InsideBuilding::select()
{
	//setFrame( (_building->getLevel() * 2) + 1 );
	_pixmap = _newpix;
	update();
}

void InsideBuilding::deselect()
{
	//setFrame( _building->getLevel() * 2 );
	_pixmap = _oldpix;
	update();
}

QImage InsideBuilding::highlight(QImage image)
{

		image.convertToFormat( QImage::Format_ARGB32 );

		int w = image.width();
		int h = image.height();

		QImage img( w, h, QImage::Format_ARGB32 );

		for ( int x = 0; x < w; x++ ) {
			for ( int y = 0; y < h; y++ ) {
				QRgb pixel = (x<w && y<h) ? image.pixel( x, y ) : 0;
				img.setPixel( x, y, colorH( pixel, 1.2 ) );
			}
		}
		return img;
}

QRgb InsideBuilding::colorH( QRgb v , double grade)
{
	int ach = qAlpha(v);

	int r = std::min ( (int) (qRed(v)* grade *255 /255),245);
	int g = std::min ( (int) (qGreen(v) * grade *255 /255),245);
	int b = std::min ( (int) (qBlue(v) * grade *255 /255),245);

	return qRgba(r,g,b,ach);
}

