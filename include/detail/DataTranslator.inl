namespace serial{

  template<class T>
  inline DataTranslator<T>::DataTranslator() noexcept
    : m_members(0)
  {

  }

  //---------------------------------------------------------------------------
  // Capacity
  //---------------------------------------------------------------------------

  template<class T>
  inline typename DataTranslator<T>::size_type
  DataTranslator<T>::members() const noexcept
  {
    return m_members;
  }

  //---------------------------------------------------------------------------
  // Adding Members
  //---------------------------------------------------------------------------

  template<class T>
  inline DataTranslator<T>& DataTranslator<T>::add_member( const std::string& str,
                                                           bool_member member )
  {
    ++m_members;
    m_bool_members[ str ] = member;
    return (*this);
  }

  template<class T>
  inline DataTranslator<T>& DataTranslator<T>::add_member( const std::string& str,
                                                           int_member member )
  {
    ++m_members;
    m_int_members[ str ] = member;
    return (*this);
  }

  template<class T>
  inline DataTranslator<T>& DataTranslator<T>::add_member( const std::string& str,
                                                           float_member member )
  {
    ++m_members;
    m_float_members[ str ] = member;
    return (*this);
  }

  template<class T>
  inline DataTranslator<T>& DataTranslator<T>::add_member( const std::string& str,
                                                           string_member member )
  {
    ++m_members;
    m_string_members[ str ] = member;
    return (*this);
  }

  //---------------------------------------------------------------------------

  template<class T>
  inline DataTranslator<T>& DataTranslator<T>::add_member( const std::string& str,
                                                           bool_array member,
                                                           size_type size )
  {
    ++m_members;
    bool_array_entry entry(member,size);
    m_bool_array_members[ str ] = entry;
    return (*this);
  }

  template<class T>
  inline DataTranslator<T>& DataTranslator<T>::add_member( const std::string& str,
                                                           int_array member,
                                                           size_type size )
  {
    ++m_members;
    int_array_entry entry(member,size);
    m_int_array_members[ str ] = entry;
    return (*this);
  }

  template<class T>
  inline DataTranslator<T>& DataTranslator<T>::add_member( const std::string& str,
                                                           float_array member,
                                                           size_type size )
  {
    ++m_members;
    float_array_entry entry(member,size);
    m_float_array_members[ str ] = entry;
    return (*this);
  }

  template<class T>
  inline DataTranslator<T>& DataTranslator<T>::add_member( const std::string& str,
                                                           string_array member,
                                                           size_type size ){
      ++m_members;
      string_array_entry entry(member,size);
      m_string_array_members[ str ] = entry;
      return (*this);
  }

  //---------------------------------------------------------------------------
  // Translating
  //---------------------------------------------------------------------------

  template<class T>
  inline typename DataTranslator<T>::size_type
  DataTranslator<T>::translate( value_type& object,
                                const DataValue* data ) const
  {
    if( !data->is_object() ) return 0;

    size_type entries_matched = 0;

    //-------------------------------------------------------------------------
    // Match up Scalar members
    //-------------------------------------------------------------------------

    typedef typename bool_member_map::const_iterator   bm_iterator;
    typedef typename int_member_map::const_iterator    im_iterator;
    typedef typename float_member_map::const_iterator  fm_iterator;
    typedef typename string_member_map::const_iterator sm_iterator;

    typedef typename bool_member_map::const_iterator   bm_iterator;
    typedef typename int_member_map::const_iterator    im_iterator;
    typedef typename float_member_map::const_iterator  fm_iterator;
    typedef typename string_member_map::const_iterator sm_iterator;

    data->for_each_object([&](const std::string& key, const DataValue& node){
      if( node.is_integral() ){
        im_iterator im_iter = m_int_members.find(key);
        if( im_iter != m_int_members.end() ){
          int_member im_ptr = im_iter->second;
          object.*im_ptr = node.as_int();
          ++entries_matched;
        }
      }else if( node.is_bool() ){
        bm_iterator bm_iter = m_bool_members.find(key);
        if( bm_iter != m_bool_members.end() ){
          bool_member  bm_ptr = bm_iter->second;
          object.*bm_ptr = node.as_bool();
          ++entries_matched;
        }
      }else if( node.is_double() ){
        fm_iterator fm_iter = m_float_members.find(key);
        if( fm_iter != m_float_members.end() ){
          float_member fm_ptr = fm_iter->second;
          object.*fm_ptr = (float) node.as_double();
          ++entries_matched;
        }
      }else if( node.is_string() ){
        sm_iterator sm_iter = m_string_members.find(key);
        if( sm_iter != m_string_members.end() ){
          string_member sm_ptr = sm_iter->second;
          object.*sm_ptr = node.as_string();
          ++entries_matched;
        }
      }
    });

    return entries_matched;
  }

  template<class T>
  inline typename DataTranslator<T>::size_type
  DataTranslator<T>::translate_uniform( value_type* objects,
                                        size_type size,
                                        const DataValue* data ) const
  {
    // Translate the first object
    size_type result = translate( objects[0], data );

    // Since objects are guaranteed to be linearly arranged, memcpy all after
    // the first one is created.
    if(size > 1 )
    {
      memcpy( &objects[1], &objects[0], (size - 1) * sizeof(value_type) );
    }
    return result;
  }

} // namespace serial
