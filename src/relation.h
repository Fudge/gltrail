#ifndef RELATION_H
#define RELATION_H

#include <QObject>
#include <QList>
#include <QProcess>
#include <QColor>
#include "element.h"

class GLWidget;

class Relation  {

 public:

  Relation(Element *s, Element *t) {
    src = s;
    target = t;
    hits   = 1;
  };
  ~Relation() {};

  Element *getSource(void) { return src; };
  Element *getTarget(void) { return target; };
  int      getHits(void) { return hits; };
  void     addHit(void) { hits ++; };
  

 private:

  GLWidget *gl;
  Element  *src;
  Element  *target;

  int       hits;

};

typedef QLinkedList<Relation *> Relations;

#endif
