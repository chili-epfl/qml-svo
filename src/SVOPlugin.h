/*
 * Copyright (C) 2014 EPFL
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

/**
 * @file SVOPlugin.h
 * @brief Object that exposes the SVO plugin components as QML objects
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-01-09
 */

#ifndef SVOPLUGIN_H
#define SVOPLUGIN_H

#include <QQmlExtensionPlugin>
#include <qqml.h>

#include "SVO.h"

/**
 * @brief Object that exposes the SVO plugin as QML objects
 */
class SVOPlugin : public QQmlExtensionPlugin
{
Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // SVOPLUGIN_H
