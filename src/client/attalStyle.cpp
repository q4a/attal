/****************************************************************************
** $Id: attalStyle.cpp,v 1.9 2002/07/20 16:00:52 audoux Exp $
**
** Copyright (C) 1992-2000 Troll Tech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "attalStyle.h"

// generic include files
// include files for QT
#include <qapplication.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <qdrawutil.h> // for now
#include <qpixmap.h> // for now
#include <qpalette.h> // for now
#include <qwidget.h>
#include <qlabel.h>
#include <qimage.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qrangecontrol.h>
#include <qscrollbar.h>
#include <limits.h>
#include <qcolor.h>
#include <qfile.h>
// application specific include
#include "conf.h"
#include "common/log.h"

extern QString IMAGE_PATH;

AttalStyle::AttalStyle( QString colorFile, int sbext ) : QWindowsStyle()
{
	init( colorFile );
	
	if ( sbext >= 0 ) {
		// XXX: obsolete in qt 3...
		//scrollBarExtent( sbext );
	}
}

bool AttalStyle::init( QString colorFile )
{

	QFile f( colorFile );
	if (! f.open(IO_ReadOnly) ) {
		logEE("Could not open file %s for reading\n", colorFile.latin1() );
		return false;
	}
						
	QTextStream ts( &f );
	
	
	while( ! ts.eof() ) {
		int colR, colG, colB;
		ts >> colR;
		ts >> colG;
		ts >> colB;
		_textColor.setRgb( colR, colG, colB );
		ts >> colR;
		ts >> colG;
		ts >> colB;
		_borderColor.setRgb( colR, colG, colB );
				
	}
		
	f.close();
	
	return true;
}

/*!
  Reimplementation from QStyle

  \sa QStyle
  */
void AttalStyle::polish( QApplication *app)
{

	oldPalette = app->palette();

	// we simply create a nice QColorGroup with a couple of fancy wood
	// pixmaps here and apply to it all widgets

	QImage img( IMAGE_PATH + "misc/background.png" );
	QImage orig = img;
	orig.detach();
	QPixmap button;
	button.convertFromImage(img);


	int i;
	for (i=0; i<img.numColors(); i++) {
		QRgb rgb = img.color(i);
		QColor c(rgb);
		rgb = c.dark(120).rgb();
		img.setColor(i,rgb);
	}
	QPixmap mid;
	mid.convertFromImage(img);
    
	img = orig;
	img.detach();
	for (i=0; i<img.numColors(); i++) {
		QRgb rgb = img.color(i);
		QColor c(rgb);
		rgb = c.light(180).rgb();
		img.setColor(i,rgb);
	}
	QPixmap light;
	light.convertFromImage(img);

	img = orig;
	img.detach();
	for (i=0; i<img.numColors(); i++) {
		QRgb rgb = img.color(i);
		QColor c(rgb);
		rgb = c.dark(180).rgb();
		img.setColor(i,rgb);
	}
	QPixmap dark;
	dark.convertFromImage(img);

    
	QImage bgimage( IMAGE_PATH + "misc/background.png" );
    
	QPixmap background;
	background.convertFromImage(bgimage);

	img = bgimage;
	img.detach();
	for (i=0; i<img.numColors(); i++) {
		QRgb rgb = img.color(i);
		QColor c(rgb);
		rgb = c.dark(180).rgb();
		img.setColor(i,rgb);
	}
	sunkenDark = new QPixmap;
	sunkenDark->convertFromImage(img);
    
    
	img = bgimage;
	img.detach();
	for (i=0; i<img.numColors(); i++) {
		QRgb rgb = img.color(i);
		QColor c(rgb);
		rgb = c.light(130).rgb();
		img.setColor(i,rgb);
	}
	sunkenLight= new QPixmap;
	sunkenLight->convertFromImage(img);
    
    
    
	QPalette op(QColor(212,140,95));

	QColorGroup active;	
	active.setBrush( QColorGroup::Foreground      , op.normal().foreground()                     );
	active.setBrush( QColorGroup::Button          , QBrush(op.normal().button(), button) );
	active.setBrush( QColorGroup::Background      , QBrush(op.normal().background(), background) );
	active.setBrush( QColorGroup::Light           , QBrush(op.normal().light(), light)           );
	active.setBrush( QColorGroup::Mid             , QBrush(op.normal().mid(), mid)               );
	active.setBrush( QColorGroup::Dark            , QBrush(op.normal().dark(), dark)             );
	
	active.setColor( QColorGroup::Base            , QColor(212,140,95) );
	active.setColor( QColorGroup::Text            , white     );
	active.setColor( QColorGroup::BrightText      , white      );
	active.setColor( QColorGroup::Foreground      , _borderColor );	
	active.setColor( QColorGroup::Highlight       , black     );
	active.setColor( QColorGroup::HighlightedText , white     );
	active.setColor( QColorGroup::ButtonText      , _textColor );
	 
	app->setPalette(QPalette(active, active , active), TRUE );

}

