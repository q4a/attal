/****************************************************************
**
** Attal : Lords of Doom
**
** attalStyle.cpp
** Manages style of client
**
** Version : $Id: attalStyle.cpp,v 1.12 2009/09/11 16:55:03 lusum Exp $
**
** Author(s) : Cyrille Verrier - Pascal Audoux
**
** Date : 17/08/2000
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

#include "attalStyle.h"

// generic include files
#include <limits.h>

// include files for QT
#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QColor>
#include <qdrawutil.h> // for now
#include <QFile>
#include <QImage>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPolygon>
#include <QPushButton>
#include <QScrollBar>
#include <QStyleOption>
#include <QTextStream>
#include <QToolButton>
#include <QWidget>


// application specific include
#include "conf.h"

#include "libCommon/log.h"

#include "libClient/attalButton.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"

extern QString IMAGE_PATH;

AttalStyle::AttalStyle( const QString & colorFile ) : QWindowsStyle()
{
	init( colorFile );
}

bool AttalStyle::init( const QString & colorFile )
{
	QFile f( colorFile );
	if (! f.open(QIODevice::ReadOnly) ) {
		logEE("Could not open file %s for reading\n", qPrintable(colorFile) );
		return false;
	}
						
	QTextStream ts( &f );

	if( ts.atEnd() ) {
		return false;
	}
	int colR, colG, colB;
	ts >> colR;
	ts >> colG;
	ts >> colB;
	_textColor.setRgb( colR, colG, colB );
	ts >> colR;
	ts >> colG;
	ts >> colB;
	_buttonColor.setRgb( colR, colG, colB );
	ts >> colR;
	ts >> colG;
	ts >> colB;
	_borderColor.setRgb( colR, colG, colB );				

	f.close();

	backgroundImage = QPixmap( IMAGE_PATH + "misc/background.png" );

	return true;
}

void AttalStyle::polish( QPalette & palette )
{
	QColor brown(212, 140, 95);
	QColor beige(236, 182, 120);
	QColor slightlyOpaqueBlack(0, 0, 0, 63);

	palette = QPalette(brown);

	palette.setBrush( QPalette::BrightText, Qt::white);
	palette.setBrush( QPalette::Base, beige);		
	palette.setBrush( QPalette::AlternateBase, Qt::green ); /// XXX: it is a test...
	palette.setBrush( QPalette::Highlight, Qt::darkGray ); /// XXX: to improve 

	palette.setBrush( QPalette::Button, QBrush( brown, backgroundImage ) );

	palette.setBrush( QPalette::Text, _textColor ); // color in entry widget (lineedit, checkbox...)
	palette.setBrush( QPalette::ButtonText, _buttonColor ); // color in buttons
	palette.setBrush( QPalette::WindowText, _buttonColor ); // color in labels, checkbox label

	//palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
	//setTexture( palette, QPalette::Foreground, backgroundImage);
	setTexture( palette, QPalette::Background, backgroundImage);

	QBrush brush = palette.background();
	brush.setColor( brush.color().dark() );

	palette.setBrush(QPalette::Disabled, QPalette::Foreground, brush);
	palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
	palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
	palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
	palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
	palette.setBrush(QPalette::Disabled, QPalette::Mid, brush);
}
    
void AttalStyle::setTexture( QPalette & palette, QPalette::ColorRole role, const QPixmap & pixmap )
{
	for( int i = 0; i < QPalette::NColorGroups; ++i ) {
		QColor color = palette.brush(QPalette::ColorGroup(i), role).color();
		palette.setBrush(QPalette::ColorGroup(i), role, QBrush(color, pixmap));
	}
}

void AttalStyle::polish( QWidget * w )
{
	// the polish function sets some widgets to transparent mode and
	// some to translate background mode in order to get the full
	// benefit from the nice pixmaps in the color group.

	if (w->inherits("QTipLabel") || w->inherits("QLCDNumber") ){
		return;
	}

	if ( ! w->isTopLevel() ) {
		if ( w->inherits("QPushButton")
		     || w->inherits("QToolButton")
		     || w->inherits("QGroupBox")
		     || w->inherits("QTabWidget")
		     || w->inherits("QComboBox") ) {
			/// XXX: no more in q4 ? w->setAutoMask( TRUE );
			return;
		}
		/*
		if( w->inherits("QLabel") || w->inherits("QButton") ) {
			//w->setBackgroundOrigin( QWidget::ParentOrigin );
		}
		*/
	}
}

