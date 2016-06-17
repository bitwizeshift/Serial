/**
 * \todo Add description
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 * \date   June 16, 2016
 */
#include <DataValue.hpp>

#include <limits>
#include <algorithm>

namespace serial{

  //--------------------------------------------------------------------------
  // Static Constants
  //--------------------------------------------------------------------------

  static constexpr std::int32_t int32_t_min = std::numeric_limits<std::int32_t>::min();
  static constexpr std::int32_t int32_t_max = std::numeric_limits<std::int32_t>::max();

  static constexpr std::uint32_t uint32_t_min = std::numeric_limits<std::uint32_t>::min();
  static constexpr std::uint32_t uint32_t_max = std::numeric_limits<std::uint32_t>::max();

  static constexpr std::int64_t int64_t_min = std::numeric_limits<std::int64_t>::min();
  static constexpr std::int64_t int64_t_max = std::numeric_limits<std::int64_t>::max();

  static constexpr std::uint64_t uint64_t_min = std::numeric_limits<std::uint64_t>::min();
  static constexpr std::uint64_t uint64_t_max = std::numeric_limits<std::uint64_t>::max();

  //--------------------------------------------------------------------------
  // Constructor/Destructor
  //--------------------------------------------------------------------------

  DataValue::DataValue( data_type type )
    : m_type(type)
  {
    switch(m_type)
    {
    case type_null:
      set_null();
      break;
    case type_bool:
      set_bool();
      break;
    case type_int:
      set_int();
      break;
    case type_uint:
      set_uint();
      break;
    case type_int64:
      set_int64();
      break;
    case type_uint64:
      set_uint64();
      break;
    case type_double:
      set_double();
      break;
    case type_string:
      set_string();
      break;
    case type_array:
      set_array();
      break;
    case type_object:
      set_object();
      break;
    }
  }

  DataValue::DataValue( bool x )
    : m_data(x),
      m_type(type_bool)
  {

  }

  DataValue::DataValue( std::int32_t x )
    : m_data(x),
      m_type(type_int)
  {

  }

  DataValue::DataValue( std::uint32_t x )
    : m_data(x),
      m_type(type_uint)
  {

  }


  DataValue::DataValue( std::int64_t x )
    : m_data(x),
      m_type(type_int64)
  {

  }

  DataValue::DataValue( std::uint64_t x )
    : m_data(x),
      m_type(type_uint64)
  {

  }

  DataValue::DataValue( double x )
    : m_data(x),
      m_type(type_double)
  {

  }


  DataValue& DataValue::operator = ( const DataValue& x )
  {
    clear();
    m_type = x.m_type;

    // Copy the value depending on the type
    switch(m_type)
    {
    case type_null:
      m_data.m_null = nullptr;
      break;
    case type_bool:
      m_data.m_bool = x.m_data.m_bool;
      break;
    case type_int:
      // no break
    case type_uint:
      m_data.m_uint = x.m_data.m_uint;
      break;
    case type_int64:
      // no break
    case type_uint64:
      m_data.m_uint64 = x.m_data.m_uint64;
      break;
    case type_double:
      m_data.m_double = x.m_data.m_double;
      break;
    case type_string:
      new (&m_data) std::string(x.m_data.m_string);
      break;
    case type_array:
      new (&m_data) array_values();
      break;
    case type_object:
      new (&m_data) object_values();
      break;
    }
    return (*this);
  }

  //--------------------------------------------------------------------------

  DataValue::~DataValue()
  {
    clear();
  }

  //--------------------------------------------------------------------------
  // Conversion
  //--------------------------------------------------------------------------

  DataValue::operator bool() const noexcept
  {
    return !is_null();
  }

  //--------------------------------------------------------------------------
  // Capacity
  //--------------------------------------------------------------------------

  DataValue::size_type DataValue::size() const
  {
    switch(m_type){
    case type_object: return m_data.m_object.size();
    case type_array:  return m_data.m_array.size();
    case type_null:   return 0;
    default:
      break;
    }
    return 1;
  }

  bool DataValue::empty() const
  {
    return size() == 0;
  }

  //--------------------------------------------------------------------------
  // Setters
  //--------------------------------------------------------------------------

  void DataValue::set_null()
  {
    clear();

    m_type        = type_null;
    m_data.m_null = nullptr;
  }

  void DataValue::set_bool( bool x )
  {
    clear();

    m_type        = type_bool;
    m_data.m_bool = x;
  }

  void DataValue::set_int( std::int32_t x )
  {
    clear();

    m_type       = type_int;
    m_data.m_int = x;
  }

