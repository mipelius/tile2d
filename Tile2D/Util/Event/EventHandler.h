// This file is part of SpaceGame.
// Copyright (C) 2015 Miika Pelkonen
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

#ifndef __EventHandler_H_
#define __EventHandler_H_

#include "IEventHandler.h"

template <typename THandlerOwner, typename TEventOwner, typename TArgs>
class EventHandler : public IEventHandler<TEventOwner, TArgs> {

public:
    EventHandler(THandlerOwner* handlerOwner, void (*handle)(THandlerOwner, TEventOwner, TArgs));
    void handle(TEventOwner *eventOwner, TArgs args);

private:
    void (*handleActual_)(THandlerOwner, TEventOwner, TArgs);
    THandlerOwner* handlerOwner_;
};

template <typename THandlerOwner, typename TEventOwner, typename TArgs>
EventHandler<THandlerOwner, TEventOwner, TArgs>::
EventHandler(
        THandlerOwner *handlerOwner,
        void (*handle)(THandlerOwner, TEventOwner, TArgs)
)

{
    handlerOwner_ = handlerOwner;
    handleActual_ = handle;
}

template <typename THandlerOwner, typename TEventOwner, typename TArgs>
void EventHandler<THandlerOwner, TEventOwner, TArgs>::handle(TEventOwner *eventOwner, TArgs args) {
    handleActual_(handlerOwner_, eventOwner, args);
}

#endif //__EventHandler_H_
