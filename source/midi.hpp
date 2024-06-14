
#ifndef __MIDI_HPP_H_
#define __MIDI_HPP_H_
#include <iostream>
#include <cstdlib>
#include "RtMidi.h"


typedef void (*callback)(std::vector<unsigned char> *);

callback cb;
RtMidiIn *midiin = 0;

void mycallback( double deltatime, std::vector< unsigned char > *message, void */*userData*/ )
{
    cb(message);
}

bool chooseMidiPort( RtMidiIn *rtmidi );
RtMidi::Api chooseMidiApi();
void midi_cleanup();

int setup_main(callback cbk)
{
  cb = cbk;

  try {

    midiin = new RtMidiIn(chooseMidiApi());
    if ( chooseMidiPort( midiin ) == false ) {
        midi_cleanup();
        return 0;
    }
    midiin->setCallback( &mycallback );
    midiin->ignoreTypes( false, false, false );
    
  } catch ( RtMidiError &error ) {
    error.printMessage();
  }
    return 0;
}

void midi_cleanup() {
  if(midiin != NULL)
      delete midiin;
    midiin = NULL;
}

bool chooseMidiPort( RtMidiIn *rtmidi )
{
  rtmidi->openVirtualPort();
  std::string portName;
  unsigned int i = 0, nPorts = rtmidi->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No input ports available!" << std::endl;
    return false;
  }

  if ( nPorts == 1 ) {
    std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
  }
  else {
    for ( i=0; i<nPorts; i++ ) {
      portName = rtmidi->getPortName(i);
      std::cout << "  Input port #" << i << ": " << portName << '\n';
    }

    do {
      std::cout << "\nChoose a port number: ";
      std::cin >> i;
    } while ( i >= nPorts );
      std::string keyHit;
    std::getline( std::cin, keyHit );  // used to clear out stdin
  }

  rtmidi->openPort( i );

  return true;
}

RtMidi::Api chooseMidiApi()
{
  std::vector< RtMidi::Api > apis;
  RtMidi::getCompiledApi( apis );

  if (apis.size() <= 1)
      return RtMidi::Api::UNSPECIFIED;

  std::cout << "\nAPIs\n  API #0: unspecified / default\n";
  for (size_t n = 0; n < apis.size(); n++)
      std::cout << "  API #" << apis[n] << ": " << RtMidi::getApiDisplayName(apis[n]) << "\n";

  std::cout << "\nChoose an API number: ";
  unsigned int i;
  std::cin >> i;

  std::string dummy;
  std::getline( std::cin, dummy );  // used to clear out stdin

  return static_cast< RtMidi::Api >( i );
}

#endif