  void DataValue::set_uint( std::uint32_t x )
  {
    clear();

    m_type        = type_uint;
    m_data.m_uint = x;
  }

  void DataValue::set_int64( std::int64_t x )
  {
    clear();

    m_type         = type_int64;
    m_data.m_int64 = x;
  }

  void DataValue::set_uint64( std::uint64_t x )
  {
    clear();

    m_type = type_uint64;
    m_data.m_uint64 = x;
  }

  void DataValue::set_double( double x )
  {
    clear();

    m_type = type_double;
    m_data.m_double = x;
  }

  void DataValue::set_string( const std::string& str )
  {
    clear();

    m_type = type_string;
    new (&m_data) std::string(str);
  }

  void DataValue::set_array()
  {
    if( m_type == type_array ) return;
    clear();

    m_type  = type_array;
    new (&m_data) std::unique_ptr<array_values>();
  }

  void DataValue::set_object()
  {
    if( m_type == type_object ) return;
    clear();

    m_type  = type_object;
    new (&m_data) std::unique_ptr<object_values>();
  }

  //--------------------------------------------------------------------------

  DataValue& DataValue::add_member( const DataValue& value )
  {
    if(is_null()){
      set_array();
    }

    data_entry entry = data_entry(new DataValue(value));

    m_data.m_array.push_back(std::move(entry));
    return (*this);
  }

  DataValue& DataValue::add_member( const std::string& name, const DataValue& value )
  {
    set_object();

    data_entry entry = data_entry(new DataValue(value));
    m_data.m_object[name] = std::move(entry);
    return (*this);
  }

  void DataValue::clear()
  {
    if( (m_type==type_object || m_type == type_array || m_type == type_string) && m_data.m_ptr )
    {
      // Destruct the types
      switch(m_type){
      case type_string:
        m_data.m_string.~basic_string<char>();
        break;
      case type_array:
        m_data.m_array.~vector<data_entry>();
        break;
      case type_object:
        m_data.m_object.~map<std::string,data_entry>();
        break;
      default:
        break;
      }
    }
  }

  //--------------------------------------------------------------------------
  // Type Queries
  //--------------------------------------------------------------------------

  bool DataValue::is_null() const
  {
    return (m_type == type_null);
  }

  bool DataValue::is_bool() const
  {
    return m_type == type_bool;
  }

  bool DataValue::is_numeric() const
  {
    return (is_integral() || is_double());
  }

  bool DataValue::is_integral() const
  {
    return (is_int() || is_uint() || is_int64() || is_uint64());
  }

  bool DataValue::is_int() const
  {
    switch(m_type){
    case type_int:
      return true;
    case type_uint:
      return m_data.m_uint <= static_cast<std::uint32_t>(int32_t_max);
    case type_int64:
      return m_data.m_int64 >= static_cast<std::int64_t>(int32_t_min) &&
             m_data.m_int64 <= static_cast<std::int64_t>(int32_t_max);
    case type_uint64:
      return m_data.m_uint64 <= static_cast<std::uint64_t>(int32_t_max);
    case type_double:
      return m_data.m_double >= static_cast<double>(int32_t_min) &&
             m_data.m_double <= static_cast<double>(int32_t_max);
    default:
      break;
    }
    return false;
  }

  bool DataValue::is_uint() const
  {
    switch(m_type){
    case type_int:
      return m_data.m_int >= static_cast<std::uint32_t>(uint32_t_min) &&
             m_data.m_int <= static_cast<std::uint32_t>(uint32_t_max);
    case type_uint:
      return true;
    case type_int64:
      return m_data.m_int64 >= static_cast<std::int64_t>(uint32_t_min) &&
             m_data.m_int64 <= static_cast<std::int64_t>(uint32_t_max);
    case type_uint64:
      return m_data.m_uint64 <= static_cast<std::uint64_t>(uint32_t_max);
    case type_double:
      return m_data.m_double >= static_cast<double>(uint32_t_min) &&
             m_data.m_double <= static_cast<double>(uint32_t_max);
    default:
      break;
    }
    return false;
  }

  bool DataValue::is_int64() const
  {
    switch(m_type){
    case type_int:
      return true;
    case type_uint:
      return true;
    case type_int64:
      return true;
    case type_uint64:
      return m_data.m_uint64 <= static_cast<std::uint64_t>(int64_t_max);
    case type_double:
      return m_data.m_double >= static_cast<double>(int64_t_min) &&
             m_data.m_double <= static_cast<double>(int64_t_max);
    default:
      break;
    }
    return false;
  }

