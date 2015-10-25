/****************************************************************
**
** Attal : Lords of Doom
**
** genericPlayer.h
** class that manage a player
**
** Version : $Id: genericPlayer.h,v 1.35 2009/09/05 17:51:21 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 08/08/2000
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


#ifndef GENERICPLAYER_H
#define GENERICPLAYER_H
 
 
// generic include files
// include files for QT
#include <QList>
#include <QVector>

// application specific include
#include "libCommon/define.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericBuilding.h"
#include "libCommon/genericBase.h"
#include "libCommon/genericRessources.h"


class QTextStream;

class Creature;
class GenericMap;
class PriceMarket;
class InsideBuildingModel;

/*              ------------------------------
 *                         GenericPlayer
 *              ------------------------------ */


class GenericPlayer
{

public:
	/** Constructor */
	GenericPlayer( GenericMap * theMap = 0 );

	GenericPlayer& operator = (GenericPlayer& other);

	/** Destructor */
	virtual ~GenericPlayer();

	/** load players data */
	bool loadVision( QTextStream * ts , int width, int height);

	/** save players data */
	void save( QTextStream * ts, int indent = 0 );

	void newTurn();

	void newWeek();

	QString getAutoDescription();

	/** Set a name to the player */
	void setName( QString name ) { _name = name; }

	/** Return the name of the player */
	QString getName() { return _name; }

	/** Set a connection name to the player */
	void setConnectionName( QString name ) { _connectionName = name; }

	/** Return the connection name of the player */
	QString getConnectionName() { return _connectionName; }

	/** Give a number to the player */
	void setNum( int num );

	/** Return the number of the player */
	int getNum() { return _num; }

	void setTeam( uint teamId ) {
		_teamId = teamId;
		_isTeamSet = true;
	}
	
	uint getTeamId() { return _teamId; }

	/** Associate a map to the player */
	void setMap( GenericMap * theMap ) { _map = theMap; }

	/** Return the map associated to the player */
	GenericMap * getMap() { return _map; }

	GenericResourceList * getResourceList() { return _ress; }

	/** Return the 'num'-th lord */
	GenericLord * getLord( int num ) { return _theLords.at( num ); }

	/** Return the lord with Id 'id' */
	GenericLord * getLordById( int id );

	/** Remove a lord from the player's list */
	void removeLord( int num );
	void removeLord( GenericLord * lord );

	/** Add a new lord to the player */
	void addLord( GenericLord * l );

	/** Return the number of lords */
	uint numLord() { return _theLords.count(); }
	
	void cleanData();
		
	/** Tell if the player owns this lord */
	bool hasLord( GenericLord * lord ) { return _theLords.contains( lord ) ; }
	
	/** Unselect lords (if any) */
	virtual void unSelectLord() { _selectedLord = 0; }
	
	/** Select the first active lord */
	bool selectFirstLord();
	
	/** Return the lord selected in the player */
	GenericLord * getSelectedLord( ) { return _selectedLord; }

	/** Cycle to the next lord of the player */
	virtual GenericLord * nextLord();
	
	/** Return the 'num'-th building */
	GenericBuilding * getBuilding( int num ) { return _theBuild.at( num ); }
	
	/** Remove a building from the list */
	void removeBuilding( int num ) { _theBuild.removeAt( num ); } 
	
	/** Remove a building from the list */
	void removeBuilding( GenericBuilding * building ) { _theBuild.removeAll( building );}
	
	/** Add a building to the list */
	void addBuilding( GenericBuilding * b );
	
	/** Return the number of building */
	uint numBuilding() { return _theBuild.count(); }

	bool hasBuilding( GenericBuilding * building ) { return _theBuild.count( building ); }
	
	/** Tell if the player owns this base */
	bool hasBase( GenericBase * base ) { return _theBase.count( base ) ; }
	
	/** Select a base in the player list */
	virtual void setSelectedBase( GenericBase * base ) { 
		_selectedBase = base;
		_selectedLord = 0;
	}	

	/** Unselect bases (if any) */
	void unSelectBase() { _selectedBase = 0; }
	
	/** Return the base selected in the player */
	GenericBase * getSelectedBase( ) { return _selectedBase; }
	
	/** Cycle to the next lord of the player */
	GenericBase * nextBase();
		
	/** Return the 'num'-th base */
	GenericBase * getBase( int num ) { return _theBase.at( num ); }

	GenericBase * getBaseById( int id );

	/** Remove a base from the list */
	void removeBase( int num ) { _theBase.removeAt( num ); }

	/** Remove a base from the list */
	void removeBase( GenericBase * base );
	
	/** Add a building to the list */
	void addBase( GenericBase * base );
	
	/** Return the number of bases */
	uint numBase() { return _theBase.count(); }
	
	/** Select the first base */
	bool selectFirstBase() { return true; }
	
	/** Give its advice about the end of the turn */
	bool shouldEnd();
	
	/** Compute the end of the turn */
	void endTurn();
	
	/** Is the player alive ? */
	bool isAlive() { return _alive; }
	
	void setAlive(bool alive) { _alive = alive; }

	bool hasLost();

	bool isRuledByAi () {return _isRuledByAi;}

	void setRuledByAi (bool ruledByAi) {_isRuledByAi = ruledByAi;}

	bool canBuy( InsideBuildingModel * building );

	bool canBuy( Creature * creature, int number );
	
	bool canBuy( GenericLordModel * lord );

	void buy( Creature * creature, int number );

	uint computeBuyCreatureMax( Creature * creature );

	void initMapVision();
	
	void clearVision();

	QList<GenericCell *> addCellVision( GenericCell * cell, int vision );

	QList<GenericCell *> removeCellVision( GenericCell * cell, int vision );

	QList<GenericCell *> addLordVision( GenericLord * lord );

	QList<GenericCell *> removeLordVision( GenericLord * lord );

	QList<GenericCell *> addBaseVision( GenericBase * base );

	QList<GenericCell *> removeBaseVision( GenericBase * base );

	QList<GenericCell *> addBuildingVision( GenericBuilding * building );

	QList<GenericCell *> removeBuildingVision( GenericBuilding * building );

	bool canSee( uint row, uint col );

	bool canSee( GenericCell * cell );

	bool canMove( GenericLord * lord );
	
	PriceMarket * getPriceMarket() { return _price;}
	
	void setPriceMarket( PriceMarket * price ) { _price = price;}
	
	void setGameId( int id ) { _gameId = id; }

	int getGameId() { return _gameId; }

	void printLords();

	static const int NO_PLAYER=-1;

protected:

	/** Select a lord in the player list */
	virtual void setSelectedLord( GenericLord * lord ) { _selectedLord = lord ; _selectedBase = 0; }
	
	int _num;
	QString _name;
	QString _connectionName;
	bool _alive;
	bool _isRuledByAi;
	uint _teamId;
	bool _isTeamSet;
	int _gameId;

	GenericResourceList * _ress;
	QList<GenericLord *> _theLords;
	QList<GenericBase *> _theBase;
	QList<GenericBuilding *> _theBuild;

	GenericLord * _selectedLord;
	GenericBase * _selectedBase;
	GenericMap * _map;
	PriceMarket  * _price;

	int ** _vision;
	int _theight;
};

#endif // GENERICPLAYER_H
