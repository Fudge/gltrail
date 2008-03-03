#include "background_reader.h"
#include <iostream>

BackgroundReader::BackgroundReader(Hosts *h, Elements *e) {
  hosts = h;
  elements = e;
}

void BackgroundReader::run(void ) {
  forever {
    for(Hosts::const_iterator it = hosts->constBegin(); it != hosts->constEnd(); ++it ) {
      (*it)->decayMax();
    }

    int num = 0;
    for(Elements::const_iterator iter = elements->constBegin(); iter != elements->constEnd(); ++iter) {
      (*iter)->update_stats();
      num ++;
    }

    std::cout << "Elements: " << num << std::endl;

    sleep(1);
  }
}

