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
 * @file SVOThread.cpp
 * @brief Runs SVO in a separate thread
 * @author Ayberk Özgür
 * @version 1.0
 * @date 2015-01-09
 */

#include "SVOThread.h"

//*****************************************************************************
// SVOTask implementation
//*****************************************************************************

SVOTask::SVOTask(svo::FrameHandlerMono* vo)
{
    this->vo = vo;
    vo->start();
    timer.start();
}

SVOTask::~SVOTask()
{
}

void SVOTask::presentFrame(cv::Mat frame)
{
    timestamp = timer.elapsed()/1000.0f;

    frameLock.lock();
    switch(state){

        //Invalid state, do not present
        case NONE:
            break;

        //Currently processing, copy buffer over the next frame
        case BUSY:
            nextFrame = frame.clone(); //TODO: We're screwed if this thing does not memcpy the buffer behind the curtains
            nextFrameAvailable = true;
            break;

        //Currently waiting, do not copy buffer but present directly
        case WAITING_FOR_FRAME:
            nextFrame = frame;
            nextFrameAvailable = true;
            nextFrameCond.wakeAll();
            break;
    }
    frameLock.unlock();
}

void SVOTask::stop()
{
    running = false;
    nextFrameCond.wakeAll();
}

void SVOTask::doWork()
{
    running = true;

#ifdef QT_DEBUG
    QElapsedTimer fpsTimer;
    float fps = 0.0f;
    int millisElapsed = 0;
    int millis;
    fpsTimer.start();
#endif

    frameLock.lock();
    while(running){

        //Process frame if possible
        if(nextFrameAvailable){

            //Signal that we're consuming the next frame now
            nextFrameAvailable = false;
            state = BUSY;

            //Unlock the lock so that we can present a new frame while processing the image
            frameLock.unlock();

            vo->addImage(nextFrame, timestamp);
            if(vo->lastFrame() != NULL)
                qDebug() << vo->lastProcessingTime()*1000 << " ms; quality: " << vo->trackingQuality();
            else
                qDebug() << "LAST FRAME IS NULL!!!";
            frameLock.lock();

            //emit tagsReady(tags);
        }

        //Wait for next frame
        state = WAITING_FOR_FRAME;
        //frameLock.unlock() is performed by wait below
        nextFrameCond.wait(&frameLock);
        //frameLock.lock() is performed by wait above

#ifdef QT_DEBUG
        millis = (int)fpsTimer.restart();
        millisElapsed += millis;
        fps = FPS_RATE*fps + (1.0f - FPS_RATE)*(1000.0f/millis);
        if(millisElapsed >= FPS_PRINT_PERIOD){
            qDebug("SVO is running at %f FPS",fps);
            millisElapsed = 0;
        }
#endif
    }
    state = NONE;
    frameLock.unlock();
}

//*****************************************************************************
// SVOThread implementation
//*****************************************************************************

SVOThread::SVOThread(svo::FrameHandlerMono* vo)
{
    task = new SVOTask(vo);
    task->moveToThread(&workerThread);
    connect(&workerThread, SIGNAL(started()), task, SLOT(doWork()));
    //connect(
    //        task, SIGNAL(tagsReady(chilitags::Chilitags3D_<qreal>::TagPoseMap)),
    //        this, SIGNAL(tagsReady(chilitags::Chilitags3D_<qreal>::TagPoseMap)));
}

SVOThread::~SVOThread()
{
    stop();
    delete task;
}

void SVOThread::start()
{
    workerThread.start();
}

void SVOThread::stop()
{
    if(task != nullptr)
        task->stop();
    workerThread.quit();
    workerThread.wait();
}

void SVOThread::presentFrame(cv::Mat frame)
{
    if(task != nullptr)
        task->presentFrame(frame);
}

