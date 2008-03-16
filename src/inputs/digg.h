#ifndef DIGG_H
#define DIGG_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QThread>
#include <QColor>
#include <QHttp>
#include "../input.h"

class GLWidget;

struct Dugg {
  qlonglong date;
  QString story;
  QString user;
  QString status;
};

class Digg : public QThread, public Input  {

  Q_OBJECT

 public:

  Digg(QObject *parent);
  ~Digg() {};


  void fetchDiggs();
  void fetchStory(QString story);
  void startProcessing() {};
  void endProcessing() {};

 public slots:

  void parse(bool);

 protected:
  void run();

 private:
  QHttp *http;

  qlonglong maxDate;
  qlonglong curDate;

  QList<Dugg> duggQueue;
  QList<Dugg> storiesQueue;
  QHash<QString,QString> stories;

};

#endif
