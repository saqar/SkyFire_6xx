#include "SharedDefines.h"
#include "GarrisonMgr.h"
#include "Common.h"
#include "WorldSession.h"
#include "WorldPacket.h"

//#define FOLLOWER_MAX_LEVEL          100
//#define FOLLOWER_MAX_ITEM_LEVEL     665
//#define MAX_BUILDINGS_LEVEL1        3
//#define MAX_BUILDINGS_LEVEL2        8
//#define MAX_BUILDINGS_LEVEL3        12
//#define WORK_ORDERS_LEVE1           7
//#define WORK_ORDERS_LEVEL2          14
//#define WORK_ORDERS_LEVEL3          21
//
//enum GarrisonDBState
//{
//    GARR_DB_STATE_NONE = 0,
//    GARR_DB_STATE_DELETE = 1,
//    GARR_DB_STATE_SAVE = 2,
//    GARR_DB_STATE_TEST = 3
//};
//
//class Garrison
//{
//public:
//
//    Garrison(uint64 id, uint32 accountId, uint32 garrId, uint32 garrLevel, uint32 currentResources, uint8 spec, uint32 buildings,
//        uint32 followers, uint32 workOrders) : m_id(id), m_accountId(accountId), m_garrId(garrId), m_garrLevel(garrLevel), m_currentResources(currentResources),
//        m_specialization(spec), m_buildings(buildings), m_followers(followers), m_workOrders(workOrders) { }
//    ~Garrison();
//
//    uint64 GetId()                const { return m_id; }
//    uint32 GetAccountId()         const { return m_accountId; }
//    uint32 GetGarrisonId()        const { return m_garrId; }
//    uint32 GetGarrisonLevel()     const { return m_garrLevel; }
//    uint32 GetResources()         const { return m_currentResources; }
//    uint8  GetGarrisonSpec()      const { return m_specialization; }
//    uint32 GetGarrisonBuildings() const { return m_buildings; }
//    uint32 GetGarrisonFollowers() const { return m_followers; }
//    uint32 GetWorkOrders()        const { return m_workOrders; }
//
//    GarrPlotEntry GetPlotSize() const { return m_plot; }
//    GarrMissionEntry GetTimeToComplete() const { return m_mission; }
//
//    GarrisonDBState GetDBState() const { return m_dbState; }
//    void SetDBState(GarrisonDBState state) { m_dbState = state; }
//
//private:
//
//     Struct of garrison table
//    uint64 m_id;
//    uint32 m_accountId;
//    uint32 m_garrId;
//    uint32 m_garrLevel;
//    uint32 m_currentResources;
//    uint8 m_specialization;
//    uint32 m_buildings;
//    uint32 m_followers;
//    uint32 m_workOrders;
//
//    GarrPlotEntry m_plot;
//    GarrMissionEntry m_mission;
//
//    GarrisonDBState m_dbState;
//};