void AttalStyle::unPolish( QApplication *app)
{
	app->setPalette(oldPalette, TRUE);
}

/*!
  Reimplementation from QStyle

  \sa QStyle
  */
void AttalStyle::polish( QWidget* w)
{

	// the polish function sets some widgets to transparent mode and
	// some to translate background mode in order to get the full
	// benefit from the nice pixmaps in the color group.

	if (w->inherits("QTipLabel") || w->inherits("QLCDNumber") ){
		return;
	}

	if ( !w->isTopLevel() ) {
		if ( w->inherits("QPushButton")
		     || w->inherits("QToolButton")
		     || w->inherits("QGroupBox")
		     || w->inherits("QTabWidget")
		     || w->inherits("QComboBox") ) {
			w->setAutoMask( TRUE );
			return;
		}
		if (w->inherits("QLabel")
		    || w->inherits("QButton") ) {
			w->setBackgroundOrigin( QWidget::ParentOrigin );
		}
	}
}

void AttalStyle::unPolish( QWidget* w)
{
	// the polish function sets some widgets to transparent mode and
	// some to translate background mode in order to get the full
	// benefit from the nice pixmaps in the color group.
	if (w->inherits("QTipLabel") || w->inherits("QLCDNumber") ){
		return;
	}
	if ( !w->isTopLevel() ) {
		if ( w->inherits("QPushButton")
		     || w->inherits("QToolButton")
		     || w->inherits("QGroupBox")
		     || w->inherits("QTabWidget")
		     || w->inherits("QComboBox") ) {
			w->setAutoMask( FALSE );
			return;
		}
		if (w->inherits("QLabel")
		    || w->inherits("QButton") ) {
			w->setBackgroundOrigin( QWidget::WidgetOrigin );
		}
	}
}

void AttalStyle::drawPanel( QPainter * p, int x, int y, int w, int h, 
			    const QColorGroup & , 
			    bool , 
			    int , 
			    const QBrush * )
{
	
	p->fillRect( x,y,w,h, _textColor );
}

/*
void AttalStyle::drawScrollBarControls( QPainter* p, const QScrollBar* sb,
						int sliderStart, uint controls,
						uint activeControl )
{
    QWindowsStyle::drawScrollBarControls( p, sb, sliderStart, controls & ~(AddLine|SubLine),
					activeControl & ~(AddLine|SubLine) );
    bool horz = sb->orientation() == QScrollBar::Horizontal;
    int b = 2;
    int w = horz ? sb->height() : sb->width();

    QColorGroup g = sb->colorGroup();
    
    if ( controls & AddLine ) {
	bool sunken = activeControl & AddLine;
	QRect r( b, b, w-2*b, w-2*b ) ;
	if ( horz ) {
	    r.moveBy( sb->width() - w, 0 );
	    drawSemicircleButton( p, r, PointRight, sunken, g );
	} else {
	    r.moveBy( 0, sb->height() - w );
	    drawSemicircleButton( p, r, PointDown, sunken, g );
	}
    } 
    if ( controls & SubLine ) {
	bool sunken = activeControl & SubLine;
	QRect r( b, b, w-2*b, w-2*b ) ;
	if ( horz ) {
	    drawSemicircleButton( p, r, PointLeft, sunken, g );
	} else {
	    drawSemicircleButton( p, r, PointUp, sunken, g );
	}
    }
}
*/
static inline int buttonthickness( int d )
{ return  d > 20 ? 5 : ( d < 10 ? 2: 3 ); }      

