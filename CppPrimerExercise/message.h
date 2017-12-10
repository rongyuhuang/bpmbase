#ifndef MESSAGE_H
#define MESSAGE_H

#include<string>
#include<set>

class Folder;
class Message{
    friend void swap(Message&,Message&);
    friend class Folder;
public:
    //folders被隐式初始化为空集合
    explicit Message(const std::string & str=""):contents(str){}
    Message(const Message&);  //拷贝构造函数
    Message& operator =(const Message&);  //拷贝赋值运算法
    ~Message();
    void save(Folder&);
    void remove(Folder&);
    void printInfo();

private:
    std::string contents;
    std::set<Folder*> folders;

    void add_to_folder(const Message&);
    void remove_from_folders();
    // used by Folder class to add self to this Message's set of Folder's
    void addFolder(Folder *f) { folders.insert(f); }
    void rmvFolder(Folder *f) { folders.erase(f); }
};
inline void swap(Message&,Message&);


class Folder{
    friend void swap(Message &lhs, Message &rhs);
    friend class Message;

public:
    Folder()=default;
    Folder(const Folder&);
    Folder& operator = (const Folder&);
    ~Folder();
    void save(Message& m);
    void remove(Message& m);
    void printInfo();
private:
    std::set<Message*> msgs;
    void add_to_messages(const Folder&);
    void remove_from_messages();

    void addMsg(Message*);
    void rmvMsg(Message*);
    void remove_message(Folder*);
};
#endif // MESSAGE_H
