#ifndef SEQDRIVER_H
#define SEQDRIVER_H

#include <QWidget>
#include <QList>
#include <QSocketNotifier>
#include <alsa/asoundlib.h>

#include "midiarp.h"
#include "midilfo.h"
#include "main.h"

class SeqDriver : public QWidget {
    
  Q_OBJECT
  
  private:
    int portCount;
    QList<MidiArp *> *midiArpList; 
    QList<MidiLfo *> *midiLfoList; 
    QSocketNotifier *seqNotifier;
    snd_seq_t *seq_handle;
    int clientid;
    int portid_out[MAX_PORTS];
	int lfoMinPacketSize, lfoPacketSize[20];
    int portid_in;
    int queue_id;
    bool startQueue;
    snd_seq_tick_time_t tick;
    QList<int> sustainBufferList;
    int firstArpTick, lastLfoTick[20], nextLfoTick;
	int lfoCCnumber;
	QList<LfoSample> lfoData;
	
  protected: 
    int midiTime;
    int midiclock_tpb, mute_cnumber;
	bool midi_mutable;
    double m_ratio;
    int sustain;
	
  public:
    bool discardUnmatched, runQueueIfArp, runArp;
    int portUnmatched;
    int tempo;
    int grooveTick, grooveVelocity, grooveLength;
    bool use_midiclock;
	
  private:
    void initSeqNotifier();

  public:
    SeqDriver(QList<MidiArp*> *p_midiArpList, 
				QList<MidiLfo *> *p_midiLfoList, QWidget* parent=0);
    ~SeqDriver();
    void registerPorts(int num);
    int getPortCount();
    void initArpQueue();
    snd_seq_tick_time_t get_tick();
    void setQueueStatus(bool run);
    
  signals:
    void midiEvent(snd_seq_event_t *ev);
    void nextStep(snd_seq_tick_time_t tick);

  public slots:
    void procEvents(int fd);
    void setDiscardUnmatched(bool on);
    void setPortUnmatched(int id);
    void setQueueTempo(int bpm);
    void runQueue(bool);
    void setGrooveTick(int);
    void setGrooveVelocity(int);
    void setGrooveLength(int);
    void sendGroove();
    void setUseMidiClock(bool on);
    void updateMIDItpb(int midiTpb);
	void setMidiMutable(bool on);
    void updateCnumber(int cnumber);
};
                              
#endif
