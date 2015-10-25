/****************************************************************
**
** Attal : Lords of Doom
**
** widget.h
** a set of widget used in Attal
**
** Version : $Id: widget.h,v 1.18 2002/12/15 12:23:31 audoux Exp $
**
** Author(s) : Cyrille Verrier
**
** Date : 29/08/2000
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

#ifndef WIDGET_H
#define WIDGET_H

// generic include files
// include files for QT
#include <qpixmap.h>
#include <qlabel.h>
#include <qframe.h>
#include <qpainter.h>
#include <qpopupmenu.h>
// application specific includes
#include "common/skill.h"

class GenericFightUnit;

/**              ------------------------------
 *                         PopupMessage
 **              ------------------------------ */

class PopupMessage : public QPopupMenu
{
public:
	/** constructor */
	PopupMessage( QWidget * parent = 0, const char * name = 0 );

	/** Sets the text of the popup */
	void setText( QString text );

protected:
	QLabel * _label;

};

/*              ------------------------------
 *                         Label
 *              ------------------------------ */

/**
   * A Label
   * display a popup when right button is pressed
   *
   * this is an abstract class !!
   *
   */

class Label : public QLabel
{
	Q_OBJECT
public:
	/** Constructor */
	Label( QWidget * parent = 0, const char * name = 0 );

	/** sets the text of the popup */
	void setPopupText( QString text ) { _popupText = text; }

	/** Clears the label */
	virtual void clear();

signals:
	/** Signal when Label is clicked */
	void sig_clicked();

protected:
	/** Subclass must implement this function */
	virtual void showPopup( QPoint p );

	/** Right button event for popup menus */
	virtual void mousePressEvent( QMouseEvent *e );

	virtual void mouseReleaseEvent( QMouseEvent * event );

	PopupMessage * _popup;
	QString _popupText;
};




/*              ------------------------------
 *                         Icon
 *              ------------------------------ */

/**
   * An Icon is a Label but
   * it has a fixed size
   * used to display a pixmap
   *
   */

class Icon : public Label
{

public:
	/** Construct an Icon */
	Icon( QWidget * parent = 0, const char * name = 0 );

};


/**              ------------------------------
 *                          IconArtefact
 **              ------------------------------ */

class IconArtefact : public Icon
{
public:
	/* Constructor */
	IconArtefact( QWidget * parent = 0, const char * name = 0 );

	/** Sets type of icon */
	void setType( int type );

	/** Clears the icon */
	void clear();

protected:
	virtual void showPopup( QPoint p );

	//virtual void mouseReleaseEvent( QMouseEvent * event );

private:
	int _type;
};


/*              ------------------------------
 *                         InfoLabel
 *              ------------------------------ */

/**
   * InfoLabel display informations
   * contains an Icon and a Label
   * must be reimplmented
   *
   */

class InfoLabel : public QFrame
{
public:
	/** Constructor */
	InfoLabel( QWidget * parent, const char * name = 0 );

	/** Set text to this InfoLabel */
	void setText( QString text );

protected:
	Icon  * _icon  ;
	Label * _label ;
};



/*              ------------------------------
 *                         InfoLabelSkill
 *              ------------------------------ */

/**
   * InfoLabelSkill display informations about skills
   *
   */

class InfoLabelSkill : public InfoLabel
{
public:
	/** Constructor */
	InfoLabelSkill( QWidget * parent, const char * name = 0 );

	/** Set skill type and level to this Label */
	void setSkill( SkillModel::SkillType type, int level );

protected:
	void drawContents( QPainter * ) ;

private:
	QPixmap * _pixmap;
	QString _label;
};

/**              ------------------------------
 *                          Sentence
 **              ------------------------------ */

class Sentence : public QLabel
{
public:
	/** Constructor */
	Sentence( QWidget * parent = 0, const char * name = 0 );
};

/**              ------------------------------
 *                         RessourceIcon
 **              ------------------------------ */

class RessourceIcon : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	RessourceIcon( QWidget * parent = 0, const char * name = 0 );

	/** Set num of the ressource for this Icon */
	void setRessource( int num );

	/** Set value to the Icon */
	void setValue( QString text ) {
		_value->setText( text );
	}

	/** Clear datas */
	void clear();

signals:
	/** Signal when clicked */
	void sig_clicked();

private:
	Icon * _icon;
	QLabel * _value;

};

/** Label for displaying ressources */
class ResourceLabel : public QWidget
{
public:
	/** Constructor */
	ResourceLabel( QWidget * parent = 0, const char * name = 0 );

	/** Sets the ressources of this label */
	void setResource( int num );

	/** Sets the number of ressources */
	void setNumber( int nb );

protected:
	QLabel * _icon, * _label;
};

/**              ------------------------------
 *                         PopupUnit
 **              ------------------------------ */

class PopupUnit : public QWidget
{
public:
	/** Constructor */
	PopupUnit( QWidget * parent = 0, const char * name = 0 );

	/** Sets the unit of the popup */
	void setUnit( GenericFightUnit * unit ) {
		_unit = unit;
		update();
	}

	/** Update info */
	void update();

private:
	GenericFightUnit * _unit;
	QLabel * _labNumber;
};

#endif // WIDGET_H


