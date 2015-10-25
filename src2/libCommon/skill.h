/****************************************************************
**
** Attal : Lords of Doom
**
** skill.h
** manages skills of the lords
**
** Version : $Id: skill.h,v 1.4 2005/11/11 20:47:48 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 23/08/2000
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


#ifndef SKILL_H
#define SKILL_H


// generic include files
// include files for QT
#include <QList>
#include <QString>
// application specific includes

enum PrimarySkill {
	SKI_ATTACK,
	SKI_DEFENSE
};

/**            ------------------------------
 *                         Skill
 **            ------------------------------ */

class Skill
{
public:
	/** Constructor */
	Skill();

	int getId() { return _id; }

	void setId( int id ) { _id = id; }

	/** Return level of skill */
	int getLevel() { return _level; }

	/** Set level of skill */
	void setLevel( int level ) { _level = level; }

protected:
	int _id, _level;
};

class SkillLevel
{
public:
	/** Constructor */
	SkillLevel();

	/** Destructor */
	virtual ~SkillLevel();

	void addParam( int param );

	uint getParamNumber();

	int getParam( uint num );

	void setParam( uint num, int val );

protected:
	QList<int *> _params;
};

class SkillModel {

public:
	enum SkillType {
		SKI_ATTACK,
		SKI_DEFENSE,
		SKI_POWER,
		SKI_KNOWLEDGE
	};

	/** Constructor */
	SkillModel();

	QString getName() { return _name; }

	void setName( const QString & name ) { _name = name; }

	QString getDescription() { return _description; }

	void setDescription( QString description ) { _description = description; }

	/** Return type of skill */
	SkillType getType() { return _type; }

	/** Set type of skill */
	void setType( SkillType type ) { _type = type; }

	void addLevel( SkillLevel * level );

	uint getLevelNumber();

	SkillLevel * getLevel( uint num );

private:
	QString _name, _description;
	SkillType _type;
	QList<SkillLevel *> _levels;
};

#endif // SKILL_H
