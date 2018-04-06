#pragma once

#include <ossia-cpp98.hpp>
#include "ofxOssiaTypes.h"
#include "ofxOssiaServer.h"


/*
 * Class encapsulating ossia node, parent_node and ofAbstractParameter*
 * */

class ofxOssiaNode {
  public:

    ofxOssiaNode() = default;

    /*
   *Constructor for the Root Node
   * */

    ofxOssiaNode(opp::oscquery_server& dev, ofParameterGroup& group):
      currentNode{dev.get_root_node()},
      ofParam{&group},
      path{"/"}
    {}

    /*
   *Constructor for ParameterGroup Nodes
   * */

    ofxOssiaNode(ofxOssiaNode& parentNode, ofParameterGroup& group):
      currentNode{parentNode.getNode().create_child(group.getName())},
      ofParam{&group},
      path{getHierarchy()}
    {}

    /*
   *Constructor for all Parameter Nodes
   * */
    template<typename DataValue>
    ofxOssiaNode(ofxOssiaNode& parentNode, ofParameter<DataValue>& param):
      currentNode{ossia::MatchingType<DataValue>::create_parameter(param.getName(), parentNode.getNode())},
      ofParam{&param},
      path{getHierarchy()+param.getName()}
    {
      using ossia_type = ossia::MatchingType<DataValue>;

      std::cout << "Create Param " << param.getName() << " from  parent: " << parentNode.getName() << " at address: " << path << "\n";

      std::cout << "Min: " << param.getMin() << " / Max: " << param.getMax() << endl;
      //sets value
      currentNode.set_value(ossia_type::convert(param.get()));

      //sets domain
      currentNode.set_min(ossia_type::convert(param.getMin()));
      currentNode.set_max(ossia_type::convert(param.getMax()));

      //adds callback from ofParameter to ossia Node
      param.addListener(this, &ofxOssiaNode::listen<DataValue>);

      //adds callback from ossia Node to ofParameter
      currentNode.set_value_callback([](void* context, const opp::value& val)
      {
        ofParameter<DataValue>* self = reinterpret_cast<ofParameter<DataValue>*>(context);
        //using value_type = const typename ossia_type::ossia_type;
        if(ossia_type::is_valid(val))
        {
          DataValue data = ossia_type::convertFromOssia(val);
          if(data != self->get())
          {
            self->set(data);
          }
        }
        else
        {
          std::cerr << "error [ofxOscQuery::enableRemoteUpdate()] : of and ossia types do not match \n" ;
          // Was: "<< (int) val.getType()  << " " << (int) ossia_type::val << "\n" ;
          return;
        }
      },  ofParam);

    }

    /*
   * Copy operations
   * */
    ofxOssiaNode(const ofxOssiaNode&) = default;
    ofxOssiaNode(ofxOssiaNode&&) = default;
    ofxOssiaNode& operator=(const ofxOssiaNode&) = default;
    ofxOssiaNode& operator=(ofxOssiaNode&&) = default;

    /*
   * Destructor
   * */
    ~ofxOssiaNode (){}

    /*
   * Utilities
   * */
    std::string getName() {return ofParam->getName();}
    std::string getPath() {return path;}

    //For callbacks
    template<typename DataValue>
    void listen(DataValue &data)
    {
      // check if the value to be published is not already published
      if(pullNodeValue<DataValue>() != data)
      { // i-score->GUI OK
        publishValue(data);
      }
    }


  private:

    opp::node currentNode;
    ofAbstractParameter* ofParam = nullptr;
    std::string path;
    opp::callback_index _callbackIt;



    //******************************************//
    //            Private Methods               //
    //******************************************//

    template<typename DataValue>
    void publishValue(DataValue val){
      using ossia_type = ossia::MatchingType<DataValue>;
      currentNode.set_value(ossia_type::convert(val));
    }

    template<typename DataValue>
    DataValue pullNodeValue()
    {
      using ossia_type = ossia::MatchingType<DataValue>;

      try
      {
        auto val = currentNode.get_value();
        if(ossia_type::is_valid(val))
          return ossia_type::convertFromOssia(val);
        else
          std::cerr <<  "error [ofxOssia::pullNodeValue()] : of and ossia types do not match \n" ; // Was:
        // <<(int) val.getType()  << " " << (int) ossia_type::val << "\n" ; // Can we still do that with safeC++ ??
        return {};
      }
      catch(std::exception& e)
      {
        std::cerr <<  "error [ofxOssia::pullNodeValue()] : " << e.what() << "\n" ;
        return {};

      }

      catch(...)
      {
        auto val = currentNode.get_value();
        std::cerr <<  "error [ofxOssia::pullNodeValue()] : : of and ossia types do not match \n" ; // Was:
        // << ossia::value_to_pretty_string(val)  << " " << (int) ossia_type::val << "\n" ; // Can we still do that with safeC++ ??
        return {};
      }
    }

    opp::node& getNode() {return currentNode;}

    std::string getHierarchy(){

      std::string address = "";

      if(!(*ofParam).isSerializable()) return address;

      if(ofParam->type()==typeid(ofParameterGroup).name()){
        address = "/";
        const std::vector<std::string> hierarchy = ofParam->getGroupHierarchyNames();
        for(size_t i=0;i<hierarchy.size()-1;i++){
          address+=hierarchy[i] + "/";
        }

      }else{
        address = "";
        const std::vector<std::string> hierarchy = ofParam->getGroupHierarchyNames();
        for(size_t i=0;i<hierarchy.size()-1;i++){
          address+= "/" + hierarchy[i];
        }
        if(address.length()) address += "/";
        //if (ofParam->getName() !="")
      }
      return address;
    }


};
