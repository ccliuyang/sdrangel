///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2020 Jon Beniston, M7RCE                                        //
// Copyright (C) 2020 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_FEATURE_GS232CONTROLLERGUI_H_
#define INCLUDE_FEATURE_GS232CONTROLLERGUI_H_

#include <QTimer>

#include "feature/featuregui.h"
#include "util/messagequeue.h"
#include "gs232controllersettings.h"

class PluginAPI;
class FeatureUISet;
class GS232Controller;

namespace Ui {
    class GS232ControllerGUI;
}

class GS232ControllerGUI : public FeatureGUI {
    Q_OBJECT
public:
    static GS232ControllerGUI* create(PluginAPI* pluginAPI, FeatureUISet *featureUISet, Feature *feature);
    virtual void destroy();

    void resetToDefaults();
    QByteArray serialize() const;
    bool deserialize(const QByteArray& data);
    virtual MessageQueue *getInputMessageQueue() { return &m_inputMessageQueue; }

private:
    Ui::GS232ControllerGUI* ui;
    PluginAPI* m_pluginAPI;
    FeatureUISet* m_featureUISet;
    GS232ControllerSettings m_settings;
    bool m_doApplySettings;

    GS232Controller* m_gs232Controller;
    MessageQueue m_inputMessageQueue;
    QTimer m_statusTimer;
    int m_lastFeatureState;

    explicit GS232ControllerGUI(PluginAPI* pluginAPI, FeatureUISet *featureUISet, Feature *feature, QWidget* parent = nullptr);
    virtual ~GS232ControllerGUI();

    void blockApplySettings(bool block);
    void applySettings(bool force = false);
    void displaySettings();
    void updateSerialPortList();
    void updateDeviceSetList();
    bool updateChannelList(); //!< true if channel index has changed
    bool handleMessage(const Message& message);

    void leaveEvent(QEvent*);
    void enterEvent(QEvent*);

private slots:
    void onMenuDialogCalled(const QPoint &p);
    void onWidgetRolled(QWidget* widget, bool rollDown);
    void handleInputMessages();
    void on_startStop_toggled(bool checked);
    void on_devicesRefresh_clicked();
    void on_device_currentIndexChanged(int index);
    void on_channel_currentIndexChanged(int index);
    void on_serialPort_currentIndexChanged(int index);
    void on_baudRate_currentIndexChanged(int index);
    void on_track_stateChanged(int state);
    void on_azimuth_valueChanged(int value);
    void on_elevation_valueChanged(int value);
    void updateStatus();
};


#endif // INCLUDE_FEATURE_GS232CONTROLLERGUI_H_
