// @@@LICENSE
//
//      Copyright (c) 2022 LG Electronics, Inc.
//
// Confidential computer software. Valid license from LG required for
// possession, use or copying. Consistent with FAR 12.211 and 12.212,
// Commercial Computer Software, Computer Software Documentation, and
// Technical Data for Commercial Items are licensed to the U.S. Government
// under vendor's standard commercial license.
//
// LICENSE@@@

class UwbSessionControl {//TODO:updated once multiple-session supported.

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
