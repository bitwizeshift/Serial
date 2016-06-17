/**
 * \file DataValue.hpp
 *
 * \todo Add description
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 * \date   June 16, 2016
 *
 */
#ifndef SERIAL_DATAVALUE_HPP_
#define SERIAL_DATAVALUE_HPP_

#include <cstdint>
#include <memory>
#include <vector>
#include <map>

namespace serial{

  ////////////////////////////////////////////////////////////////////////////
  /// \brief A DataValue contains a piece of generic serialized data from a
  ///        tree
  ///
  ////////////////////////////////////////////////////////////////////////////
  class DataValue final {

    //-------------------------------------------------------------------------
    // Public Types
    //-------------------------------------------------------------------------
  public:

    /// \brief Enumeration of the possible data types
    enum data_type
    {
      type_null,   ///< null ((void*)0) data type
      type_bool,   ///< bool data type
      type_int,    ///< int data type
      type_uint,   ///< unsigned int data type
      type_int64,  ///< int64 (long long) data type
      type_uint64, ///< unsigned int64 (long long) data type
      type_double, ///< double data type
      type_string, ///< string data type
      type_array,  ///< array data type
      type_object  ///< object data type
    };


    using size_type      = std::size_t;

    //-------------------------------------------------------------------------
    // Constructor / Destructor
    //-------------------------------------------------------------------------
  public:

    /// \brief Constructs a \c DataValue of the specified data type (default \c type_null)
    ///
    /// \param type the type to set this Data_Bin to
    DataValue( data_type type = data_type::type_null );

    /// \brief Constructs a \c DataValue of type bool with a value of \p x
    ///
    /// \param x the value to assign to the DataValue
    DataValue( bool x );

    /// \brief Constructs a \c DataValue of type signed int with a value of \p x
    ///
    /// \param x the value to assign to the DataValue
    DataValue( std::int32_t x );

    /// \brief Constructs a \c DataValue of type unsigned int with a value of \p x
    ///
    /// \param x the value to assign to the DataValue
    DataValue( std::uint32_t x );

    /// \brief Constructs a \c DataValue of type int64 with a value of \p x
    ///
    /// \param x the value to assign to the DataValue
    DataValue( std::int64_t x );

    /// \brief Constructs a \c DataValue of type unsigned int64 with a value of \p x
    ///
    /// \param x the value to assign to the DataValue
    DataValue( std::uint64_t x );

    /// \brief Constructs a \c DataValue of type double with a value of \p x
    ///
    /// \param x the value to assign to the DataValue
    DataValue( double x );

    /// \brief Assignment operator that moves \c rhs to \c this
    ///
    /// \note This can be an expensive operation since it will copy the entire
    ///       tree. This should be avoided in most situations
    ///
    /// \param rhs the right side of the equation
    /// \return reference to (*this)
    DataValue& operator = ( const DataValue& rhs );

    //-------------------------------------------------------------------------

    /// \brief Constructs a \c DataValue using c++11 move semantics.
    ///
    /// \param x the rvalue reference value to move
    DataValue( DataValue&& x ) = default;

    /// \brief Assigns a \c DataValue using c++11 move semantics.
    ///
    /// \param x the rvalue reference value to move
    DataValue& operator = ( DataValue&& rhs ) = default;

    //-------------------------------------------------------------------------

    /// \brief Destructs this \c DataValue
    ~DataValue( );

    //-------------------------------------------------------------------------
    // Conversion
    //-------------------------------------------------------------------------
  public:

    /// \brief Converts this \c DataValue to a boolean
    ///
    /// This will return \c true so long as \c is_null() returns \c false
    operator bool() const noexcept;

    //-------------------------------------------------------------------------
    // Capacity
    //-------------------------------------------------------------------------
  public:

    /// \brief Returns the number of elements this Data_Bin contains
    /// \note This should always be 1 for non-array elements
    ///
    /// \return the number of elements contained in this Data_Bin
    size_type size() const;

