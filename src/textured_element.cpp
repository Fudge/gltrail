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
  bool hover = fabs(gl->getX() - x) <= r*1.5f && fabs(gl->getY() - y) <= r * 1.5f;

  if(hover) {
    if( gl->getSelected() == NULL )
      gl->setSelected(this);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    size = 5.0;
    r = 0.004 + (10.0 - 1.0) / 100;
   } else {
    if( activities.size() > 0 ) {
      glColor4f(1.0, 1.0, 1.0, 1.0);
    } else {
      glColor4f(0.75, 0.75, 0.75, 0.75);
    }
  }




  if( texId == 0 && image != NULL ) {
    cout << "Binding TexturedElement[" << image->width() << "x" << image->height() << "]" << endl;

    if( image->width() == 0 || image->height() == 0 ) {
      texId = -1;
      cout << "Ignored";
    } else {
      texId = gl->bindTexture(*image);
      cout << "Binding as " << texId << endl;
    }
  }

  if( texId > 0 ) {
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

    if( showInfo > 0 || hover ) {
      QString info = QString("[%1] %2").arg( QString::number(realSize).left(5) ).arg(name().left(50));
      glColor4f(1.0, 1.0, 1.0, 1.0);
      int xi =  (int) ((1.0 + x) / 2.0 * gl->getWidth()) - info.length() * 3;
      int xy =  (int) (( gl->getAspect() - y) / (2 * gl->getAspect()) * gl->getHeight() - gl->getHeight() * 0.05 );

      gl->renderText(xi,xy, info );
    }

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

