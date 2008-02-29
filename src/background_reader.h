#ifndef BACKGROUND_READER_H
#define BACKGROUND_READER_H

#include <QThread>
#include "element.h"

/**
        @author Erlend Simonsen <mr@fudgie.org>
*/

class BackgroundReader : public QThread
{
  Q_OBJECT

public:

  BackgroundReader(Elements *e);
  ~BackgroundReader() {};

protected:
  void run();



private:
  Elements *elements;

};

#endif


