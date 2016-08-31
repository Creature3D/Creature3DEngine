#ifndef __rvrutils__PROPERTY_H
#define __rvrutils__PROPERTY_H

#include <typeinfo>

#include <map>
#include <string>
#include <set>

#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>
#include <CRCore/crVector3.h>

#include <rvrutils/CodeLineException.h>
#include <rvrutils/Notify.h>



namespace rvrutils {

/// Property functor
 template<typename T>
class  PropertyFunctor : public CRCore::Referenced {
    public:
        /// Update property
        /** If the actual value of the property is not used directly
         * the value of the property should be updated from those
         * values that are used directly. **/
        virtual void update( T& ) {};

        /// Commit property
        /** If the actual value of the property is not used directly
         * those values that are used directly should be updated from
         * the value of the property when this method is called. **/
        virtual void commit( T& ) {};
};

#define PROPERTY_TYPE(type, Type) \
    public: \
        \
        typedef std::map<std::string,CRCore::ref_ptr< ::rvrutils::PropertyFunctor<type> >,std::less<std::string> > Type ## FunctorMap;\
        \
        typedef std::map<std::string,type,std::less<std::string> > Type ## Map;\
        \
    protected: \
        \
        Type ## Map m_ ## Type ## _map;\
        \
        Type ## FunctorMap m_ ## Type ## _functor_map;\
        \
    public:\
        \
        Type ## Map& getPropertyMap ## Type() { return m_ ## Type ## _map; };\
        \
        void registerPropertyType ## Type()\
        {\
            m_types.insert( #Type );\
        };\
        \
        void updateProperty ## Type( const std::string& name )\
        {\
            getPropertyFunctor ## Type( name )->update(\
                    getProperty ## Type( name ) );\
        }\
        \
        void commitProperty ## Type( const std::string& name )\
        {\
            getPropertyFunctor ## Type( name )->commit(\
                    getProperty ## Type( name ) );\
        }\
        \
        type& addProperty ## Type( const std::string& name,\
                ::rvrutils::PropertyFunctor<type>* pf, type value = type() )\
        {\
            m_ ## Type ## _map[name] = value;\
            m_ ## Type ## _functor_map[ name ] = pf;\
            return m_ ## Type ## _map[name];\
        };\
        \
        type& addProperty ## Type( const std::string& name,\
                type value = type() )\
        {\
            m_ ## Type ## _map[name] = value;\
            return m_ ## Type ## _map[name];\
        };\
        \
        type& setProperty ## Type( const std::string& name, type value)\
        {\
            Type ## Map::iterator p = m_ ## Type ## _map.find( name );\
            EXCEPTION_IF_TRUE( p == m_ ## Type ## _map.end() );\
            p->second = value;\
            return p->second;\
        };\
        \
        bool hasProperty ## Type( const std::string& name)\
        {return m_ ## Type ## _map.find(name)!=m_ ## Type ## _map.end();};\
        \
        type& getProperty ## Type( const std::string& name)\
        {\
            Type ## Map::iterator p = m_ ## Type ## _map.find( name );\
            EXCEPTION_IF_TRUE( p == m_ ## Type ## _map.end() );\
            return p->second;\
        };\
        \
        ::rvrutils::PropertyFunctor<type>* setPropertyFunctor ## Type(\
                const std::string& name, ::rvrutils::PropertyFunctor<type>* pf )\
        {\
            m_ ## Type ## _functor_map[ name ] = pf;\
            return pf;\
        }\
        \
        ::rvrutils::PropertyFunctor<type>* getPropertyFunctor ## Type(\
                const std::string& name )\
        {\
            Type ## FunctorMap::iterator p =\
                m_ ## Type ## _functor_map.find( name );\
            EXCEPTION_IF_TRUE( p == m_ ## Type ## _functor_map.end() );\
            return p->second.get();\
        }\
        \
        template <class PC>\
        void copyProperties ## Type( PC* pc ) {\
            if ( pc->m_ ## Type ## _map.size() == 0 ) return;\
            NOTIFY_INFO << pc->m_ ## Type ## _map.size() << " " << #Type << " properties to copy." << std::endl;\
            for ( Type ## Map::iterator i = pc->m_ ## Type ## _map.begin();\
                    i != pc->m_ ## Type ## _map.end(); i++ ) {\
                m_ ## Type ## _map[i->first] = i->second;\
            }\
            for ( Type ## FunctorMap::iterator i = pc->m_ ## Type ## _functor_map.begin();\
                    i != pc->m_ ## Type ## _functor_map.end(); i++ ) {\
                m_ ## Type ## _functor_map[i->first] = i->second;\
            }\
        }\




class PropertyContainer : public CRCore::Referenced {

    protected:

        typedef std::set<std::string> TypeSet;

    protected:

        TypeSet m_types;

    public:

        PropertyContainer() {
            registerPropertyTypeInt();
            registerPropertyTypeFloat();
            registerPropertyTypeDouble();
            registerPropertyTypeBool();
            registerPropertyTypeString();
            registerPropertyTypeVec3();
        };

        virtual ~PropertyContainer() {};


        void copyProperties( PropertyContainer* pc ) {
            copyPropertiesInt( pc );
            copyPropertiesFloat( pc );
            copyPropertiesDouble( pc );
            copyPropertiesBool( pc );
            copyPropertiesString( pc );
            copyPropertiesVec3( pc );
        };

        bool hasPropertyType( const std::string& name ) {
            return m_types.find( name ) != m_types.end();
        };

        PROPERTY_TYPE(int,Int)
            
        PROPERTY_TYPE(float,Float)

        PROPERTY_TYPE(double,Double)

        PROPERTY_TYPE(bool,Bool)

        PROPERTY_TYPE(std::string,String)

        PROPERTY_TYPE(CRCore::crVector3,Vec3)
};

} // namespace rmind

#endif // __rvrutils__PROPERTY_H
