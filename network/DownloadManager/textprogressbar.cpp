#include "textprogressbar.h"
#include <QByteArray>
#include <cstdio>

using namespace  std;

void TextProgressBar::clear()
{
    printf("\n");

    fflush(stdout);

    value = 0;
    maximum = -1;
    iteration = 0;

}


void TextProgressBar::update()
{
    ++iteration;

    if(maximum > 0) {
        //我们知道最大
        //绘制进度栏
        int percent = value * 100 / maximum;
        int hashes = percent / 2;

        QByteArray progressbar(hashes, '#');
        if(percent % 2)
            progressbar += '>';

        printf("\r[%-50s] %3d%% %s    ",
               progressbar.constData(),
               percent,
               qPrintable(message));

    } else  {
        //我们不知道最大的，所以我们不能画进度栏
        int center = (iteration % 48) +1;//50空格，减2
        QByteArray before(qMax(center - 2, 0), ' ');
        QByteArray after(qMin(center + 2, 50), ' ');

        printf("\r[%s###%s]     %s    ",
               before.constData(), after.constData(), qPrintable(message));
    }
}


void TextProgressBar::setMessage(const QString &m)
{
    message = m;
}

void TextProgressBar::setStatus(qint64 val, qint64 max)
{
    value = val;
    maximum = max;
}
