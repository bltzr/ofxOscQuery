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

class ofxOssiaServer {

public:
    ofxOssiaServer():
        device(){
        updatingParameter = false;
    }

    ~ofxOssiaServer(){

    }

    /**
     * setup for ossia Device:
     * Create a root node at the specified ParameterGroup
     * with optional specific ports for OSC and WS (default 1234 & 5678)
     *  and optionnal device name  (defaults to the ParameterGroup's name)
     **/
    void setup(ofParameterGroup & group,
               int localportOSC= 1234, int localPortWS=5678,
               std::string localname = "");

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


private:
    opp::oscquery_server device;
    std::list<ofxOssiaNode> nodes;

    bool updatingParameter;

};