    /// \brief Returns whether this Data_Bin contains any elements. This is only
    ///        true on null inputs, empty objects, or empty arrays
    /// \return \c True if empty
    bool empty() const;

    //-------------------------------------------------------------------------
    // Type Assignment
    //-------------------------------------------------------------------------
  public:

    /// \brief Sets the DataValue to null
    void set_null();

    /// \brief Sets the \c DataValue to a bool, defaulting the value to \c true
    ///
    /// \param x the value to assign
    void set_bool( bool x = false );

    /// \brief Sets the \c DataValue to a int, defaulting the value to \c 0
    ///
    /// \param x the value to assign
    void set_int( std::int32_t x = 0 );

    /// \brief Sets the \c DataValue to a uint, defaulting the value to \c 0
    ///
    /// \param x the value to assign
    void set_uint( std::uint32_t x = 0 );

    /// \brief Sets the \c DataValue to a int64, defaulting the value to \c 0
    ///
    /// \param x the value to assign
    void set_int64( std::int64_t x = 0 );

    /// \brief Sets the \c DataValue to a uint64, defaulting the value to \c 0
    ///
    /// \param x the value to assign
    void set_uint64( std::uint64_t x = 0 );

    /// \brief Sets the \c DataValue to a double, defaulting the value to \c 0
    ///
    /// \param x the value to assign
    void set_double( double x = 0.0 );

    /// \brief Sets the \c DataValue to string
    ///
    /// \param str the value to assign
    void set_string( const std::string& str = std::string() );

    /// \brief Sets this \c DataValue to an array
    void set_array();

    /// \brief Sets this \c DataValue to an object
    void set_object();

    /// \brief Add a member to the end of an array-type object
    ///
    /// \param value the DataValue to add to the array
    DataValue& add_member( const DataValue& value );

    /// \brief Add a member to the object
    ///
    /// \param name  the name of the object
    /// \param value the DataValue to add to the object
    DataValue& add_member( const std::string& name, const DataValue& value );

    /// \brief Recursively destroys all heap data attached to this \c DataValue
    void clear();

    //-------------------------------------------------------------------------
    // Type Queries
    //-------------------------------------------------------------------------
  public:

    /// \brief Checks if the \c DataValue is \c nullptr
    ///
    /// \return \c true if the \c DataValue is \c nullptr
    bool is_null() const;

    /// \brief Checks if the \c DataValue is \c bool
    ///
    /// \return \c true if the \c DataValue is \c bool
    bool is_bool() const;

    /// \brief Checks if the \c DataValue is numeric
    ///
    /// \return \c true if the \c DataValue is numeric
    bool is_numeric() const;

    /// \brief Checks if the \c DataValue is integral
    ///
    /// \return \c true if the \c DataValue is integral
    bool is_integral() const;

    /// \brief Checks if the \c DataValue is int
    ///
    /// \return \c true if the \c DataValue is int
    bool is_int() const;

    /// \brief Checks if the \c DataValue is uint
    ///
    /// \return \c true if the \c DataValue is uint
    bool is_uint() const;

    /// \brief Checks if the \c DataValue is int64
    ///
    /// \return \c true if the \c DataValue is int64
    bool is_int64() const;

    /// \brief Checks if the \c DataValue is uint64
    ///
    /// \return \c true if the \c DataValue is uint64
    bool is_uint64() const;

    /// \brief Checks if the \c DataValue is double
    ///
    /// \return \c true if the \c DataValue is double
    bool is_double() const;

    /// \brief Checks if the \c DataValue is a string
    ///
    /// \return \c true if the \c DataValue is a string
    bool is_string() const;

    /// \brief Checks if the \c DataValue is an array
    ///
    /// \return \c true if the \c DataValue is an array
    bool is_array() const;

    /// \brief Checks if the \c DataValue is an object
    ///
    /// \return \c true if the \c DataValue is an object
    bool is_object() const;

