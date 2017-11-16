// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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

#ifndef __RouteRequestSender_H_
#define __RouteRequestSender_H_

class RouteResponse;
class RouteGenerator;
class RouteRequest;

#include "Vector.h"

class RouteRequestSender {
    friend class RouteGenerator;
private:
    RouteGenerator* generator;
protected:
    virtual void handleResponse(RouteResponse *response) = 0;
    virtual Vector getLocation() = 0;
public:
    RouteRequestSender();
    void setRecipient(RouteGenerator* generator);
    void sendRequest(RouteRequest* request);
};


#endif //__RouteRequestSender_H_
