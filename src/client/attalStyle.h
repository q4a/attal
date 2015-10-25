/****************************************************************************
** $Id: attalStyle.h,v 1.5 2002/10/22 18:48:23 audoux Exp $
**
** Definition of something or other
**
** Created : 979899
**
** Copyright (C) 1997 by Troll Tech AS.  All rights reserved.
**
****************************************************************************/

#ifndef WOOD_H
#define WOOD_H

// generic include files
// include files for QT
#include <qwindowsstyle.h>
#include <qpalette.h>
// application specific include

class QPushButton;

/** Attal style (depends also of theme) */
class AttalStyle : public QWindowsStyle
{
public:
	AttalStyle( QString colorFile, int sbext = -1 );
	void polish( QApplication*);
	void polish( QWidget* );
	void unPolish( QWidget* );
	void unPolish( QApplication*);
	
	void drawButton( QPainter *p, int x, int y, int w, int h,
			 const QColorGroup &g, bool sunken = FALSE,
			 const QBrush *fill = 0 );
	void drawBevelButton( QPainter *p, int x, int y, int w, int h,
			      const QColorGroup &g, bool sunken = FALSE,
			      const QBrush *fill = 0 );
	void drawPushButton( QPushButton* btn, QPainter *p);
	QRect pushButtonContentsRect( QPushButton *btn );
	
	void drawPanel( QPainter * p, int x, int y, int w, int h, const QColorGroup &, bool sunken=FALSE, int lineWidth = 1, const QBrush * fill = 0 );
	
	void drawPushButtonLabel( QPushButton* btn, QPainter *p);
	// void drawScrollBarControls( QPainter*,  const QScrollBar*, int sliderStart, uint controls, uint activeControl );

private:
	bool init( QString colorFile );
	
	QPalette oldPalette;
	QPixmap *sunkenDark;
	QPixmap *sunkenLight;

	QColor _textColor;
	QColor _borderColor;
	static const QColor goldColor;
};

#endif