  bool DataValue::is_uint64() const
  {
    switch(m_type){
    case type_int:
      return true;
    case type_uint:
      return true;
    case type_int64:
      return m_data.m_int64 >= static_cast<std::int64_t>(uint64_t_max);
    case type_uint64:
      return true;
    case type_double:
      return m_data.m_double >= static_cast<double>(uint64_t_min) &&
             m_data.m_double <= static_cast<double>(uint64_t_max);
    default:
        break;
    }
    return false;
  }

  bool DataValue::is_double() const
  {
    return m_type == type_double || is_integral();
  }

  bool DataValue::is_string() const
  {
    return m_type == type_string;
  }

  bool DataValue::is_array() const
  {
    return m_type == type_array;
  }

  bool DataValue::is_object() const
  {
    return m_type == type_object;
  }

  bool DataValue::is_convertable_to( data_type x ) const
  {
    if(x == m_type) return true;

    switch(x)
    {
    case type_null:
      return (is_numeric() && as_double() == 0.0) ||
             (m_type == type_bool && as_bool() == false) ||
             (m_type == type_string && (m_data.m_ptr) && m_data.m_string.empty()) ||
             (m_type == type_array && (m_data.m_ptr) && m_data.m_array.size() == 0) ||
             (m_type == type_object && (m_data.m_ptr) && m_data.m_object.size() == 0);

    case type_bool:
      return (is_numeric()) ||
             (m_type == type_null);

    case type_int:
      return (is_int()) ||
             (m_type == type_bool) ||
             (m_type == type_null);

    case type_int64:
      return (is_int64()) ||
             (m_type == type_bool) ||
             (m_type == type_null);

    case type_uint:
      return (is_uint()) ||
             (m_type == type_bool) ||
             (m_type == type_null);

    case type_uint64:
      return (is_uint64()) ||
             (m_type == type_bool) ||
             (m_type == type_null);

    case type_double:
      return (is_double()) ||
             (m_type == type_bool) ||
             (m_type == type_null);

    case type_string:
      return (is_numeric()) ||
             (m_type == type_bool) ||
             (m_type == type_null);

    case type_array:
      return (m_type == type_null);

    case type_object:
      return (m_type == type_null);

    default:
        break;
    }
    return false;
  }

  //-------------------------------------------------------------------------
  // Type Access
  //-------------------------------------------------------------------------

  std::nullptr_t DataValue::as_null() const{
    return nullptr;
  }

  bool DataValue::as_bool() const{

    if(is_bool()){
      return m_data.m_bool;
    }else if(is_numeric()){
      return as_double() != 0.0;
    }

    return false;
  }

  std::int32_t DataValue::as_int() const
  {
    // Throw is not convertible to type_int

    switch(m_type)
    {
    case type_int:    return static_cast<std::int32_t>(m_data.m_int);
    case type_uint:   return static_cast<std::int32_t>(m_data.m_uint);
    case type_int64:  return static_cast<std::int32_t>(m_data.m_int64);
    case type_uint64: return static_cast<std::int32_t>(m_data.m_uint64);
    case type_double: return static_cast<std::int32_t>(m_data.m_double);
    case type_bool:   return (m_data.m_bool ? 1 : 0);
    case type_null:   return 0;
    default: break;
    }

    return m_data.m_int;
  }

  std::uint32_t DataValue::as_uint() const
  {
    // Throw is not convertible to type uint

    switch(m_type)
    {
    case type_int:    return static_cast<std::uint32_t>(m_data.m_int);
    case type_uint:   return static_cast<std::uint32_t>(m_data.m_uint);
    case type_int64:  return static_cast<std::uint32_t>(m_data.m_int64);
    case type_uint64: return static_cast<std::uint32_t>(m_data.m_uint64);
    case type_double: return static_cast<std::uint32_t>(m_data.m_double);
    case type_bool:   return (m_data.m_bool ? 1 : 0);
    case type_null:   return 0;
    default: break;
    }

    return m_data.m_uint;
  }

  std::int64_t DataValue::as_int64() const
  {

    // Throw is not convertible to int64

    switch(m_type)
    {
    case type_int:    return static_cast<std::int64_t>(m_data.m_int);
    case type_uint:   return static_cast<std::int64_t>(m_data.m_uint);
    case type_int64:  return static_cast<std::int64_t>(m_data.m_int64);
    case type_uint64: return static_cast<std::int64_t>(m_data.m_uint64);
    case type_double: return static_cast<std::int64_t>(m_data.m_double);
    case type_bool:   return (m_data.m_bool ? 1 : 0);
    case type_null:   return 0;
    default: break;
    }

    return m_data.m_int64;
  }

