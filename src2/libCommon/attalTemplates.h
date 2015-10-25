/****************************************************************
**
** attal : lords of doom
**
** attalTemplates.h
** some useful templates
**
** version : $id: engine.cpp,v 1.109 2008/01/16 23:19:58 lusum exp $
**
** author(s) : sardi carlo
**
** date : 22/01/2008
**
** licence :
**	this program is free software; you can redistribute it and/or modify
**   	it under the terms of the gnu general public license as published by
**     	the free software foundation; either version 2, or (at your option)
**      any later version.
**
**	this program is distributed in the hope that it will be useful,
** 	but without any warranty; without even the implied warranty of
**	merchantability or fitness for a particular purpose.  see the
**	gnu general public license for more details.
**
****************************************************************/


template <class T> 
void socketModifRess( T * obj , AttalSocket * socket )
{
	char ress = socket->readChar();
	if( obj ) 
		obj->getResourceList()->setValue( ress, socket->readInt() );
}
