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
    cam = new vk::PinholeCamera(640, 480, 356.0, 356.0, 320.0, 240.0);
    vo = new svo::FrameHandlerMono(cam);
    thread = new SVOThread(vo);
    thread->start();
}

SVO::~SVO()
{
    thread->stop();
    delete thread;
    delete vo;
    delete cam;
}

void SVO::setSourceImage(QVariant sourceImage)
{
    cv::Mat yuvimg = sourceImage.value<cv::Mat>();
    cv::Mat grayimg;
    cv::cvtColor(yuvimg, grayimg, cv::COLOR_YUV2GRAY_NV21);
    thread->presentFrame(grayimg);
}

