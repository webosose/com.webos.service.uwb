#include <cstdint>
#include <string>

using namespace std;

class UwbRangingInfo {
public:

    ~UwbRangingInfo();
    class RangingData {
    public:
        RangingData(){};
        RangingData(string status, int64_t angle, int64_t distance) {
            m_status = status;
            m_angle = angle;
            m_distance =distance;
        }

        string getStatus() {
            return m_status;
        }

        int64_t getAngle() {
            return m_angle;
        }

        int64_t getDistance() {
            return m_distance;
        }

        void setStatus(string status) {
            m_status = status;
        }

        void setAngle(int64_t angle) {
            m_angle = angle;
        }

        void setDistance(int64_t distance) {
            m_distance =distance;
        }
    private:
        string m_status;
        int64_t m_angle;
        int64_t m_distance;
    };


    UwbRangingInfo();

    UwbRangingInfo(bool connectionStatus, int condition, string remoteDevAddr, string status, int64_t angle, int64_t distance) {
        m_connectionStatus = connectionStatus;
        m_condition = condition;
        m_remoteDevAddr = remoteDevAddr;
        m_receivedData = new RangingData(status, angle, distance);
    }

    bool getConnectionStatus() {
        return m_connectionStatus;
    }

    int getCondition() {
        return m_condition;
    }

    string getRemoteDevAddr() {
        return m_remoteDevAddr;
    }

    RangingData* getData() {
        return m_receivedData;
    }

    void setConnectionStatus(bool connectionStatus) {
        m_connectionStatus = connectionStatus;
    }

    void setCondition(int condition) {
        m_condition = condition;
    }

    void setRemoteDevAddr(string remoteDevAddr) {
         m_remoteDevAddr = remoteDevAddr;
    }

    void setData(RangingData* data) {
        m_receivedData = data;
    }



private:

    uint16_t m_sessionId;
    bool m_connectionStatus;
    int m_condition;
    string m_remoteDevAddr;
    RangingData* m_receivedData = nullptr;

};
