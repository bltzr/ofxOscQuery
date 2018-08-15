#pragma once
#include <ossia-cpp98.hpp>
#include <types/ofBaseTypes.h>
#include <math/ofVectorMath.h>
#include <string>
#include <array>

#undef Status
#undef Bool
#undef bool
#undef False
#undef status
#undef None

namespace ossia
{
/**
 * These classes contain the conversion mechanism from and to
 * the compatible OSSIA & OpenFrameworks types.
 * Copied from https://github.com/OSSIA/ofxOssia/blob/master/src/OssiaTypes.h
 *
 */
template<typename> struct MatchingType;

template<> struct MatchingType<float> {
    using ofx_type = float;
    using ossia_type = float;

    static opp::node create_parameter(const std::string& name,
                                      opp::node parent)
    {return parent.create_float(name);}

    static bool is_valid(opp::value v){ return v.is_float(); }

    static ofx_type convertFromOssia(const opp::value& v)
    {
      return v.to_float();
    }

    static ossia_type convert(ofx_type f)
    {
      return float(f);
    }
};


template<> struct MatchingType<int> {
    using ofx_type = int;
    using ossia_type = int;

    static opp::node create_parameter(const std::string& name,
                                      opp::node parent)
    {return parent.create_int(name);}

    static bool is_valid(opp::value v){ return v.is_int(); }

    static ofx_type convertFromOssia(const opp::value& v)
    {
      return v.to_int();
    }

    static ossia_type convert(ofx_type f)
    {
      return int(f);
    }
};


template<> struct MatchingType<bool> {
    using ofx_type = bool;
    using ossia_type = bool;

    static opp::node create_parameter(const std::string& name,
                                      opp::node parent)
    {return parent.create_bool(name);}

    static bool is_valid(opp::value v){ return v.is_bool(); }

    static ofx_type convertFromOssia(const opp::value& v)
    {
      return v.to_bool();
    }

    static ossia_type convert(ofx_type f)
    {
      return bool(f);
    }
};

template<> struct MatchingType<double> {
    using ofx_type = double;
    using ossia_type = float;

    static opp::node create_parameter(const std::string& name, opp::node parent)
    {return parent.create_float(name);}

    static bool is_valid(opp::value v){ return v.is_float(); }

    static ofx_type convertFromOssia(const opp::value& v)
    {
      return ofx_type(v.to_float());
    }

    static ossia_type convert(ofx_type f)
    {
      return float(f);
    }
};
    
template<> struct MatchingType<glm::vec2> {
    using ofx_type = glm::vec2;
    using ossia_type = opp::value::vec2f;
    
    static opp::node create_parameter(const std::string& name, opp::node parent)
    {return parent.create_vec2f(name);}
    
    static bool is_valid(opp::value v){ return v.is_vec2f(); }
    
    static ofx_type convertFromOssia(const opp::value& v)
    {
        return ofx_type(v.to_vec2f()[0], v.to_vec2f()[1]);
    }
    
    static ossia_type convert(ofx_type f)
    {
        return ossia_type{f.x, f.y};
    }
};

template<> struct MatchingType<glm::vec3> {
    using ofx_type = glm::vec3;
    using ossia_type = opp::value::vec3f;
    
    static opp::node create_parameter(const std::string& name, opp::node parent)
    {return parent.create_vec3f(name);}
    
    static bool is_valid(opp::value v){ return v.is_vec3f(); }
    
    static ofx_type convertFromOssia(const opp::value& v)
    {
        return ofx_type(v.to_vec3f()[0], v.to_vec3f()[1], v.to_vec3f()[2]);
    }
    
    static ossia_type convert(ofx_type f)
    {
        return ossia_type{f.x, f.y, f.z};
    }
};

template<> struct MatchingType<glm::vec4> {
    using ofx_type = glm::vec4;
    using ossia_type = opp::value::vec4f;
    
    static opp::node create_parameter(const std::string& name, opp::node parent)
    {return parent.create_vec4f(name);}
    
    static bool is_valid(opp::value v){ return v.is_vec4f(); }
    
    static ofx_type convertFromOssia(const opp::value& v)
    {
        return ofx_type(v.to_vec4f()[0], v.to_vec4f()[1], v.to_vec4f()[2], v.to_vec4f()[3]);
    }
    
    static ossia_type convert(ofx_type f)
    {
        return ossia_type{f.x, f.y, f.z, f.w};
    }
};
    
template<> struct MatchingType<ofVec2f> {
    using ofx_type = ofVec2f;
    using ossia_type = opp::value::vec2f;
    
    static opp::node create_parameter(const std::string& name, opp::node parent)
    {return parent.create_vec2f(name);}
    
    static bool is_valid(opp::value v){ return v.is_vec2f(); }
    
    static ofx_type convertFromOssia(const opp::value& v)
    {
        return ofx_type(v.to_vec2f()[0], v.to_vec2f()[1]);
    }
    
    static ossia_type convert(ofx_type f)
    {
        return ossia_type{f.x, f.y};
    }
};

template<> struct MatchingType<ofVec3f> {
    using ofx_type = ofVec3f;
    using ossia_type = opp::value::vec3f;
    
    static opp::node create_parameter(const std::string& name, opp::node parent)
    {return parent.create_vec3f(name);}
    
    static bool is_valid(opp::value v){ return v.is_vec3f(); }
    
    static ofx_type convertFromOssia(const opp::value& v)
    {
        return ofx_type(v.to_vec3f()[0], v.to_vec3f()[1], v.to_vec3f()[2]);
    }
    
    static ossia_type convert(ofx_type f)
    {
        return ossia_type{f.x, f.y, f.z};
    }
};

template<> struct MatchingType<ofVec4f> {
    using ofx_type = ofVec4f;
    using ossia_type = opp::value::vec4f;
    
    static opp::node create_parameter(const std::string& name, opp::node parent)
    {return parent.create_vec4f(name);}
    
    static bool is_valid(opp::value v){ return v.is_vec4f(); }
    
    static ofx_type convertFromOssia(const opp::value& v)
    {
        return ofx_type(v.to_vec4f()[0], v.to_vec4f()[1], v.to_vec4f()[2], v.to_vec4f()[3]);
    }
    
    static ossia_type convert(ofx_type f)
    {
        return ossia_type{f.x, f.y, f.z, f.w};
    }
};

template<> struct MatchingType<ofColor> {
    using ofx_type = ofColor;
    using ossia_type = opp::value::vec4f;
    
    static opp::node create_parameter(const std::string& name, opp::node parent)
    {return parent.create_rgba(name);}
    
    static bool is_valid(opp::value v){ return v.is_vec4f(); }
    
    static ofx_type convertFromOssia(const opp::value& v)
    {
        return ofx_type(v.to_vec4f()[0]*255., v.to_vec4f()[1]*255., v.to_vec4f()[2]*255., v.to_vec4f()[3]*255.);
    }
    
    static ossia_type convert(ofx_type f)
    {
        return ossia_type{float(f.r / 255.), float(f.g / 255.), float(f.b / 255.), float(f.a / 255.)};
    }
};

template<> struct MatchingType<ofFloatColor> {
    using ofx_type = ofFloatColor;
    using ossia_type = opp::value::vec4f;
    
    static opp::node create_parameter(const std::string& name, opp::node parent)
    {return parent.create_rgba(name);}

    static bool is_valid(opp::value v){ return v.is_vec4f(); }
    
    static ofx_type convertFromOssia(const opp::value& v)
    {
        return ofx_type(v.to_vec4f()[0], v.to_vec4f()[1], v.to_vec4f()[2], v.to_vec4f()[3]);
    }
    
    static ossia_type convert(ofx_type f)
    {
        return ossia_type{f.r, f.g, f.b, f.a};
    }
};


template<> struct MatchingType<std::string> {
    using ofx_type = std::string;
    using ossia_type = std::string;

    static opp::node create_parameter(const std::string& name,
                                      opp::node parent)
    {return parent.create_string(name);}

    static bool is_valid(opp::value v){ return v.is_string(); }

    static ofx_type convertFromOssia(const opp::value& v)
    {
      return v.to_string();
    }

    static ossia_type convert(ofx_type f)
    {
      return std::string(f);
    }
};

} // namespace ossia
