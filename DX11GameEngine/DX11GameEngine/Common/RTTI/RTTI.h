    #pragma once

#include <string>


namespace GNF::Common::RTTI
{

    class RTTI
    {
    public:
        virtual const size_t TypeID() const = 0;
        virtual void QueryInterface(const size_t, GNF::Common::RTTI::RTTI** rtti) const
        {
            (*rtti) = nullptr;
            return;
        }

        virtual bool Is(const size_t id) const
        {
            return false;
        }

        virtual bool Is(const std::string& name) const
        {
            return false;
        }

        template <typename T>
        T* As()
        {
            if (Is(T::sTypeID()))
            {
                return (T*)this;
            }

            return nullptr;
        }
        template <typename T>
        const T* As() const
        {
            if (Is(T::sTypeID()))
            {
                return (T*)this;
            }

            return nullptr;
        }
    };

#define RTTI_DECLARATIONS(Type, ParentType)                            \
    public:                                                            \
                                                             \
        static std::string TypeName() { return std::string(#Type); }   \
        virtual const size_t TypeID() const                    \
        { return Type::sTypeID(); }                     \
        static const size_t sTypeID()                              \
        { static int d = 0; return (size_t) &d; }                               \
        virtual void QueryInterface( const size_t id,GNF::Common::RTTI::RTTI** rtti )       \
        {                                                              \
            if (id == sTypeID())                                   \
                { (*rtti) = (RTTI*)this; }                                \
            else                                                       \
                { ParentType::QueryInterface(id,rtti); }             \
        }                                                              \
                                                          \
        virtual bool Is(const size_t id) const                         \
        {                                                              \
            if (id == sTypeID())                                   \
                { return true; }                                       \
            else                                                       \
                { return ParentType::Is(id); }                         \
        }                                                              \
        virtual bool Is(const std::string& name) const                 \
        {                                                              \
            if (name == TypeName())                                    \
                { return true; }                                       \
            else                                                       \
                { return ParentType::Is(name); }                       \
        }   


}