#ifndef SMBASE_H
#define SMBASE_H

class EventBase;
class StateBase;
class StateMachineBase;

//
// 可以通过type自定义多种类型，子类可以添加自己的数据成员
//
class EventBase {
public:
    int iType;
    double openPrice;
    double highPrice;
    double lowPrice;
    double closePrice;
    long long actionDatetime;
    int index;
};

//
// 不要在构造和析构函数里面干事情，只有onEnter onLeave onEvent三个
// 对于时间，要么丢弃事件 要么终止状态机 要么迁移状态
// 对于迁移状态，返回目标状态的对象
//
// 子类可以通过sm_来访问状态机的数据和成员函数
//
class StateBase {
public:
    virtual void onEnter(StateMachineBase* sm) { sm_ = sm; }
    virtual void onLeave() { sm_ = nullptr; }
    virtual StateBase* onEvent(EventBase* e,
        bool& discard,
        bool& terminate,
        bool& transit) = 0;

protected:
    StateMachineBase* sm_ = nullptr;
};

//
// 构造对象后，需要initiate指定初始状态
// 派发事件并主动检查是否terminated，如果是terminated就不要在派发了
// current_state可以返回当前状态
//
class StateMachineBase {
public:
    StateMachineBase() {}
    virtual ~StateMachineBase()
    {
        if (current_state_) {
            delete current_state_;
            current_state_ = nullptr;
        }
    }

    void initiate(StateBase* s)
    {
        current_state_ = s;
        terminated_ = false;
        current_state_->onEnter(this);
    }
    bool terminated() { return terminated_; }
    void terminate() { terminated_ = true; }
    void process_event(EventBase* e)
    {
        if (terminated_ || current_state_ == nullptr) {
            return;
        }

        StateBase* s = nullptr;
        bool discard = false;
        bool terminate = false;
        bool transit = false;
        s = current_state_->onEvent(e, discard, terminate, transit);
        if (terminate) {
            terminated_ = true;
        }
        else if (transit) {
            current_state_->onLeave();
            delete current_state_;
            current_state_ = s;
            current_state_->onEnter(this);
        }
    }
    StateBase* current_state() { return current_state_; }

protected:
    StateBase* current_state_ = nullptr;
    bool terminated_ = true;
};

#endif // SMBASE_H
