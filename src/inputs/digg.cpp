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
  http = new QHttp("services.digg.com", 80, this);

  connect(http, SIGNAL(requestFinished(int,bool)),
          this, SLOT(parse(int,bool)) );

  httpImages = new QHttp("digg.com", 80, this);

  connect(httpImages, SIGNAL(requestFinished(int,bool)),
          this, SLOT(getImage(int,bool)) );

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

  //  http->setHost("services.digg.com");

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

  //  http->setHost("services.digg.com");

  cout << "Fetching[" << url.toString().toStdString() << "]" << endl;

  http->request(header);
}


void Digg::fetchImage( QString img ) {
  QUrl url;
  url.setPath(img );

  QHttpRequestHeader header("GET", url.toString() );

  header.setValue("Host", "digg.com");
  header.setValue("User-Agent", "glTrail v0.01 (http://www.fudgie.org/gltrail.html)" );

  //  http->setHost("services.digg.com");

  cout << "Fetching[" << url.toString().toStdString() << "]" << endl;

  httpImages->request(header);
}


void Digg::run( void ) {

  fetchDiggs();
  forever {

    if( duggQueue.size() > 0 && curDate > 5) {
      cout << duggQueue.size() << " pending diggs" << endl;

      Dugg d = duggQueue.first();

      cout << d.date << " <= " << curDate << endl;

      if( d.date > curDate + 5 ) {
        curDate = d.date;
      }

      while( d.date <= curDate && duggQueue.size() > 0 ) {

        if( d.image.isEmpty() ) {
          gl->addRelation(this, d.story, d.user, true);
        } else {
          if( images[d.image] == NULL )
            cout << "Adding rel with MISSING image[" << d.image.toStdString() << "]" << endl;

          gl->addRelation(this, d.story, d.user, true, images[d.image] );
        }

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

    curDate += 1;
    sleep(1);
  }
}

void Digg::getImage( int id, bool error ) {
  if(!error) {
    cout << "Got image[" << httpImages->currentRequest().path().toStdString() << "]" << endl;
    QByteArray buf = httpImages->readAll();
    if( !images.contains(httpImages->currentRequest().path()) ) {
      QImage *img = new QImage();
      img->loadFromData(buf);
      images.insert( httpImages->currentRequest().path(), img);
      cout << "Added width: " << img->width() << ", height: " << img->height() << endl;
    } else {
      cout << "Already in cache[" << httpImages->currentRequest().path().toStdString() << "]" << endl;
    }
  } else {
    cout << "Error getting [" << httpImages->currentRequest().path().toStdString() << "]" << endl;
  }
}

void Digg::parse( int id, bool error ) {
  if( !error ) {
    cout << "Got[" << http->currentRequest().path().toStdString() << "]" << endl;

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
        QString image;

        QString storyPattern("<story id=\"(\\d+)\" link=\"([^\"]+)\".*diggs=\"(\\d+)\" comments=\"(\\d+)\"");
        QString titlePattern("<title>(.*)</title>");
        QString imagePattern("<thumbnail .* src=\"([^\"]+)\"");

        QRegExp rx(storyPattern);
        QRegExp rxTitle(titlePattern);
        QRegExp rxImage(imagePattern);

        for( int i = 2; i < lines.size(); i++ ) {
          if( rx.indexIn(lines[i]) > -1 ) {
            id = rx.cap(1);
          } else if( rxTitle.indexIn(lines[i]) > -1 ) {
            title = rxTitle.cap(1);
          } else if( rxImage.indexIn(lines[i]) > -1 ) {
            image = rxImage.cap(1);
            fetchImage(image);
          }
        }

        cout << "id[" << id.toStdString() << "], title[" << title.toStdString() << "], image[" << image.toStdString() << "]" << endl;

        if( !id.isEmpty() && !title.isEmpty() ) {
          stories.insert(id, title);
          d.story = title;
          d.image = image;
          duggQueue << d;
        }
      }
    }

  } else {
    cout << "Http error" << endl;
  }

}
