#include "slider.h"

Slider::Slider(QWidget *parent):
    QFrame(parent),
    av_slideDelay(10),
    av_slideSpeed(5),
    av_slideVStep(0),
    av_slideHStep(0),
    myParent(parent),
    pageTotal(0)
{
    at_slideUp = new QTimer(this);
    at_slideDown = new QTimer(this);
    at_slideLeft = new QTimer(this);
    at_slideRight = new QTimer(this);
    connect(at_slideUp,SIGNAL(timeout()),this,SLOT(as_slideUp()));
    connect(at_slideDown,SIGNAL(timeout()),this,SLOT(as_slideDown()));
    connect(at_slideLeft,SIGNAL(timeout()),this,SLOT(as_slideLeft()));
    connect(at_slideRight,SIGNAL(timeout()),this,SLOT(as_slideRight()));

    this->autoSize();
    av_slideSpeed = parent->geometry().width()/60;
}

void Slider::slide(uint8_t slide_direction)
{
    QRect pgeom = this->myParent->geometry();
    this->displayW = pgeom.width();
    this->displayH = pgeom.height();
    switch (slide_direction)
    {
        case SLIDE_DIRECTION_DOWN:  /*--= SCROLL_DIRECTION_UP */
            if (this->av_slideVStep<=-this->pageTotal+1)
                break;
            this->av_slideVStep += SLIDE_DOWN;
            this->at_slideDown->start(this->av_slideDelay);
            break;
        case SLIDE_DIRECTION_UP:    /*--= SCROLL_DIRECTION_UP //*/
            if (this->av_slideVStep>=0)
                break;
            this->av_slideVStep += SLIDE_UP;
            this->at_slideUp->start(this->av_slideDelay);
            break;
        case SLIDE_DIRECTION_LEFT:  /*--= SCROLL_DIRECTION_RIGHT //*/
            if (this->av_slideHStep<=-this->pageTotal+1)
                break;
            this->av_slideHStep += SLIDE_LEFT;
            this->at_slideLeft->start(this->av_slideDelay);
            break;
        case SLIDE_DIRECTION_RIGHT: /*--= SCROLL_DIRECTION_LEFT //*/
            if (this->av_slideHStep>=0)
                break;
            this->av_slideHStep += SLIDE_RIGHT;
            this->at_slideRight->start(this->av_slideDelay);
            break;
        default:
            break;
    }
}
void Slider::scroll(uint8_t scroll_direction)
{
    this->slide(scroll_direction);
}

void Slider::as_slideUp()
{
    QRect geom = this->geometry();
    if (geom.top() > (this->av_slideVStep*this->displayH))
    {
        this->setGeometry(
            geom.left(),
            geom.top()-this->av_slideSpeed,
            geom.width(),
            geom.height()
        );
    }
    else
        this->at_slideUp->stop();
}

void Slider::as_slideDown()
{
    QRect geom = this->geometry();
    if (geom.top() < (this->av_slideVStep*this->displayH))
    {
        this->setGeometry(
            geom.left(),
            geom.top()+this->av_slideSpeed,
            geom.width(),
            geom.height()
        );
    }
    else
        this->at_slideDown->stop();
}

void Slider::as_slideLeft()
{
    QRect geom = this->geometry();
    if (geom.left() > (this->av_slideHStep*this->displayW))
    {
        this->setGeometry(
            geom.left()-this->av_slideSpeed,
            geom.top(),
            geom.width(),
            geom.height()
        );
    }
    else
        this->at_slideLeft->stop();
}

void Slider::as_slideRight()
{
    QRect geom = this->geometry();
    if (geom.left() < (this->av_slideHStep*this->displayW))
    {
        this->setGeometry(
            geom.left()+this->av_slideSpeed,
            geom.top(),
            geom.width(),
            geom.height()
        );
    }
    else
        this->at_slideRight->stop();
}

void Slider::autoSize()
{
    QList<QFrame*> widgets = this->findChildren<QFrame*>();
    int mostx = -1;
    int mosty = -1;
    int curw = -1;
    int curh = -1;
    int i;
    for (i=0;i<widgets.count();i++)
    {
        QFrame* page = widgets.at(i);
        QRect cgeom = page->geometry();
        if (cgeom.left()>mostx)
        {
            mostx = cgeom.left();
            curw = cgeom.width();
        }
        if (cgeom.top()>mosty)
        {
            mosty = cgeom.top();
            curh = cgeom.height();
        }
    }
    this->setGeometry(0,0,mostx+curw,mosty+curh);
}

void Slider::addPage(int8_t append_direction, QFrame* page)
{
    page->setParent(this);
    if (append_direction==APPEND_RIGHT_MOST)
    {
        page->setGeometry(
            this->geometry().width(),
            0,
            myParent->geometry().width(),
            myParent->geometry().height()
        );
    }
    else if (append_direction==APPEND_BOTTOM_MOST)
    {
        page->setGeometry(
            0,
            this->geometry().height(),
            myParent->geometry().width(),
            myParent->geometry().height()
        );
    }
    page->show();
    this->autoSize();
    pageTotal += 1;
}

void Slider::setSlideDelay(int delay_ms)
{
    av_slideDelay = delay_ms;
}
void Slider::setSlideSpeed(int speed_px)
{
    av_slideSpeed = speed_px;
}

bool Slider::isFirstPage()
{
    if (av_slideVStep>=0 || av_slideHStep>=0)
        return true;
    return false;
}
bool Slider::isLastPage()
{
    if (av_slideVStep<=(-pageTotal+1) || av_slideHStep<=(-pageTotal+1))
        return true;
    return false;
}
