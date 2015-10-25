/****************************************************************
**
** Attal : Lords of Doom
**
** previewBaseDialog.h
** dialog for preview of a base with buildings
**
** Version : $Id: previewBaseDialog.h,v 1.5 2006/10/05 20:50:08 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 28/06/2003
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


#ifndef PREVIEWBASEDIALOG_H
#define PREVIEWBASEDIALOG_H

// generic include files
// include files for QT
#include <QDialog>
// application specific include files

class QTreeWidget;
class QTreeWidgetItem;

class GenericBase;
class GenericBaseModel;
class InsideBase;

/*              ------------------------------
 *                      PreviewBaseDialog
 *              ------------------------------ */


class PreviewBaseDialog : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	PreviewBaseDialog( QWidget * parent = 0, const char * name = 0 );

	~PreviewBaseDialog();

	void init( GenericBaseModel * model );

public slots:
	void slot_clickBuilding( QTreeWidgetItem * , int column );

protected:
	InsideBase * _inside;
	GenericBase * _base;
	QTreeWidget * _listBuilding;
};
 
#endif // PREVIEWBASEDIALOG_H   