  std::uint64_t DataValue::as_uint64() const
  {
    // Throw is not convertible to uint64

    switch(m_type)
    {
    case type_int:    return static_cast<std::uint64_t>(m_data.m_int);
    case type_uint:   return static_cast<std::uint64_t>(m_data.m_uint);
    case type_int64:  return static_cast<std::uint64_t>(m_data.m_int64);
    case type_uint64: return static_cast<std::uint64_t>(m_data.m_uint64);
    case type_double: return static_cast<std::uint64_t>(m_data.m_double);
    case type_bool:   return (m_data.m_bool ? 1 : 0);
    case type_null:   return 0;
    default: break;
    }

    return m_data.m_uint64;
  }

  double DataValue::as_double() const
  {
    // Throw is not convertible to double

    switch(m_type)
    {
    case type_int:    return static_cast<double>(m_data.m_int);
    case type_uint:   return static_cast<double>(m_data.m_uint);
    case type_int64:  return static_cast<double>(m_data.m_int64);
    case type_uint64: return static_cast<double>(m_data.m_uint64);
    case type_double: return m_data.m_double;
    case type_bool:   return (m_data.m_bool ? 1.0 : 0.0);
    case type_null:   return 0.0;
    default: break;
    }

    return m_data.m_double;
  }

  std::string DataValue::as_string() const
  {
    // Throw is not string
    return m_data.m_string;
  }

  DataValue* DataValue::as_array()
  {
    // Throw is not array
    return this;
  }

  DataValue* DataValue::as_object()
  {
    // Throw is not object
    return this;
  }

  //--------------------------------------------------------------------------
  // Member Access
  //--------------------------------------------------------------------------

  bool DataValue::has_member( const std::string& name ) const
  {
      if(!is_object()) return false;

      return m_data.m_object.find(name) != m_data.m_object.end();
  }

  DataValue& DataValue::at( size_t i )
  {
    // Throw is not array
    // Throw i < size()

    return *m_data.m_array.at(i);
  }

  const DataValue& DataValue::at( size_t i ) const
  {
    // Throw is not array
    // Throw i < size()

    return *m_data.m_array.at(i).get();
  }

  DataValue& DataValue::at( const std::string& name )
  {
    // Throw is not object

    return *m_data.m_object.at(name).get();
  }

  const DataValue& DataValue::at( const std::string& name ) const
  {
    // Throw is not object

    return *m_data.m_object.at(name).get();
  }

  //--------------------------------------------------------------------------
  // Boolean Operations
  //--------------------------------------------------------------------------

  bool DataValue::operator!() const
  {
    return is_null();
  }

  int DataValue::compare( const DataValue& value ) const
  {
    int type_delta = static_cast<int>(m_type) - static_cast<int>(value.m_type);
    if(type_delta){
      return type_delta;
    }

    switch(m_type)
    {
    case type_null:
      return 0;
    case type_int:
      return m_data.m_int - value.m_data.m_int;
    case type_uint:
      return (m_data.m_uint < value.m_data.m_uint) ? -1 : (m_data.m_uint > value.m_data.m_uint) ? 1 : 0;
    case type_int64:
      return m_data.m_int64 - value.m_data.m_int64;
    case type_uint64:
      return (m_data.m_uint64 < value.m_data.m_uint64) ? -1 : (m_data.m_uint64 > value.m_data.m_uint64) ? 1 : 0;
    case type_double:
      return (m_data.m_double < value.m_data.m_double) ? -1 : (m_data.m_double > value.m_data.m_double) ? 1 : 0;
    case type_bool:
      return m_data.m_bool - value.m_data.m_bool;
    default:
      break;
    }
    return 0;
  }

  //--------------------------------------------------------------------------
  // Private Constructor
  //--------------------------------------------------------------------------

  DataValue::DataValue( const DataValue& x )
    : m_type(x.m_type)
  {
    switch(m_type)
    {
    case type_null:
      m_data.m_null = nullptr;
      break;
    case type_bool:
      m_data.m_bool = x.m_data.m_bool;
      break;
    case type_int:
      // no break
    case type_uint:
      m_data.m_uint = x.m_data.m_uint;
      break;
    case type_int64:
      // no break
    case type_uint64:
      m_data.m_uint64 = x.m_data.m_uint64;
      break;
    case type_double:
      m_data.m_double = x.m_data.m_double;
      break;
    case type_string:
      new (&m_data) std::string(x.m_data.m_string);
      break;
    case type_array:
      new (&m_data) array_values();
      break;
    case type_object:
      new (&m_data) object_values();
      break;
    }
  }

} // namespace serial
