/**
 * @file midicontrol.cpp
 * @brief Implements the MidiControl QWidget class.
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
 */

#include <QString>
#include <QStringList>
#include "midicontrol.h"

MidiControl::MidiControl(const QStringList &p_names)
{
    ID = 0;
    parentDockID = 0;
    names = p_names;
    // QSignalMappers allow identifying signal senders for MIDI learn/forget
    learnSignalMapper = new QSignalMapper(this);
    connect(learnSignalMapper, SIGNAL(mapped(int)),
             this, SLOT(midiLearn(int)));

    forgetSignalMapper = new QSignalMapper(this);
    connect(forgetSignalMapper, SIGNAL(mapped(int)),
             this, SLOT(midiForget(int)));

    // we need the cancel MIDI Learn action only once for all
    cancelMidiLearnAction = new QAction(tr("Cancel MIDI &Learning"), this);
    connect(cancelMidiLearnAction, SIGNAL(triggered()), this, SLOT(midiLearnCancel()));
    cancelMidiLearnAction->setEnabled(false);
    ccList.clear();
    modified = false;
}

MidiControl::~MidiControl()
{
}

void MidiControl::appendMidiCC(int controlID, int ccnumber, int channel, int min, int max)
{
    MidiCC midiCC;
    int l1 = 0;
    midiCC.name = names.at(controlID);
    midiCC.ID = controlID;
    midiCC.ccnumber = ccnumber;
    midiCC.channel = channel;
    midiCC.min = min;
    midiCC.max = max;

    while ( (l1 < ccList.count()) &&
        ((controlID != ccList.at(l1).ID) ||
        (ccnumber != ccList.at(l1).ccnumber) ||
        (channel != ccList.at(l1).channel)) ) l1++;

    if (ccList.count() == l1) {
        ccList.append(midiCC);
        qWarning("MIDI Controller %d appended for %s"
        , ccnumber, qPrintable(midiCC.name));
    }
    else {
        qWarning("MIDI Controller %d already attributed to %s"
                , ccnumber, qPrintable(midiCC.name));
    }

    cancelMidiLearnAction->setEnabled(false);
    modified = true;
}

bool MidiControl::isModified()
{
    return modified;
}

void MidiControl::setModified(bool m)
{
    modified = m;
}

void MidiControl::removeMidiCC(int controlID, int ccnumber, int channel)
{
    for (int l1 = 0; l1 < ccList.count(); l1++) {
        if (ccList.at(l1).ID == controlID) {
            if (((ccList.at(l1).ccnumber == ccnumber)
                    && (ccList.at(l1).channel == channel))
                    || (0 > channel)) {
                ccList.remove(l1);
                l1--;
                qWarning("controller removed");
            }
        }
    }
    modified = true;
}

void MidiControl::midiLearn(int controlID)
{
    emit setMidiLearn(parentDockID, ID, controlID);
    qWarning("Requesting Midi Learn for %s", qPrintable(names.at(controlID)));
    cancelMidiLearnAction->setEnabled(true);
}

void MidiControl::midiForget(int controlID)
{
    removeMidiCC(controlID, 0, -1);
}

void MidiControl::midiLearnCancel()
{
    emit setMidiLearn(parentDockID, ID, -1);
    qWarning("Cancelling Midi Learn request");
    cancelMidiLearnAction->setEnabled(false);
}

void MidiControl::addMidiLearnMenu(QWidget *widget, int count)
{
    widget->setContextMenuPolicy(Qt::ContextMenuPolicy(Qt::ActionsContextMenu));
    QAction *learnAction = new QAction(tr("MIDI &Learn"), this);
    widget->addAction(learnAction);
    connect(learnAction, SIGNAL(triggered()), learnSignalMapper, SLOT(map()));
    learnSignalMapper->setMapping(learnAction, count);

    QAction *forgetAction = new QAction(tr("MIDI &Forget"), this);
    widget->addAction(forgetAction);
    connect(forgetAction, SIGNAL(triggered()), forgetSignalMapper, SLOT(map()));
    forgetSignalMapper->setMapping(forgetAction, count);

    widget->addAction(cancelMidiLearnAction);
}

void MidiControl::readData(QXmlStreamReader& xml)
{
    int controlID, ccnumber, channel, min, max;
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isEndElement())
            break;
        if (xml.isStartElement() && (xml.name() == "MIDICC")) {
            controlID = xml.attributes().value("CtrlID").toString().toInt();
            ccnumber = -1;
            channel = -1;
            min = -1;
            max = -1;
            while (!xml.atEnd()) {
                xml.readNext();
                if (xml.isEndElement())
                    break;
                if (xml.name() == "ccnumber")
                    ccnumber = xml.readElementText().toInt();
                else if (xml.name() == "channel")
                    channel = xml.readElementText().toInt();
                else if (xml.name() == "min")
                    min = xml.readElementText().toInt();
                else if (xml.name() == "max")
                    max = xml.readElementText().toInt();
                else skipXmlElement(xml);
            }
            if ((-1 < ccnumber) && (-1 < channel) && (-1 < min) && (-1 < max))
                appendMidiCC(controlID, ccnumber, channel, min, max);
            else qWarning("Controller data incomplete");
        }
        else skipXmlElement(xml);
    }
}
void MidiControl::writeData(QXmlStreamWriter& xml)
{
    xml.writeStartElement("midiControllers");
    for (int l1 = 0; l1 < ccList.count(); l1++) {
        xml.writeStartElement("MIDICC");
        xml.writeAttribute("CtrlID", QString::number(ccList.at(l1).ID));
            xml.writeTextElement("ccnumber", QString::number(
                ccList.at(l1).ccnumber));
            xml.writeTextElement("channel", QString::number(
                ccList.at(l1).channel));
            xml.writeTextElement("min", QString::number(
                ccList.at(l1).min));
            xml.writeTextElement("max", QString::number(
                ccList.at(l1).max));
        xml.writeEndElement();
    }
    xml.writeEndElement();
}

void MidiControl::skipXmlElement(QXmlStreamReader& xml)
{
    if (xml.isStartElement()) {
        qWarning("Unknown Element in XML File: %s",qPrintable(xml.name().toString()));
        while (!xml.atEnd()) {
            xml.readNext();

            if (xml.isEndElement())
                break;

            if (xml.isStartElement()) {
                skipXmlElement(xml);
            }
        }
    }
}

void MidiControl::setCcList(const QVector<MidiCC> &p_ccList)
{
    ccList = p_ccList;
}