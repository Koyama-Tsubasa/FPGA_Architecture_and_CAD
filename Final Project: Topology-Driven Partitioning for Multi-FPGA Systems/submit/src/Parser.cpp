#include "../include/Parser.hpp"

namespace FPGA_BASE {

const std::vector<std::string> split(const std::string& str, const std::string& pattern) {
    std::vector<std::string> result;
    std::string::size_type begin, end;

    end = str.find(pattern);
    begin = 0;

    while (end != std::string::npos) {
        if (end - begin != 0) {
            result.push_back(str.substr(begin, end-begin)); 
        }    
        begin = end + pattern.size();
        end = str.find(pattern, begin);
    }

    if (begin != str.length()) {
        result.push_back(str.substr(begin));
    }
    return result;        
}

std::tuple<int, int, int, int, int, int> Parser::parseFPGAParameters(std::istream &input){
    int fpga_nums = 0;
    int fpga_channel_nums = 0; 
    int fpga_cap = 0;
    int total_nodes = 0;
    int total_nets = 0;
    int total_fixed_nodes = 0;
    input >> fpga_nums >> fpga_channel_nums >> fpga_cap >> total_nodes >> total_nets >> total_fixed_nodes;
    return std::make_tuple(fpga_nums, fpga_channel_nums, fpga_cap, total_nodes, total_nets, total_fixed_nodes);
}

FPGA_Channel Parser::parseFPGAChannel(std::istream &input){
    int channel1 = 0;
    int channel2 = 0;
    input >> channel1 >> channel2;
    FPGA_Channel fpga_channel(channel1, channel2);
    return fpga_channel;
}

FPGA_Net* Parser::parseFPGANet(std::istream &input){
    auto net = new FPGA_Net(0);
    FPGA_Node *source_node;
    std::vector<FPGA_Node*> sink_node;
    std::string str;
    std::getline(input, str);
    std::vector<std::string> ret = split(str, " ");

    source_node = NodeMap[ret[0]];
    for(size_t i = 1; i < ret.size(); i++){
        sink_node.emplace_back(NodeMap[ret[i]]);
        sink_node.back()->add_neighbor_node(source_node);
        source_node->add_neighbor_node(NodeMap[ret[i]]);
    }
    net->set_source_node(source_node);
    net->set_sink_node(sink_node);
    return net;
}

void Parser::parseFPGAFixedNode(std::istream &input){
    std::string fixedNodeId, FPGAId;
    input >> fixedNodeId >> FPGAId;
    NodeMap[fixedNodeId]->set_is_fixed(true);
    NodeMap[fixedNodeId]->set_fixed_fpga_id(std::stoi(FPGAId)); //update m_fpga_nodes too...
}

void Parser::clear(){
    std::vector<FPGA_Node*>().swap(m_fpga_nodes);
    std::vector<FPGA_Net*>().swap(m_fpga_nets);
    std::map<std::string, FPGA_Node*>().swap(NodeMap);
}

std::unique_ptr<FPGA_BASE::Input> Parser::parser(std::istream &input){
    std::string line;
    int fpga_nums;
    int fpga_channel_nums;
    int fpga_cap;
    int total_nodes;
    int total_nets;
    int total_fixed_nodes;
    std::tie(fpga_nums, fpga_channel_nums, fpga_cap, total_nodes, total_nets, total_fixed_nodes) = 
        parseFPGAParameters(input);
    FPGA_BASE::FPGA_Parameter fpga_parameter(fpga_nums, fpga_channel_nums, fpga_cap, total_nodes, total_nets, total_fixed_nodes);
    
    std::vector<FPGA_Channel> fpga_channels;
    fpga_channels.reserve(fpga_channel_nums);
    while (fpga_channel_nums--)
    {
        fpga_channels.push_back(parseFPGAChannel(input));
    }
    std::getline(input, line);//read endl

    //bulid node map
    for(int i = 0; i < total_nodes; i++){
        auto str = std::to_string(i);
        auto node = new FPGA_Node(0);
        node->set_node_id(i);
        NodeMap.insert(std::pair<std::string, FPGA_Node*>(str, node));
        m_fpga_nodes.push_back(node);
    }

    for(size_t i = 0; i < total_nets; i++){
        m_fpga_nets.push_back(parseFPGANet(input));
        m_fpga_nets.back()->set_net_id(i);
        m_fpga_nets.back()->get_source_node()->add_net_id(i);
        for(auto &node : m_fpga_nets.back()->get_sink_node()){
            node->add_net_id(i);
        }
        NetMap.insert(std::pair<int, FPGA_Net*>(i, m_fpga_nets.back()));
    }

    while(total_fixed_nodes--){
        parseFPGAFixedNode(input);
    }

    return std::unique_ptr<FPGA_BASE::Input>(new FPGA_BASE::Input(std::move(fpga_parameter), std::move(fpga_channels), 
                                                                  std::move(m_fpga_nets), std::move(m_fpga_nodes), 
                                                                  std::move(NodeMap), std::move(NetMap)));
}

} // namespace FPGA_BASE
