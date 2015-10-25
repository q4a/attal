/****************************************************************
**
** Attal : Lords of Doom
**
** widget.cpp
** a set of widget used in Attal
**
** Version : $Id: widget.cpp,v 1.20 2002/12/15 12:23:31 audoux Exp $
**
** Author(s) : Pascal Audoux
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
#include <qcanvas.h>
#include <qlayout.h>
// application specific include file
#include "common/dataTheme.h"
#include "common/log.h"
#include "common/skill.h"
#include "common/unit.h"

#include "client/imageTheme.h"
#include "client/gui.h"

extern ImageTheme ImageTheme;
extern QString IMAGE_PATH;
extern DataTheme DataTheme;

PopupMessage::PopupMessage( QWidget * parent, const char * name )
	:QPopupMenu( parent, name )
{
	setFrameStyle( QFrame::Box | QFrame::Raised );
	setLineWidth( 2 );
	setMidLineWidth( 0 );

	_label = new QLabel( this );
	insertItem( _label );
}

void PopupMessage::setText( QString text )
{
	_label->setText( text );
	FIXEDSIZE( _label );
}

//
// ----- Label -----
//

Label::Label( QWidget *parent, const char *name)
	: QLabel( parent, name )
{
	setFrameStyle( Panel | Raised );
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
	if( e->button() == RightButton ) {
		showPopup( e->globalPos() );
	} else {
		emit sig_clicked();
	}
}

void Label::mouseReleaseEvent( QMouseEvent * event )
{
	if( event->button() == RightButton ) {
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

Icon::Icon( QWidget * parent, const char *name )
	: Label( parent, name )
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

InfoLabel::InfoLabel( QWidget *parent, const char *name)
	: QFrame( parent, name )
{
	setFrameStyle( Panel | Sunken );
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
// ----- InfoLabelSkill -----
//


InfoLabelSkill::InfoLabelSkill( QWidget * parent, const char * name )
	: InfoLabel( parent, name )
{
}

void InfoLabelSkill::setSkill( SkillModel::SkillType type, int level )
{
	/// XXX: use _icon instead of _pixmap ??
	switch( type ) {
	case SKI_ATTACK:
		_pixmap = (QPixmap *)ImageTheme.skills->image( 0 );
		break;

	case SKI_DEFENSE:
		_pixmap = (QPixmap *)ImageTheme.skills->image( 1 );
		break;
	default:
		_pixmap = NULL;
		break;
	}

	QString s;
	switch( level ) {
	case 0:
		s = "Basic"   ; break;
	case 1:
		s = "Advanced"; break;
	case 2:
		s = "Expert"  ; break;
	}

	_label = s ;

	update();
}

void InfoLabelSkill::drawContents( QPainter *p )
{
	QRect rect = p->boundingRect ( 0,0, 50,10, AlignHCenter, _label );

	p->drawText( rect.x(),rect.height(), _label);
	p->drawPixmap( 0,rect.height(),  *_pixmap );
}

//
// ----- Sentence -----
//

Sentence::Sentence( QWidget * parent, const char * name )
	: QLabel( parent, name )
{
	setFrameStyle( Box | Raised );
	setLineWidth( 1 );
	setMidLineWidth( 1 );	
	setAlignment( AlignCenter );
	setFixedHeight( 25 );
}

//
// ----- RessourceIcon -----
//

RessourceIcon::RessourceIcon( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_icon = new Icon( this );
	
	_value = new QLabel( this );
	_value->setFixedSize( 50, 25 );
	_value->move( 0, 50 );
	
	connect( _icon, SIGNAL( sig_clicked() ), SIGNAL( sig_clicked() ) );
	
	setFixedSize( 50, 75 );
}

void RessourceIcon::setRessource( int num )
{
	QString name;
	name.sprintf( "ressources/ress_%02d.png", num+1 );
	_icon->setPixmap( QPixmap( IMAGE_PATH + name ) );
}

void RessourceIcon::clear()
{
	_icon->clear();
	_value->clear();
}

//
// ----- ResourceLabel -----
//

ResourceLabel::ResourceLabel( QWidget * parent, const char * name )
: QWidget( parent, name )
{
	_icon = new QLabel( this );
	_label = new QLabel( this );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->addWidget( _icon );
	layout->addSpacing( 5 );
	layout->addWidget( _label, 1 );
	layout->activate();
}

void ResourceLabel::setResource( int num )
{
	QString name;
	name.sprintf( "ressources/smallResource_%02d.png", num+1 );
	_icon->setPixmap( QPixmap( IMAGE_PATH + name ) );
}

void ResourceLabel::setNumber( int nb )
{
	_label->setText( QString::number( nb ) );
}


//
// ----- PopupUnit -----
//

PopupUnit::PopupUnit( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_unit = 0;
	_labNumber = new QLabel( "Number :", this );
	
	setFixedSize( 100, 30 );
}

void PopupUnit::update()
{
	if( _unit ) {
		_labNumber->setText( "Number : " + QString::number( _unit->getNumber() ) );
	}
	
	QWidget::update();
}
