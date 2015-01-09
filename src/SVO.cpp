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
 * @file SVO.cpp
 * @brief Implementation of the QML wrapper for a Semi-direct Visual Odometry object
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-01-09
 */

#include "SVO.h"

SVO::SVO(QQuickItem *parent) :
    QQuickItem(parent)
{
    cam = new vk::PinholeCamera(752, 480, 315.5, 315.5, 376.0, 240.0);
    vo = new svo::FrameHandlerMono(cam);
    vo->start();
    timer.start();
}

SVO::~SVO()
{
}

void SVO::setSourceImage(QVariant sourceImage)
{
    vo->addImage(sourceImage.value<cv::Mat>(), timer.elapsed()/1000.0);
    Sophus::SE3 tf = vo->lastFrame()->T_f_w_;
    qDebug() << tf.translation()[0] << " " << tf.translation()[1] << " " << tf.translation()[2];
    qDebug() << tf.unit_quaternion().w() << " " << tf.unit_quaternion().x() << " " << tf.unit_quaternion().y() << " " << tf.unit_quaternion().z();
    qDebug() << "***************************";
}

