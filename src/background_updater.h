#ifndef BACKGROUND_READER_H
#define BACKGROUND_READER_H

#include <QThread>
#include "element.h"
#include "input.h"

/**
        @author Erlend Simonsen <mr@fudgie.org>
*/

class BackgroundUpdater : public QThread
{
  Q_OBJECT

public:

  BackgroundUpdater(Inputs *h, Elements *e);
  ~BackgroundUpdater() {};

protected:
  void run();



private:
  Inputs *hosts;
  Elements *elements;

};

#endif


