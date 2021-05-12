class UwbSessionControl {

public:
    static UwbSessionControl *getInstance() {
        static UwbSessionControl uwbSessionCtl;
        return &uwbSessionCtl;
    }

    ~UwbSessionControl();
    bool init();

private:
    UwbSessionControl();

};