    /// \brief Checks if this \c DataValue is convertible to \p x
    ///
    /// \param x the parameter to convert
    /// \return \c true if \c this is convertible to \p x
    bool is_convertable_to( data_type x ) const;

    //-------------------------------------------------------------------------
    // Type Access
    //-------------------------------------------------------------------------
  public:

    /// \brief Gets the type of this \c DataValue
    ///
    /// \return the \c data_type of this \c DataValue
    data_type type() const;

    /// \brief Gets this \c DataValue as a null value
    ///
    /// \return \c nullptr_t
    std::nullptr_t as_null() const;

    /// \brief Gets this value as a boolean
    ///
    /// \return this value as a boolean
    bool as_bool() const;

    /// \brief Gets this value as an integer
    ///
    /// \return this value as an integer
    std::int32_t as_int() const;

    /// \brief Gets this value as an unsigned integer
    ///
    /// \return this value as an unsigned integer
    std::uint32_t as_uint() const;

    /// \brief Gets this value as a 64-bit integer
    ///
    /// \return this value as a 64-bit integer
    std::int64_t as_int64() const;

    /// \brief Gets this value as an unsigned 64-bit integer
    ///
    /// \return this value as an unsigned 64-bit integer
    std::uint64_t as_uint64() const;

    /// \brief Gets this value as a double
    ///
    /// \return this value as a double
    double as_double() const;

    /// \brief Gets this value as a \c std::string
    ///
    /// \return this value as a \c std::string
    std::string as_string() const;

    /// \brief Gets this value as an array
    ///
    /// \return this value as an array
    DataValue* as_array();

    /// \brief Gets this value as an object
    ///
    /// \return this value as an object
    DataValue* as_object();

    //-------------------------------------------------------------------------
    // Member Access
    //-------------------------------------------------------------------------
  public:

    /// \brief Checks if this \c DataValue has a member with the name \c name
    ///
    /// \param name the name of the entity to check existence
    /// \return \c true if found
    bool has_member( const std::string& name ) const;

    /// \brief Retrieves the value at array index i
    ///
    /// \note DataValue must be Array or this method will assert
    ///
    /// \param i the index
    /// \return the \c DataValue at i
    DataValue& at( size_t i );

    /// \brief Retrieves the value at array index i
    ///
    /// \note \c DataValue must be Array or this method will assert
    ///
    /// \param i the index
    /// \return the \c DataValue at i
    const DataValue& at( size_t i ) const;

    /// \brief Retrieves the value with the given name
    ///
    /// \note \c DataValue must be Object or this method will assert
    ///
    /// \param i the index
    /// \return the \c DataValue with the given name
    DataValue& at( const std::string& name );

    /// \brief Retrieves the value with the given name
    ///
    /// \note \c DataValue must be Object or this method will assert
    ///
    /// \param i the index
    /// \return the \c DataValue with the given name
    const DataValue& at( const std::string& name ) const;

    /// \copydoc DataValue::at( size_t i )
    DataValue& operator[]( size_t i );

    /// \copydoc DataValue::at( size_t i )
    const DataValue& operator[]( size_t i ) const;

    /// \copydoc DataValue::at( const std::string& name )
    DataValue& operator[]( const std::string& name );

    /// \copydoc DataValue::at( const std::string& name )
    const DataValue& operator[]( const std::string& name ) const;

    //-------------------------------------------------------------------------
    // Boolean Operations
    //-------------------------------------------------------------------------
  public:

    /// \brief Compares \c this with \p x
    ///
    /// \param x the value to compare to
    /// \return 0 if \c x equals \c this, negative if \c this is less than \c x,
    ///         and positive if \c this is greater than \c x
    int compare( const DataValue& x ) const;

    /// \brief Returns \c true if \c this is null
    ///
    /// \return \c true if \c this is null
    bool operator !() const;

    //-----------------------------------------------------------------------------
    // Iteration
    //-----------------------------------------------------------------------------
  public:

    /// \brief Iterates through all members within this \c DataValue array
    ///
    /// \param func the function to call on each iterated element
    template<typename Func>
    void for_each_array(const Func& function) const;

