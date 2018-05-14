#ifndef TYPES_HPP
#define TYPES_HPP

#include <memory>

namespace age{

class Types{
public:
    typedef std::shared_ptr<Types> Ptr;
    typedef std::shared_ptr<const Types> ConstPtr;
};

class Integer : public Types {
public:

    Integer(int val) : value(val){}
    Integer(const Integer& t) : value(t.value){}

    int value;
};

class Float : public Types {
public:

    Float(float val): value(val){}
    Float(const Float& t) : value(t.value){}

    float value;
};

class Double : public Types {
public:

    Double(double val) : value(val){}
    Double(const Double& t) : value(t.value){}

    double value;
};

class String : public Types {
public:

    String(std::string val) : value(val){}
    String(const String& t) : value(t.value){}

    std::string value;
};

class Bool : public Types {
public:

    Bool(bool val) : value(val){}
    Bool(const Bool& t) : value(t.value){}

    bool value;
};

}

#define INTEGER_PTR(val) std::make_shared<age::Integer>(age::Integer(val))
#define FLOAT_PTR(val) std::make_shared<age::Float>(age::Float(val))
#define DOUBLE_PTR(val) std::make_shared<age::Double>(age::Double(val))
#define STRING_PTR(val) std::make_shared<age::String>(age::String(val))
#define BOOL_PTR(val) std::make_shared<age::Bool>(age::Bool(val))

#define TO_INTEGER(ptr) std::static_pointer_cast<age::Integer>(ptr)
#define TO_FLOAT(ptr) std::static_pointer_cast<age::Float>(ptr)
#define TO_DOUBLE(ptr) std::static_pointer_cast<age::Double>(ptr)
#define TO_STRING(ptr) std::static_pointer_cast<age::String>(ptr)
#define TO_BOOL(ptr) std::static_pointer_cast<age::Bool>(ptr)

#endif // TYPES_HPP

