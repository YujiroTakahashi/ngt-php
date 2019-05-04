#ifndef __CROCO_INDEX_H__
#define __CROCO_INDEX_H__

#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include <NGT/Index.h>

#include "repository.pb.h"

namespace croco {

class Index : public NGT::Index {
private:
    croco::ObjectSpace _objectSpace;
    croco::DistanceSpace _distanceSpace;
    croco::LeafNodeSpace _leafNodeSpace; 
    croco::InternalNodeSpace _internalNodeSpace;

public:
    void create(
        const std::string &path, 
        std::size_t dimension, 
        int edgeSizeForCreation = 10, 
        int edgeSizeForSearch = 40, 
        const std::string distanceType = "L2", 
        const std::string objectType = "Float"
    );
    int getDimension();
    void exportIndex();
    void getObjectString(std::string* output);
    void getDistanceString(std::string* output);
    void getLeafNodeString(std::string* output);
    void getInternalNodeString(std::string* output);
    void setObjectString(const std::string& data);
    void setDistanceString(const std::string& data);
    void setLeafNodeString(const std::string& data);
    void setInternalNodeString(const std::string& data);
    void addObjectString(const std::string& data);
    void addDistanceString(const std::string& data);
    void addLeafNodeString(const std::string& data);
    void addInternalNodeString(const std::string& data);
    void importIndex();
    void batchInsert(std::vector<float> data, std::size_t objectCount = 1, std::size_t numThreads = 8);
    int insert(std::vector<float> data);
    std::vector<std::pair<int, float>> search(
        std::vector<float> query,
        std::size_t size = 10,  // the number of resultant objects
        float epsilon = 0.1,    // search parameter epsilon. the adequate range is from 0.0 to 0.15. minus value is acceptable.
        int edgeSize = -1       // the number of used edges for each node during the exploration of the graph.
    );
    std::vector<float> getObject(std::size_t id);
private:
    void _exportObject();
    void _exportDistance();
    void _exportLeafNode();
    void _exportInternalNode();
    void _importObject();
    void _importDistance();
    void _importLeafNode();
    void _importInternalNode();
};

} // namespace croco

#endif // #define __CROCO_INDEX_H__

