/*
portal
Copyright (C) 2018	Will Townsend <will@townsend.io>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <usbmuxd.h>
#include <vector>
#include <map>
#include <algorithm>

#include "Device.hpp"

typedef void (*portal_channel_receive_cb_t)(char *buffer, int buffer_len, void *user_data);

namespace portal
{

class PortalDelegate
{
  public:
    virtual void portalDeviceDidReceivePacket(std::vector<char> packet) = 0;
    virtual ~PortalDelegate(){};
};

class Portal : public ChannelDelegate, public std::enable_shared_from_this<Portal>
{
  public:
    Portal();
    ~Portal();

    std::shared_ptr<Portal> getptr()
    {
        return shared_from_this();
    }

    int startListeningForDevices();
    void stopListeningForDevices();
    bool isListening();
    
    void disconnectAllDevices();
    void connectAllDevices();

    PortalDelegate *delegate;

  private:
    typedef std::map<int, Device::shared_ptr> DeviceMap;

    bool _listening;
    Portal::DeviceMap _devices;

    Portal(const Portal &other);
    Portal &operator=(const Portal &other);

    void addDevice(const usbmuxd_device_info_t &device);
    void removeDevice(const usbmuxd_device_info_t &device);

    friend void pt_usbmuxd_cb(const usbmuxd_event_t *event, void *user_data);

    void channelDidReceivePacket(std::vector<char> packet);
    void channelDidStop();
};

// Namespace
}
