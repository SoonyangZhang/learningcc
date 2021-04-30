/** Network topology
 *
 *    10Mb/s, 2ms                            10Mb/s, 4ms
 * n0--------------|                    |---------------n4
 *                 |   1.5Mbps/s, 20ms  |
 *                 n2------------------n3
 *    10Mb/s, 3ms  |                    |    10Mb/s, 5ms
 * n1--------------|                    |---------------n5
 *
 *
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traffic-control-module.h"
#include "ns3/dqc-module.h"
#include "ns3/log.h"
#include<stdio.h>
#include<iostream>
#include <sstream>
#include <string>
#include <utility>
#include <unistd.h>
#include <vector>
#include <memory>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h> // stat
bool isDirExist(const std::string& path)
{
#if defined(_WIN32)
    struct _stat info;
    if (_stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & _S_IFDIR) != 0;
#else 
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
#endif
}
bool makePath(const std::string& path)
{
#if defined(_WIN32)
    int ret = _mkdir(path.c_str());
#else
    mode_t mode = 0777;
    int ret = mkdir(path.c_str(), mode);
#endif
    if (ret == 0)
        return true;

    switch (errno)
    {
    case ENOENT:
        // parent didn't exist, try to create it
        {
            int pos = path.find_last_of('/');
            if (pos == std::string::npos)
#if defined(_WIN32)
                pos = path.find_last_of('\\');
            if (pos == std::string::npos)
#endif
                return false;
            if (!makePath( path.substr(0, pos) ))
                return false;
        }
        // now, try to create again
#if defined(_WIN32)
        return 0 == _mkdir(path.c_str());
#else 
        return 0 == mkdir(path.c_str(), mode);
#endif

    case EEXIST:
        // done!
        return isDirExist(path);

    default:
        return false;
    }
}
using namespace ns3;
using namespace dqc;
using namespace std;
NS_LOG_COMPONENT_DEFINE ("learn-cc");

uint32_t checkTimes;
double avgQueueSize;

// The times
double global_start_time;
double global_stop_time;
double sink_start_time;
double sink_stop_time;
double client_start_time;
double client_stop_time;

NodeContainer n0n2;
NodeContainer n2n3;
NodeContainer n3n4;
NodeContainer n1n2;
NodeContainer n3n5;

Ipv4InterfaceContainer i0i2;
Ipv4InterfaceContainer i1i2;
Ipv4InterfaceContainer i2i3;
Ipv4InterfaceContainer i3i4;
Ipv4InterfaceContainer i3i5;

typedef struct
{
uint64_t bps;
uint32_t msDelay;
uint32_t msQdelay;	
}link_config_t;
//unrelated topology
/*
configuration same as the above dumbbell topology
n0--L0--n2--L1--n3--L2--n4
n1--L3--n2--L1--n3--L4--n5
*/
link_config_t p4pLinks1[]={
[0]={100*1000000,10,60},
[1]={5*1000000,10,60},
[2]={100*1000000,10,60},
[3]={100*1000000,10,60},
[4]={100*1000000,10,60},
};
link_config_t p4pLinks2[]={
[0]={100*1000000,10,120},
[1]={5*1000000,10,120},
[2]={100*1000000,10,120},
[3]={100*1000000,20,120},
[4]={100*1000000,10,120},
};
link_config_t p4pLinks3[]={
[0]={100*1000000,30,100},
[1]={6*1000000,10,100},
[2]={100*1000000,10,100},
[3]={100*1000000,20,100},
[4]={100*1000000,20,100},
};
link_config_t p4pLinks4[]={
[0]={100*1000000,10,150},
[1]={6*1000000,10,150},
[2]={100*1000000,10,150},
[3]={100*1000000,20,150},
[4]={100*1000000,20,150},
};
link_config_t p4pLinks5[]={
[0]={100*1000000,5,90},
[1]={8*1000000,10,90},
[2]={100*1000000,10,90},
[3]={100*1000000,15,90},
[4]={100*1000000,5,90},
};
link_config_t p4pLinks6[]={
[0]={100*1000000,20,120},
[1]={8*1000000,20,120},
[2]={100*1000000,20,120},
[3]={100*1000000,20,120},
[4]={100*1000000,20,120},
};
class TriggerLoss{
public:
    TriggerLoss(){}
    ~TriggerLoss(){
        if(m_timer.IsRunning()){
            m_timer.Cancel();
        }
    }
    void RegisterDevice(Ptr<NetDevice> device){
        m_netDevice=device;
    }
    void Start(){
        Time next=Seconds(2);
        m_timer=Simulator::Schedule(next,&TriggerLoss::ConfigureRandomLoss,this);
    }
    void ConfigureRandomLoss(){
        if(m_timer.IsExpired()){
            std::string errorModelType = "ns3::RateErrorModel";
            ObjectFactory factory;
            factory.SetTypeId (errorModelType);
            Ptr<ErrorModel> em = factory.Create<ErrorModel> ();
            m_netDevice->SetAttribute ("ReceiveErrorModel", PointerValue (em));            
            m_timer.Cancel();
        }
        NS_ASSERT(m_counter==1);
        m_counter++;
    }
private:
    Ptr<NetDevice> m_netDevice;
    EventId m_timer;
    int m_counter{1};
};
static NodeContainer BuildTopology(link_config_t *config,TriggerLoss *trigger=nullptr){
    NodeContainer c;
    c.Create (6);
    n0n2 = NodeContainer (c.Get (0), c.Get (2));
    n1n2 = NodeContainer (c.Get (1), c.Get (2));
    n2n3 = NodeContainer (c.Get (2), c.Get (3));
    n3n4 = NodeContainer (c.Get (3), c.Get (4));
    n3n5 = NodeContainer (c.Get (3), c.Get (5));
    uint32_t bufSize=0;
    
    InternetStackHelper internet;
    internet.Install (c);
    
    NS_LOG_INFO ("Create channels");
    PointToPointHelper p2p;
    TrafficControlHelper tch;
    //L0
    bufSize =config[0].bps * config[0].msQdelay/8000;
    p2p.SetQueue ("ns3::DropTailQueue",
                "Mode", StringValue ("QUEUE_MODE_BYTES"),
                "MaxBytes", UintegerValue (bufSize));
    p2p.SetDeviceAttribute ("DataRate", DataRateValue(DataRate (config[0].bps)));
    p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (config[0].msDelay)));
    NetDeviceContainer devn0n2 = p2p.Install (n0n2);
    //L3
    bufSize =config[3].bps * config[3].msQdelay/8000;
    p2p.SetQueue ("ns3::DropTailQueue",
                "Mode", StringValue ("QUEUE_MODE_BYTES"),
                "MaxBytes", UintegerValue (bufSize));
    p2p.SetDeviceAttribute ("DataRate", DataRateValue(DataRate (config[3].bps)));
    p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (config[3].msDelay)));
    NetDeviceContainer devn1n2 = p2p.Install (n1n2);
    //L1
    bufSize =config[1].bps * config[1].msQdelay/8000;
    p2p.SetQueue ("ns3::DropTailQueue",
                "Mode", StringValue ("QUEUE_MODE_BYTES"),
                "MaxBytes", UintegerValue (bufSize)); 
    p2p.SetDeviceAttribute ("DataRate", DataRateValue(DataRate (config[1].bps)));
    p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (config[1].msDelay)));
    NetDeviceContainer devn2n3 = p2p.Install (n2n3);
    //L2
    bufSize =config[2].bps * config[2].msQdelay/8000;
    p2p.SetQueue ("ns3::DropTailQueue",
                "Mode", StringValue ("QUEUE_MODE_BYTES"),
                "MaxBytes", UintegerValue (bufSize));
    p2p.SetDeviceAttribute ("DataRate", DataRateValue(DataRate (config[2].bps)));
    p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (config[2].msDelay)));
    NetDeviceContainer devn3n4 = p2p.Install (n3n4);
    //L4
    bufSize =config[4].bps * config[4].msQdelay/8000;
    p2p.SetQueue ("ns3::DropTailQueue",
                "Mode", StringValue ("QUEUE_MODE_BYTES"),
                "MaxBytes", UintegerValue (bufSize));
    p2p.SetDeviceAttribute ("DataRate", DataRateValue(DataRate (config[4].bps)));
    p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (config[4].msDelay)));
    NetDeviceContainer devn3n5 = p2p.Install (n3n5);
    
    Ipv4AddressHelper ipv4;
    
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    i0i2 = ipv4.Assign (devn0n2);
    tch.Uninstall (devn0n2);
    
    ipv4.SetBase ("10.1.2.0", "255.255.255.0");
    i1i2 = ipv4.Assign (devn1n2);
    tch.Uninstall (devn1n2);
    
    ipv4.SetBase ("10.1.3.0", "255.255.255.0");
    i2i3 = ipv4.Assign (devn2n3);
    tch.Uninstall (devn2n3);
    if(trigger){
        Ptr<NetDevice> lossDevice=devn2n3.Get(1);
        trigger->RegisterDevice(lossDevice);
    }
    
    ipv4.SetBase ("10.1.4.0", "255.255.255.0");
    i3i4 = ipv4.Assign (devn3n4);
    tch.Uninstall (devn3n4);
    
    ipv4.SetBase ("10.1.5.0", "255.255.255.0");
    i3i5 = ipv4.Assign (devn3n5);
    tch.Uninstall (devn3n5);

    // Set up the routing
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    return c;
}
static void InstallDqc( dqc::CongestionControlType cc_type,
                        Ptr<Node> sender,
                        Ptr<Node> receiver,
                        uint16_t send_port,
                        uint16_t recv_port,
                        float startTime,
                        float stopTime,
                        DqcTrace *trace,
                        uint32_t max_bps=0,uint32_t cid=0,uint32_t emucons=1)
{
    Ptr<DqcSender> sendApp = CreateObject<DqcSender> (cc_type);
    Ptr<DqcReceiver> recvApp = CreateObject<DqcReceiver>();
    sender->AddApplication (sendApp);
    receiver->AddApplication (recvApp);
    if(cid){
        sendApp->SetCongestionId(cid);
    }
    sendApp->SetNumEmulatedConnections(emucons);
    Ptr<Ipv4> ipv4 = receiver->GetObject<Ipv4> ();
    Ipv4Address receiverIp = ipv4->GetAddress (1, 0).GetLocal ();
    recvApp->Bind(recv_port);
    sendApp->Bind(send_port);
    sendApp->ConfigurePeer(receiverIp,recv_port);
    sendApp->SetStartTime (Seconds (startTime));
    sendApp->SetStopTime (Seconds (stopTime));
    recvApp->SetStartTime (Seconds (startTime));
    recvApp->SetStopTime (Seconds (stopTime));
    if(max_bps>0){
        sendApp->SetMaxBandwidth(max_bps);
    }
    if(trace){
        sendApp->SetBwTraceFuc(MakeCallback(&DqcTrace::OnBw,trace));
        recvApp->SetOwdTraceFuc(MakeCallback(&DqcTrace::OnOwd,trace));
    }
}
static double simDuration=200;
uint16_t sendPort=5432;
uint16_t recvPort=5000;
float appStart=0.0;
float appStop=simDuration;
float kLatencyFactorTable[]={0.5,0.6,0.7,0.8,0.9};
float kBetaFactorTable[]={0.5,0.6,0.7,0.8,0.9};
/*
    ./waf --run "scratch/learn-cc --it=1 --cc=learning  --bench=reno --trace=beta --tune=beta --tpi=3"
*/
int main (int argc, char *argv[]){
    LogComponentEnable("dqcsender",LOG_LEVEL_ALL);
    LogComponentEnable("proto_connection",LOG_LEVEL_ALL);
    CommandLine cmd;
    std::string instance=std::string("1");
    std::string cc_tmp("reno");
    std::string bench_algo("learning");
    std::string loss_str("0");
    std::string program_name;
    std::string tune_param("no"); //theta beta
    std::string tune_param_index("0");
    std::string trace_name("trace");
    cmd.AddValue ("it", "instacne", instance);
    cmd.AddValue ("cc", "cctype", cc_tmp);
    cmd.AddValue ("bench", "bench", bench_algo);
    cmd.AddValue ("lo", "loss",loss_str);
    cmd.AddValue ("name", "proname",program_name);
    cmd.AddValue ("tune", "tuneparam",tune_param);
    cmd.AddValue ("tpi", "tuneparamindex",tune_param_index);
    cmd.AddValue ("trace", "tracename",trace_name);
    cmd.Parse (argc, argv);
    std::string trace_folder;
    {
        char buf[FILENAME_MAX];
        memset(buf,0,FILENAME_MAX);
        std::string trace_folder= std::string (getcwd(buf, FILENAME_MAX)) + "/traces/"+trace_name+"/";
        if(!isDirExist(trace_folder)){
            makePath(trace_folder);
        }
        set_dqc_trace_folder(trace_folder);
    }
    set_reno_beta(0.5);
    int loss_integer=std::stoi(loss_str);
    double loss_rate=loss_integer*1.0/1000;
    std::cout<<"l "<<loss_rate<<std::endl;
    if(loss_integer>0){
        Config::SetDefault ("ns3::RateErrorModel::ErrorRate", DoubleValue (loss_rate));
        Config::SetDefault ("ns3::RateErrorModel::ErrorUnit", StringValue ("ERROR_UNIT_PACKET"));
        Config::SetDefault ("ns3::BurstErrorModel::ErrorRate", DoubleValue (loss_rate));
        Config::SetDefault ("ns3::BurstErrorModel::BurstSize", StringValue ("ns3::UniformRandomVariable[Min=1|Max=3]"));
    }
    std::string delimiter="_";
    std::string cc_name;
    std::string log_prefix=program_name;
    if(0==cc_tmp.compare("learning")){
        if(0==tune_param.compare("theta")){
            int sz=sizeof(kLatencyFactorTable)/sizeof(kLatencyFactorTable[0]);
            int index=std::stoi(tune_param_index);
            if(index>sz){
                return 0;
            }
            set_learningcc_latency_factor(kLatencyFactorTable[index]);
            log_prefix=log_prefix+tune_param_index+delimiter;
            std::cout<<"theta "<<get_learningcc_latency_factor()<<std::endl;
            std::cout<<"beta "<<get_learningcc_backoff_factor()<<std::endl;
        }else if(0==tune_param.compare("beta")){
            int sz=sizeof(kBetaFactorTable)/sizeof(kBetaFactorTable[0]);
            int index=std::stoi(tune_param_index);
            if(index>sz){
                return 0;
            }
            set_learningcc_backoff_factor(kBetaFactorTable[index]);
            log_prefix=log_prefix+tune_param_index+delimiter;
            std::cout<<"theta "<<get_learningcc_latency_factor()<<std::endl;
            std::cout<<"beta "<<get_learningcc_backoff_factor()<<std::endl;
        }        
    }
    
    link_config_t *p4p=p4pLinks1;
    if(instance==std::string("1")){
        p4p=p4pLinks1;
    }else if(instance==std::string("2")){
        p4p=p4pLinks2;      
    }else if(instance==std::string("3")){
        p4p=p4pLinks3;
    }else if(instance==std::string("4")){
        p4p=p4pLinks4;
    }else if(instance==std::string("5")){
        p4p=p4pLinks5;
    }else if(instance==std::string("6")){
        p4p=p4pLinks6;
    }else{
        return 0;
    }
    std::unique_ptr<TriggerLoss> triggerloss(new TriggerLoss());
    NodeContainer topo=BuildTopology(p4p,triggerloss.get());
    if(loss_integer>0){
        triggerloss->Start();
    }
    dqc::CongestionControlType cc=kRenoBytes;
    dqc::CongestionControlType cc1=kRenoBytes;
    if(0==cc_tmp.compare("learning")){
        cc=kLearningBytes;
    }else if(0==cc_tmp.compare("viva")){
        cc=kVivace;
    }else if(0==cc_tmp.compare("reno")){
        cc=kRenoBytes;
    }else if(0==cc_tmp.compare("cubic")){
        cc=kCubicBytes; 
    }else{
        std::cout<<"wrong config "<<cc_tmp<<std::endl;
        return 0;
    }
    if(0==bench_algo.compare("learning")){
        cc1=kLearningBytes;
    }else if(0==bench_algo.compare("viva")){
        cc1=kVivace;
    }else if(0==bench_algo.compare("reno")){
        cc1=kRenoBytes;
    }else if(0==bench_algo.compare("cubic")){
        cc1=kCubicBytes;
    }else{
        std::cout<<"wrong config "<<bench_algo<<std::endl;
        return 0;
    }
    if(loss_integer>0){
        cc_name=delimiter+cc_tmp+"l"+std::to_string(loss_integer)+delimiter;
    }else{
        cc_name=delimiter+cc_tmp+delimiter;
    }
    std::cout<<"cc "<<cc_tmp<<std::endl;
    std::cout<<"cc1 "<<bench_algo<<std::endl;
    uint32_t max_bps=0;
    int test_pair=1;
    uint32_t cc_id=1;
    
    
    DqcTrace trace1;
    std::string log=log_prefix+instance+cc_name+std::to_string(test_pair);
    trace1.Log(log,DqcTraceEnable::E_DQC_OWD|DqcTraceEnable::E_DQC_BW);
    InstallDqc(cc,topo.Get(0),topo.Get(4),sendPort,recvPort,appStart,appStop,&trace1,max_bps,cc_id);
    test_pair++;
    sendPort++;
    recvPort++;
    cc_id++;
    
    DqcTrace trace2;
    log=log_prefix+instance+cc_name+std::to_string(test_pair);
    trace2.Log(log,DqcTraceEnable::E_DQC_OWD|DqcTraceEnable::E_DQC_BW);
    InstallDqc(cc1,topo.Get(0),topo.Get(4),sendPort,recvPort,appStart,appStop,&trace2,max_bps,cc_id);
    test_pair++;
    sendPort++;
    recvPort++;
    cc_id++;


    DqcTrace trace3;
    log=log_prefix+instance+cc_name+std::to_string(test_pair);
    trace3.Log(log,DqcTraceEnable::E_DQC_OWD|DqcTraceEnable::E_DQC_BW);
    InstallDqc(cc,topo.Get(1),topo.Get(5),sendPort,recvPort,appStart,appStop,&trace3,max_bps,cc_id);
    test_pair++;
    sendPort++;
    recvPort++;
    cc_id++;
    
    
    DqcTrace trace4;
    log=log_prefix+instance+cc_name+std::to_string(test_pair);
    trace4.Log(log,DqcTraceEnable::E_DQC_OWD|DqcTraceEnable::E_DQC_BW);
    InstallDqc(cc1,topo.Get(1),topo.Get(5),sendPort,recvPort,appStart,appStop,&trace4,max_bps,cc_id);
    test_pair++;
    sendPort++;
    recvPort++;
    cc_id++;
    
    Simulator::Stop (Seconds(simDuration));
    Simulator::Run ();
    Simulator::Destroy();
}

