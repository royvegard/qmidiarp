/*!
 * @file globstore.h
 * @brief Headers for the GlobStore UI class.
 *
 * @section LICENSE
 *
 *      Copyright 2009, 2010, 2011 <qmidiarp-devel@lists.sourceforge.net>
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
#ifndef GLOBSTORE_H
#define GLOBSTORE_H

#include <QAction>
#include <QBoxLayout>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QList>
#include <QSignalMapper>
#include <QString>

#include "indicator.h"

/*!
 * The GlobStore class is a small QGroupBox UI that allows storing
 * and restoring global parameters of all modules for QMidiArp.
 * It is instantiated by MainWindow.
 * It is shown in the bottom part of the MainWindow and is the central
 * widget for it.

 * @brief Global Parameter Storage UI. Instantiated by MainWindow.
 */
class GlobStore : public QGroupBox

{
  Q_OBJECT

  private:
    QSignalMapper *storeSignalMapper;
    QSignalMapper *restoreSignalMapper;
    QHBoxLayout* rowLayout;
    int activeStore;
    int currentRequest;

  public:
    GlobStore(QWidget* parent=0);
    ~GlobStore();

    QComboBox *timeModuleBox;
    QComboBox *timeModeBox;
    QComboBox *switchAtBeatBox;
    Indicator *indicator;
    QList<QWidget*> widgetList;
    int switchAtBeat;

/*!
* @brief This function creates and adds a new group of global parameter
*  storage and retrieval buttons
*/
    void add();

  signals:

  void globStore(int);
  void requestGlobRestore(int);
  void updateGlobRestoreTimeMode(const QString&);
  void removeParStores(int);

  public slots:
/*!
* @brief This function removes the group of global parameter
*  storage and retrieval buttons at index ix
*
* @param ix Index at which the group is removed, if ix is -1, the last
* group is removed
*/
    void remove(int ix = -1);
/*!
* @brief This function causes all module widgets to store their current
* parameters in a global parameter store list setup in each module widget.
*
* @param ix Index in the module store lists
*/
    void store(int ix);
/*!
* @brief This function causes all module widgets to set their parameters
* to those found at index ix in their global parameter store lists
*
* @param ix Index in the module store lists
*/
    void restore(int ix);
/*!
* @brief This function selects a module who, if its pattern
* reaches the end, will cause a global store selection to become active.
*
* It determines the type (Arp, LFO, Seq) and the index of the selected
* module the storage list held by Engine, and transfer these parameters
* to Engine.
*
* @param ix Index in the module window store list, i.e. the dockWidget list
*/
    void updateTimeModule(int ix);
    void updateTimeModeBox(int ix);
    void updateSwitchAtBeat(int ix);
    void setDispState(int ix, int selected);
};

#endif
