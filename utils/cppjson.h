#ifndef CPPJSON_H
#define CPPJSON_H
#include<string>
#include<rapidjson/writer.h>
#include<rapidjson/document.h>
#include<rapidjson/stringbuffer.h>

using namespace rapidjson;

class CppJson{
    typedef Writer<StringBuffer> JsonWriter;

public:
    CppJson():m_writer(m_buff){}

    ~CppJson(){}

    ///序列化结构体数组之前需要调用该接口，然后再循环去序列化
    void StartArray()
    {
        m_writer.StartArray();
    }

    void EndArray()
    {
        m_writer.EndArray();
    }

    void StartObject()
    {
        m_writer.StartObject();
    }

    void EndObject()
    {
        m_writer.EndObject();
    }

    ///写键值对
    template<typename T>
    void WriteJson(std::string& k,T&& v)
    {
        m_writer.String(k.c_str());
        WriteValue(std::forward<T>(v));
    }

    template<typename T>
    void WriteJson(const char* k,T&&v)
    {
        m_writer.String(k);
        WriteValue(std::forward<T>(v));
    }

    ///返回对象序列化后端json字符串
    const char* GetString()const
    {
        return m_buff.GetString();
    }
private:
    template<typename V>
    typename std::enable_if<std::is_same<V,int>::value>::type WriteValue(V value)
    {
        m_writer.Int(value);
    }
    template<typename V>
    typename std::enable_if<std::is_same<V,unsigned int>::value>::type WriteValue(V value)
    {
        m_writer.Uint(value);
    }

    template <typename V>
    typename std::enable_if<std::is_same<V,int64_t>::value>::type WriteValue(V value)
    {
        m_writer.Int64(value);
    }

    template <typename V>
    typename std::enable_if<std::is_same<V,bool>::value>::type WriteValue(V value)
    {
        m_writer.Bool(value);
    }

    template <typename V>
    typename std::enable_if<std::is_same<V,std::nullptr_t>::value>::type WriteValue(V value)
    {
        m_writer.Null();
    }

    template <typename V>
    typename std::enable_if<std::is_floating_point<V>::value>::type WriteValue(V value)
    {
        m_writer.Double(value);
    }

    template <typename V>
    typename std::enable_if<std::is_pointer<V>::value>::type WriteValue(V value)
    {
        m_writer.String(value);
    }

    template <typename V>
    typename std::enable_if<std::is_array<V>::value>::type WriteValue(V value)
    {
        m_writer.String(value);
    }
private:
    Document m_doc;
    JsonWriter m_writer;
    StringBuffer m_buff;
};
#endif // CPPJSON_H
