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
    
    /**
     @brief get this node's ofParameter object
     @return a pointer to this node's ofAbstractParameter object
     */
     ofAbstractParameter* getParam() {return ofParam;}

    //************************************//
    //           Manage attributes        //
    //************************************//

    ofxOssiaNode& setDescription(const std::string& attrVal) {getNode().set_description(attrVal); return *this;}

    ofxOssiaNode& setTags(std::vector<std::string> attrVal) {getNode().set_tags(attrVal); return *this; }

    //*********    Access mode:    ************//
    
    
    /**Access mode is a metadata that categorizes parameters between:
     *
     * - GET / RO : read-only
     * - SET / WO : write-only
     * -  BI / RW : read-write
     *
     * For instance:
     *
     * - The value of a vu-meter should be GET
     * - A "play" button should be SET.
     * - The cutoff of a filter or a controllable color should be BI.
     * @brief sets the access_mode attribute of this node's parameter
     * @param v an opp::access_mode of the chosen mode
     * @return a reference to this node
     */
    ofxOssiaNode& setAccess(std::string m){
        opp::access_mode mode;
        if      (m == "GET" || m == "RO") mode = opp::access_mode::Get;
        else if (m == "SET" || m == "WO") mode = opp::access_mode::Set;
        else if (m == "BI"  || m == "RW") mode = opp::access_mode::Bi;
        getNode().set_access(mode);
        return *this;
    }
    /**
     * @brief gets the access_mode attribute of this node's parameter
     * @return an opp::access_mode with this node's parameter's access mode
     * @see opp::node::set_acess
     */
    std::string getAccess() {
        opp::access_mode mode = getNode().get_access();
        std::string m;
        if      ( mode == opp::access_mode::Get ) (m = "GET" );
        else if ( mode == opp::access_mode::Set ) (m = "SET" );
        else if ( mode == opp::access_mode::Bi )  (m = "BI"  );
        return m;
    }
    
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
    /**
     * @brief gets a list of the values accepted by this node's parameter ("values" attribute)
     * @return a vector of values with the list of this node's parameter's accepted values
     * @see setValues
     * @see setBound
     */
    template<typename DataValue> std::vector<DataValue> getValues() {
        using ossia_type = ossia::MatchingType<DataValue>;
        auto vals = getNode().get_accepted_values();
        std::vector<DataValue> res;
        for (auto& v : vals) res.push_back(ossia_type::convertFromOssia(v));
        return res;
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
    
    /**
     * @brief gets the bounding_mode attribute of this node's parameter
     * @return a std::string with this node's parameter's access mode
     */
    std::string getBound() {
        auto mode = getNode().get_bounding();
        std::string res;
        if       (mode == opp::bounding_mode::Clip) res = "CLIP" ;
        else if  (mode == opp::bounding_mode::Low)  res = "LOW"  ;
        else if  (mode == opp::bounding_mode::High) res = "HIGH" ;
        else if  (mode == opp::bounding_mode::Wrap) res = "WRAP" ;
        else if  (mode == opp::bounding_mode::Fold) res = "FOLD" ;
        else if  (mode == opp::bounding_mode::Free) res = "FREE" ;
        return res;
    }
    
    //*********    Units:    ************//
    
    /**Units give a semantic meaning to the value of a parameter. <br>
     * Units are sorted by categories (coined "dataspace" ): every unit in a category is convertible to the other units in the same category. <br>
     * Every category has a neutral unit to/from which conversions are made. <br>
     *
     * An unit, when setting it as a parameter's attribute, can be expressed as a string in the form:<br>
     * - "category.unit" (such as "position.cart2D"),<br>
     * - only with the unit name (such as "cart2D", those being all unique), <br>
     * - or with "nicknames", that are indicated in parentheses, after the unit name <br>
     * A list of all supported units is given below.
     *
     * @brief sets the unit attribute of this node's parameter
     * @param v a string with this unit's name
     * @return a reference to this node
     */
    ofxOssiaNode& setUnit(const std::string& attrVal) {getNode().set_unit(attrVal); return *this;}
    /**< @details
     *
     * - **Position**
     *
     *   + **cart3D** (*xyz*, *pos*, *point*, *point3d*, *3d*, *cartesian3d*, *coord*, *coordinate*, *coordinates*, *pvector* *, *vertex*):
     * Cartesian 3-dimensional position (ie. X, Y, Z) in the OpenGL coordinate reference system
     *   + **cart2D** (*xy*, *complex*, *point2d*, *2d*, *cartesian2d*):
     * Cartesian 2-dimensional position (i.e. X, Y)
     *   + **opengl** (*gl*, *position.gl*):
     * Cartesian 3-dimensional position (ie. X, Y, Z) in the OpenGL coordinate reference system
     *   + **spherical** (*aed*):
     * Polar 3-dimensional position (ie. aed: amplitude, elevation, distance)
     *   + **polar** (*ad*):
     * Polar 2-dimensional position (ie. ad: amplitude, distance)
     *   + **cylindrical** (*daz*):
     * Mixed 3-dimensional position (ie. daz: distance, amplitude, Z)
     *
     *
     *
     * - **Orientation**
     *
     *   + **quaternion**:
     * An extension of the complex numbers for 3D orientation, in the form a+bi+cj+dk
     *   + **euler**:
     * A triplet of angles (in degrees) describing the orientation of a rigid body with respect to a fixed coordinate system
     *   + **axis**:
     * An angle (a, in degrees) relative to a 3-dimensional vector, expressed in the order X, Y, Z, a
     *
     *
     *
     * - **Color**
     *
     *   + **argb** (*col*):
     * 4 float numbers between 0. and 1. describing respectively Alpha, Red, Green and Blue color values
     *   + **rgba**:
     * 4 float numbers between 0. and 1. describing respectively Red, Green, Blue and Alpha color values
     *   + **rgb**:
     * 3 float numbers between 0. and 1. describing respectively Red, Green and Blue color values
     *   + **bgr**:
     * 3 float numbers between 0. and 1. describing respectively Blue, Green and Red color values
     *   + **argb8**:
     * 4 int numbers between 0 and 255 describing respectively Alpha, Red, Green and Blue color values
     *   + **hsv**:
     * 3 float numbers between 0. and 1. describing respectively Hue, Saturation and Value (Luminosity) color values in the * HSV colorspace
     *   + **cmy8**:
     * 3 int numbers between 0 and 255 describing respectively Cyan, Magenta, and Yellow color values
     *   + **cmyk8**:
     * 4 int numbers between 0 and 255 describing respectively Cyan, Magenta, Yellow and Black color values
     *
     * - **Angle**
     *
     *   + **radian**
     *   + **degree**
     *
     *
     * - **Distance**
     *
     *   + **meter**
     *   + **kilometer**
     *   + **decimeter**
     *   + **centimeter**
     *   + **millimeter**
     *   + **micrometer**
     *   + **nanometer**
     *   + **picometer**
     *   + **inch**
     *   + **foot**
     *   + **mile**
     *
     * - **Time**
     *
     *   + **second**
     *   + **bark**
     *   + **bpm**
     *   + **cent**
     *   + **frequency** (*freq*, *frequence*, *Hz*, *hz*, *Hertz*):
     *   + **mel**
     *   + **midi_pitch** (*midinote*):
     *   + **millisecond** (*ms*)
     *   + **playback_speed**
     *   + **sample** (the length of a sample, for a sample_rate of 44100Hz)
     *
     * - **Gain**
     *
     *   + **linear**:
     * A linear gain in the [0. 1.] range, with 1. being the nominal level
     *   + **midigain**:
     * A value in the [0 127] range mimicing a MIDI gain controller. 100 for the nominal level, 127 for +12dB
     *   + **decibel** (*db*, *dB*):
     * A single float value expressed in a logarithmic scale, typically to describe an audio gain (0dB being the nominal * gain, < 0dB describing a signal attenuation, clipped at -96dB)
     *   + **decibel_raw**
     * Same as deciBel, but unclipped.
     *
     * - **Speed**
     *
     *   + **meter_per_second**
     *   + **miles_per_hour**
     *   + **kilometer_per_hour**
     *   + **knot**
     *   + **foot_per_second**
     *   + **foot_per_hour**
     */
    
    /**
     * @brief gets the unit of this node's parameter
     * @return a string with this node's parameter's unit name
     * @see ossia::unit_t
     */
    std::string getUnit() {
        return getNode().get_unit();
    }
    
    
    
    
    
    //*********************//
    //     Constructors    //
    //*********************//

    
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

    opp::node& getNode()       {return currentNode;}

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
