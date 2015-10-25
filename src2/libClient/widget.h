/****************************************************************
**
** Attal : Lords of Doom
**
** widget.h
** a set of widget used in Attal
**
** Version : $Id: widget.h,v 1.15 2010/01/06 00:18:21 lusum Exp $
**
** Author(s) : Cyrille Verrier - Sardi Carlo
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
#include <QFrame>
#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

// application specific includes

class AttalButton;
class GenericFightUnit;
class GenericLord;
class GenericBase;
class GenericBuilding;
class GenericEvent;
class GenericMapCreature;

class QVBoxLayout;

/**              ------------------------------
 *                         PopupMessage
 **              ------------------------------ */

class PopupMessage : public QLabel
{
public:
	/** constructor */
	PopupMessage( QWidget * parent = 0, const char * name = 0 );

protected:
	void mouseReleaseEvent( QMouseEvent * event );


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
 *                         ResourceIcon
 **              ------------------------------ */

class ResourceIcon : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ResourceIcon( QWidget * parent = 0, const char * name = 0 );

	/** Set num of the ressource for this Icon */
	void setResource( int num );

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
 *                         AttalPopup
 **              ------------------------------ */

class AttalPopup : public QFrame
{
public:

	enum PopupType {
		PT_NONE,
		PT_EVENT,
		PT_CREA,
		PT_TEXT
	};


	/** Constructor */
	AttalPopup( QWidget * parent = 0 );
	
	void setType( PopupType type = PT_NONE );

	void setCreature( GenericMapCreature * crea );
	
	void setEvent( GenericEvent * event );

	void setText( const QString & text );

	/** Update info */
	void update();

	//center popup in the middle of the parent widget
	void centerParent();

protected:
	PopupType _type;

private:
	GenericEvent * _event;
	GenericMapCreature * _crea;
	QLabel * _labParams;
	QString _message;
};

/**              ------------------------------
 *                         AttalMessage
 **              ------------------------------ */

/** Dialog for displaying message*/
class AttalMessage : public QDialog
{
public:
	/** Constructor */
	AttalMessage( QWidget * parent = 0, bool ok = true );

	/** Adds text to the dialog */
	void addText( QString text );

	/** Adds pixmap to the dialog */
	void addPixmap( QPixmap * pixmap );

protected:
	QVBoxLayout * _layout;
	bool _ok;

};

/**              ------------------------------
 *                         InfoWidget
 **              ------------------------------ */

class InfoWidget : public QFrame
{
	Q_OBJECT
public:
	/** Constructor */
	InfoWidget( QWidget * parent = 0 );

	void clear();

	void setIcon( const QPixmap & pixmap );

	void setText( const QString & text );

	void setPopup( const QString & text );

	void setUnit( GenericFightUnit * unit );

signals:
	void sig_clicked();

protected:
	Icon * _photo;
	Label * _label;
};


#endif // WIDGET_H


