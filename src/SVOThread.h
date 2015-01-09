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
 * @file SVOThread.h
 * @brief Runs SVO in a separate thread
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-01-09
 */

#ifndef SVOTHREAD_H
#define SVOTHREAD_H

#include<QDebug>
#include<QThread>
#include<QObject>
#include<QElapsedTimer>
#include<QMutex>
#include<QWaitCondition>

#include<opencv2/core.hpp>

#include<string>
#include<map>

#include<svo/frame_handler_mono.h>

//Q_DECLARE_METATYPE(chilitags::Chilitags3D_<qreal>::TagPoseMap)

/**
 * @brief Object that contains the svo::FrameHandlerMono::addImage() loop and its parameters
 */
class SVOTask : public QObject{
Q_OBJECT

public:

    /**
     * @brief Creates a new SVO task
     *
     * @param vo Visual odometry estimator, must be already initialized
     */
    SVOTask(svo::FrameHandlerMono* vo);

    /**
     * @brief Destroys this SVO task, does not touch the camera or the frame handler
     */
    virtual ~SVOTask();

    /**
     * @brief Presents a new frame to be processed by SVO
     *
     * If the thread is waiting for a frame, presents the frame directly.
     * If the thread is busy, copies the frame as the next frame to be processed.
     * Either way, does not busy wait and returns immediately.
     *
     * @param frame The new frame
     */
    void presentFrame(cv::Mat frame);

    /**
     * @brief Asks for the main loop to stop
     */
    void stop();

public slots:

    /**
     * @brief Continuously processes the most recent frame
     */
    void doWork();

signals:

    /**
     * @brief Emitted when svo::FrameHandlerMono::addImage() returns
     */
    //void tagsReady(chilitags::Chilitags3D_<qreal>::TagPoseMap tags);

private:

#ifdef QT_DEBUG
    const float FPS_RATE = 0.9f;            ///< Rate of using the older FPS estimates
    const int FPS_PRINT_PERIOD = 500;       ///< Period of printing the FPS estimate, in milliseconds
#endif

    /**
     * @brief Thread's possible states
     */
    enum State{
        NONE,               ///< Invalid state, new frames will not be presented
        BUSY,               ///< Currently busy with svo::FrameHandlerMono::addImage()
        WAITING_FOR_FRAME   ///< Waiting for a new frame
    };

    svo::FrameHandlerMono* vo;          ///< Visual odometry estimator
    QElapsedTimer timer;                ///< Calculates the timestamp that will be presented to SVO
    double timestamp = 0.0f;            ///< Timestamp that will be presented to SVO

    QMutex frameLock;                   ///< Mutex that locks the frame transaction
    QWaitCondition nextFrameCond;       ///< Condition to wait on until the next frame arrives
    State state = NONE;                 ///< Thread's current state

    //TODO: PREALLOCATE A BUFFER FOR THIS!!!!!
    cv::Mat nextFrame;                  ///< The frame that will be processed next

    bool nextFrameAvailable = false;    ///< Whether the next frame is ready and in place
    bool running = false;               ///< Whether the should keep running, we don't need a mutex for this
};

/**
 * @brief Object that starts and stops the loop that calls svo::FrameHandlerMono::addImage()
 */
class SVOThread : public QObject{
Q_OBJECT

public:

    /**
     * @brief Creates a new SVO thread controller
     *
     * @param vo Visual odometry estimator, must be already initialized
     */
    SVOThread(svo::FrameHandlerMono* vo);

    /**
     * @brief Destroys this SVO thread controller
     */
    virtual ~SVOThread();

    /**
     * @brief Starts the SVO loop
     */
    void start();

    /**
     * @brief Asks the SVO loop to stop and waits until it stops
     */
    void stop();

public slots:

    /**
     * @brief Presents a new frame to be processed by the visual odometry estimator
     *
     * If the thread is waiting for a frame, presents the frame directly.
     * If the thread is busy, copies the frame as the next frame to be processed.
     * Either way, does not busy wait and returns immediately.
     *
     * @param frame The new frame
     */
    void presentFrame(cv::Mat frame);

signals:

    /**
     * @brief Emitted when svo::FrameHandlerMono::addImage() returns
     */
    //void tagsReady(chilitags::Chilitags3D_<qreal>::TagPoseMap tags);

private:

    QThread workerThread;           ///< The thread that SVO will work in
    SVOTask* task = nullptr;        ///< The loop method and parameter container

};

#endif /* SVOTHREAD_H */

