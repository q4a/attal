/****************************************************************
**
** Attal : Lords of Doom
**
** action.h
** Manage actions of 'buildings'
**
** Version : $Id: action.h,v 1.8 2002/12/15 12:18:15 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 23/01/2002
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

 
#ifndef ACTION_H
#define ACTION_H
 
 
// generic include files
// include files for QT
#include <qlist.h>
// application specific include files
 

class Calendar;
class GenericLord;
class ElementaryAction;

/**              ------------------------------
 *                           Action
 **              ------------------------------ */

/** comment for the class */
class Action
{

public:
	enum ActionType {
		DATE,
		FIRSTTIME,
		FIRSTTIMELORD,
		NEXTTIME
	};

	/** Constructor */
	Action();

	/** \return Returns the type of action */
	ActionType getType() { return _type; }

	/** Sets the type of action */
	void setType( ActionType type ) { _type = type; }

	/** Adds an elementary action */
	void addElementaryAction( ElementaryAction * action ) { _list.append( action ); }

	/** \return Returns the nb of elementary actions */
	int getElementaryNumber() { return _list.count(); }

	/** \return Returns the elementary action nb 'i' */
	ElementaryAction * getElementaryAction( int i ) {
		return _list.at( i );
	}

protected:
	ActionType _type;
	QList<ElementaryAction> _list;
};

/** Describes an elementary action */
class ElementaryAction
{
public:
	enum ElementaryType {
		ATTACK = 1,
		DEFENSE,
		POWER,
		KNOWLEDGE,
		MOVE,
		MAXMOVE,
		TECHNICPOINT,
		MAXTECHNICPOINT,
		MORALE,
		LUCK,
		VISION,
		EXPERIENCE,
		SKILL,
		ARTEFACT,
		UNIT,
		RESSOURCE,
		RANDRESSOURCE,
	};

	/** Constructor */
	ElementaryAction();

	/** \return Returns the type of elementary action */
	ElementaryType getType() { return _type; }

	/** Sets the type of elementary action */
	void setType( ElementaryType type ) { _type = type; }

	/** \return Returns the arg */
	int getArg() { return _arg; }

	/** Sets the arg of the action */
	void setArg( int arg ) { _arg = arg; }

	/** \return Returns the coefficient */
	int getCoeff() { return _coeff; }

	/** Sets the coefficient */
	void setCoeff( int coeff ) { _coeff = coeff; }

private:
	ElementaryType _type;
	int _arg;
	int _coeff;
};

/** Describe the actions inside bases */
class InsideAction
{
public:
	/** Constructor */
	InsideAction();

	/** \return Returns the type of action */
	int getType() { return _type; }

	/** Sets the type of action */
	void setType( int type ) { _type = type; }

	/** \return Returns the number of parameters of this action */
	uint getParamNumber() { return _params.count(); }

	/** Add a parameter to this action */
	void addParam( int param );

	/** \return Returns the parameter 'num' */
	int getParam( uint num );


protected:
	int _type;
	QList<int> _params;
};

#endif // ACTION_H
