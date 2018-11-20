#ifndef _SIGNAL
#define _SIGNAL

#include <vector>
#include <memory>

// ---------- Slot ----------

template<typename... Args>
class Slot{
private:
    // _TA = SlotTypeArgs
    template<typename... _TA>
    class SlotType{
    public:
        virtual void run(_TA... args) = 0;
    };

    // _HC = SlotHolderCls, _HR = SlotHolderRet, _HA = SlotHolderArgs
    template<typename _HC, typename _HR, typename... _HA>
    class SlotHolder: public SlotType<Args...>{
    public:
        SlotHolder(_HC &ins, _HR(_HC::*m)(_HA...)): instance(ins), method(m) { }

        void run(_HA... args) override{
            (instance.*method)(args...);
        }

    private:
        _HC &instance;
        _HR(_HC::*method)(_HA...);
    };

public:
    template<typename Cls, typename Ret>
    Slot(Cls &ins, Ret(Cls::*m)(Args...)):
        slot_pointer(std::make_shared<SlotHolder<Cls, Ret, Args...>>(ins, m)) { }
    
    void operator() (Args... args){
        slot_pointer->run(args...);
    }

private:
    std::shared_ptr<SlotType<Args...>> slot_pointer;
};

// ---------- Signal ----------

template<typename... Args>
class Signal{
public:
    Signal(): slot_list() { }

    void add_slot(const Slot<Args...> &slot){
        slot_list.push_back(slot);
    }

    void operator() (Args... args){
        for(auto &slot : slot_list){
            slot(args...);
        }
    }

private:
    std::vector<Slot<Args...>> slot_list;
};

// ---------- Helper Functions ----------

template<typename Cls, typename Ret, typename... Args>
void link_ss(Signal<Args...> &sig, Cls &ins, Ret(Cls::*m)(Args...)){
    sig.add_slot(Slot<Args...>(ins, m));
}

#define connect(sender, sig, recver, slot){ link_ss(sender.sig, recver, &decltype(recver)::slot); }
#define connect_p(sender, sig, recver, slot){ link_ss(sender->sig, recver, &decltype(recver)::slot); }

#define signal(name, args...) Signal<args> name


#endif /* _SIGNAL */

