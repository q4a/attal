/****************************************************************
**
** Attal : Lords of Doom
**
** widget.cpp
** a set of widget used in Attal
**
** Version : $Id: widget.cpp,v 1.26 2010/01/06 00:18:21 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 09/08/2000
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

#include "widget.h"

// generic include files
// include files for QT

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QMouseEvent>
#include <QPixmap>

// application specific include file
#include "libCommon/dataTheme.h"
#include "libCommon/genericBase.h"
#include "libCommon/genericBonus.h"
#include "libCommon/genericBuilding.h"
#include "libCommon/genericEvent.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericMapCreature.h"
#include "libCommon/log.h"
#include "libCommon/unit.h"

#include "libClient/attalButton.h"
#include "libClient/imageTheme.h"
#include "libClient/gui.h"

PopupMessage::PopupMessage( QWidget * parent, const char * /* name */ )
	:QLabel( parent, Qt::Popup )
{
	setFrameStyle( QFrame::Box | QFrame::Raised );
	setLineWidth( 2 );
	setMidLineWidth( 0 );
	setAttribute( Qt::WA_NoMouseReplay );
	setMouseTracking(true);

}

void PopupMessage::mouseReleaseEvent( QMouseEvent * event )
{
	if( event->button() == Qt::RightButton ) {
		hide();
	}
}

//
// ----- Label -----
//

Label::Label( QWidget *parent, const char * /* name */)
	: QLabel( parent )
{
	setFrameStyle( QFrame::Panel | QFrame::Raised );
	setLineWidth( 1 );
	setMidLineWidth( 1 );

	_popup = 0;
	_popupText = "";
}

void Label::showPopup( QPoint p )
{
	if( _popupText != "" ) {
		if( ! _popup ) {
			_popup = new PopupMessage( this );
		}
		_popup->setText( _popupText );
		_popup->move( p );
		_popup->show();
	}
}

void Label::mousePressEvent( QMouseEvent *e )
{
	if( e->button() == Qt::RightButton ) {
		showPopup( e->globalPos() );
	} else {
		emit sig_clicked();
	}
}

void Label::mouseReleaseEvent( QMouseEvent * event )
{
	if( event->button() == Qt::RightButton ) {
		if( _popup ) {
			_popup->hide();
		}
	}
}

void Label::clear()
{
	_popupText = "";
	QLabel::clear();
}

//
// ----- Icon -----
//

Icon::Icon( QWidget * parent, const char * /* name */)
	: Label( parent )
{
	setFixedSize( 60, 60 );
}

//
// ----- IconArtefact -----
//

IconArtefact::IconArtefact( QWidget * parent, const char *name )
	: Icon( parent, name )
{
	_type = -1;
}

void IconArtefact::setType( int type )
{
	_type = type;
	setPixmap( * ImageTheme.getArtefactIcon( _type ) );
}

void IconArtefact::clear()
{
	setPixmap( QPixmap() );
	_type = -1;
}

void IconArtefact::showPopup( QPoint p )
{
	if( _type != -1 ) {
		if( ! _popup ) {
			_popup = new PopupMessage( this );
		}
		_popup->setText( DataTheme.artefacts.at( _type )->getName() );
		_popup->move( p );
		_popup->show();
	}
}

//
// ----- InfoLabel -----
//

InfoLabel::InfoLabel( QWidget *parent, const char * /*name */)
	: QFrame( parent )
{
	setFrameStyle( QFrame::Panel | QFrame::Sunken );
	setLineWidth( 1 );
	setMidLineWidth( 1 );
	_icon = new Icon( this );
	
	_label = new Label( this );
	_label->move( 60, 0 );
	_label->setFixedSize( 90, 60 );
	setFixedSize( 150 , 60 );
}

void InfoLabel::setText( QString text )
{
	_label->setText( text );
}

//
// ----- Sentence -----
//

Sentence::Sentence( QWidget * parent, const char * /* name*/ )
	: QLabel( parent )
{
	setFrameStyle( QFrame::Box | QFrame::Raised );
	setLineWidth( 1 );
	setMidLineWidth( 1 );	
	setAlignment( Qt::AlignCenter );
	setFixedHeight( 25 );
}

//
// ----- ResourceIcon -----
//

ResourceIcon::ResourceIcon( QWidget * parent, const char * /* name*/ )
	: QWidget( parent )
{
	_icon = new Icon( this );

	_value = new QLabel( this );
	_value->setFixedSize( 50, 25 );
	_value->move( 0, 50 );

	connect( _icon, SIGNAL( sig_clicked() ), SIGNAL( sig_clicked() ) );

	setFixedSize( 50, 75 );
}

void ResourceIcon::setResource( int num )
{
	_icon->setPixmap( * ( ImageTheme.getResourceIcon( num ) ) );
}

void ResourceIcon::clear()
{
	_icon->clear();
	_value->clear();
}

//
// ----- ResourceLabel -----
//

ResourceLabel::ResourceLabel( QWidget * parent, const char * /* name */ )
: QWidget( parent )
{
	_icon = new QLabel( this );
	_label = new QLabel( this );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->addWidget( _icon );
	layout->addSpacing( 5 );
	layout->addWidget( _label, 1, Qt::AlignRight );
	layout->activate();
}

