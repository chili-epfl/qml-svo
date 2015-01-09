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
 * @file SVO.h
 * @brief QML wrapper for a Semi-direct Visual Odometry Object
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-01-09
 */

#ifndef SVO_H
#define SVO_H

#include<QQuickItem>
#include<QMatrix4x4>
#include<QMetaType>
#include<QMetaObject>

#include<opencv2/core/core.hpp>

Q_DECLARE_METATYPE(cv::Mat)

/**
 * @brief QML wrapper for a Semi-direct Visual Odometry object
 */
class SVO : public QQuickItem {
Q_OBJECT
    Q_PROPERTY(QVariant sourceImage WRITE setSourceImage)

public:

    /**
     * @brief Creates a new SVO QML wrapper
     *
     * @param parent Parent of this QML item
     */
    explicit SVO(QQuickItem *parent = 0);

    /**
     * @brief Destroys this SVO QML wrapper
     */
    virtual ~SVO();

public slots:

    /**
     * @brief Pushes a new image to be processed
     *
     * @param sourceImage Must contain a cv::Mat that is e.g the latest camera image
     */
    void setSourceImage(QVariant sourceImage);

signals:

private slots:

private:

};

#endif // SVO_H