void AttalStyle::unPolish( QWidget * w )
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
			/// XXX: no more in q4 ? w->setAutoMask( FALSE );
			return;
		}
		/*
		if (w->inherits("QLabel")
		    || w->inherits("QButton") ) {
			w->setBackgroundOrigin( QWidget::WidgetOrigin );
		}
		*/
	}
}

/* function not used cause without this the graphic seem more faster
	 otherwise is the 'right' method to handle controls for qt, so for 
	 future use i will leave this one function (that work, btw)
*/

/*
void AttalStyle::drawControl(ControlElement element, const QStyleOption *option,QPainter *painter,const QWidget *widget) const
{
	    switch (element) {
				    case CE_PushButton:
							        {
												QStyleOptionButton myButtonOption;
												const QStyleOptionButton *buttonOption = qstyleoption_cast<const QStyleOptionButton *>(option);
												myButtonOption = *buttonOption;

												QColor brown(212, 140, 95);
												QColor beige(236, 182, 120);
												QColor slightlyOpaqueBlack(0, 0, 0, 63);

												QPixmap backgroundImage( IMAGE_PATH + "misc/background.png" );

												myButtonOption.palette = QPalette(brown);


												myButtonOption.palette.setBrush( QPalette::BrightText, Qt::white);
												myButtonOption.palette.setBrush( QPalette::Base, beige);		
												myButtonOption.palette.setBrush( QPalette::Window, beige);		
												myButtonOption.palette.setBrush( QPalette::AlternateBase, Qt::green ); /// XXX: it is a test...
												myButtonOption.palette.setBrush( QPalette::Highlight, Qt::darkGray ); /// XXX: to improve 

												myButtonOption.palette.setBrush( QPalette::Button, QBrush( brown, backgroundImage ) );

												myButtonOption.palette.setBrush( QPalette::Text, Qt::yellow ); // color in entry widget (lineedit, checkbox...)
												myButtonOption.palette.setBrush( QPalette::ButtonText, Qt::yellow ); // color in buttons
												myButtonOption.palette.setBrush( QPalette::WindowText, Qt::yellow ); // color in labels, checkbox label

												for( int i = 0; i < QPalette::NColorGroups; ++i ) {
													QColor color = myButtonOption.palette.brush(QPalette::ColorGroup(i), QPalette::Background).color();
													myButtonOption.palette.setBrush(QPalette::ColorGroup(i), QPalette::Background, QBrush(color, backgroundImage));
												}


												QBrush brush = myButtonOption.palette.background();
												brush.setColor( brush.color().dark() );

												myButtonOption.palette.setBrush(QPalette::Disabled, QPalette::Foreground, brush);
												myButtonOption.palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
												myButtonOption.palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
												myButtonOption.palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
												myButtonOption.palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
												myButtonOption.palette.setBrush(QPalette::Disabled, QPalette::Mid, brush);
												QWindowsStyle::drawControl(element, &myButtonOption, painter, widget);

											}
											break;
						default:
											QWindowsStyle::drawControl(element, option, painter, widget);
			}

}
*/												

static inline int buttonthickness( int d )
{ 
	return  d > 20 ? 5 : ( d < 10 ? 2: 3 ); 
}

//
// ----- TestAttalStyle -----
//


TestAttalStyle::TestAttalStyle()
: QDialog( 0 )
{
	QPushButton * pb = new QPushButton( this );
	pb->setText( "test pushbutton" );
	FIXEDSIZE( pb );

	QPushButton * pbDisabled = new QPushButton( this );
	pbDisabled->setText( "test pushbutton disabled" );
	pbDisabled->setEnabled( false );
	FIXEDSIZE( pbDisabled );

	QLabel * lab = new QLabel( "test label", this );
	FIXEDSIZE( lab );

	QLineEdit * le = new QLineEdit( this );
	FIXEDSIZE( le );

	QCheckBox * cb = new QCheckBox( this );
	cb->setText( "Checkbox" );
	FIXEDSIZE( cb );

	QPushButton * pbImage = new QPushButton( this );
	pbImage->setFixedSize( 52, 42 );
	pbImage->setIconSize( ImageTheme.getLordSmallPixmap( 5 )->size() );
	pbImage->setIcon( * ImageTheme.getLordSmallPixmap( 5 ) );
	
	AttalButton * butLord = new AttalButton( this, AttalButton::BT_LORD );
	butLord->setLord( 5 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->addWidget( pb );
	layout->addWidget( pbDisabled );
	layout->addWidget( lab );
	layout->addWidget( le );
	layout->addWidget( cb );
	layout->addWidget( pbImage );
	layout->addWidget( butLord );
	layout->activate();
}


