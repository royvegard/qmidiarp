/*!
 * @file arpscreen.h
 * @brief Header for the ArpScreen class
 *
 * @section LICENSE
 *
 *      Copyright 2009, 2010, 2011, 2012 <qmidiarp-devel@lists.sourceforge.net>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 *
 */
#ifndef ARPSCREEN_H
#define ARPSCREEN_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QTimer>
#include <QSizePolicy>
#include <QSize>

#include "midilfo.h"

#define ARPSCR_MIN_W    250
#define ARPSCR_MIN_H    120
#define ARPSCR_VMARG    10
#define ARPSCR_HMARG    16




/*! @brief Drawing widget for visualization of arp patterns using QPainter
 *
 * ArpScreen is created and embedded by ArpWidget. The painter callback
 * analyses the pattern string and produces a streak map of its content
 * similar to a piano roll display. The display is updated
 * by calling ArpScreen::updateScreen() with the pattern text string as
 * and argument. A cursor is placed at the corresponding pattern index
 * by calling ArpScreen::updateScreen() with the integer current pattern
 * index as an overloaded member.
 */
class ArpScreen : public QWidget
{
  Q_OBJECT

  private:
    QTimer *timer;
    int grooveTick, grooveVelocity, grooveLength;
    QString pattern;
    int pattern_updated, currentIndex;
    bool isMuted;
    bool needsRedraw;

  protected:
    virtual void paintEvent(QPaintEvent *);

  public:
    ArpScreen(QWidget* parent=0);
    ~ArpScreen();
    virtual QSize sizeHint() const;
    virtual QSizePolicy sizePolicy() const;
    int maxOctave;
    int minOctave;
    double minStepWidth;
    double nSteps;
    int patternMaxIndex;

  public slots:
    void updateScreen(const QString& pattern, int p_minOct, int p_maxOct,
                            double p_minStepWidth, double p_nSteps,
                            int p_patternMaxIndex);
    void updateScreen(int p_index);
    void newGrooveValues(int tick, int vel, int length);
    void setMuted(bool on);
    void updateDraw();
};

#endif
