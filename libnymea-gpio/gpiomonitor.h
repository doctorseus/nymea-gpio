/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by
* copyright law, and remains the property of nymea GmbH. All rights, including
* reproduction, publication, editing and translation, are reserved. The use of
* this project is subject to the terms of a license agreement to be concluded
* with nymea GmbH in accordance with the terms of use of nymea GmbH, available
* under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; version 3. This project is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this project. If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under
* contact@nymea.io or see our FAQ/Licensing Information on
* https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GPIOMONITOR_H
#define GPIOMONITOR_H

#include <QMutex>
#include <QThread>
#include <QObject>

#include "gpio.h"

class GpioMonitor : public QThread
{
    Q_OBJECT
public:
    explicit GpioMonitor(int gpio, QObject *parent = nullptr);
    ~GpioMonitor() override;

    int gpioNumber() const;

    Gpio::Edge edge() const;
    void setEdge(Gpio::Edge edge);

    bool activeLow() const;
    void setActiveLow(bool activeLow);

    Gpio::Value value();

    bool enabled() const;

private:
    int m_gpioNumber = -1;
    Gpio::Edge m_edge = Gpio::EdgeBoth;
    bool m_activeLow = true;
    bool m_enabled = false;

    // Thread stuff
    QMutex m_valueMutex;
    Gpio::Value m_value = Gpio::ValueInvalid;

    QMutex m_stopMutex;
    bool m_stop = false;

    void setValue(Gpio::Value value);
    void setEnabled(bool enabled);

protected:
    void run() override;

signals:
    void interruptOccurred(bool value);
    void enabledChanged(bool enabled);

private slots:
    void onThreadStarted();
    void onThreadFinished();

public slots:
    bool enable();
    void disable();

};

#endif // GPIOMONITOR_H
