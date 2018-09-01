# ofxOscQuery

## Introduction

This openFrameworks addon allows to create an OSCQuery server, based on the specifications available at: https://github.com/mrRay/OSCQueryProposal.
Basically it is a wrapper for [libossia](https://github.com/OSSIA/libossia) ([safeC++ implementation](https://ossia.github.io/?cpp--98)) but it follows OSC Query’s dialect instead of ossia’s.

It works in a similar way to ofxOscParameterSync from the ofxOsc addon: 
* declare an ofxOscQueryServer in your ofApp, 
* set it up by attaching it to an *ofParameterGroup*, and declaring OSC and WS ports. 
* All children *ofParameter*s and *ofParameterGroup*s will then be scanned recursively, exposed to the OSCQuery protocol
* libossia will manage all network communication and queries from then on.
* supplementary OSCQuery attributes can be set to the OSCQuery nodes attached to *ofParameter*s and *ofParameterGroup*s

This is all demonstrated in **example-singleServer** - further documentation is included in the source, and in particular in ofxOssiaNode, which is the core of it all.

Several Servers can be set up in the same ofApp by attaching them to several *ofParameterGroup*s (see **example-twoServers**). As mentioned in the Roadmap below, this might change soon to using one pool object for holding several servers.

## OSCQuery Clients

Several client examples are provided in the **example-singleServer** folder: for pure-data, Max, and with a simple html example.

It's also possible to view the ofApp's namespace as JSON by opening a browser (preferably FireFox for pretty formatting), by opening the URL: [http://yourServersIP:port/](http://localhost:5678/) - the default port (e.g. in **example-singleServer**  being 5678).

One can also download mrRay's (from VidVox) nice [OSCQueryBrowser](https://docs.vidvox.net/freebies_oscquery_browser.html) for more advanced browsing.

Of course it really gets interesting when trying to control your ofApp, which you can do with those OSCQuery-supporting sequencers:
- [ossia score](https://ossia.io) 
- [Vezér](https://imimot.com/vezer/) 

## Compatibility

* Works with of 0.10 - for 0.9.8, see specific legacy release  
* Tested on macOS and Ubuntu 
* ARM (aka raspberry Pi) and Win (VS) support should come soon
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

