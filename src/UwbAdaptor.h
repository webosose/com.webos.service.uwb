class UwbAdaptor {

public:
    static UwbAdaptor *getInstance() {
        static UwbAdaptor uwbAdaptor;
        return &uwbAdaptor;
    }

    ~UwbAdaptor();
    bool init();

private:
    UwbAdaptor();

};
