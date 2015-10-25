/****************************************************************
**
** Attal : Lords of Doom
**
** mapDispositionEditor.h
** Edit map disposition
**
** Version : $Id: mapDispositionEditor.h,v 1.11 2007/05/14 21:58:53 lusum Exp $
**
** Author(s) : Pascal Audoux
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


#ifndef MAPDISPOSITIONEDITOR_H
#define MAPDISPOSITIONEDITOR_H

// generic include files
// include files for QT
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QWidget>

// application specific include files
#include "libCommon/genericMapDisposition.h"
#include "libClient/attalSprite.h"

class QSpinBox;

class DispositionSprite;
class MapDispositionCanvas;
class MapDispositionCanvasView;

/*              ------------------------------
 *                   MapDispositionEditor
 *              ------------------------------ */

class MapDispositionEditor : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	MapDispositionEditor( QWidget * parent = 0, const char * name = 0 );

	void init( GenericMapDisposition * mapDisposition, const QString & pixmapPath );
	
	void clear();

public slots:
	void slot_clicked( int id );
	void slot_resize_row( int value);
	void slot_resize_col( int value);

private:
	void dispoResize( bool isrow , int value );

protected:
	GenericMapDisposition * _mapDisposition;
	QString _pixmapPath;
	MapDispositionCanvas * _dispositionCanvas;
	MapDispositionCanvasView * _dispositionCanvasView;
	QSpinBox * _spinCol, * _spinRow;
};

/*              ------------------------------
 *                   MapDispositionCanvas
 *              ------------------------------ */
class MapDispositionCanvas : public QGraphicsScene
{
public:
	/** Constructor */
	MapDispositionCanvas( QObject * parent = 0, const char * name = 0 );

	virtual ~MapDispositionCanvas();

	void changeCell( uint row, uint col );

	void init( GenericMapDisposition * mapDisposition, const QString & pixmapPath );

	void reinit();

	QList<QPixmap> * getPixmaps() { return _pixmaps; }

	void setCurrentType( GenericMapDisposition::DispositionType type ) { _currentType = type; }
	
	void clear();

protected:
	DispositionSprite * _background;
	QList<QPixmap> _listPixmaps;
	QList<QPixmap> * _pixmaps;
	GenericMapDisposition * _disposition;
	GenericMapDisposition::DispositionType _currentType;
	DispositionSprite *** _dispositionSprites;
	uint _height, _width;
};

/*              ------------------------------
 *                 MapDispositionCanvasView
 *              ------------------------------ */
class MapDispositionCanvasView : public QGraphicsView
{
public:
	/** Constructor */
	MapDispositionCanvasView( MapDispositionCanvas * canvas , QWidget * parent = 0, const char * name = 0, Qt::WFlags f = 0 );

protected:
	void mouseReleaseEvent( QMouseEvent * e );

	MapDispositionCanvas * _canvas;
};

/*              ------------------------------
 *                    DispositionSprite
 *              ------------------------------ */
class DispositionSprite : public AttalSprite
{

public:
	/** Constructor */
	DispositionSprite( MapDispositionCanvas * );

	/** Destructor */
	virtual ~DispositionSprite();
	
	enum { 
		Type = QGraphicsItem::UserType + 61 
	};

	void setSpriteType( GenericMapDisposition::DispositionType type );
	
	void setBackground( const QString & pixmapPath );

	static const int RTTI;
	
	int type() const;
	
	void setType( const int type );

protected:
	void clearData();

	GenericMapDisposition::DispositionType _type;

	QList<QPixmap> _listPixmap;
	QList<QPixmap>  * _tryi;
};

#endif // MAPDISPOSITIONEDITOR_H
