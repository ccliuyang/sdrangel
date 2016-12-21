///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "samplesourcefifo.h"

SampleSourceFifo::SampleSourceFifo(uint32_t size, uint32_t samplesChunkSize) :
    m_size(size),
    m_samplesChunkSize(samplesChunkSize),
    m_init(false)
{
    assert(samplesChunkSize <= m_size/4);
    m_data.resize(2*m_size);
    init();
}


SampleSourceFifo::~SampleSourceFifo()
{}

void SampleSourceFifo::resize(uint32_t size, uint32_t samplesChunkSize)
{
    qDebug("SampleSourceFifo::resize: %d, %d", size, samplesChunkSize);
    assert(samplesChunkSize <= size/4);

    m_size = size;
    m_samplesChunkSize = samplesChunkSize;
    m_data.resize(2*m_size);
    init();
}

void SampleSourceFifo::init()
{
    memset(&m_data[0], 0, sizeof(2*m_size*sizeof(Sample)));
    m_ir = 0;
    m_iw = m_samplesChunkSize*2;
    m_init = true;
}

void SampleSourceFifo::readAdvance(SampleVector::iterator& readUntil, unsigned int nbSamples)
{
//    QMutexLocker mutexLocker(&m_mutex);
    assert(nbSamples < m_samplesChunkSize/2);

    m_ir = (m_ir + nbSamples) % m_size;
    readUntil =  m_data.begin() + m_size + m_ir;
    emit dataRead(nbSamples);

    int i_delta = m_iw - m_ir;

    if (m_init)
    {
        emit dataWrite(m_size);
        m_init = false;
    }
    else if (i_delta > 0)
    {
        if (i_delta <= m_size/2) // m_samplesChunkSize)
        {
            emit dataWrite(m_size/2);
        }
    }
    else
    {
        if (i_delta + m_size <= m_size/2) //m_samplesChunkSize)
        {
            emit dataWrite(m_size/2);
        }
    }
}

void SampleSourceFifo::write(const Sample& sample)
{
    m_data[m_iw] = sample;
    m_data[m_iw+m_size] = sample;

    {
//        QMutexLocker mutexLocker(&m_mutex);
        m_iw = (m_iw+1) % m_size;
    }
}

void SampleSourceFifo::getReadIterator(SampleVector::iterator& readUntil)
{
    readUntil = m_data.begin() + m_size + m_ir;
}

void SampleSourceFifo::getWriteIterator(SampleVector::iterator& writeAt)
{
    writeAt = m_data.begin() + m_iw;
}

void SampleSourceFifo::bumpIndex(SampleVector::iterator& writeAt)
{
    m_data[m_iw+m_size] = m_data[m_iw];

    {
//        QMutexLocker mutexLocker(&m_mutex);
        m_iw = (m_iw+1) % m_size;
    }

    writeAt = m_data.begin() + m_iw;
}
