//
//  ofxOssiaServer.cpp
//  ofxOSSIA
//
//  Created by Pascal Baltazar in 04/2018
//
//

#include "ofxOscQueryServer.h"


void ofxOscQueryServer::setup(ofParameterGroup& group,
                           int localportOSC, int localPortWS,
                           std::string localname)
{

  if(localname=="") localname = group.getName();

  // declare a distant program as an OSCQuery device
  device.setup(localname, localportOSC, localPortWS);
  nodes.emplace_back(device, group);

  // Then build ossia tree up from the chosen parameterGroup
  buildTreeFrom(group, nodes.front());

}

void ofxOscQueryServer::buildTreeFrom(ofParameterGroup& group, ofxOssiaNode& node)
{

  // Traverse all children recursively and create Nodes for each of them
  for(std::size_t i = 0; i < group.size(); i++){
    string type = group.get(i).type();

    if(type==typeid(ofParameterGroup).name()){

      // Create a Node for this Group at the end of the list
      nodes.emplace_back(node, group.getGroup(i));

      // Create sub-tree for Group
      buildTreeFrom(group.getGroup(i), nodes.back());

    } else {  // This is a Parameter

      // Check parameter type, and create Node accordingly:
      if(type == typeid(ofParameter <int32_t>).name())
        nodes.emplace_back(node, group.get<int32_t>(i));
      else if(type == typeid(ofParameter <int>).name())
        nodes.emplace_back(node, group.get<int>(i));
      else if(type == typeid(ofParameter <float>).name())
        nodes.emplace_back(node, group.get<float>(i));
      else if(type == typeid(ofParameter <double>).name())
        nodes.emplace_back(node, group.get<double>(i));
      else if(type == typeid(ofParameter <bool>).name())
        nodes.emplace_back(node, group.get<bool>(i));
      else if(type == typeid(ofParameter <ofVec2f>).name())
        nodes.emplace_back(node, group.get<ofVec2f>(i));
      else if(type == typeid(ofParameter <ofVec3f>).name())
        nodes.emplace_back(node, group.get<ofVec3f>(i));
      else if(type == typeid(ofParameter <ofVec4f>).name())
        nodes.emplace_back(node, group.get<ofVec4f>(i));
      else if(type == typeid(ofParameter <ofColor>).name())
        nodes.emplace_back(node, group.get<ofColor>(i));
      else if(type == typeid(ofParameter <ofFloatColor>).name())
        nodes.emplace_back(node, group.get<ofFloatColor>(i));
      else if(type == typeid(ofParameter <std::string>).name())
        nodes.emplace_back(node, group.get<std::string>(i));
      else{ ofLogWarning() <<
           "ofxBaseGroup; no support for parameter of type " << type; break; }

    }

  }

}

ofxOssiaNode& ofxOscQueryServer::findNode(string targetPath)
{
  std::string tPath = targetPath;
  if (targetPath.back()!='/') tPath=tPath+'/';
  if (targetPath.front()!='/') tPath='/'+tPath;
  auto found = find_if(nodes.begin(), nodes.end(),
       [&](ofxOssiaNode& n){return n.getPath()==tPath;});
  if (found!=nodes.end()) return *found;
  return getRootNode();
}


ofxOssiaNode& ofxOscQueryServer::findNode(ofAbstractParameter& targetParam)
{
  auto found = find_if(nodes.begin(), nodes.end(),
       [&](ofxOssiaNode& n) {return (n.getParam()->isReferenceTo(targetParam));});
  if (found!=nodes.end()) return *found;
  return getRootNode();
}





