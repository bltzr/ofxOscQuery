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
 *
 */
template<typename> struct MatchingType;

template<> struct MatchingType<float> {
  using ofx_type = float;
  using ossia_type = float;

  static opp::node create_parameter(const std::string& _name,
                             opp::node _parent)
       {return _parent.create_float(_name);}

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

  static opp::node create_parameter(const std::string& _name,
                             opp::node _parent)
      {return _parent.create_int(_name);}

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

  static opp::node create_parameter(const std::string& _name,
                             opp::node _parent)
       {return _parent.create_bool(_name);}

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


template<> struct MatchingType<std::string> {
  using ofx_type = string;
  using ossia_type = string;

  static opp::node create_parameter(const std::string& _name,
                             opp::node _parent)
             {return _parent.create_string(_name);}

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

template<> struct MatchingType<double> {
  using ofx_type = double;
  using ossia_type = float;

  static opp::node create_parameter(const std::string& _name, opp::node _parent)
            {return _parent.create_float(_name);}

  static bool is_valid(opp::value v){ return v.is_float(); }

  static ofx_type convertFromOssia(const opp::value& v)
  {
    return double(v.to_float());
  }

  static ossia_type convert(ofx_type f)
  {
    return float(f);
  }
};

template<> struct MatchingType<ofVec2f> {
  using ofx_type = ofVec2f;
  using ossia_type = std::array<float, 2ul>;

  static opp::node create_parameter(const std::string& _name, opp::node _parent)
        {return _parent.create_vec2f(_name);}

  static bool is_valid(opp::value v){ return v.is_list(); }

  static ofx_type convertFromOssia(const opp::value& v)
  {
    const auto& t = v.to_list();
    return ofx_type(t[0].to_float(),
                    t[1].to_float());
  }

  static ossia_type convert(ofx_type f)
  {
    // This could probably be done otherwise, see editor/value/vec.hpp (from where this comes), 
    // but for now (and for all vec types below), it will be done this way 
    return ossia_type{f.x, f.y};
  }
};

template<> struct MatchingType<ofVec3f> {
  using ofx_type = ofVec3f;
  using ossia_type = std::array<float, 3ul>;

  static opp::node create_parameter(const std::string& _name, opp::node _parent)
                 {return _parent.create_vec3f(_name);}

  static bool is_valid(opp::value v){ return v.is_list(); }

  static ofx_type convertFromOssia(const opp::value& v)
  {
    const auto& t = v.to_list();
    return ofx_type(t[0].to_float(),
                    t[1].to_float(),
                    t[2].to_float());
  }

  static ossia_type convert(ofx_type f)
  {
      return ossia_type{f.x, f.y, f.z};
  }
};

template<> struct MatchingType<ofVec4f> {
  using ofx_type = ofVec4f;
  using ossia_type = std::array<float, 4ul>;

  static opp::node create_parameter(const std::string& _name, opp::node _parent)
            {return _parent.create_vec4f(_name);}

  static bool is_valid(opp::value v){ return v.is_list(); }

  static ofx_type convertFromOssia(const opp::value& v)
  {
    const auto& t = v.to_list();
    return ofx_type(t[0].to_float(),
                    t[1].to_float(),
                    t[2].to_float(),
                    t[3].to_float());
  }

  static ossia_type convert(ofx_type f)
  {
    return ossia_type{f.x, f.y, f.z, f.w};
  }
};

template<> struct MatchingType<ofColor> {
  using ofx_type = ofColor;
  using ossia_type = std::array<float, 4ul>;

  static opp::node create_parameter(const std::string& _name, opp::node _parent)
              {return _parent.create_rgba(_name);}

  static bool is_valid(opp::value v){ return v.is_list(); }

  static ofx_type convertFromOssia(const opp::value& v)
  {
    const auto& t = v.to_list();
    return ofx_type(t[0].to_float() * 255.,
                    t[1].to_float() * 255.,
                    t[2].to_float() * 255.,
                    t[3].to_float() * 255.);
  }

  static ossia_type convert(ofx_type f)
  {
    return ossia_type{float(f.r / 255.), float(f.g / 255.), float(f.b / 255.), float(f.a / 255.)};
  }
};

template<> struct MatchingType<ofFloatColor> {
  using ofx_type = ofFloatColor;
  using ossia_type = std::array<float, 4ul>;

    static opp::node create_parameter(const std::string& _name, opp::node _parent)
                {return _parent.create_argb8(_name);}

    static bool is_valid(opp::value v){ return v.is_list(); }

// For those conversions, as there is no rgba8 type in ossia, we swap the 1st and 4th values
  static ofx_type convertFromOssia(const opp::value& v)
  {
    const auto& t = v.to_list();
    return ofx_type(t[1].to_float(),
                    t[2].to_float(),
                    t[3].to_float(),
                    t[0].to_float());
  }

  static ossia_type convert(ofx_type f)
  {
    return ossia_type{f.a, f.r, f.g, f.b};
  }
};
} // namespace ossia
