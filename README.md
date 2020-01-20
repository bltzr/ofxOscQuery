# ofxOscQuery

## Introduction

This openFrameworks addon allows to create an OSCQuery server, based on the specifications available at: https://github.com/Vidvox/OSCQueryProposal.


What will this addon do ? 

It will expose all of your `ofParameter`s (and `ofPArameterGroup`s) as a tree of OSC nodes, create OSC senders and receivers, and implement an OSCQuery server in order to manage queries and replies.  


Basically it is a wrapper for [libossia](https://github.com/OSSIA/libossia) ([safeC++ API](https://ossia.github.io/?cpp--98)) but it follows OSC Query’s dialect instead of ossia’s. Though, as does ossia, it covers all the features from the OSCQuery specifications (including listening, and notification for path changes).

Here is [a video showing the addon in use with two other apps (VDMX and Vezér)](https://vimeo.com/288203374)



This addon works in a similar way to ofxOscParameterSync from the ofxOsc addon: 
* declare an ofxOscQueryServer in your ofApp, 
* set it up by attaching it to an *ofParameterGroup*, and declaring OSC and WS ports. 
* All children *ofParameter*s and *ofParameterGroup*s will then be scanned recursively, exposed to the OSCQuery protocol
* libossia will manage all network communication and queries from then on.
* supplementary OSCQuery attributes can be set to the OSCQuery nodes attached to *ofParameter*s and *ofParameterGroup*s

Here is [a video showing how to add OSCQuery support to one of of's "vanilla" examples](https://vimeo.com/288202877) 

This is all demonstrated in **example-singleServer** - further documentation is included in the source, and in particular in ofxOssiaNode, which is the core of it all.

Several Servers can be set up in the same ofApp by attaching them to several *ofParameterGroup*s (see **example-twoServers**). As mentioned in the Roadmap below, another option will soon be added (without breaking the current usage) for managing this better: by using one pool object for holding several servers. Stay tuned!

## Installation

1. Download the latest .zip from the [releases](https://github.com/bltzr/ofxOscQuery/releases).
2. Unzip and move the directory to the openFrameworks addon directory.

## OSCQuery Clients

Several client examples are provided in the **example-singleServer** folder: for pure-data, Max, and with a simple html example.

It's also possible to view the ofApp's namespace as JSON by opening a browser (preferably FireFox for pretty formatting), by opening the URL: [http://yourServersIP:port/](http://localhost:5678/) - the default port (e.g. in **example-singleServer**  being 5678).

One can also download mrRay's (from VidVox) nice [OSCQueryBrowser](https://docs.vidvox.net/freebies_oscquery_browser.html) for more advanced browsing.

Of course it really gets interesting when trying to control your ofApp, which you can do with those OSCQuery-supporting sequencers:
- [ossia score](https://ossia.io) 
- [Vezér](https://imimot.com/vezer/) 

## Compatibility

* Works with of 0.10 - for 0.9.8, see specific legacy release  
* 0.9.8 version works with ofNode (aka with cmake), 1.0.0 doesn't since ofNode hasn't been ported to 0.10 (yet)
* Available for:
  - macOS, 
  - Linux (tested on Ubuntu and archlinux, should work on other distros with gcc >= 6)
  - Windows (32 and 64 bits) 
* ARM (aka raspberry Pi) should come soon
* CodeBlocks support for Win should be doable if one wants to investigate on that - Please feel free to ask for help and guidance in the issue, and as always, PRs are welcome!

## Dependencies 

A compiled version of [libossia](https://ossia.github.io/?cpp--98#introduction) is included in the release. 

## Roadmap

- add a, more generic, ofxOscQuery object, that acts as a pool of servers, and allows to add them dynamically 
- make the OSC Query namespace dynamic (i.e. adding OSC nodes when adding `ofParameter`s and `ofParameterGroup`s)
  -  first manually by adding a method to add specific nodes (should come soon)
  -  then automatically by adding some listener to `ofParameterGroup` when a new child `ofParameter` is created

## Version history

### Version 1.0 (09/2018):
Operational version, complying with the official OSC Query specs.
- updated with the final 1.0 version of the OSC Query specifications
- updated for of 0.10
- methods have been added to manage OSC Query Attributes 
- the `findNode` method has been replaced by subscripting 

## version 0.9.8 (09/2018)
Legacy release for openframworks 0.9.8 - Same as version 1.0, except that it will build in 0.9.8 
    
### Version 0.1 (04/2018):
Initial version of the addon, with only basic functionality

