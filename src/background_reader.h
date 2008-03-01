#ifndef BACKGROUND_READER_H
#define BACKGROUND_READER_H

#include <QThread>
#include "element.h"
#include "host.h"

/**
        @author Erlend Simonsen <mr@fudgie.org>
*/

class BackgroundReader : public QThread
{
  Q_OBJECT

public:

  BackgroundReader(Hosts *h, Elements *e);
  ~BackgroundReader() {};

protected:
  void run();



private:
  Hosts *hosts;
  Elements *elements;

};

#endif


