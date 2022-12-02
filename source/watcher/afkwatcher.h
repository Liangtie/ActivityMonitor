#ifndef AFKWATCHER_H
#define AFKWATCHER_H

#include "watcher.h"

class AFKWatcher : public Watcher
{
  public:
    AFKWatcher();

    int eventTypeId() const override;

  private:
    std::string pollEvent() override;
};

#endif  // AFKWATCHER_H
