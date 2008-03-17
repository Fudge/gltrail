#include "inputs/digg.h"

#include <QStringList>
#include <iostream>
#include "glwidget.h"

using namespace std;

//<?xml version="1.0" encoding="utf-8" ?>
//<stories timestamp="1205656343" total="1" offset="0" count="1">
// <story id="5703843" link="http://www.youtube.com/watch?v=j_GK-9O8glU" submit_date="1205628446" diggs="3459" comments="606" promote_date="1205634005" status="popular" media="videos" href="http://digg.com/odd_stuff/Video_of_Riot_Police_Arresting_Anonymous_Leader_in_Atlanta">
//  <title>Video of Riot Police Arresting &quot;Anonymous Leader&quot; in Atlanta</title>
//  <description>Just kidding - there's no such thing as an anonymous leader.  Cops are also pulling over random people for showing support to anonymous.  Protest cult - go to jail.</description>
//  <user name="mindmods" icon="http://digg.com/img/udl.png" registered="1160789117" profileviews="485" />
//  <topic name="Odd Stuff" short_name="odd_stuff" />
//  <container name="Offbeat" short_name="offbeat" />
//  <thumbnail originalwidth="130" originalheight="97" contentType="image/jpeg" src="http://digg.com/odd_stuff/Video_of_Riot_Police_Arresting_Anonymous_Leader_in_Atlanta/t.jpg" width="80" height="80" />
// </story>
//</stories>


Digg::Digg(QObject *parent) {
  http = new QHttp(this);

  connect(http, SIGNAL(done(bool)),
          this, SLOT(parse(bool)) );


  maxDate = 0;
  curDate = 0;
}

void Digg::fetchDiggs( void ) {
  QUrl url;
  url.setPath("/stories/popular/diggs");
  url.addQueryItem("count", "100");

  if(maxDate > 0) {
    url.addQueryItem("min_date", QString::number(maxDate));
  }
  url.addQueryItem("appkey", "http%3A%2F%2Fwww.fudgie.org%2Fgltrail.html");
  url.addQueryItem("sort", "date-asc");

  QHttpRequestHeader header("GET", url.toString() );

  header.setValue("Host", "services.digg.com");
  header.setValue("User-Agent", "glTrail v0.01 (http://www.fudgie.org/gltrail.html)" );

  http->setHost("services.digg.com");

  cout << "Fetching[" << url.toString().toStdString() << "]" << endl;

  http->request(header);
}

void Digg::fetchStory( QString story ) {
  QUrl url;
  url.setPath("/story/" + story );

  url.addQueryItem("appkey", "http%3A%2F%2Fwww.fudgie.org%2Fgltrail.html");

  QHttpRequestHeader header("GET", url.toString() );

  header.setValue("Host", "services.digg.com");
  header.setValue("User-Agent", "glTrail v0.01 (http://www.fudgie.org/gltrail.html)" );

  http->setHost("services.digg.com");

  cout << "Fetching[" << url.toString().toStdString() << "]" << endl;

  http->request(header);
}


void Digg::run( void ) {

  fetchDiggs();
  forever {
    if( duggQueue.size() > 0 ) {
      cout << duggQueue.size() << " pending diggs" << endl;

      Dugg d = duggQueue.first();

      cout << d.date << " <=> " << curDate << endl;

      if( d.date > curDate + 5 ) {
        curDate = d.date;
      }

      while( d.date <= curDate && duggQueue.size() > 0 ) {
        gl->addRelation(this, d.story, d.user, true);

        duggQueue.removeFirst();
        if( duggQueue.size() > 0 ) {
          d = duggQueue.first();
        }
      }
    }

    if( curDate % 30 == 0 && curDate != 0)
      fetchDiggs();

    if( storiesQueue.size() > 0 ) {
      if( stories.contains( storiesQueue.first().story ) ) {
        Dugg d = storiesQueue.takeFirst();
        d.story = stories[d.story];
        duggQueue << d;
      } else {
        fetchStory(storiesQueue.first().story);
      }
    }

    if( curDate > 0 ) {
      curDate += 1;
    }

    sleep(1);
  }
}

void Digg::parse( bool error ) {
  if( !error ) {
    QString result = http->readAll();
    //    cout << "Got " << result.toStdString() << endl;

    QStringList lines = result.split('\n');

    //    cout << "lines: " << lines.size() << endl;

    if( lines.size() > 2 ) {

      if( lines[1].contains("events") ) {
        QString storiesPattern( " <digg date=\"(\\d+)\" story=\"(\\d+)\" id=\"\\d+\" user=\"([^\"]+)\" status=\"(popular|upcoming)\" />");
        //        cout << "pattern[" << storiesPattern.toStdString() << "]" << endl;

        QRegExp rx(storiesPattern);

        for( int i = 0; i < lines.size(); i++ ) {
          if( rx.indexIn( lines[i] ) > -1 ) {
            Dugg d;
            d.date = rx.cap(1).toLong();
            d.story = rx.cap(2);
            d.user = rx.cap(3);
            d.status = rx.cap(4);

            if( d.date > maxDate )
              maxDate = d.date;

            if( !stories.contains( d.story ) ) {
              storiesQueue << d;
            } else {
              d.story = stories[d.story];
              duggQueue << d;
            }
          }
        }
      } else if( lines[1].contains("stories") ) {

        Dugg d = storiesQueue.takeFirst();

        QString id;
        QString title;

        QString storyPattern("<story id=\"(\\d+)\" link=\"([^\"]+)\".*diggs=\"(\\d+)\" comments=\"(\\d+)\"");
        QString titlePattern("<title>(.*)</title>");
        //        cout << "pattern[" << storyPattern.toStdString() << "]" << endl;

        QRegExp rx(storyPattern);
        QRegExp rxTitle(titlePattern);

        for( int i = 2; i < lines.size(); i++ ) {
          if( rx.indexIn(lines[i]) > -1 ) {
            id = rx.cap(1);
          } else if( rxTitle.indexIn(lines[i]) > -1 ) {
            title = rxTitle.cap(1);
          }
        }

        cout << "id[" << id.toStdString() << "], title[" << title.toStdString() << "]" << endl;

        if( !id.isEmpty() && !title.isEmpty() ) {
          stories.insert(id, title);
          d.story = title;
        }
        duggQueue << d;
      }
    }

  } else {
    cout << "Http error" << endl;
  }

}
