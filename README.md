# ofxOscQuery

## Introduction

This openFrameworks addon allows to create an OSCQuery server, based on the specifications available at: https://github.com/Vidvox/OSCQueryProposal.
Basically it is a wrapper for [libossia](https://github.com/OSSIA/libossia) ([safeC++ implementation](https://ossia.github.io/?cpp--98)). 

It works in a similar way to ofxOscParameterSync from the ofxOsc addon: 
* declare an ofxOscQueryServer in your ofApp, 
* set it up by attaching it to an *ofParameterGroup*, and declaring OSC and WS ports. 
* All children *ofParameter*s and *ofParameterGroup*s will then be scanned recursively, exposed to the OSCQuery protocol
* libossia will manage all network communication and queries from then on.
* supplementary OSCQuery attributes can be set to the OSCQuery nodes attached to *ofParameter*s and *ofParameterGroup*s

This is all demonstrated in **example-singleServer** - further documentation is included in the source, and in particular in ofxOssiaNode, which is the core of it all.

Several Servers can be set up in the same ofApp by attaching them to several *ofParameterGroup*s (see **example-twoServers**)

## OSCQuery Clients

Several client examples are provided in the **example-singleServer** folder: for pure-data, Max, and with a simple html example. 

It's also possible to view the ofApp's namespace as JSON by opening a browser (preferably FireFox for pretty formatting), by opening the URL: [http://yourServersIP:port/](http://localhost:5678/) - the default port (e.g. in **example-singleServer**  being 5678).

One can also download @mrRay's nice [OSCQueryBrowser](https://docs.vidvox.net/freebies_oscquery_browser.html) for more advanced browsing.


Of course it really gets interesting when trying to control your ofApp, which you can do with those OSCQuery-supporting sequencers:
- [ossia score](https://ossia.io) 
- [Vezér](https://imimot.com/vezer/) 

If you’re using VDMX, you can also create an OSC Query client, and control your ofApp from there.


## Dependencies 

* Tested on macOS and Ubuntu only for now

* a compiled version of libossia is included in the release 

* The master branch is for of 0.9.8, there is a 0.10 branch with a few changes
    

