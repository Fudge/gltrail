#ifndef TWITTER_H
#define TWITTER_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QThread>
#include <QColor>
#include <QHttp>
#include "../input.h"

class GLWidget;

struct Tweet {
  qlonglong id;
  QString title;
  QString user;
};

class Twitter : public QThread, public Input  {

  Q_OBJECT

 public:

  Twitter(QObject *parent);
  ~Twitter() {};

  void fetchTweets();
  void startProcessing() {};
  void endProcessing() {};

 public slots:

  void parse(bool);

 protected:
  void run();

 private:
  QHttp *http;

  ulong curId;
  int   fetch;
  QList<Tweet> tweets;

};

#endif
