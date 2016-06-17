/**
 * \file DataTranslator.hpp
 *
 * \todo Add description
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 * \date   Jun 16, 2016
 *
 */

#ifndef SERIAL_DATATRANSLATOR_HPP_
#define SERIAL_DATATRANSLATOR_HPP_

#include <string>
#include <vector>
#include <map>

namespace serial{

  class DataValue;

  /////////////////////////////////////////////////////////////////////////////
  /// \brief Translator to transform a generic \c DataBin object into a
  ///        struct at runtime.
  /////////////////////////////////////////////////////////////////////////////
  template<class T>
  class DataTranslator final{

    //-------------------------------------------------------------------------
    // Public Members
    //-------------------------------------------------------------------------
  public:

    typedef DataTranslator<T> this_type; ///< This object

    typedef T        value_type;      ///< Type of this Data Translator
    typedef T*       pointer;         ///< Type representing pointer
    typedef const T* const_pointer;   ///< Type representing constant pointer
    typedef T&       reference;       ///< Type representing reference
    typedef const T& const_reference; ///< Type representing constant reference

    typedef int size_type; ///< Signed size type used for Data Translator

    // Member pointers to scalar types
    typedef bool        value_type::*bool_member;  ///< Class pointer to bool member
    typedef int         value_type::*int_member;   ///< Class pointer to int member
    typedef float       value_type::*float_member; ///< Class pointer to float member
    typedef std::string value_type::*string_member;///< Class pointer to string member

    // Member pointers to array types (pointers)
    typedef bool*         value_type::*bool_array;   ///< Class pointer to bool array member
    typedef int*          value_type::*int_array;    ///< Class pointer to int array member
    typedef float*        value_type::*float_array;  ///< Class pointer to float array member
    typedef std::string*  value_type::*string_array; ///< Class pointer to string array member

    // Member pointers to vector types
    typedef std::vector<bool>        value_type::*bool_vector;  ///< Class pointer to bool vector member
    typedef std::vector<int>         value_type::*int_vector;   ///< Class pointer to int vector member
    typedef std::vector<float>       value_type::*float_vector; ///< Class pointer to float vector member
    typedef std::vector<std::string> value_type::*string_vector;///< Class pointer to string vector member

    //-------------------------------------------------------------------------
    // Constructor / Destructor
    //-------------------------------------------------------------------------
  public:

    /// \brief Initializes the Data Translator with the specified binary data
    DataTranslator();

    //-------------------------------------------------------------------------
    // Capacity
    //-------------------------------------------------------------------------
  public:

    /// \brief Gets total number of members this \c DataTranslator will
    ///        translate
    ///
    /// \return the total number of members to translate
    size_type members() const noexcept;

    //-------------------------------------------------------------------------
    // Set up member loaders
    //-------------------------------------------------------------------------
  public:

    // Scalar types
    DataTranslator& add_member( const std::string& name, bool_member   member );
    DataTranslator& add_member( const std::string& name, int_member    member );
    DataTranslator& add_member( const std::string& name, float_member  member );
    DataTranslator& add_member( const std::string& name, string_member member );

    // Array types
    DataTranslator& add_member( const std::string& name, bool_array   member, size_type size );
    DataTranslator& add_member( const std::string& name, int_array    member, size_type size );
    DataTranslator& add_member( const std::string& name, float_array  member, size_type size );
    DataTranslator& add_member( const std::string& name, string_array member, size_type size );

    // Vector types
    DataTranslator& add_member( const std::string& name, bool_vector   member );
    DataTranslator& add_member( const std::string& name, int_vector    member );
    DataTranslator& add_member( const std::string& name, float_vector  member );
    DataTranslator& add_member( const std::string& name, string_vector member );

    //-------------------------------------------------------------------------
    // Loaders
    //-------------------------------------------------------------------------
  public:

    /// \brief Translates a data bin into a single data structure
    ///
    /// \param object The object to be populated with data
    /// \param data   The data to translate into the structures
    /// \return the number of members initialized, -1 on error
    size_type translate( value_type& object, const DataValue* data ) const;

    /// \brief Translates a single data bin into an array of structures
    ///
    /// \param objects Array of objects to be populated with data
    /// \param size    The size of the array to translate
    /// \param data    The binary data to translate into the structure
    /// \return the number of members initialized, -1 on error
    size_type translate_uniform( value_type* objects, size_type size, const DataValue* data ) const;

    //-------------------------------------------------------------------------
    // Private Member Types
    //-------------------------------------------------------------------------
  private:

    // Array entries must contain their respective sizes with the pointers
    typedef std::pair<bool_array, size_type>   bool_array_entry;
    typedef std::pair<int_array, size_type>    int_array_entry;
    typedef std::pair<float_array, size_type>  float_array_entry;
    typedef std::pair<string_array, size_type> string_array_entry;

    // Scalar member mapping
    typedef std::map<std::string, bool_member>   bool_member_map;
    typedef std::map<std::string, int_member>    int_member_map;
    typedef std::map<std::string, float_member>  float_member_map;
    typedef std::map<std::string, string_member> string_member_map;

    // Array member mapping
    typedef std::map<std::string, bool_array_entry>   bool_array_map;
    typedef std::map<std::string, int_array_entry>    int_array_map;
    typedef std::map<std::string, float_array_entry>  float_array_map;
    typedef std::map<std::string, string_array_entry> string_array_map;

    // Vector member mapping
    typedef std::map<std::string, bool_vector>   bool_vector_map;
    typedef std::map<std::string, int_vector>    int_vector_map;
    typedef std::map<std::string, float_vector>  float_vector_map;
    typedef std::map<std::string, string_vector> string_vector_map;

    //-------------------------------------------------------------------------
    // Private Members
    //-------------------------------------------------------------------------
  private:

    size_type m_members;

    // Scalar members
    bool_member_map   m_bool_members;   ///< Map of bool member pointers
    int_member_map    m_int_members;    ///< Map of int member pointers
    float_member_map  m_float_members;  ///< Map of float member pointers
    string_member_map m_string_members; ///< Map of string member pointers

    // Array members
    bool_array_map   m_bool_array_members;   ///< Map of bool array member pointers
    int_array_map    m_int_array_members;    ///< Map of int array member pointers
    float_array_map  m_float_array_members;  ///< Map of float array member pointers
    string_array_map m_string_array_members; ///< Map of string array member pointers

    // Vector members
    bool_vector_map   m_bool_vector_members;   ///< Vector of bool array member pointers
    int_vector_map    m_int_vector_members;    ///< Vector of int array member pointers
    float_vector_map  m_float_vector_members;  ///< Vector of float array member pointers
    string_vector_map m_string_vector_members; ///< Vector of string array member pointers
  };

} // namespace serial

#include "detail/DataTranslator.inl"

#endif /* SERIAL_DATATRANSLATOR_HPP_ */
