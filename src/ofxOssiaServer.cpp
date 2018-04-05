//
//  ofxOssiaServer.cpp
//  ofxOSSIA
//
//  Created by Pascal Baltazar in 04/2018
//
//

#include "ofxOssiaServer.h"


void ofxOssiaServer::setup(ofParameterGroup& group,
                   int localportOSC, int localPortWS,
                   std::string localname)
{

    if(localname=="") localname = group.getName();

    cout << "Setup device " <<  localname << " for ParameterGroup: " << group.getName() << " with ports OSC: " << localportOSC << " & WS: " << localPortWS << "\n";


    // declare a distant program as an OSCQuery device
    device.setup(localname, localportOSC, localPortWS);
    nodes.emplace_back(device, group);
    //nodes.emplace_back(device.get_root_node(), group);

    // Then build ossia tree up from the chosen parameterGroup
    buildTreeFrom(group, nodes.front());

    //ofAddListener(serverGroup.parameterChangedE(),this,&ofxOssiaServer::parameterChanged);
}

void ofxOssiaServer::buildTreeFrom(ofParameterGroup& group, ofxOssiaNode& node)
{
    cout << endl << "Build tree from group: " << group.getName()
         << " -> group size: " << group.size() << endl;

    // Traverse all children recursively and create Nodes for each of them
    for(std::size_t i = 0; i < group.size(); i++){
        string type = group.get(i).type();

        if(type==typeid(ofParameterGroup).name()){

            cout << "Group: ";
            //groupgetHierarchy(.get(i)); // Just a log

            // Create a Node for this Group at the end of the list
            nodes.emplace_back(node, group.getGroup(i));

            // Create sub-tree for Group
            buildTreeFrom(group.getGroup(i), nodes.back());


        } else {  // This is a Parameter

            cout << "Param: ";


            // Check parameter type:
            if(type == typeid(ofParameter <int32_t> ).name())
                nodes.emplace_back(node, group.getInt(i));
            else if(type == typeid(ofParameter <float> ).name())
                nodes.emplace_back(node, group.getFloat(i));
            else if(type == typeid(ofParameter <double> ).name())
                nodes.emplace_back(node, group.get<double>(i));
            else if(type == typeid(ofParameter <bool> ).name())
                nodes.emplace_back(node, group.getBool(i));
            else if(type == typeid(ofParameter <ofVec2f> ).name())
                nodes.emplace_back(node, group.getVec2f(i));
            else if(type == typeid(ofParameter <ofColor> ).name())
                nodes.emplace_back(node, group.getColor(i));
            else{   ofLogWarning() << "ofxBaseGroup; no support for parameter of type " << type; break; }

            //ofxOssiaNode<typeName> n{node, group.getName()};
            //nodes.push_back(std::move(n));

            //getHierarchy(group.get(i)); // Just a log


        }

    }
    cout << "Finished Group: " << group.getName() << endl << endl;

}


/*
 * Callback method for changed methods (probably useless)

void ofxOssiaServer::parameterChanged( ofAbstractParameter & parameter ){
    if(updatingParameter) return;
    // TODO: implement sending via ossia:
    //sender.sendParameter(parameter);
    cout << '\n' << parameter.getName() << " has changed \n";
    if(!parameter.isSerializable()) return;
    getHierarchy(parameter);
}
*/





