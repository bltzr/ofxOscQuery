#pragma once

#include <ossia-cpp98.hpp>
#include "ofParameterGroup.h"
#include "ofxOssiaTypes.h"
#include "ofxOscQueryServer.h"

/*
 * Class encapsulating ossia node, parent_node and ofAbstractParameter*
 * Largely inspired from https://github.com/OSSIA/ofxOssia/blob/master/src/ParamNode.h
 * */

class ofxOssiaNode {
    
  public:

    // Methods to set ossia nodes' attributes:

    ofxOssiaNode& setUnit(const std::string& attrVal) {getNode().set_unit(attrVal); return *this;}
    // see https://ossia.github.io/?cpp--ofx#units for a list of units

    ofxOssiaNode& setDescription(const std::string& attrVal) {getNode().set_description(attrVal); return *this;}

    ofxOssiaNode& setTags(std::vector<std::string> attrVal) {getNode().set_tags(attrVal); return *this; }

    template<typename DataValue>
    ofxOssiaNode& setMin(const DataValue& attrVal) {
        getNode().set_min(ossia::MatchingType<DataValue>::convert(attrVal));
        static_cast<ofParameter<DataValue>*>(ofParam)->setMin(attrVal);
        return *this;
    }

    template<typename DataValue>
    ofxOssiaNode& setMax(const DataValue& attrVal) {
        getNode().set_max(ossia::MatchingType<DataValue>::convert(attrVal));
        static_cast<ofParameter<DataValue>*>(ofParam)->setMax(attrVal);
        return *this;
    }
    
    template<typename DataValue>
    ofxOssiaNode& setValues(const std::vector<DataValue>& attrVals) {
        using ossia_type = ossia::MatchingType<DataValue>;
        std::vector<opp::value> res;
        for (const auto & v : attrVals) { res.push_back(ossia_type::convert(v)); }
        getNode().set_accepted_values(res);
        return *this;
    }
    
    // possible values are: FREE, CLIP, LOW, HIGH, WRAP and FOLD
    ofxOssiaNode& setBound(const std::string& attrVal) {
        std::string a = attrVal;
        opp::bounding_mode mode;
        if      (a == "CLIP") mode = opp::bounding_mode::Clip;
        else if (a == "LOW" ) mode = opp::bounding_mode::Low;
        else if (a == "HIGH") mode = opp::bounding_mode::High;
        else if (a == "WRAP") mode = opp::bounding_mode::Wrap;
        else if (a == "FOLD") mode = opp::bounding_mode::Fold;
        else                  mode = opp::bounding_mode::Free;
        getNode().set_bounding(mode);
        return *this;
    }
    
    
    /*
   *    Constructors for the Root Node
   */


    ofxOssiaNode() = default;

    /*
   *Constructor for the Root Node
   */

    ofxOssiaNode(opp::oscquery_server& dev, ofParameterGroup& group):
      currentNode{dev.get_root_node()},
      ofParam{&group},
      path{"/"}
    {}

    /*
   *Constructor for ParameterGroup Nodes
   */

    ofxOssiaNode(ofxOssiaNode& parentNode, ofParameterGroup& group):
      currentNode{parentNode.getNode().create_child(group.getName())},
      ofParam{&group},
      path{parentNode.getPath()+currentNode.get_name()+"/"}
    {
      ofParam->setName(currentNode.get_name());
    }

    /*
   *Constructor for all Parameter Nodes
   * */
    template<typename DataValue>
    ofxOssiaNode(ofxOssiaNode& parentNode, ofParameter<DataValue>& param):
      currentNode{ossia::MatchingType<DataValue>::create_parameter(param.getName(), parentNode.getNode())},
      ofParam{&param},
      path{parentNode.getPath()+currentNode.get_name()+"/"}
    {
      using ossia_type = ossia::MatchingType<DataValue>;

      ofParam->setName(currentNode.get_name());

      //sets value
      currentNode.set_value(ossia_type::convert(param.get()));
      currentNode.set_default_value(ossia_type::convert(param.get()));

      //sets domain
      // cout << getName() << ": " << param.getMin() << " / " << param.getMax() << endl;
      currentNode.set_min(ossia_type::convert(param.getMin())); // TODO: fix this in ossia-cpp
      currentNode.set_max(ossia_type::convert(param.getMax())); // TODO: fix this in ossia-cpp

      //adds callback from ofParameter to ossia Node
      param.addListener(this, &ofxOssiaNode::listen<DataValue>);


      //adds callback from ossia Node to ofParameter
      callbackIt = currentNode.set_value_callback([](void* context, const opp::value& val)
      {
        ofParameter<DataValue>* self = reinterpret_cast<ofParameter<DataValue>*>(context);
          
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
    ~ofxOssiaNode (){
      if (callbackIt) currentNode.remove_value_callback(callbackIt);
      //removeParamListener();
    }

    /*
   * Utilities
   */
    std::string getName() {return ofParam->getName();}
    ofAbstractParameter* getParam() {return ofParam;}
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
    opp::callback_index callbackIt;
    friend class ofxOscQueryServer;



    //////////////////////////////////////////////
    //            Private Methods               //
    //////////////////////////////////////////////

    opp::node& getNode() {return currentNode;}

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
          if(ossia_type::is_valid(val)){
              return ossia_type::convertFromOssia(val);
          }
        else
          std::cerr <<  "error [ofxOssia::pullNodeValue()] : of and ossia types do not match \n" ;
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
        std::cerr <<  "error [ofxOssia::pullNodeValue()] : : of and ossia types do not match \n" ;
        return {};
      }
    }

};
