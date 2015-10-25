/****************************************************************
**
** Attal : Lords of Doom
**
** aiLord.h
** manages ai lord
**
** Version : $Id: aiLord.h,v 1.9 2008/07/10 15:52:37 lusum Exp $
**
** Author(s) : Sardi Carlo
**
** Date : 27/07/2005
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

 
#ifndef AILORD_H
#define AILORD_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "libCommon/genericLord.h"


enum AILORD_STATUS {
	NOFREE,
	FREE
};



/*              ------------------------------
 *                         AiLord
 *              ------------------------------ */

class AiLord : public GenericLord
{

public:

	/** Constructor */
	AiLord( int id );

	/** Destructor */
	virtual ~AiLord();

	void reinit();
	
	void initParams();

	void setPriority( LordPriority num, int value ) { _priorities[ num ] = value; }
	
	/** Return the unit 'num' */
	int getPriority( LordPriority num ) { return _priorities[ num ]; }
	
	void setCurPrio( int value ) { _curPrio = value; }
	
	int getCurPrio() { return _curPrio; }
	
	void setMincost( int value ) { _mincost = value; }
	
	int getMincost() { return _mincost; }
	
	void setStatus( int value ) { _status = value; }
	
	int getStatus() { return _status; }
	
	void setDestCell( GenericCell * cell ) { _destinationCell = cell; }
	
  GenericCell *  getDestCell() { return _destinationCell; }

	void setStop( bool value ) { _shouldStop = value; }
	
	bool isStop() { return _shouldStop; }


protected:

	int _priorities[ PR_LAST + 1 ];

private:

		int _curPrio;
		int _mincost;
		int _status;
		bool _shouldStop;
		GenericCell * _destinationCell;

};

#endif // AILORD_H
