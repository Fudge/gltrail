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
#include "textured_element.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

TexturedElement::TexturedElement(GLWidget *glWidget, Input *h, QString name, QColor col, QImage *img, bool referrer)
  : Element(h, name, col, referrer)
{
  cout << "Creating textured..." << endl;
  gl = glWidget;
  image = img;
  texId = 0;

  if( image != NULL ) {
    cout << "TexturedElement[" << image->width() << "x" << image->height() << "]" << endl;
  }

}

TexturedElement::~TexturedElement()
{
  gl->deleteTexture(texId);
}

void TexturedElement::render(GLWidget *gl) {
  GLfloat r = 0.004 + (size - 1.0) / 100;

  if( texId == 0 && image != NULL ) {
    cout << "Binding TexturedElement[" << image->width() << "x" << image->height() << "]" << endl;
    texId = gl->bindTexture(*image);
    cout << "Binding as " << texId << endl;
  }

  if( texId > 0 ) {
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glBegin(GL_QUADS);

    glTexCoord2d(0,0);
    glVertex3d( x-r, y-r, 0.0);

    glTexCoord2d(1,0);
    glVertex3d( x+r, y-r, 0.0);

    glTexCoord2d(1,1);
    glVertex3d( x+r, y+r, 0.0);

    glTexCoord2d(0,1);
    glVertex3d( x-r, y+r, 0.0);

    glEnd();

    if( activity_queue.size() > 0 && rand() % (60/activity_queue.size()) == 0 ) {

      Activity *a = activity_queue.takeFirst();
      activities << a;
      if( gl->useRecoil() ) {
        a->fire();
      }
    }

    glBegin(GL_POINTS);
    for(Activities::iterator it = activities.begin(); it != activities.end(); ++it) {
      if( (*it)->render(gl) ) {
        if( gl->useRecoil() ) {
          (*it)->impact();
        }
        delete *it;
        it = activities.erase(it);
      }
    }
    glEnd();
    glPointSize(1.0);
  } else {
    Element::render(gl);
  }
}

void TexturedElement::renderRelations(GLWidget *gl) {
  Element::renderRelations(gl);
}

