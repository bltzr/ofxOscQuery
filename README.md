# ofxOscQuery

## Introduction

This openFrameworks addon allows to create an OSCQuery server, based on the specifications available at: https://github.com/mrRay/OSCQueryProposal.
Basically it is a wrapper for [libossia](https://github.com/OSSIA/libossia) ([safeC++ implementation](https://ossia.github.io/?cpp--98)). 

It works in a similar way to ofxOscParameterSync from the ofxOsc addon: 
* declare an ofxOscQueryServer in your ofApp, 
* set it up by attaching it to an *ofParameterGroup*, and declaring OSC and WS ports. 
* All children *ofParameter*s and *ofParameterGroup*s will then be scanned recursively, exposed to the OSCQuery protocol
* libossia will manage all network communication and queries from then on.
* supplementary OSCQuery attributes can be set to the OSCQuery nodes attached to *ofParameter*s and *ofParameterGroup*s

This is all demonstrated in **example-singleServer** 

Several Servers can be set up in the same ofApp by attaching them to several *ofParameterGroup*s (see **example-twoServers**)

## Dependencies 

* Tested on macOS and Ubuntu only for now

* a compiled version of libossia is included in the release 
    

