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
    
    //************************************//
    //           General utilities        //
    //************************************//
    
    /**
      @brief get this node's name
      @return this node's name as a string
     */
    std::string getName() {return ofParam->getName();}
    
    /**
     @brief get this node's OSC address, relative to the server
     @return this node's OSC address as a string
     */
    std::string getPath() {return path;}

    //************************************//
    //           Manage attributes        //
    //************************************//

    ofxOssiaNode& setUnit(const std::string& attrVal) {getNode().set_unit(attrVal); return *this;}
    // see https://ossia.github.io/?cpp--ofx#units for a list of units

    ofxOssiaNode& setDescription(const std::string& attrVal) {getNode().set_description(attrVal); return *this;}

    ofxOssiaNode& setTags(std::vector<std::string> attrVal) {getNode().set_tags(attrVal); return *this; }

    //*********    Domain:    ************//
    
    /**Domains allow to set a range of accepted values for a given parameter.<br>
     * This range can be continuous (between a min and max), or discrete:  a set of accepted values.<br>
     * This function defines the minimum of a continuous range.<br>
     * This is only meaningful for nodes with parameters of numerical types (ie ints, floats, vecnfs and some lists)<br>
     * @brief sets the 'min' attribute of this node's parameter (minimum value)
     * @param min a value with the desired minimum value
     * @return a reference to this node
     * @see setBound
     */
    template<typename DataValue>
    ofxOssiaNode& setMin(const DataValue& attrVal) {
        getNode().set_min(ossia::MatchingType<DataValue>::convert(attrVal));
        static_cast<ofParameter<DataValue>*>(ofParam)->setMin(attrVal);
        return *this;
    }

    /**
    * @brief gets the 'min' attribute of this node's parameter (minimum value)
    * @return a value with this node's parameter's minimum value
    * @see setMin
    * @see getBound
    */
    template<typename DataValue> DataValue getMin() {
        return ossia::MatchingType<DataValue>::convertFromOssia(getNode().get_min());
    }
    
    /**
     * This function defines the minimum of a continuous range.<br>
     * This is only meaningful for nodes with parameters of numerical types (ie ints, floats, vecnfs and some lists)<br>
     * @brief sets the 'max' attribute of this node's parameter (maximum value)
     * @param min a value with the desired mmaximum value
     * @return a reference to this node
     * @see opp::node::set_bounding
     */
    template<typename DataValue>
    ofxOssiaNode& setMax(const DataValue& attrVal) {
        getNode().set_max(ossia::MatchingType<DataValue>::convert(attrVal));
        static_cast<ofParameter<DataValue>*>(ofParam)->setMax(attrVal);
        return *this;
    }
    
    /**
     * @brief gets the 'max' attribute of this node's parameter (maximum value)
     * @return a value with this node's parameter's maximum value
     * @see setMax
     * @see getBound
     */
    template<typename DataValue> DataValue getMax() {
        return ossia::MatchingType<DataValue>::convertFromOssia(getNode().get_max());
    }
    
    /**Domains allow to set a range of accepted values for a given parameter.<br>
     * This function defines a set of accepted values.<br>
     * This is only meaningful for nodes with parameters of about any types except Impulse<br>
     * @brief sets a list of the values accepted by this node's parameter ("values" attribute)
     * @param v a std::vector of values with the desired list of accepted values
     * @return a reference to this node
     * @see opp::node::set_bounding
     */
    template<typename DataValue>
    ofxOssiaNode& setValues(const std::vector<DataValue>& attrVals) {
        using ossia_type = ossia::MatchingType<DataValue>;
        std::vector<opp::value> res;
        for (const auto & v : attrVals) { res.push_back(ossia_type::convert(v)); }
        getNode().set_accepted_values(res);
        return *this;
    }
    
    //*********    Bounding mode:    ************//
    
    
    /**The bounding mode tells what happens when a value is outside of the min / max:
     *
     * * **FREE** : no clipping; domain is only indicative.
     * * **CLIP** : clipped to the closest value in the range.
     * * **LOW** : only clips values lower than the min.
     * * **HIGH** : only clips values higher than the max.
     * * **WRAP** : wraps values around the range
     * * **FOLD** : folds back values into the range
     *
     * The default is **FREE**.
     * @brief sets the bounding_mode attribute of this node's parameter
     * @param v a string describing the chosen mode
     * @return a reference to this node
     * @see setMin
     * @see setMax
     */
    
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
    
    
    ofAbstractParameter* getParam() {return ofParam;}
    
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
    ~ofxOssiaNode () {
        if (callbackIt) currentNode.remove_value_callback(callbackIt);
        
        auto type = ofParam->type();
        if(type == typeid(ofParameter <int32_t>).name())
            static_cast<ofParameter<int32_t>*>(ofParam)->removeListener(this, &ofxOssiaNode::listen<int32_t>);
        else if(type == typeid(ofParameter <int>).name())
            static_cast<ofParameter<int>*>(ofParam)->removeListener(this, &ofxOssiaNode::listen<int>);
        else if(type == typeid(ofParameter <float>).name())
            static_cast<ofParameter<float>*>(ofParam)->removeListener(this, &ofxOssiaNode::listen<float>);
        else if(type == typeid(ofParameter <double>).name())
            static_cast<ofParameter<double>*>(ofParam)->removeListener(this, &ofxOssiaNode::listen<double>);
        else if(type == typeid(ofParameter <bool>).name())
            static_cast<ofParameter<bool>*>(ofParam)->removeListener(this, &ofxOssiaNode::listen<bool>);
        else if(type == typeid(ofParameter <ofVec2f>).name())
            static_cast<ofParameter<ofVec2f>*>(ofParam)->removeListener(this, &ofxOssiaNode::listen<ofVec2f>);
        else if(type == typeid(ofParameter <ofVec3f>).name())
            static_cast<ofParameter<ofVec3f>*>(ofParam)->removeListener(this, &ofxOssiaNode::listen<ofVec3f>);
        else if(type == typeid(ofParameter <ofVec4f>).name())
            static_cast<ofParameter<ofVec4f>*>(ofParam)->removeListener(this, &ofxOssiaNode::listen<ofVec4f>);
        else if(type == typeid(ofParameter <ofColor>).name())
            static_cast<ofParameter<ofColor>*>(ofParam)->removeListener(this, &ofxOssiaNode::listen<ofColor>);
        else if(type == typeid(ofParameter <ofFloatColor>).name())
            static_cast<ofParameter<ofFloatColor>*>(ofParam)->removeListener(this, &ofxOssiaNode::listen<ofFloatColor>);
        else if(type == typeid(ofParameter <std::string>).name())
            static_cast<ofParameter<std::string>*>(ofParam)->removeListener(this, &ofxOssiaNode::listen<std::string>);
    }
    
    

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
