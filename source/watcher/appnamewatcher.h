#ifndef APPNAMEWATCHER_H
#define APPNAMEWATCHER_H

#include "watcher.h"

class AppNameWatcher : public Watcher
{
  public:
    AppNameWatcher();
    int eventTypeId() const override;

  protected:
    std::string pollEvent() override;
};

#endif  // APPNAMEWATCHER_H
