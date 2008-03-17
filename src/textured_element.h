/***************************************************************************
 *   Copyright (C) 2008 by Erlend Simonsen                                 *
 *   mr@fudgie.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef TEXTURED_ELEMENT_H
#define TEXTURED_ELEMENT_H

#include <list>
#include <QWidget>
#include <QtOpenGL>
#include "element.h"

/**
        @author Erlend Simonsen <mr@fudgie.org>
*/

class TexturedElement : public Element {

public:
  TexturedElement(GLWidget *glWidget, Input *h, QString name, QColor col, bool referrer = false);

 ~TexturedElement();

  void render(GLWidget *gl);
  void renderRelations(GLWidget *gl);

 private:
  GLuint texId;
  GLWidget *gl;

};


#endif
