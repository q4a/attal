/****************************************************************
**
** Attal : Lords of Doom
**
** insideBuilding.h
** display building on the inside view
**
** Version : $Id: insideBuilding.h,v 1.8 2007/04/14 16:15:32 lusum Exp $
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

 
#ifndef INSIDEBUILDING_H
#define INSIDEBUILDING_H
 
 
// generic include files
// include files for QT
#include <QGraphicsScene>
#include <QGraphicsRectItem>


#include <QImage>
#include <QPixmap>

// application specific include files


class GenericInsideBuilding;

/*              ------------------------------
 *                         InsideBuilding
 *              ------------------------------ */

class InsideBuilding : public QGraphicsRectItem
{

public:
	/** Constructor */
	InsideBuilding( int type, GenericInsideBuilding * build, QGraphicsScene * canvas );
	
	~InsideBuilding();
   
	enum { 
		Type = QGraphicsItem::UserType + 35 
	};
	
	//ImageItem( QImage img, QCanvas *canvas );
    
	bool hit( const QPointF & ) const;

	void select();

	void deselect();

	virtual void move( int x, int y );

	GenericInsideBuilding * getBuilding() { return _building; }

	static const int RTTI;

	QRgb colorH( QRgb v, double grade );

	QImage highlight(QImage image);
	
	int type() const;

protected:
	void paint( QPainter * p , const QStyleOptionGraphicsItem * option, QWidget * widget  );
	
	GenericInsideBuilding * _building;
	QPixmap _pixmap, _newpix, _oldpix;
	QImage _image;
	uint _type;
};
 
#endif // INSIDEBUILDING_H   
