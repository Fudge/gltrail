#ifndef DOMAIN_H
#define DOMAIN_H

#include <QObject>
#include <QList>
#include <QProcess>
#include <QColor>
#include "element.h"

class GLWidget;

class Domain  {

 public:

  Domain(QString domain) {
    name = domain;
    maxHits = 0.0;
    maxSize = 0.0;
  };
  ~Domain() {};

  float       maxHits;
  float       maxSize;
};

#endif
