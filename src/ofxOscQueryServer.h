#pragma once
#undef Status
#undef Bool
#undef bool
#undef False
#undef status
#undef None
#include <ossia-cpp98.hpp>
#include "ofxOssiaNode.h"
#include "ofxOssiaTypes.h"
#include <types/ofParameter.h>
#include <iostream>
#include <list>

#define DEFAULT_OSC 1234
#define DEFAULT_WS  5678
#define DEFAULT_NAME "ofxOscQuery"

class ofxOscQueryServer {

  public:
    
    ofxOscQueryServer(int localportOSC = DEFAULT_OSC,
                      int localPortWS  = DEFAULT_WS,
                      std::string name = DEFAULT_NAME):
      OSCport(localportOSC), WSport(localPortWS), serverName(name),
      device(name, localportOSC, localPortWS){
    }
    
    ~ofxOscQueryServer() = default;

    /**
     * setup for ossia Device:
     * Create a root node at the specified ParameterGroup
     * with optional specific ports for OSC and WS (default 1234 & 5678)
     * and optionnal device name  (defaults to the ParameterGroup's name)
     **/
    void setup(ofParameterGroup & group, int localportOSC = DEFAULT_OSC, int localPortWS = DEFAULT_WS, std::string localname = "");

    /**
     * Build tree from a specific ofParameterGroup/ossia::node pair
     * scans for children and create subnodes accordingly
     **/
    void buildTreeFrom(ofParameterGroup& group, ofxOssiaNode& node);

    /**
     * Address-space utilities:
     **/
    opp::oscquery_server& getDevice(){return device;}

    ofxOssiaNode& getRootNode(){ return nodes.front();}

    // Find a specific node by:
    // - path (ossia, relative to the server)
    ofxOssiaNode& findNode(std::string targetPath);
    // - or ofParameter reference
    ofxOssiaNode& findNode(ofAbstractParameter& targetParam);
    // When no corresponding node is found, the reference to the root node
    // (aka serverName.getRootNode()) is returned

  private:
    opp::oscquery_server device;
    std::string serverName;
    int OSCport, WSport;
    std::list<ofxOssiaNode> nodes;
    
    friend class ofxOssiaNode;

};


