// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
//
// SpaceGame is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SpaceGame is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SpaceGame.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __Event_H_
#define __Event_H_

#include <list>

template <typename TOwner, typename TArgs> class IEventHandler;

template <typename TOwner, typename TArgs> class Event {

public:
    Event(TOwner* eventOwner);
    ~Event();

    void raise(TArgs eventArgs);
    void add(IEventHandler<TOwner, TArgs> * eventHandler);

private:
    std::list<IEventHandler<TOwner, TArgs>* > eventHandlers_;
    TOwner* eventOwner_;
};

#include "IEventHandler.h"

template <typename TOwner, typename TArgs>
Event<TOwner, TArgs>::Event(TOwner *eventOwner) {
    this->eventOwner_ = eventOwner;
}

template <typename TOwner, typename TArgs>
Event<TOwner, TArgs>::~Event() {
    for (typename std::list<IEventHandler<TOwner, TArgs> *>::iterator i = eventHandlers_.begin(); i != eventHandlers_.end(); i++) {
        delete (*i);
    }
}

template <typename TOwner, typename TArgs>
void Event<TOwner, TArgs>::raise(TArgs eventArgs) {
    for (typename std::list<IEventHandler<TOwner, TArgs> *>::iterator i = eventHandlers_.begin(); i != eventHandlers_.end(); i++) {
        (*i)->handle(eventOwner_, eventArgs);
    }
}

template <typename TOwner, typename TArgs>
void Event<TOwner, TArgs>::add(IEventHandler<TOwner, TArgs> * eventHandler) {
    eventHandlers_.push_back(eventHandler);
}


#endif //__Event_H_