    /// \brief Iterates through all members within this \c DataValue object
    ///
    /// \param function the function to call on each iterated element
    template<typename Func>
    void for_each_object(const Func& function) const;

    //-------------------------------------------------------------------------
    // Private Member Types
    //-------------------------------------------------------------------------
  private:

    using data_entry     = std::unique_ptr<DataValue>;
    using array_values   = std::vector<data_entry>;
    using object_values  = std::map<std::string,data_entry>;

    //-------------------------------------------------------------------------
    // Private Members Types
    //-------------------------------------------------------------------------
  private:

    /// \brief Discriminating union for the possible data types
    union data_union{
      void*         m_null;   ///<
      bool          m_bool;   ///<
      std::int32_t  m_int;    ///<
      std::uint32_t m_uint;   ///<
      std::int64_t  m_int64;  ///<
      std::uint64_t m_uint64; ///<
      double        m_double; ///<
      void*         m_ptr;    ///<

      std::string   m_string; ///<
      array_values  m_array;  ///<
      object_values m_object; ///< Object

      data_union() : m_null(nullptr){}
      data_union( std::nullptr_t ) : m_null(nullptr){}
      data_union( bool b ) : m_bool(b){}
      data_union( std::int32_t i ) : m_int(i){}
      data_union( std::uint32_t u ) : m_uint(u){}
      data_union( std::int64_t i ) : m_int64(i){}
      data_union( std::uint64_t u ) : m_uint64(u){}
      data_union( double d ) : m_double(d){}
      data_union( const std::string& x ) : m_string(x){}
      ~data_union(){}
    } m_data;

    data_type m_type; ///< Tag for the type of data in the union

    //-------------------------------------------------------------------------
    // Private Constructor
    //-------------------------------------------------------------------------
  private:

    /// \brief Constructs a \c DataValue by deep-copying another DataValue
    ///
    /// \param x the DataValue to copy
    DataValue( const DataValue& rhs );

  };

  //---------------------------------------------------------------------------
  // Inline Operations
  //---------------------------------------------------------------------------

  inline DataValue::data_type DataValue::type() const
  {
    return m_type;
  }

  inline DataValue& DataValue::operator []( size_t i )
  {
    return at( i );
  }

  inline const DataValue& DataValue::operator []( size_t i ) const
  {
    return at( i );
  }

  inline DataValue& DataValue::operator []( const std::string& name )
  {
    return at(name);
  }

  inline const DataValue& DataValue::operator []( const std::string& name ) const
  {
    return at(name);
  }

  //---------------------------------------------------------------------------
  // Inline Equality
  //---------------------------------------------------------------------------

  inline bool operator ==( const DataValue& lhs, const DataValue& rhs )
  {
    return lhs.compare(rhs) == 0;
  }

  inline bool operator <( const DataValue& lhs, const DataValue& rhs )
  {
    return lhs.compare(rhs) < 0;
  }

  inline bool operator >( const DataValue& lhs, const DataValue& rhs )
  {
    return (rhs < lhs);
  }

  inline bool operator <=( const DataValue& lhs, const DataValue& rhs )
  {
    return !(rhs < lhs);
  }

  inline bool operator >=( const DataValue& lhs, const DataValue& rhs )
  {
    return !(lhs < rhs);
  }

  inline bool operator !=( const DataValue& lhs, const DataValue& rhs )
  {
    return !(rhs == lhs);
  }

  template<typename Func>
  inline void DataValue::for_each_array(const Func& function) const
  {
    if(m_type!=type_array){
      // throw
    }

    for(const auto& x : m_data.m_array){
      function(x);
    }
  }

  template<typename Func>
  inline void DataValue::for_each_object(const Func& function) const
  {
    if(m_type!=type_object){
      // throw
    }

    for(const auto& x : m_data.m_object){
      function(x.first,x.second);
    }
  }

} // namespace serial

#endif /* SERIAL_DATAVALUE_HPP_ */
