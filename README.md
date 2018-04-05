# ofxOscQuery

## Introduction

This openFrameworks addon allows to create an OSCQuery server, based on the specifications available at: https://github.com/mrRay/OSCQueryProposal
Basically it is a wrapper for the [libossia](https://github.com/OSSIA/libossia) [safeC++ implementation](https://ossia.github.io/?cpp--98). 

It works in a similar way to ofxOscParameterSync from the ofxOsc addon: 
* declare an ofxOscQuery in your ofApp, 
* set it up by attaching it to an *ofParameterGroup*, and declaring OSC and WS ports. 
* It will then scan for all children *Parameter*s and *ParameterGroup*s recursively, expose them to the OSCQuery protocol and manage all network communication and queries from then on.

Several Servers can be set up in the same ofApp by attaching them to several *ParameterGroup*s

## Dependencies 

* Tested on macOS only for now

* a compiled version of libossia is included in the release
    

