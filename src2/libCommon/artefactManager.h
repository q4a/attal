/****************************************************************
**
** Attal : Lords of Doom
**
** artefactManager.h
** Manages artefacts of a lord
**
** Version : $Id: artefactManager.h,v 1.5 2005/11/11 20:47:48 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 30/11/2003
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


#ifndef ARTEFACTMANAGER_H
#define ARTEFACTMANAGER_H


// generic include files
// include files for QT
#include <QList>

// application specific include files
#include "libCommon/artefact.h"
#include "libCommon/define.h"

class QTextStream;
class GenericLord;

/*              ------------------------------
 *                         ArtefactManager
 *              ------------------------------ */

class ArtefactManager
{

public:
	/** Constructor */
	ArtefactManager( GenericLord * lord );

	/** Destructor */
	~ArtefactManager();

	void save( QTextStream * ts, int indent );

	/** Add artefact */
	void addArtefact( GenericLordArtefact * artefact );

	/** Add artefact */
	void addArtefact( uint type );

	/** Get artefact 'num' */
	GenericLordArtefact * getArtefact( int num );

	/** Get artefact 'num' */
	GenericLordArtefact * getArtefactByType( uint type );

	/** Remove artefact */
	void removeArtefact( int num );

	void removeArtefact( GenericLordArtefact * artefact );

	void removeArtefactByType( uint type );

	bool hasArtefactType( uint type );

	void clearArtefacts();

	/** Return the nb of artefacts */
	uint getArtefactNumber();

	bool addArtefact( int position, int num, GenericLordArtefact * artefact );

	GenericLordArtefact * getArtefact( int position, int num );

	bool removeArtefact( int position, int num );

	int computeCharacModif( LordCharac type, int value );

protected:
	QList<GenericLordArtefact *> _artefacts;
	GenericLord * _lord;
};

#endif // ARTEFACTMANAGER_H