void ResourceLabel::setResource( int num )
{
	QPixmap pix = ImageTheme.getResourceSmallIcon( num )->copy(ImageTheme.getResourceSmallIcon( num )->rect());
	_icon->setPixmap( pix );
}

void ResourceLabel::setNumber( int nb )
{
	_label->setText( QString::number( nb ) );
}


//
// ----- PopupUnit -----
//

AttalPopup::AttalPopup( QWidget * parent  )
	: QFrame( parent )
{
	_type = PT_NONE;
	_labParams = new QLabel( "?", this );

	_labParams->setFrameStyle( QFrame::Box | QFrame::Raised );
	_labParams->setAutoFillBackground ( true );
	_labParams->setLineWidth (2);
}

void AttalPopup::update()
{

	QString params = "?";

	switch( _type ) {
		case PT_NONE:
			break;
		case PT_EVENT:
			{
				if( _event ) {
					switch( _event->getType() ) {
						case GenericEvent::EventArtefact: 
							{
								GenericArtefact * artefact = _event->getArtefact();
								uint type = artefact->getType();
								params = DataTheme.artefacts.at( type )->getName();
							}
							break;
						case GenericEvent::EventBonus:
							{
								GenericBonus * bonus = _event->getBonus();
								params = bonus->getBonusDescription();
							}
							break;
						case GenericEvent::EventChest:
							params = "Chest";
							break;
						default:
							break;
					}
				}
			}
			break;
		case PT_CREA:
			if( _crea ) {
				params =  tr("About ") + QString::number(DataTheme.getRandomInCategory( _crea->getCategoryNumber()))  + "  " + _crea->getCreature()->getName();
			}
			break;
		case PT_TEXT:
			params = _message;
			break;
		default:
			break;
	}

	_labParams->setText( params );

	_labParams->adjustSize();
	adjustSize ();

	QFrame::update();
}

void AttalPopup::centerParent() {
	
	QWidget * parent = parentWidget();
	int x = parent->pos().x() + parent->size().width() / 2 - size().width() / 2;
	int y = parent->pos().y() + parent->size().height() / 2 - size().height() / 2; 
	QPoint point( x , y );
	move( point  );

}

void AttalPopup::setType( PopupType type ) {
	_type = type;
	update();
}

void AttalPopup::setCreature( GenericMapCreature * crea ) {
	_crea = crea;
	setType( PT_CREA );
	update();
}

void AttalPopup::setEvent( GenericEvent * event ) {
	_event = event;
	setType( PT_EVENT );
	update();
}

void AttalPopup::setText( const QString & text ) {
	_message = text;
	setType( PT_TEXT );
	update();
}

//
// ----- AttalMessage -----
//

AttalMessage::AttalMessage( QWidget * parent, bool ok )
	:QDialog( parent, Qt::Dialog )
{
	_ok = ok;
	_layout = new QVBoxLayout( this );
	_layout->addStretch( 1 );

	if( _ok ) {
		AttalButton * butOk = new AttalButton( this, AttalButton::BT_OK );

		_layout->addWidget( butOk );
		connect( butOk, SIGNAL( clicked() ), SLOT( accept() ) );
	}

	_layout->activate();
	
}

void AttalMessage::addText( QString text )
{
	QLabel * label = new QLabel( this );
	label->setText( text );
	FIXEDSIZE( label );
	_layout->insertWidget( _layout->count()-2 , label );
	_layout->insertStretch( _layout->count()-1, 1 );
}

void AttalMessage::addPixmap( QPixmap * pixmap )
{
	QLabel * label = new QLabel( this );
	label->setPixmap( * pixmap );
	FIXEDSIZE( label );
	_layout->insertWidget( _layout->count()-1 , label );
	_layout->insertStretch( _layout->count()-1, 1 );
}

//
// ----- InfoWidget -----
//

InfoWidget::InfoWidget( QWidget * parent )
: QFrame( parent )
{
	setFrameStyle( QFrame::Panel | QFrame::Sunken );
	setLineWidth( 1 );
	setMidLineWidth( 1 );

	_photo = new Icon( this );
	_label = new Label( this );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setSpacing( 5 );
	layout->setMargin( 5 );
	layout->addWidget( _photo );
	layout->addWidget( _label, 1 );
	layout->activate();

	connect( _photo, SIGNAL( sig_clicked() ), SIGNAL( sig_clicked() ) );
}

void InfoWidget::setIcon( const QPixmap & pixmap )
{
	_photo->setPixmap( pixmap ); 
}

void InfoWidget::clear()
{
	_photo->clear();
	_label->clear(); 
}

void InfoWidget::setText( const QString & text )
{
	_label->setText( text );
}

void InfoWidget::setPopup( const QString & text )
{
	_label->setPopupText( text  );
}

void InfoWidget::setUnit( GenericFightUnit * unit )
{
	if( unit ) {
		_photo->setPixmap( ImageTheme.getPhotoCreature( unit )  );
		QString text = unit->getUnitDescription();
		_label->setPopupText( text  );
		_photo->setPopupText( text  );
		text.sprintf( "%ld %s", unit->getNumber(), qPrintable( unit->getCreature()->getName() ) );
		_label->setText( text );
	} else {
		_photo->clear();
		_label->clear();
	}
}