void AttalStyle::drawButton( QPainter *p, int x, int y, int w, int h,
			     const QColorGroup &g, bool sunken, const QBrush* fill)
{
	int d = QMIN(w,h)/2;

	int b = buttonthickness( d );
    
	QRegion internR = QRect(x+b, y+b, w-2*b, h-2*b);
	//    qDrawShadePanel( p, x, y, w, h, g, sunken, 5);
	QPen oldPen = p->pen();

	QBrush brush( fill ? *fill : g.brush( QColorGroup::Button ));
	p->setClipRegion( internR );
	p->fillRect( x, y, w, h, brush );

    
	int e = QMIN( w, h )/2; 
    
	QPoint p2(x+w-1-e,y+e);
	QPoint p3(x+e, y+h-1-e);
    
	QPointArray a;
	a.setPoints( 5, x,y, x+w-1, y, p2.x(),p2.y(), p3.x(),p3.y(), x, y+h-1 );
    
	p->setClipRegion( QRegion( a )- internR );
    
	p->fillRect( x,y,w,h, g.brush( QColorGroup::Light ) );

    
	a.setPoint( 0, x+w-1, y+w-1 );
	p->setClipRegion( QRegion( a ) - internR );
    
	p->fillRect( x,y,w,h, g.brush( QColorGroup::Dark ));

	p->setClipRegion( internR );
	p->setClipping( FALSE );
	p->setPen( g.foreground() );
	p->drawRect( x, y, w, h );
	p->setPen( oldPen );
}

void AttalStyle::drawBevelButton( QPainter *p, int x, int y, int w, int h,
				  const QColorGroup &g, bool , const QBrush* fill)
{
	p->fillRect( x,y, w,h, red );
}


void AttalStyle::drawPushButton( QPushButton* btn, QPainter *p)
{	
	QColorGroup g = btn->colorGroup();
	int x1, y1, x2, y2;

	btn->rect().coords( &x1, &y1, &x2, &y2 );	// get coordinates

	p->setPen( g.foreground() );
	p->setBrush( QBrush(g.button(),NoBrush) );

	QBrush fill;	
	fill = g.brush( QColorGroup::Button );	
	
	if( ! btn->isDown() )
		drawButton( p, x1, y1, x2-x1-1, y2-y1-1, g, btn->isOn() || btn->isDown(), &fill);
	else
		drawButton( p, x1+1, y1+1, x2-x1-1, y2-y1-1, g, btn->isOn() || btn->isDown(), &fill);	
    
	if ( btn->isDefault() ) {
		QPen pen( _textColor, 4 , SolidLine);
		pen.setCapStyle( Qt::SquareCap );
		pen.setJoinStyle( Qt::MiterJoin );
		p->setPen( pen );
		p->drawRect( x1-1, y1-1, x2-x1+3, y2-y1+3 );
	}


	if ( btn->isMenuButton() ) {
		int dx = (y1-y2-4)/3;
#if QT_VERSION >= 300
		QRect rec = QRect( x2 - dx, dx, y1, y2 - y1 );
		drawPrimitive( PE_ArrowDown, p, rec, g );
#else
		drawArrow( p, DownArrow, FALSE,
			   x2 - dx, dx, y1, y2 - y1,
			   g, btn->isEnabled() );
#endif
	}

	if ( p->brush().style() != NoBrush )
		p->setBrush( NoBrush );

}


void AttalStyle::drawPushButtonLabel( QPushButton* btn, QPainter *p)
{
	QRect r = pushButtonContentsRect( btn );
	int x,y, w,h;
	r.rect( &x, &y, &w, &h );

	if( btn->isDown() ) {
		x += 1;
		y += 1;
	}

#if QT_VERSION >= 300
	drawItem( p, r /*x, y, w, h,*/,
		  AlignCenter|ShowPrefix,
		  btn->colorGroup(), btn->isEnabled(),
		  btn->pixmap(), btn->text(), -1,
		  &btn->colorGroup().buttonText());
#else
	drawItem( p, x, y, w, h,
		  AlignCenter|ShowPrefix,
		  btn->colorGroup(), btn->isEnabled(),
		  btn->pixmap(), btn->text(), -1,
		  &btn->colorGroup().buttonText());
#endif

}

QRect AttalStyle::pushButtonContentsRect( QPushButton *btn )
{
	QRect r = btn->rect();
	int x,y, w,h;
	r.rect( &x, &y, &w, &h );
	
	return QRect( x,y, w-2,h-2 );
}
