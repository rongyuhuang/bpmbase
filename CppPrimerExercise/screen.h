#ifndef SCREEN_H
#define SCREEN_H
#include<string>
#include<vector>

class Screen
{
    friend class Window_mgr;
    //friend void Window_mgr::clear(ScreenIndex);
    //如果令成员函数作为友元，需要在之前声明该成员函数
    //如果该成员函数有重载，需要在之前声明该函数
    //友元声明之后，其它类的函数才能调用？
public:
    typedef std::string::size_type pos;
    //using pos=std::string::size_type;

    Screen()=default;
    Screen(pos ht,pos wd,char c):height(ht),width(wd),contents(ht*wd,c){}
    char get()const
    {return contents[cursor];}
    inline char get(pos ht,pos wd)const;
    Screen& move(pos r,pos c);
    void some_member() const;

    inline Screen& set(char);
    inline Screen& set(pos,pos,char);

    Screen& display(std::ostream& os)
    {
        doDisplay(os);
        return *this;
    }

    const Screen& display(std::ostream &os)const
    {
        doDisplay(os);
        return *this;
    }
private:
    pos cursor=0;
    pos height=0,width=0;
    std::string contents;
    mutable size_t access_ctrl; //可变数据成员

    void doDisplay(std::ostream& os)const
    {
        os<<contents;
    }
};


Screen& Screen::move(pos r, pos c)
{
   pos row=r*width;
   cursor = row+c;
   return *this;
}

inline char Screen::get(pos r,pos c)const
{
   pos row = r*width;
   return contents[row+c];
}

void Screen::some_member()const
{
   access_ctrl++;
}

inline Screen& Screen::set(char c)
{
   contents[cursor]=c;
   return *this;
}

inline Screen& Screen::set(pos r, pos col, char c)
{
   contents[r*width+col] = c;
   return *this;
}


class Window_mgr
{
public:
    using ScreenIndex= std::vector<Screen>::size_type;
    void clear(ScreenIndex);
    ScreenIndex addScreen(const Screen&);
private:
    std::vector<Screen> screens{Screen(24,80,' ')};
};

void Window_mgr::clear(ScreenIndex i)
{
    Screen& screen = screens[i];
    screen.contents=std::string(screen.height*screen.width,' ');
}

Window_mgr::ScreenIndex Window_mgr::addScreen(const Screen & s)
{
    screens.push_back(s);
    return screens.size()-1;
}
#endif // SCREEN_H
