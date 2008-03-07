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
    hits   = 1.0;
  };
  ~Relation() {};

  Element *getSource(void) { return src; };
  Element *getTarget(void) { return target; };
  float    getHits(void) { return hits; };
  void     addHit(void) { hits += 1.0; };
  void     decayHits(void) { hits *= 0.995; };



 private:

  GLWidget *gl;
  Element  *src;
  Element  *target;

  float    hits;

};

typedef QLinkedList<Relation *> Relations;

#endif
