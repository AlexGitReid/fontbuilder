/**
 * Copyright (c) 2010-2010 Andrey AndryBlack Kunitsyn
 * email:support.andryblack@gmail.com
 *
 * Report bugs and download new versions at http://code.google.com/p/fontbuilder
 *
 * This software is distributed under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "vertlayouter.h"
#include "../rendererdata.h"
#include "../layoutdata.h"

VertLayouter::VertLayouter(QObject *parent) :
    AbstractLayouter(parent)
{
}


void VertLayouter::PlaceImages(const QVector<LayoutChar>& chars) {
    if (chars.isEmpty()) return;
    int num_c = chars.size();
    int min_y = chars.front().y;
    int max_y = chars.front().y + chars.front().h;
    int min_x = chars.front().x;
    int max_x = chars.front().x + chars.front().w;
    foreach (const LayoutChar& c, chars) {
        if (c.y<min_y)
            min_y = c.y;
        if ((c.y+c.h)>max_y)
            max_y = c.y+c.h;
        if (c.x<min_x)
            min_x = c.x;
        if ((c.x+c.w)>max_x)
            max_x = c.x+c.w;
    }
    resize(max_x-min_x,(max_y-min_y)*num_c);
    //int x = 0;
    int y = 0;
    foreach (const LayoutChar& c, chars) {
        LayoutChar l = c;
        l.x = (c.x-min_x);
        l.y = (c.y-min_y)+y;
        place(l);
        y+=(max_y-min_y);
    }
}


AbstractLayouter* VertLayouterFactoryFunc (QObject* parent) {
    return new VertLayouter(parent);
}

