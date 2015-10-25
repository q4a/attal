/****************************************************************
**
** Attal : Lords of Doom
**
** questConditionProperties.h
** Display/edit properties of quest conditions
**
** Version : $Id: questConditionProperties.h,v 1.13 2007/06/01 20:07:17 audoux Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 20/06/2004
**
** Licence :
**  This program is free software; you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation; either version 2, or (at your option)
**      any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
****************************************************************/


#ifndef QUESTCONDITIONPROPERTIES_H
#define QUESTCONDITIONPROPERTIES_H


// generic include files
// include files for QT
#include <QDialog>
#include <QLabel>
// application specific include files
#include "libClient/askWidget.h"

class QComboBox;
class QLabel;
class QRadioButton;
class QSpinBox;

//class AskCheck;
class AskLordCharac;
class QuestConditionDate;
class QuestConditionLord;
class QuestConditionPlayer;

/*              ------------------------------
 *                 QuestConditionProperties
 *              ------------------------------ */

class QuestConditionProperties : public QDialog
{
    Q_OBJECT
public:
    QuestConditionProperties( QWidget * parent = 0, const char * name = 0 );

    virtual ~QuestConditionProperties() = 0;
};

/*              ------------------------------
 *               QuestConditionDateProperties
 *              ------------------------------ */

class QuestConditionDateProperties : public QuestConditionProperties
{
    Q_OBJECT
public:
    QuestConditionDateProperties( QWidget * parent = 0, const char * name = 0 );

    ~QuestConditionDateProperties() {}

    void setCondition( QuestConditionDate * condition );

public slots:
    void slot_date();

    void slot_delay();

    void slot_accept();

protected:
    QuestConditionDate * _condition;
    QRadioButton * _radioDate, * _radioDelay;
    QSpinBox * _delay, * _day, * _week, * _month, * _year;
    QLabel * _labDay, * _labWeek, * _labMonth, * _labDelay, * _labYear;
};

/*              ------------------------------
 *               QuestConditionLordProperties
 *              ------------------------------ */

class QuestConditionLordProperties : public QuestConditionProperties
{
    Q_OBJECT
public:
    QuestConditionLordProperties( QWidget * parent = 0, const char * name = 0 );

    ~QuestConditionLordProperties() {}

    void setCondition( QuestConditionLord * condition );

public slots:
    void slot_primarySkill();

    void slot_secondarySkill();

    void slot_artefact();

    void slot_spell();

    void slot_number();

    void slot_accept();

protected:
    QuestConditionLord * _condition;
    QRadioButton * _radioPrimary, * _radioSecondary, * _radioArtefact, * _radioSpell,* _radioNum;
    AskInt * _primValue, * _numValue;
    AskLordCharac * _primCharac;
    AskCombo * _artefact;
    QComboBox * _primCheck, * _numCheck;
};

/*              ------------------------------
 *               QuestConditionPlayerProperties
 *              ------------------------------ */

class QuestConditionPlayerProperties : public QuestConditionProperties
{
    Q_OBJECT
public:
    QuestConditionPlayerProperties( QWidget * parent = 0, const char * name = 0 );

    ~QuestConditionPlayerProperties() {}

    void setCondition( QuestConditionPlayer * condition );

public slots:
    void slot_accept();

    void slot_nbLords();

    void slot_nbBases();

    void slot_basename();

protected:
    QuestConditionPlayer * _condition;
    QRadioButton * _radioNbLords, * _radioNbBases, * _radioBasename;
    QComboBox * _lordCheck, * _baseCheck;
    AskInt * _lordValue, * _baseValue;
    AskString * _basename;
};

class AskLordCharac : public AskCombo
{
	Q_OBJECT
public:
	AskLordCharac( QWidget * parent = 0, const char * name = 0 );
};

#endif // QUESTCONDITIONPROPERTIES_H
