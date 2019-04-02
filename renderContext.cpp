#include "renderContext.h"

RenderContext::RenderContext() {
    //ctor
}

RenderContext::RenderContext(unsigned int width, unsigned int height, unsigned int channels): Bitmap(width, height, channels)
{
    m_scan_buffer = new int[height * 2];
}

void RenderContext::initialize(unsigned int width, unsigned int height, unsigned int channels) {
    Bitmap::initialize(width, height, channels);
    m_scan_buffer = new int[height * 2];
}

void RenderContext::drawScanBuffer(int yCoord, int xMin, int xMax) {
    m_scan_buffer[yCoord * 2] = xMin;
    m_scan_buffer[yCoord * 2 + 1] = xMax;
}

void RenderContext::fillShape(int yMin, int yMax) {
    for(int i = yMin; i < yMax; i++) {
        int xMin = m_scan_buffer[i * 2];
        int xMax = m_scan_buffer[i * 2 + 1];

        for(int j = xMin; j < xMax; j++) {
            drawPixel(j, i, 255, 255, 255);
        }
    }
}

RenderContext::~RenderContext()
{
    //dtor
}
